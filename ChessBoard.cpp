#include "ChessBoard.h"
#include <QLayout>
#include <QDebug>
#include <common.h>
#include <algorithm>

using namespace std;

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true)
{
	//TODO: initialize chessPieces
	//GUI

    this->setFixedSize(800, 800);

    // set wedget background
    QPixmap backGround(":/pieces/chessPieces/chess/150.png");
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
	std::vector<Position> canMove;
	bool notLookChess[8] = { true, true, true, true, true, true, true, true };

	for (int i = 1; i < 8; i++) {
		Position newPos[8] = {
			Position{ pos.row, pos.col - i },
			Position{ pos.row, pos.col + i },
			Position{ pos.row - i, pos.col },
			Position{ pos.row + i, pos.col },
			Position{ pos.row - i, pos.col - i },
			Position{ pos.row - i, pos.col + i },
			Position{ pos.row + i, pos.col - i },
			Position{ pos.row + i, pos.col + i }
		};
		for (int i = 0; i < 8; i++) {
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
	return canMove;
}

void ChessBoard::chessPiecesClicked(Position pos) {
    // if click on empty
    if (isEmpty(pos)) {

		if (firstClick) {
			return; // do nothing
		}
		else {
            vector<Position> canGo = getCanMove(beforeClickPos);

            for (auto i : canGo) {
                chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
            }

            if (std::find(canGo.begin(), canGo.end(), pos) != canGo.end()) {
                move(beforeClickPos, pos);
                emit changedTurn();
            }
		}
	}
	else {
		if (firstClick) {
            vector<Position> canGo = getCanMove(pos);

            for (auto i : canGo) {
                chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
            }

            beforeClickPos = pos;
		}
		else {
            vector<Position> canGo = getCanMove(beforeClickPos);

            for (auto i : canGo) {
                chessPieces[i.row][i.col]->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
            }
			// eat chessPieces
			// if eat success
				// change turn
			// else
			// do nothing
		}
	}

	firstClick = !firstClick;
}

// have bug
void ChessBoard::move(Position from, Position to) {
    qDebug() << "Move Action from" << "(" << from.row << ", " << from.col << ") : " << "(" << to.row << ", " << to.col << ")" << "\n";

    // 將from和to上的棋子交換
    chessPieces[from.row][from.col]->swapChessInfo(*chessPieces[to.row][to.col]);
}


