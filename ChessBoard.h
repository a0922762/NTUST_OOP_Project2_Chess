#pragma once
#include <QWidget>
#include <QResizeEvent>
#include <memory>
#include <vector>
#include "ChessPieces.h"
#include "common.h"

class ChessBoard : public QWidget
{
	Q_OBJECT
private:
	ChessPieces* chessPieces[8][8];
	bool firstClick;
    Position beforeClickPos;

	// get information
	bool posIsOk(int row, int col) const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
	bool posIsOk(Position pos) const { return posIsOk(pos.row, pos.col); }
	bool isEmpty(int row, int col) const { return chessPieces[row][col]->getType() == TYPE::EMPTY; }
	bool isEmpty(Position pos)  const { return isEmpty(pos.row, pos.col); }
	bool isWhite(int row, int col) const { return chessPieces[row][col]->isWhite(); }
	bool isWhite(Position pos) const { return isWhite(pos.row, pos.col); }
	bool isBlack(int row, int col) const { return chessPieces[row][col]->isBlack(); }
	bool isBlack(Position pos) const { return isBlack(pos.row, pos.col); }

	bool pawnIsFirstMove(Position pos) const { return (pos.row == 6 && isWhite(pos)) || (pos.row == 1 && isBlack(pos)); }

public:
	ChessBoard(QWidget* parent);
    std::vector<Position> getCanMove(Position pos) const;
	std::vector<Position> getCanEat(Position pos) const;

	std::vector<Position> getPawnCanMove(Position pos) const;
	std::vector<Position> getKnightCanMove(Position pos) const;
	std::vector<Position> getBishopCanMove(Position pos) const;
	std::vector<Position> getRookCanMove(Position pos) const;
	std::vector<Position> getQueenCanMove(Position pos) const;
	std::vector<Position> getKingCanMove(Position pos) const;

    void move(Position from, Position to);

	// bool movePieces(Position from, Position to);

    void resizeEvent(QResizeEvent* event) override;

signals:
	void changedTurn();
public slots:
	void chessPiecesClicked(Position pos);
	// bool eatPieces(Position from, Position to);

};
