
#include "GameManager.h"
#include <sstream>
#include <cstring>
#include <QDebug>

void GameManager::load(QString FEN, ChessPieces *pieces[8][8], COLOR &moveTeam, int &castlingFlag, Position &enPassant, int &halfmove, int &fullmove)
{
    std::istringstream iss(FEN.toStdString());

    // pieces placement
    for (int row = 0; row < 8; ++row) {
        int col = 0;
        char c;
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
            else if (c == '/') {
                if (col != 8)
                    throw QString("Num of pieces mismatched: On row ") + QString::number(row + 1) + " which has " + QString::number(col) + " pieces.";
                break;
            }
            // 棋子配置結束
            else if (c == ' ') {
                if (col != 8)
                    throw QString("Num of pieces mismatched: On row ") + QString::number(row + 1) + " which has " + QString::number(col) + " pieces.";
                if (row != 7)
                    throw QString("Num of rows mismatched: need 8 rows but got ") + QString::number(row + 1) + " rows.";
                break;
            }
            else {
                COLOR color = isupper(c) ? COLOR::WHITE : COLOR::BLACK;
                c = tolower(c);
                TYPE type = (c == 'r' ? TYPE::ROOK :
                             c == 'n' ? TYPE::KNIGHT :
                             c == 'b' ? TYPE::BISHOP :
                             c == 'q' ? TYPE::QUEEN :
                             c == 'k' ? TYPE::KING :
                                        TYPE::PAWN);
                pieces[row][col]->setType(type, color);
                ++col;
            }
        }
    }

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
