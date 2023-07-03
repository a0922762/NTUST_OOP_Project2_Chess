/*****************************************************************//**
 * File: ChessPieces.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: this is a chess pieces implention
 *********************************************************************/
#include "ChessPieces.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPixmap>
#include <QPalette>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

// Intent: constructor
// Pre: none
// Post: make a chess pieces with type and color and position
ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent)
    : QLabel(parent), pos(pos), type(type), color(color)
{
    setAcceptDrops(true);
    setType(type, color);
}

// Intent: constructor
// Pre: none
// Post: make a chess pieces by default row and col
ChessPieces::ChessPieces(int row, int col, QWidget* parent) : QLabel(parent){
    pos = Position{ row, col };
    int id = row * 8 + col; // 0 ~ 63

    this->setFixedSize(INIT_PIECE_SIZE, INIT_PIECE_SIZE);

    // set chess pieces by id
    switch (id) {
    case 0: case 7:
        type  = TYPE::ROOK;
        color = COLOR::BLACK;
        break;
    case 1: case 6:
        type  = TYPE::KNIGHT;
        color = COLOR::BLACK;
        break;
    case 2: case 5:
        type  = TYPE::BISHOP;
        color = COLOR::BLACK;
        break;
    case 3:
        type  = TYPE::QUEEN;
        color = COLOR::BLACK;
        break;
    case 4:
        type  = TYPE::KING;
        color = COLOR::BLACK;
        break;

    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        type  = TYPE::PAWN;
        color = COLOR::BLACK;
        break;
    case 56: case 63:
        type  = TYPE::ROOK;
        color = COLOR::WHITE;
        break;
    case 57: case 62:
        type  = TYPE::KNIGHT;
        color = COLOR::WHITE;
        break;
    case 58: case 61:
        type  = TYPE::BISHOP;
        color = COLOR::WHITE;
        break;
    case 59:
        type  = TYPE::QUEEN;
        color = COLOR::WHITE;
        break;
    case 60:
        type  = TYPE::KING;
        color = COLOR::WHITE;
        break;
    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
        type  = TYPE::PAWN;
        color = COLOR::WHITE;
        break;
    default:
        type = TYPE::EMPTY;
        color = COLOR::WHITE;
        break;
    }

    setAcceptDrops(true);
    setType(type, color);
}

// Intent: swap chess pieces
// Pre: none
// Post: swap chess pieces
void ChessPieces::swapChessInfo(ChessPieces &rhs) {
    // 三角交換
    QPixmap tmp = rhs.pixmap(Qt::ReturnByValue);
    rhs.setPixmap(this->pixmap(Qt::ReturnByValue));
    this->setPixmap(tmp);

    std::swap(type, rhs.type);
    std::swap(color, rhs.color);
    std::swap(imageAddress, rhs.imageAddress);
}

// Intent: set chess pieces type
// Pre: none
// Post: set chess pieces type
void ChessPieces::setType(TYPE _type, COLOR _color)
{
    type = _type;
    color = _color;

    if (type == TYPE::EMPTY) {
        imageAddress = "";
        setPixmap(QPixmap());
    }
    else {
        imageAddress = ":/pieces/chessPieces/chess/";
        imageAddress += (color == COLOR::WHITE ? "w" : "b");
        imageAddress += (type == TYPE::BISHOP ?  "b" :
                         type == TYPE::KING ?    "k" :
                         type == TYPE::KNIGHT ?  "n" :
                         type == TYPE::PAWN ?    "p" :
                         type == TYPE::QUEEN ?   "q" :
                                                 "r");
        imageAddress += ".png";
        setPixmap(QPixmap(imageAddress).scaled(this->size(), Qt::KeepAspectRatio));
    }
}

// Intent: swap chess pieces
// Pre: none
// Post: swap chess pieces
void swap(ChessPieces& lhs, ChessPieces& rhs) {
	lhs.swapChessInfo(rhs);
}

// Intent: get chess pieces position by algebraic notation
// Pre: none
// Post: get chess pieces position by algebraic notation
Position posFromAlgebraic(std::string notation)
{
    Position pos;
    pos.row = 8 - (notation.at(1) - '0');
    pos.col = notation.at(0) - 'a';
    return pos;
}

// Intent: when pieces is clicked, emit clicked signal
// Pre: clicked
// Post: emit clicked signal
void ChessPieces::mousePressEvent(QMouseEvent* event)
{
    // if left click
    if (event->button() == Qt::LeftButton) {
        qDebug() << pos.row << ',' << pos.col << "click" << (int)type << (int)color << imageAddress;
        startDrag = event->pos();
        emit clicked(pos);
    }
}

// Intend: 當按著左鍵移動時觸發drag
// Pre: 按著左鍵移動一段距離
// Post: 開始drag，拖移的mime data: "custom/chess", "W" or "B"（白或黑）
void ChessPieces::mouseMoveEvent(QMouseEvent *event)
{
    if (this->type == TYPE::EMPTY)
        return;

    QPoint delta = event->pos() - startDrag;

    if (event->buttons() & Qt::LeftButton && delta.manhattanLength() >= QApplication::startDragDistance())
    {
        QScopedPointer<QDrag> drag(new QDrag(this));
        QMimeData* mime = new QMimeData;

        // initialize data
        mime->setData("custom/chess", this->color == COLOR::WHITE ? "W" : "B");
        drag->setMimeData(mime);

        // initialize pixmap
        QPixmap pix(this->pixmap(Qt::ReturnByValue));
        drag->setPixmap(pix);
        drag->setHotSpot(QPoint(pix.width() / 2, pix.height() / 2));

        this->setPixmap(QPixmap());
        drag->exec();
        if (!this->imageAddress.isEmpty())
            this->setPixmap(QPixmap(this->imageAddress).scaled(this->size()));
    }
}

// Intend: drag進來時，只接受"custom/chess"這個mime type
// Pre: drag enter
// Post: accept proposed action if mime type contains "custom/chess"
void ChessPieces::dragEnterEvent(QDragEnterEvent *event)
{
    // 只接受有"custom/chess"這個mime type的mime data
    if (event->mimeData()->hasFormat("custom/chess")) {
        event->acceptProposedAction();
    }
}

// Intend: drop時，判斷是否要emit clicked signal
// Pre: drop
// Post: 若該格不為空且拖上來的棋子是同隊的，則return；否則emit clicked signal
void ChessPieces::dropEvent(QDropEvent *event)
{
    QByteArray arr = event->mimeData()->data("custom/chess");
    // 若不是空的格子，則只接受敵隊棋子拖上來
    if (this->type != TYPE::EMPTY && arr.startsWith(this->color == COLOR::WHITE ? 'W' : 'B')) {
        return;
    }

    emit clicked(pos);

    if (!this->imageAddress.isEmpty())
        this->setPixmap(QPixmap(this->imageAddress).scaled(this->size()));

    event->acceptProposedAction();
}


