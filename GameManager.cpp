
#include "GameManager.h"
#include "ChessBoard.h"
#include "promotiondialog.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <QDebug>

// 1. 若走出入堡，移動城堡。 2. 依移動更新castlingFlag
void GameManager::updateForCastling(ChessPieces *board[8][8], int &castlingFlag, Position pre, Position post)
{
    // 移國王，檢查入堡
    if (board[post.row][post.col]->getType() == TYPE::KING) {
        // 清掉國王方的入堡旗標
        castlingFlag &= ~(board[post.row][post.col]->isWhite() ? (int)CASTLING::WHITE_K | (int)CASTLING::WHITE_Q
                                                               : (int)CASTLING::BLACK_k | (int)CASTLING::BLACK_q);
        qDebug() << QString::number(castlingFlag, 2);

        Position whiteKingInit = {7, 4};
        Position blackKingInit = {0, 4};

        // white kingside
        if (pre == whiteKingInit && post == Position{7, 6}) {
            board[7][5]->setType(TYPE::ROOK, COLOR::WHITE);
            board[7][7]->setEmpty();
        }
        // white queenside
        else if (pre == whiteKingInit && post == Position{7, 2}) {
            board[7][3]->setType(TYPE::ROOK, COLOR::WHITE);
            board[7][0]->setEmpty();
        }
        // black kingside
        else if (pre == blackKingInit && post == Position{0, 6}) {
            board[0][5]->setType(TYPE::ROOK, COLOR::BLACK);
            board[0][7]->setEmpty();
        }
        // black queenside
        else if (pre == blackKingInit && post == Position{0, 2}) {
            board[0][3]->setType(TYPE::ROOK, COLOR::BLACK);
            board[0][0]->setEmpty();
        }
    }
    // 移城堡，清flag
    else if (board[post.row][post.col]->getType() == TYPE::ROOK) {
        if (pre == Position{0, 0})
            castlingFlag &= ~(int)CASTLING::BLACK_q;
        else if (pre == Position{0, 7})
            castlingFlag &= ~(int)CASTLING::BLACK_k;
        else if (pre == Position{7, 0})
            castlingFlag &= ~(int)CASTLING::WHITE_Q;
        else if (pre == Position{7, 7})
            castlingFlag &= ~(int)CASTLING::WHITE_K;
        qDebug() << QString::number(castlingFlag, 2);
    }
}

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

QString GameManager::toFEN(const ChessPieces* pieces[8][8], COLOR moveTeam, int castlingFlag, Position enPassant, int halfmove, int fullmove)
{
    std::ostringstream oss;

    // chess placement
    for (int row = 0; row < 8; ++row) {
        int col = 0;
        do {
            if (pieces[row][col]->getType() == TYPE::EMPTY) {
                int blankCount = 0;
                do {
                    ++blankCount;
                    ++col;
                } while (col < 8 && pieces[row][col]->getType() == TYPE::EMPTY);
                oss << blankCount;
            }
            else {
                TYPE type = pieces[row][col]->getType();
                char icon = type == TYPE::BISHOP ? 'b' :
                            type == TYPE::KING ?   'k' :
                            type == TYPE::KNIGHT ? 'n' :
                            type == TYPE::PAWN ?   'p' :
                            type == TYPE::QUEEN ?  'q' :
                                                   'r' ;
                oss << (pieces[row][col]->isWhite() ? (char)toupper(icon) : (char)icon);
                ++col;
            }
        } while (col < 8);
        oss << (row == 7 ? ' ' : '/');
    }

    // white or black
    oss << (moveTeam == COLOR::WHITE ? 'w' : 'b') << ' ';

    // castling
    bool hasoutput = false;
    if (castlingFlag & (int)CASTLING::WHITE_K) oss << 'K', hasoutput = true;
    if (castlingFlag & (int)CASTLING::WHITE_Q) oss << 'Q', hasoutput = true;
    if (castlingFlag & (int)CASTLING::BLACK_k) oss << 'k', hasoutput = true;
    if (castlingFlag & (int)CASTLING::BLACK_q) oss << 'q', hasoutput = true;
    if (!hasoutput) oss << '-';
    oss << ' ';

    // enPassant
    if (enPassant == Position{-1, -1})
        oss << "- ";
    else {
        oss << (char)('a' + enPassant.col) << (8 - enPassant.row) << ' ';
    }

    // halfmove fullmove
    oss << halfmove << ' ' << fullmove;

    return QString(oss.str().c_str());
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

void GameManager::drawTerritoryAndUpdateState(ChessBoard *board)
{
    // 清空enemyTerritory和numOfChecking
    board->numOfChecking = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board->enemyTerritory[r][c] = { false, false };
        }
    }

    // 計算enemyTerritory和numOfChecking
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (!board->isTurn({r, c}))
                drawTerritory(board, {r, c});
        }
    }
    qDebug() << "";

    // print enemyTerritory (for test)
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            std::cout << (board->enemyTerritory[r][c].isDonimated ? 'D' : '_')
                      << (board->enemyTerritory[r][c].isChecking ? 'C' : '_')
                      << ' ';
        }
        std::cout << '\n';
    }
    std::cout << board->numOfChecking << std::endl;

    // 計算currentTeam可走的棋步
    bool hasMove = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 我方 非空格
            if (board->isTurn({i, j}) && !board->isEmpty(i, j)) {
                if (board->getCanEat({i, j}).empty() && board->getCanMove({i, j}).empty())
                    continue;
                hasMove = true;
                break;
            }
        }
    }

    // 更新state
    if (hasMove)
        board->gameState = GameManager::State::PLAYING;
    else {
        if (board->numOfChecking == 0)
            board->gameState = GameManager::State::DRAW;
        else {
            if (board->currentTeam == COLOR::WHITE)
                board->gameState = GameManager::State::BLACK_WIN;
            else
                board->gameState = GameManager::State::WHITE_WIN;
        }
    }
}

void GameManager::drawTerritory(ChessBoard* board, Position pos) {
    switch (board->chessPieces[pos.row][pos.col]->getType()) {
    case TYPE::PAWN:
        drawTerritoryPawn(board, pos);
        return;
    case TYPE::KNIGHT:
        drawTerritoryKnight(board, pos);
        return;
    case TYPE::KING:
        drawTerritoryKing(board, pos);
        return;
    case TYPE::QUEEN:
        drawTerritoryQueen(board, pos);
        return;
    case TYPE::ROOK:
        drawTerritoryRook(board, pos);
        return;
    case TYPE::BISHOP:
        drawTerritoryBishop(board, pos);
        return;
    case TYPE::EMPTY:
        return;
    }
}

void GameManager::drawHelperWithoutPinned(ChessBoard* board, Position pos, Position from) {
    if (board->posIsOk(pos)) {
        // 空格或（被保護的）敵方棋子（相對於currentTeam）
        if (board->isEmpty(pos) || !board->isTurn(pos)) {
            board->enemyTerritory[pos.row][pos.col].isDonimated = true;
        }
        // 我方國王（相對於currentTeam）
        else if (board->chessPieces[pos.row][pos.col]->getType() == TYPE::KING) {
            board->enemyTerritory[pos.row][pos.col].isDonimated = true;
            board->enemyTerritory[from.row][from.col].isChecking = true;
            board->numOfChecking++;

        }
    }
}

void GameManager::drawTerritoryPawn(ChessBoard* board, Position pos) {
    qDebug() << "Enemy Pawn at " << pos.row << pos.col;
    int deltaR = board->isWhite(pos) ? -1 : 1;

    for (int deltaC = -1; deltaC <= 1; deltaC +=2) {
        Position newPos = {pos.row + deltaR, pos.col + deltaC};
        drawHelperWithoutPinned(board, newPos, pos);
    }
}

void GameManager::drawTerritoryKnight(ChessBoard* board, Position pos) {
    qDebug() << "Enemy Knight at " << pos.row << pos.col;
    Position delta[8] = {{-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}};

    for (int i = 0; i < 8; ++i) {
        Position newPos = pos + delta[i];
        drawHelperWithoutPinned(board, newPos, pos);
    }
}

void GameManager::drawTerritoryKing(ChessBoard* board, Position pos) {
    qDebug() << "Enemy King at " << pos.row << pos.col;

    for (int deltaR = -1; deltaR <= 1; ++deltaR) {
        for (int deltaC = -1; deltaC <= 1; ++deltaC) {
            if (deltaR == 0 && deltaC == 0) continue;

            Position newPos = {pos.row + deltaR, pos.col + deltaC};
            drawHelperWithoutPinned(board, newPos, pos);
        }
    }
}

void GameManager::drawHelperWithPinned(ChessBoard* board, Position from, Position delta) {
    Position newPos = from + delta;

    while (board->posIsOk(newPos)) {
        // 空格
        if (board->isEmpty(newPos)) {
            board->enemyTerritory[newPos.row][newPos.col].isDonimated = true;
            newPos += delta;
        }
        // 被保護的敵方棋子
        else if (!board->isTurn(newPos)) {
            board->enemyTerritory[newPos.row][newPos.col].isDonimated = true;
            return;
        }
        // 我方國王
        else if (board->chessPieces[newPos.row][newPos.col]->getType() == TYPE::KING) {
            board->numOfChecking++;
            // 將from和newPos連線上每點標記為isChecking（含from、不含newPos）
            for (Position p = from; !(p == newPos); p += delta) {
                board->enemyTerritory[p.row][p.col].isChecking = true;
            }
            board->enemyTerritory[newPos.row][newPos.col].isDonimated = true;
            // 國王後方那格
            newPos += delta;
            if (board->posIsOk(newPos))
                board->enemyTerritory[newPos.row][newPos.col].isDonimated = true;

            return;
        }
        // 我方的其他棋子（檢查是否被pinned）
        else {
            bool isPinned = false;
            // 從newPos向後找，若碰到「第一個非空的格子」為我方國王，則isPinned = true;
            for (Position p = newPos + delta; board->posIsOk(p); p += delta) {
                // 空的
                if (board->isEmpty(p))
                    continue;
                // 敵方
                else if (!board->isTurn(p))
                    break;
                // 不是我方國王
                else if (board->chessPieces[p.row][p.col]->getType() != TYPE::KING)
                    break;
                // 否則是我方國王
                isPinned = true;
                break;
            }
            if (isPinned) board->enemyTerritory[newPos.row][newPos.col].isDonimated = true;
            return;
        }
    }
}

void GameManager::drawTerritoryQueen(ChessBoard* board, Position pos) {
    qDebug() << "Enemy Queen at " << pos.row << pos.col;
    drawTerritoryBishop(board, pos);
    drawTerritoryRook(board, pos);
}

void GameManager::drawTerritoryRook(ChessBoard* board, Position pos) {
    qDebug() << "Enemy Rook at " << pos.row << pos.col;
    drawHelperWithPinned(board, pos, {1, 0});
    drawHelperWithPinned(board, pos, {0, 1});
    drawHelperWithPinned(board, pos, {-1, 0});
    drawHelperWithPinned(board, pos, {0, -1});
}

void GameManager::drawTerritoryBishop(ChessBoard* board, Position pos) {
    qDebug() << "Enemy Bishop at " << pos.row << pos.col;
    drawHelperWithPinned(board, pos, {1, 1});
    drawHelperWithPinned(board, pos, {1, -1});
    drawHelperWithPinned(board, pos, {-1, 1});
    drawHelperWithPinned(board, pos, {-1, -1});
}
