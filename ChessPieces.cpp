#include "ChessPieces.h"
#include <QMouseEvent>

ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent)
	: QLabel(parent), pos(pos), type(type), color(color), firstClick(true)
{

}

ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color)
	: pos(pos), type(type), color(color)
{

}

// a1 -> Position{7, 0}
Position posFromAlgebraic(std::string notation)
{
	Position pos;
	pos.row = 8 - (notation.at(1) - '0');
	pos.col = notation.at(0) - 'a';
    return pos;
}

bool isOnBoard(const Position &pos)
{
    return 0 <= pos.row && pos.row < 8 && 0 <= pos.col && pos.col < 8;
}

void ChessPieces::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        firstClick = !firstClick;
        emit clicked(pos, firstClick);
        
    }
}