#pragma once
#include <QWidget>
#include <QResizeEvent>
#include <memory>
#include <vector>
#include "GameManager.h"
#include "ChessPieces.h"
#include "common.h"

/**
 * @brief Player
 * @details 負責處理棋子的點擊事件+移動棋子
 */
class ChessBoard : public QWidget
{
	Q_OBJECT
    friend class GameManager;

private:
    // 點擊資訊
	bool firstClick;
	Position beforeClickPos;

    // 盤面資訊
    ChessPieces* chessPieces[8][8];
	COLOR currentTeam;
    int castlingFlag = 0b1111;
    Position enPassant = { -1, -1 };
    int halfmove = 0;
    int fullmove = 0;

    // 遊戲狀態
    GameManager::State gameState;
    int numOfChecking;
    struct {
        bool isDonimated;
        bool isChecking;
    } enemyTerritory[8][8]; //!< 敵方的勢力範圍
    std::vector<QString> moves; //!< 以FEN儲存每回合盤面
    int currentMove; //!< 目前盤面在moves中的index

private:
	// get information
	bool posIsOk(int row, int col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
	bool posIsOk(Position pos) const { return posIsOk(pos.row, pos.col); }
	bool isEmpty(int row, int col) const { return chessPieces[row][col]->getType() == TYPE::EMPTY; }
	bool isEmpty(Position pos)  const { return isEmpty(pos.row, pos.col); }
	bool isPieces(Position pos) const { return !isEmpty(pos); }
	bool isWhite(int row, int col) const { return chessPieces[row][col]->isWhite(); }
	bool isWhite(Position pos) const { return isWhite(pos.row, pos.col); }
	bool isBlack(int row, int col) const { return chessPieces[row][col]->isBlack(); }
	bool isBlack(Position pos) const { return isBlack(pos.row, pos.col); }
	bool isEnemy(Position own, Position enemy) const { return isWhite(own) != isWhite(enemy); }
    bool isTurn(Position pos) const { return isWhite(pos) == (currentTeam == COLOR::WHITE); }

    bool isDonimated(Position pos) const { return enemyTerritory[pos.row][pos.col].isDonimated; }
    bool isRealAttacker(Position pos) const { return !isEmpty(pos) && enemyTerritory[pos.row][pos.col].isChecking;}
    bool isGhostAttacker(Position pos) const { return isEmpty(pos) && enemyTerritory[pos.row][pos.col].isChecking;}

	// get can move position
    // gameState != PLAYING時，回傳空vector
    // 回傳可走到的空格
	std::vector<Position> getCanMove(Position pos) const;
	std::vector<Position> getPawnCanMove(Position pos) const;
	std::vector<Position> getKnightCanMove(Position pos) const;
	std::vector<Position> getBishopCanMove(Position pos) const;
	std::vector<Position> getRookCanMove(Position pos) const;
	std::vector<Position> getQueenCanMove(Position pos) const;
	std::vector<Position> getKingCanMove(Position pos) const;

	// get can eat position
    // gameState != PLAYING時，回傳空vector
    // 回傳可吃的enemy
	std::vector<Position> getCanEat(Position pos) const;
	std::vector<Position> getPawnCanEat(Position pos) const;
	std::vector<Position> getKnightCanEat(Position pos) const;
	std::vector<Position> getBishopCanEat(Position pos) const;
	std::vector<Position> getRookCanEat(Position pos) const;
	std::vector<Position> getQueenCanEat(Position pos) const;
	std::vector<Position> getKingCanEat(Position pos) const;

	void emphasizeCan(Position pos);
	void emphasizeClear(Position pos);
    void emphasizeClearAll();

	bool pawnIsFirstMove(Position pos) const { return (pos.row == 6 && isWhite(pos)) || (pos.row == 1 && isBlack(pos)); }

    // autoChangeTeam == true -> 改currentTeam+fullmove+更新moves
    // false時，只計算遊戲狀態
    void changeTurn(bool autoChangeTeam = true);

public:
	ChessBoard(QWidget* parent);

    void move(Position from, Position to);
	void eat(Position eater, Position eaten);

    void resizeEvent(QResizeEvent* event) override;

    COLOR getTurn() const { return currentTeam; }
    int getHalfMove() const { return halfmove; }
    int getFullMove() const { return fullmove; }

    void load(QString FEN);
    QString toFEN() { return GameManager::toFEN((const ChessPieces*(*)[8])chessPieces, currentTeam, castlingFlag, enPassant, halfmove, fullmove);}
    const QString& getCurrentFEN() { return moves[currentMove]; }

signals:
	void changedTurnSignal(COLOR currentTeam);
    void gameOver(GameManager::State);

public slots:
	void chessPiecesClicked(Position pos);
    void undo();
    void redo();
    void setState(GameManager::State state) { gameState = state; }
};
