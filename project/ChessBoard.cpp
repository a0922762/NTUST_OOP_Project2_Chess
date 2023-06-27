/*****************************************************************//**
 * File: ChessBoard.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: ChessBoard class implementation 
 *********************************************************************/
#include "ChessBoard.h"
#include <QLayout>
#include <QDebug>
#include <common.h>
#include <algorithm>


#define BACKGROUND ":/pieces/chessPieces/chess/150.png" // background image

using namespace std;

// Intent: defalut constructor
// Pre: none
// Post: make a chess board
ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true), currentTeam(COLOR::WHITE)
{

    // set size
    this->setMinimumSize(INIT_PIECE_SIZE * 8, INIT_PIECE_SIZE * 8);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // set wedget background
    QPixmap backGround(BACKGROUND);
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(backGround.scaled(this->size(), Qt::KeepAspectRatio)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    // set chess pieces layout
	QGridLayout* layout = new QGridLayout;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
            chessPieces[row][col] = new ChessPieces(row, col, this); // create chess pieces object
			
			layout->addWidget(chessPieces[row][col], row, col);

            connect(chessPieces[row][col], &ChessPieces::clicked, this, &ChessBoard::chessPiecesClicked);
		}
	}
    
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
	this->setLayout(layout);
}

// Intent : get the king position
// Pre : none
// Post : return the king position
Position ChessBoard::getOurKing() const
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            // find out king
            if (isTurn({ r, c }) && chessPieces[r][c]->getType() == TYPE::KING) {
                return { r, c };
            }
        }
    }
    return {-1, -1};
}

// Intent: get the pieces can move position
// Pre: none
// Post: return the pieces can move position
std::vector<Position> ChessBoard::getCanMove(Position pos) const {
	int row = pos.row;
	int col = pos.col;
    std::vector<Position> canGo;
    TYPE chessTYPE = chessPieces[row][col]->getType();

    // if game is not playing, return empty vector
    if (gameState != GameManager::State::PLAYING) {
        return canGo;
	}

    // 被multiple check，則只有國王可動
    if (numOfChecking >= 2 && chessTYPE != TYPE::KING) {
        return canGo;
    }

    // 被pin住的主教、騎士 不能走
    if (isDonimated(pos) && (chessTYPE == TYPE::BISHOP || chessTYPE == TYPE::KNIGHT)) {
        return canGo;
    }

    switch (chessTYPE) {
	case TYPE::PAWN:
        canGo = getPawnCanMove(pos);
        break;
	case TYPE::ROOK:
        canGo = getRookCanMove(pos);
        break;
	case TYPE::KNIGHT:
        canGo = getKnightCanMove(pos);
        break;
	case TYPE::BISHOP:
        canGo = getBishopCanMove(pos);
        break;
	case TYPE::QUEEN:
        canGo = getQueenCanMove(pos);
        break;
	case TYPE::KING:
        // 被將軍時，國王走法的處理放在函式裡
        return getKingCanMove(pos);
    default:
        return canGo;
	}

    // 國王被將軍
    if (numOfChecking == 1) {
        // 必需要能擋在城堡、皇后、主教中間（吃掉幽靈攻擊者）
        for (auto& p : canGo) {
            if (!isGhostAttacker(p))
                p = {-1, -1};
        }
        canGo.erase(std::remove(canGo.begin(), canGo.end(), Position{-1, -1}), canGo.end());
    }
    // 被pin住，將所有不在和國王同列或同欄的移動清掉
    if (isDonimated(pos)){
        Position kingPos = getOurKing();
        // 在同一列
        if (kingPos.row == pos.row) {
            // 將所有移到不同列的走法清掉
            canGo.erase(
                std::remove_if(canGo.begin(), canGo.end(), [r = kingPos.row](const Position& p) { return p.row != r; })
                , canGo.end());
        }
        // 在同一欄
        else {
            // 將所有移到不同欄的走法清掉
            canGo.erase(
                std::remove_if(canGo.begin(), canGo.end(), [c = kingPos.col](const Position& p) { return p.col != c; })
                , canGo.end());
        }
    }

    return canGo;
}

// Intent: get the pawn pieces can move position
// Pre: none
// Post: return the pawn pieces can move position
std::vector<Position> ChessBoard::getPawnCanMove(Position pos) const {
	std::vector<Position> canMove;

    // change direction by color
	int deltaRow = isWhite(pos) ? -1 : 1;

    // check if can move forward one step
	if (posIsOk(pos.row + deltaRow, pos.col) && isEmpty(pos.row + deltaRow, pos.col)) {
		canMove.push_back(Position{pos.row + deltaRow, pos.col});
	}
    // check if can move forward two step
    if (pawnIsFirstMove(pos) && posIsOk(pos.row + deltaRow * 2, pos.col) && isEmpty(pos.row + deltaRow * 2, pos.col)) {
		canMove.push_back(Position{ pos.row + deltaRow * 2, pos.col });
	}

	return canMove;
}

// Intent: get the rook pieces can move position
// Pre: none
// Post: return the rook pieces can move position
std::vector<Position> ChessBoard::getRookCanMove(Position pos) const {
	vector<Position> canMove;
	bool notLookChess[4] = { true, true, true, true }; // left, right, up, down

	for (int i = 1; i < 8; i++) {
		Position newPos[4] = {
			Position{ pos.row, pos.col - i },
			Position{ pos.row, pos.col + i },
			Position{ pos.row - i, pos.col },
			Position{ pos.row + i, pos.col }
		};
		
        // check if can move
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

// Intent: get the knight pieces can move position
// Pre: none
// Post: return the knight pieces can move position
std::vector<Position> ChessBoard::getKnightCanMove(Position pos) const {
	std::vector<Position> canMove;

	int deltaRow[8] = { -2, -2, -1, -1, 1, 1, 2, 2 }; // up, down
	int deltaCol[8] = { -1, 1, -2, 2, -2, 2, -1, 1 }; // left, right

    // check if can move
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };

		if (posIsOk(newPos) && isEmpty(newPos)) {
			canMove.push_back(newPos);
		}
	}

	return canMove;
}

// Intent: get the bishop pieces can move position
// Pre: none
// Post: return the bishop pieces can move position
std::vector<Position> ChessBoard::getBishopCanMove(Position pos) const {
	std::vector<Position> canMove;
	bool notLookChess[4] = { true, true, true, true }; // left up, right up, left down, right down

    // check if can move
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

// Intent: get the queen pieces can move position
// Pre: none
// Post: return the queen pieces can move position
std::vector<Position> ChessBoard::getQueenCanMove(Position pos) const {
	vector<Position> canMove;
	vector<Position> canMoveRook = getRookCanMove(pos);
	vector<Position> canMoveBishop = getBishopCanMove(pos);

    // queen can move position is the rook and bishop can move position
    // combine the rook and bishop can move position
	canMove.insert(canMove.end(), canMoveRook.begin(), canMoveRook.end());
	canMove.insert(canMove.end(), canMoveBishop.begin(), canMoveBishop.end());

	return canMove;
}

// Intent: get the king pieces can move position
// Pre: none
// Post: return the king pieces can move position
std::vector<Position> ChessBoard::getKingCanMove(Position pos) const {
	std::vector<Position> canMove;
	int deltaRow[8] = { -1, -1, -1, 0, 0, 1, 1, 1 }; // up, down
	int deltaCol[8] = { -1, 0, 1, -1, 1, -1, 0, 1 }; // left, right
	// check if can move
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
        if (posIsOk(newPos) && isEmpty(newPos) && !isDonimated(newPos)) {
			canMove.push_back(newPos);
		}
	}

    //! 入堡，只為行動方考慮入堡
    if (numOfChecking == 0) {
        auto allEmptyBetween = [this](Position p1, const Position& p2)->bool {
            const Position delta = { 0, p2.col > p1.col ? 1 : -1 };
            int needAvoidCheck = 2; // 檢查2次
            while (true) {
                p1 += delta;
                if (p1 == p2) return true;
                if (!isEmpty(p1)) break;
                //! TODO: 檢查是否會被check
                if (needAvoidCheck) {
                    if (isDonimated(p1))
                        break;
                    --needAvoidCheck;
                }
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
    }


	return canMove;
}

// Intent: get the pieces can eat position
// Pre: none
// Post: return the pieces can eat position
std::vector<Position> ChessBoard::getCanEat(Position pos) const {
    std::vector<Position> canEat;
    TYPE chessTYPE = chessPieces[pos.row][pos.col]->getType();

    // if not playing, return empty
    if (gameState != GameManager::State::PLAYING) {
        return canEat;
    }

    // 被multiple check，只有王能動
    if (numOfChecking >= 2 && chessTYPE != TYPE::KING) {
        return canEat;
    }

    // 被pin住的主教、騎士 不能走
    if (isDonimated(pos) && (chessTYPE == TYPE::BISHOP || chessTYPE == TYPE::KNIGHT)) {
        return canEat;
    }

    switch (chessTYPE) {
	case TYPE::PAWN:
        canEat = getPawnCanEat(pos);
        break;
	case TYPE::ROOK:
        canEat = getRookCanEat(pos);
        break;
	case TYPE::KNIGHT:
        canEat = getKnightCanEat(pos);
        break;
	case TYPE::BISHOP:
        canEat = getBishopCanEat(pos);
        break;
	case TYPE::QUEEN:
        canEat = getQueenCanEat(pos);
        break;
	case TYPE::KING:
        // 國王走法的處理寫在函式中
        return getKingCanEat(pos);
	default:
        return canEat;
	}

    // 若被check
    if (numOfChecking == 1) {
        // 必須要能吃掉實際攻擊者
        for (auto& p : canEat) {
            if (!isRealAttacker(p))
                p = {-1, -1};
        }
        canEat.erase(std::remove(canEat.begin(), canEat.end(), Position{-1, -1}), canEat.end());
    }
    // 被pin住，將所有不在和國王同列或同欄的移動清掉
    if (isDonimated(pos)){
        Position kingPos = getOurKing();
        // 在同一列
        if (kingPos.row == pos.row) {
            // 將所有移到不同列的走法清掉
            canEat.erase(
                std::remove_if(canEat.begin(), canEat.end(), [r = kingPos.row](const Position& p) { return p.row != r; })
                , canEat.end());
        }
        // 在同一欄
        else {
            // 將所有移到不同欄的走法清掉
            canEat.erase(
                std::remove_if(canEat.begin(), canEat.end(), [c = kingPos.col](const Position& p) { return p.col != c; })
                , canEat.end());
        }
    }

    return canEat;
}

// Intent: get the pawn pieces can eat position
// Pre: none
// Post: return the pawn pieces can eat position
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

// Intent: get the rook pieces can eat position
// Pre: none
// Post: return the rook pieces can eat position
std::vector<Position> ChessBoard::getRookCanEat(Position pos) const {
	vector<Position> canEat;

    // find the can eat position in the direction of down
	for (int i = pos.row - 1; i >= 0; i--) {
		Position newPos = Position{ i, pos.col };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
    // find the can eat position in the direction of up
	for (int i = pos.row + 1; i < 8; i++) {
		Position newPos = Position{ i, pos.col };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
    // find the can eat position in the direction of left
	for (int i = pos.col - 1; i >= 0; i--) {
		Position newPos = Position{ pos.row, i };
		if (posIsOk(newPos) && !isEmpty(newPos)) {
			if (isEnemy(pos, newPos)) {
				canEat.push_back(newPos);
			}
			break;
		}
	}
    // find the can eat position in the direction of right
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

// Intent: get the knight pieces can eat position
// Pre: none
// Post: return the knight pieces can eat position
std::vector<Position> ChessBoard::getKnightCanEat(Position pos) const {
	vector<Position> canEat;
    // the delta of row and col
	int deltaRow[8] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int deltaCol[8] = { -1, 1, -2, 2, -2, 2, -1, 1 };

    // find the can eat position
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
		if (posIsOk(newPos) && !isEmpty(newPos) && isEnemy(pos, newPos)) {
			canEat.push_back(newPos);
		}
	}
	return canEat;
}

// Intent: get the bishop pieces can eat position
// Pre: none
// Post: return the bishop pieces can eat position
std::vector<Position> ChessBoard::getBishopCanEat(Position pos) const {
	vector<Position> canEat;
	bool notLookChess[4] = { true, true, true, true }; // left up, right up, left down, right down

	// find the can eat position
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

// Intent: get the queen pieces can eat position
// Pre: none
// Post: return the queen pieces can eat position
std::vector<Position> ChessBoard::getQueenCanEat(Position pos) const {
	vector<Position> canEat;
	vector<Position> canEatRook = getRookCanEat(pos);
	vector<Position> canEatBishop = getBishopCanEat(pos);

    // queen can eat position is the union of rook and bishop
    // combine the rook and bishop can eat position
	canEat.insert(canEat.end(), canEatRook.begin(), canEatRook.end());
	canEat.insert(canEat.end(), canEatBishop.begin(), canEatBishop.end());
	return canEat;
}

// Intent: get the king pieces can eat position
// Pre: none
// Post: return the king pieces can eat position
std::vector<Position> ChessBoard::getKingCanEat(Position pos) const {
	vector<Position> canEat;
	int deltaRow[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int deltaCol[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	for (int i = 0; i < 8; i++) {
		Position newPos = Position{ pos.row + deltaRow[i], pos.col + deltaCol[i] };
        if (posIsOk(newPos) && !isEmpty(newPos) && isEnemy(pos, newPos) && !isDonimated(newPos)) {
			canEat.push_back(newPos);
		}
	}
	return canEat;
}

// Intent: do difference action when chess pieces clicked
// Pre: none
// Post: do difference action when chess pieces clicked
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
            halfmove = 0;
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

            // enPassant and halfmove
            if (chessPieces[pos.row][pos.col]->getType() == TYPE::PAWN) { // if move pawn
                if (std::abs(pos.row - beforeClickPos.row) == 2)  // if move forward 2 blocks
                    enPassant = {(currentTeam == COLOR::WHITE ? pos.row + 1 : pos.row - 1), pos.col};
                else
                    enPassant = {-1, -1};
                halfmove = 0;
            }
            else {
                enPassant = {-1, -1};
                ++halfmove;
            }
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
            halfmove = 0;
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

// Intent: from pos1 to pos2
// Pre: none
// Post: swap the chess pieces from pos1 to pos2
void ChessBoard::move(Position from, Position to) {
    qDebug() << "Move Action from" << "(" << from.row << ", " << from.col << ") : " << "(" << to.row << ", " << to.col << ")" << "\n";

	// 交換旗子資訊
	swap(*chessPieces[from.row][from.col], *chessPieces[to.row][to.col]);
}

// Intent: eater eat beEat
// Pre: none
// Post: set beEat empty and swap the chess pieces from eater to beEat
void ChessBoard::eat(Position eater, Position beEat) {
	qDebug() << "Eat Action from" << "(" << eater.row << ", " << eater.col << ") : " << "(" << beEat.row << ", " << beEat.col << ")" << "\n";

	// 交換旗子資訊
	chessPieces[beEat.row][beEat.col]->setEmpty();
	chessPieces[beEat.row][beEat.col]->setPixmap(QPixmap());
	swap(*chessPieces[beEat.row][beEat.col], *chessPieces[eater.row][eater.col]);
}

// Intent: emphasize the can move and can eat position on GUI
// Pre: none
// Post: emphasize the can move and can eat position on GUI
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

// Intent: clear the position make emphasize on GUI
// Pre: position
// Post: clear the position make emphasize on GUI
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

// Intent: clear all emphasize
// Pre: none
// Post: clear all emphasize
void ChessBoard::emphasizeClearAll()
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            chessPieces[i][j]->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
        }
    }
}

// Intent: change the turn
// Pre: none
// Post: change the turn
void ChessBoard::changeTurn(bool autoChangeTeam) {
    if (autoChangeTeam) {
        if (currentTeam == COLOR::BLACK) {
            currentTeam = COLOR::WHITE;
            fullmove++;
        }
        else {
            currentTeam = COLOR::BLACK;
        }

        // 紀錄盤面
        moves.resize(currentMove + 2);
        ++currentMove;
        moves[currentMove] = this->toFEN();
    }

    // fifty move rule
    if (halfmove >= 50) {
        gameState = GameManager::State::DRAW;
    }
    else {
        GameManager::drawTerritoryAndUpdateState(this);     
    }

    emit changedTurnSignal(currentTeam);
    emit gameOver(gameState);
}

// Intent: resize the chess board
// Pre: none
// Post: resize the chess board
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

// Intent: load FEN code
// Pre: none
// Post: load FEN code
void ChessBoard::load(QString FEN) {
    GameManager::load(FEN, chessPieces, currentTeam, castlingFlag, enPassant, halfmove, fullmove);

    // 紀錄盤面
    emphasizeClearAll();
    firstClick = true;
    currentMove = 0;
    moves.resize(1);
    moves[currentMove] = FEN;
    changeTurn(false);
}

// Intent: undo to previous move
// Pre: none
// Post: undo to previous move
void ChessBoard::undo()
{
    if (currentMove == 0)
        return;

    --currentMove;
    GameManager::load(moves[currentMove], chessPieces, currentTeam, castlingFlag, enPassant, halfmove, fullmove);
    emphasizeClearAll();
    firstClick = true;
    changeTurn(false);
    emit changedTurnSignal(currentTeam);
}

// Intent: redo to next move
// Pre: none
// Post: redo to next move
void ChessBoard::redo() {
    if (currentMove == moves.size() - 1)
        return;

    ++currentMove;
    GameManager::load(moves[currentMove], chessPieces, currentTeam, castlingFlag, enPassant, halfmove, fullmove);
    emphasizeClearAll();
    firstClick = true;
    changeTurn(false);
    emit changedTurnSignal(currentTeam);
}


