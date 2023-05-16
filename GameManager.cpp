
#include "GameManager.h"
#include "promotiondialog.h"
#include <sstream>
#include <cstring>
#include <QDebug>

void GameManager::checkForPromotion(ChessPieces *chess)
{
    if (chess->getType() == TYPE::PAWN) {
        if (chess->isWhite() && chess->getRow() == 0) {
            chess->setType(PromotionDialog::showDialog(COLOR::WHITE), COLOR::WHITE);
        }
        else if (chess->isBlack() && chess->getRow() == 7) {
            chess->setType(PromotionDialog::showDialog(COLOR::BLACK), COLOR::BLACK);
        }
    }
}

void GameManager::load(QString FEN, ChessPieces *pieces[8][8], COLOR &moveTeam, int &castlingFlag, Position &enPassant, int &halfmove, int &fullmove)
{
    std::istringstream iss(FEN.toStdString());

    // pieces placement
    int numOfWhiteKing = 0;
    int numOfBlackKing = 0;
    for (int row = 0; row < 8; ++row) {
        // extract a row into aRow
        std::string aRow;
        do {
            aRow.push_back(iss.get());
        } while(aRow.back() != '/' && aRow.back() != ' ');

        // 檢查一列的結尾
        if (row < 7) {
            if (aRow.back() != '/')
                throw QString("Num of rows mismatched: need 8 rows but got less than 8.");
        }
        else {
            if (aRow.back() != ' ')
                throw QString("Num of rows mismatched: need 8 rows but got more than 8.");
        }

        // validate num of pieces on row
        {
            int numOfPiece = 0;
            for (char c : aRow) {
                if (isdigit(c))
                    numOfPiece += c - '0';
                else if (c != '/' && c != ' ')
                    numOfPiece++;
            }
            if (numOfPiece != 8)
                throw QString("Num of pieces mismatched: On row ") + QString::number(row) + " (rank " + QString::number(8 - row) + ") which has " + QString::number(numOfPiece) + " pieces.";
        }


        int col = 0;
        char c;
        std::istringstream iss(aRow);  // hide the upper iss
        while (iss.get(c), iss.good()) {
            // 1 ~ 8
            if (isdigit(c)) {
                do {
                    pieces[row][col]->setEmpty();
                    ++col;
                    --c;
                } while (c != '0');
            }
            // 一列的結束
            else if (c == '/' || c == ' ') {
                break;
            }
            // 棋子
            else {
                COLOR color = isupper(c) ? COLOR::WHITE : COLOR::BLACK;
                c = tolower(c);
                TYPE type = (c == 'r' ? TYPE::ROOK :
                             c == 'n' ? TYPE::KNIGHT :
                             c == 'b' ? TYPE::BISHOP :
                             c == 'q' ? TYPE::QUEEN :
                             c == 'k' ? TYPE::KING :
                                        TYPE::PAWN);
                if (color == COLOR::WHITE && type == TYPE::KING) ++numOfWhiteKing;
                if (color == COLOR::BLACK && type == TYPE::KING) ++numOfBlackKing;
                pieces[row][col]->setType(type, color);
                ++col;
            }
        }
    }
    if (numOfWhiteKing != 1)
        throw QString("Num of white king mismatched: need 1 but got ") + QString::number(numOfWhiteKing);
    if (numOfBlackKing != 1)
        throw QString("Num of black king mismatched: need 1 but got ") + QString::number(numOfBlackKing);

    // team
    {
        char team;
        iss >> team;
        moveTeam = team == 'w' ? COLOR::WHITE : COLOR::BLACK;
    }

    // castling
    {
        castlingFlag = 0;
        char input;
        iss >> std::ws; // clear spaces
        while (iss.get(input), input != ' ') {
            castlingFlag |= (input == 'K' ? (int)CASTLING::WHITE_K :
                             input == 'Q' ? (int)CASTLING::WHITE_Q :
                             input == 'k' ? (int)CASTLING::BLACK_k :
                             input == 'q' ? (int)CASTLING::BLACK_q :
                                            0b0000);
        }
    }

    // EnPassant
    {
        char file, rank;
        iss >> file;
        if (file == '-') {
            enPassant = Position{-1, -1};
        }
        else {
            iss >> rank;
            enPassant = posFromAlgebraic(std::string{file, rank});
        }
    }

    // halfmove and fullmove
    iss >> halfmove >> fullmove;

    qDebug() << castlingFlag << enPassant.row << enPassant.col << halfmove << fullmove;
}
