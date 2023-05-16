#include "ChessBoard.h"
#include <QLayout>
#include <QDebug>
#include <common.h>
#include <algorithm>

#define BACKGROUND ":/pieces/chessPieces/chess/150.png"

using namespace std;

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true), currentTeam(COLOR::WHITE)
{
	//GUI

    this->setMinimumSize(INIT_PIECE_SIZE * 8, INIT_PIECE_SIZE * 8);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // set wedget background
    QPixmap backGround(BACKGROUND);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(backGround.scaled(this->size(), Qt::KeepAspectRatio)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

	QGridLayout* layout = new QGridLayout;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
            chessPieces[row][col] = new ChessPieces(row, col, this);
			
			layout->addWidget(chessPieces[row][col], row, col);

            connect(chessPieces[row][col], &ChessPieces::clicked, this, &ChessBoard::chessPiecesClicked);
		}
	}

    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
	this->setLayout(layout);
}

std::vector<Position> ChessBoard::getCanMove(Position pos) const {
	int row = pos.row;
	int col = pos.col;

	if (chessPieces[row][col] == nullptr) {
		return std::vector<Position>();
	}

	switch (chessPieces[row][col]->getType()) {
	case TYPE::PAWN:
		return getPawnCanMove(pos);
	case TYPE::ROOK:
		return getRookCanMove(pos);
	case TYPE::KNIGHT:
		return getKnightCanMove(pos);
	case TYPE::BISHOP:
		return getBishopCanMove(pos);
	case TYPE::QUEEN:
		return getQueenCanMove(pos);
	case TYPE::KING:
		return getKingCanMove(pos);
	}

	return std::vector<Position>();
}

std::vector<Position> ChessBoard::getPawnCanMove(Position pos) const {
	std::vector<Position> canMove;

	int deltaRow = isWhite(pos) ? -1 : 1;

	if (posIsOk(pos.row + deltaRow, pos.col) && isEmpty(pos.row + deltaRow, pos.col)) {
		canMove.push_back(Position{pos.row + deltaRow, pos.col});
	}
    if (pawnIsFirstMove(pos) && posIsOk(pos.row + deltaRow * 2, pos.col) && isEmpty(pos.row + deltaRow * 2, pos.col)) {
		canMove.push_back(Position{ pos.row + deltaRow * 2, pos.col });
	}

	return canMove;
}

std::vector<Position> ChessBoard::getRookCanMove(Position pos) const {
	vector<Position> canMove;
	bool notLookChess[4] = { true, true, true, true };

	for (int i = 1; i < 8; i++) {
		Position newPos[4] = {
			Position{ pos.row, pos.col - i },
			Position{ pos.row, pos.col + i },
			Position{ pos.row - i, pos.col },
			Position{ pos.row + i, pos.col }
		};
		
		for (int i = 0; i < 4; i++) {
			if (notLookChess[i] && posIsOk(newPos[i])) {
				if (isEmpty(newPos[i])) {
					canMove.push_back(newPos[i]);
				}
				else {
					notLookChess[i] = false;
				}
			}
		}

	}

	return canMove;
}

std::vector<Position> ChessBoard::getKnightCanMove(Position pos) const {
	std::vector<Position> canMove;

	int deltaRow[8] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int deltaCol[8] = { -1, 1, -2, 2, -2, 2, -1, 1 };

	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };

		if (posIsOk(newPos) && isEmpty(newPos)) {
			canMove.push_back(newPos);
		}
	}

	return canMove;
}

std::vector<Position> ChessBoard::getBishopCanMove(Position pos) const {
	std::vector<Position> canMove;
	bool notLookChess[4] = { true, true, true, true };

	for (int i = 1; i < 8; i++) {
		Position newPos[4] = {
			Position{ pos.row - i, pos.col - i },
			Position{ pos.row - i, pos.col + i },
			Position{ pos.row + i, pos.col - i },
			Position{ pos.row + i, pos.col + i }
		};
		for (int i = 0; i < 4; i++) {
			if (notLookChess[i] && posIsOk(newPos[i])) {
				if (isEmpty(newPos[i])) {
					canMove.push_back(newPos[i]);
				}
				else {
					notLookChess[i] = false;
				}
			}
		}
	}
	return canMove;
}

std::vector<Position> ChessBoard::getQueenCanMove(Position pos) const {
	vector<Position> canMove;
	vector<Position> canMoveRook = getRookCanMove(pos);
	vector<Position> canMoveBishop = getBishopCanMove(pos);

	canMove.insert(canMove.end(), canMoveRook.begin(), canMoveRook.end());
	canMove.insert(canMove.end(), canMoveBishop.begin(), canMoveBishop.end());

	return canMove;
}

std::vector<Position> ChessBoard::getKingCanMove(Position pos) const {
	std::vector<Position> canMove;
	int deltaRow[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int deltaCol[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
		if (posIsOk(newPos) && isEmpty(newPos)) {
			canMove.push_back(newPos);
		}
	}

    //! 入堡，只為行動方考慮入堡
    auto allEmptyBetween = [this](Position p1, const Position& p2)->bool {
        const Position delta = { p2.row - p1.row, p2.col - p1.col };
        while (true) {
            p1.row += delta.row; p1.col += delta.col;
            if (p1 == p2) return true;
            if (!isEmpty(p1)) break;
            //! TODO: 檢查是否會被check
        }
        return false;
    };
    auto isKing = [this](int row, int col)->bool {
        return chessPieces[row][col]->getType() == TYPE::KING && isTurn({row, col});
    };
    auto isRook = [this](int row, int col)->bool {
        return chessPieces[row][col]->getType() == TYPE::ROOK && isTurn({row, col});
    };
    if (isWhite(pos)) {
        // white kingside
        if (castlingFlag & (int)CASTLING::WHITE_K && isKing(7, 4) && isRook(7, 7) && allEmptyBetween({7, 4}, {7, 7})) {
            canMove.push_back({7, 6});
        }
        // white queenside
        if (castlingFlag & (int)CASTLING::WHITE_Q && isKing(7, 4) && isRook(7, 0) && allEmptyBetween({7, 4}, {7, 0})) {
            canMove.push_back({7, 2});
        }
    }
    else {
        // black kingside
        if (castlingFlag & (int)CASTLING::BLACK_k && isKing(0, 4) && isRook(0, 7) && allEmptyBetween({0, 4}, {0, 7})) {
            canMove.push_back({0, 6});
        }
        // black queenside
        if (castlingFlag & (int)CASTLING::BLACK_q && isKing(0, 4) && isRook(0, 0) && allEmptyBetween({0, 4}, {0, 0})) {
            canMove.push_back({0, 2});
        }
    }

	return canMove;
}

std::vector<Position> ChessBoard::getCanEat(Position pos) const {
	switch (chessPieces[pos.row][pos.col]->getType()) {
	case TYPE::PAWN:
		return getPawnCanEat(pos);
	case TYPE::ROOK:
		return getRookCanEat(pos);
	case TYPE::KNIGHT:
		return getKnightCanEat(pos);
	case TYPE::BISHOP:
		return getBishopCanEat(pos);
	case TYPE::QUEEN:
		return getQueenCanEat(pos);
	case TYPE::KING:
		return getKingCanEat(pos);
	default:
		return std::vector<Position>();
	}
}

std::vector<Position> ChessBoard::getPawnCanEat(Position pos) const {
	vector<Position> canEat;
	int deltaRow = isWhite(pos) ? -1 : 1;

	Position p1 = Position{ pos.row + deltaRow, pos.col + 1 };
	Position p2 = Position{ pos.row + deltaRow, pos.col - 1 };

    if ((posIsOk(p1) && !isEmpty(p1) && isEnemy(pos, p1)) || p1 == enPassant) {
		canEat.push_back(p1);
    }
    if ((posIsOk(p2) && !isEmpty(p2) && isEnemy(pos, p2)) || p2 == enPassant) {
		canEat.push_back(p2);
	}

	return canEat;
}

std::vector<Position> ChessBoard::getRookCanEat(Position pos) const {
	vector<Position> canEat;

	for (int i = pos.row - 1; i >= 0; i--) {
		Position newPos = Position{ i, pos.col };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
	for (int i = pos.row + 1; i < 8; i++) {
		Position newPos = Position{ i, pos.col };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
	for (int i = pos.col - 1; i >= 0; i--) {
		Position newPos = Position{ pos.row, i };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
	for (int i = pos.col + 1; i < 8; i++) {
		Position newPos = Position{ pos.row, i };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}

	return canEat;
}

std::vector<Position> ChessBoard::getKnightCanEat(Position pos) const {
	vector<Position> canEat;
	int deltaRow[8] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int deltaCol[8] = { -1, 1, -2, 2, -2, 2, -1, 1 };

	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
		if (posIsOk(newPos) && !isEmpty(newPos) && isEnemy(pos, newPos)) {
			canEat.push_back(newPos);
		}
	}
	return canEat;
}

std::vector<Position> ChessBoard::getBishopCanEat(Position pos) const {
	vector<Position> canEat;
	bool notLookChess[4] = { true, true, true, true };

	for (int i = 1; i < 8; i++) {
		Position newPos[4] = {
			Position{ pos.row - i, pos.col - i },
			Position{ pos.row - i, pos.col + i },
			Position{ pos.row + i, pos.col - i },
			Position{ pos.row + i, pos.col + i }
		};

		for (int i = 0; i < 4; i++) {
			if (notLookChess[i] && posIsOk(newPos[i])) {
				if (!isEmpty(newPos[i]) && isEnemy(pos, newPos[i])) {
					canEat.push_back(newPos[i]);
					notLookChess[i] = false;
				}
				else if (!isEmpty(newPos[i])) {
					notLookChess[i] = false;
				}
				else if (!isEmpty(newPos[i])) {
					notLookChess[i] = false;
				}
			}
		}
	}
	return canEat;
}

std::vector<Position> ChessBoard::getQueenCanEat(Position pos) const {
	vector<Position> canEat;
	vector<Position> canEatRook = getRookCanEat(pos);
	vector<Position> canEatBishop = getBishopCanEat(pos);

	canEat.insert(canEat.end(), canEatRook.begin(), canEatRook.end());
	canEat.insert(canEat.end(), canEatBishop.begin(), canEatBishop.end());
	return canEat;
}

std::vector<Position> ChessBoard::getKingCanEat(Position pos) const {
	vector<Position> canEat;
	int deltaRow[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int deltaCol[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
		if (posIsOk(newPos) && !isEmpty(newPos) && isEnemy(pos, newPos)) {
			canEat.push_back(newPos);
		}
	}
	return canEat;
}

void ChessBoard::chessPiecesClicked(Position pos) {
    // 檢查吃過路兵
    if (!firstClick && chessPieces[beforeClickPos.row][beforeClickPos.col]->getType() == TYPE::PAWN && pos == enPassant) {
        std::vector<Position> canEat = getCanEat(beforeClickPos);
        // 玩家可以吃過路兵
        if (std::find(canEat.begin(), canEat.end(), enPassant) != canEat.end()) {
            emphasizeClear(beforeClickPos);

            move(beforeClickPos, enPassant);
            chessPieces[beforeClickPos.row][enPassant.col]->setEmpty();
            enPassant = {-1, -1};
            changeTurn();
            firstClick = true;
            return;
        }
    }

	// 點空格
	if (firstClick && isEmpty(pos)) {
		return; // do nothing
	}
	// 點棋子
	else if (firstClick && !isEmpty(pos) && isTurn(pos)) {
		beforeClickPos = pos;

		emphasizeCan(pos);
		firstClick = false;
	}
	// 點棋子 -> 點空格
	else if (!firstClick && isEmpty(pos)) {
		vector<Position> canGo = getCanMove(beforeClickPos);

		emphasizeClear(beforeClickPos);

		if (std::find(canGo.begin(), canGo.end(), pos) != canGo.end()) {
			move(beforeClickPos, pos);
            GameManager::updateForCastling(chessPieces, castlingFlag, beforeClickPos, pos);
            GameManager::checkForPromotion(chessPieces[pos.row][pos.col]);
            // update enPassant
            if (chessPieces[pos.row][pos.col]->getType() == TYPE::PAWN && std::abs(pos.row - beforeClickPos.row) == 2)
                enPassant = {(currentTeam == COLOR::WHITE ? pos.row + 1 : pos.row - 1), pos.col};
            else
                enPassant = {-1, -1};

			changeTurn();
		}

		firstClick = true;

	}
	// 點棋子 -> 點敵軍棋子
	else if (!firstClick && !isEmpty(pos) && isEnemy(pos, beforeClickPos)) {
		vector<Position> canEat = getCanEat(beforeClickPos);

		emphasizeClear(beforeClickPos);
	
		if (std::find(canEat.begin(), canEat.end(), pos) != canEat.end()) {
            // 若城堡被吃掉，更新castlingFlag
            if (chessPieces[pos.row][pos.col]->getType() == TYPE::ROOK)
                GameManager::updateForCastling(chessPieces, castlingFlag, pos, pos);
			eat(beforeClickPos, pos);
            GameManager::updateForCastling(chessPieces, castlingFlag, beforeClickPos, pos);
            GameManager::checkForPromotion(chessPieces[pos.row][pos.col]);
            enPassant = {-1, -1};
			changeTurn();
		}
		firstClick = true;
	}
	// 點棋子 -> 點己方棋子
	else if (!firstClick && !isEmpty(pos) && !isEnemy(pos, beforeClickPos)) {
		emphasizeClear(beforeClickPos);
		emphasizeCan(pos);
		beforeClickPos = pos;
		firstClick = false;
	}
	else {
		return;
	}


}

void ChessBoard::move(Position from, Position to) {
    qDebug() << "Move Action from" << "(" << from.row << ", " << from.col << ") : " << "(" << to.row << ", " << to.col << ")" << "\n";

	// 交換旗子資訊
	swap(*chessPieces[from.row][from.col], *chessPieces[to.row][to.col]);
}

void ChessBoard::eat(Position eater, Position beEat) {
	qDebug() << "Eat Action from" << "(" << eater.row << ", " << eater.col << ") : " << "(" << beEat.row << ", " << beEat.col << ")" << "\n";

	// 交換旗子資訊
	chessPieces[beEat.row][beEat.col]->setEmpty();
	chessPieces[beEat.row][beEat.col]->setPixmap(QPixmap());
	swap(*chessPieces[beEat.row][beEat.col], *chessPieces[eater.row][eater.col]);
}

void ChessBoard::emphasizeCan(Position pos) {
	vector<Position> canGo = getCanMove(pos);
	vector<Position> canEat = getCanEat(pos);

	for (auto i : canGo) {
		chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
	}
	for (auto i : canEat) {
		chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(0, 255 , 0, 50);");
	}
}

void ChessBoard::emphasizeClear(Position pos) {
	vector<Position> canGo = getCanMove(pos);
	vector<Position> canEat = getCanEat(pos);

	for (auto i : canGo) {
		chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	}
	for (auto i : canEat) {
		chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	}
}

void ChessBoard::changeTurn() {
	if (currentTeam == COLOR::BLACK) {
		currentTeam = COLOR::WHITE;
        fullmove++;
	}
	else {
		currentTeam = COLOR::BLACK;
	}

	emit changedTurnSignal(currentTeam);
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    int boardH = event->size().height();
    int pieceH = boardH / 8;

    // 讓寬 = 高
    this->setFixedWidth(boardH);

    // 高不變
    if (boardH == event->oldSize().height()) {
        return;
    }

    // 改背景大小
    QPalette p = this->palette();
    p.setBrush(this->backgroundRole(), QPixmap(BACKGROUND).scaled(boardH, boardH));
    this->setPalette(p);

    // 改每個棋子的大小
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            ChessPieces* thePiece = chessPieces[r][c];

            thePiece->setFixedSize(pieceH, pieceH);
            if (thePiece->getImgAddr() != "") {
                thePiece->setPixmap(QPixmap(thePiece->getImgAddr()).scaled(pieceH, pieceH));
            }
        }
    }
}


