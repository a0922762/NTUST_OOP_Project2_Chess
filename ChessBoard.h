#pragma once
#include <QWidget>
#include <QResizeEvent>
#include <memory>
#include <vector>
#include "GameManager.h"
#include "ChessPieces.h"
#include "common.h"

class ChessBoard : public QWidget
{
	Q_OBJECT


private:
	ChessPieces* chessPieces[8][8];
	bool firstClick;
	Position beforeClickPos;
	COLOR currentTeam;
    int castlingFlag = 0b1111;
    Position enPassant = { -1, -1 };
    int halfmove = 0;
    int fullmove = 0;

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

	// get can move position
	std::vector<Position> getCanMove(Position pos) const;
	std::vector<Position> getPawnCanMove(Position pos) const;
	std::vector<Position> getKnightCanMove(Position pos) const;
	std::vector<Position> getBishopCanMove(Position pos) const;
	std::vector<Position> getRookCanMove(Position pos) const;
	std::vector<Position> getQueenCanMove(Position pos) const;
	std::vector<Position> getKingCanMove(Position pos) const;

	// get can eat position
	std::vector<Position> getCanEat(Position pos) const;
	std::vector<Position> getPawnCanEat(Position pos) const;
	std::vector<Position> getKnightCanEat(Position pos) const;
	std::vector<Position> getBishopCanEat(Position pos) const;
	std::vector<Position> getRookCanEat(Position pos) const;
	std::vector<Position> getQueenCanEat(Position pos) const;
	std::vector<Position> getKingCanEat(Position pos) const;

	void emphasizeCan(Position pos);
	void emphasizeClear(Position pos);

	bool pawnIsFirstMove(Position pos) const { return (pos.row == 6 && isWhite(pos)) || (pos.row == 1 && isBlack(pos)); }

	void changeTurn();

public:
	ChessBoard(QWidget* parent);

    void move(Position from, Position to);
	void eat(Position eater, Position eaten);

	// bool movePieces(Position from, Position to);

    void resizeEvent(QResizeEvent* event) override;

    COLOR getTurn() const { return currentTeam; }
    int getHalfMove() const { return halfmove; }
    int getFullMove() const { return fullmove; }

    void load(QString FEN) {GameManager::load(FEN, chessPieces, currentTeam, castlingFlag, enPassant, halfmove, fullmove);}

signals:
	void changedTurnSignal(COLOR currentTeam);
	void promotion(int &chooseId);
public slots:
	void chessPiecesClicked(Position pos);
	// bool eatPieces(Position from, Position to);

};
