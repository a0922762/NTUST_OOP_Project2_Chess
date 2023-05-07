#include "ChessBoard.h"
#include <QLayout>

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true)
{
	//TODO: initialize chessPieces
	//GUI
    //this->setFixedSize(1024, 1024);

	QGridLayout* layout = new QGridLayout;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			chessPieces[row][col] = new ChessPieces(row, col);
			
			layout->addWidget(chessPieces[row][col], row, col);
		}
	}

    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
	this->setLayout(layout);

}

void ChessBoard::chessPiecesClicked(Position pos) {
	// if click on empty
	if (chessPieces[pos.row][pos.col] == nullptr) {

		if (firstClick) {
			return; // do nothing
		}
		else {
			// move chessPieces
			// if move success
				//	change turn
			// else
				// do nothing
		}
	}
	else {
		if (firstClick) {
			// display possible move
		}
		else {
			// eat chessPieces
			// if eat success
				// change turn
			// else
			// do nothing
		}
	}

	firstClick = !firstClick;
}
