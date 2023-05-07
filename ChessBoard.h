#pragma once
#include <QWidget>
#include <memory>
#include "ChessPieces.h"
#include "common.h"

class ChessBoard : public QWidget
{
	Q_OBJECT
private:
	ChessPieces* chessPieces[8][8];
	bool firstClick;

public:
	ChessBoard(QWidget* parent);

signals:
	void changedTurn();
public slots:
	void chessPiecesClicked(Position pos);
	// bool movePieces(Position from, Position to);
	// bool eatPieces(Position from, Position to);

};
