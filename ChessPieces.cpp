#include "ChessPieces.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QPalette>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent)
    : QLabel(parent), pos(pos), type(type), color(color)
{
}

ChessPieces::ChessPieces(int row, int col, QWidget* parent) : QLabel(parent){
    pos = Position{ row, col };
    int id = row * 8 + col;

    QString pAddress;

    this->setFixedSize(INIT_PIECE_SIZE, INIT_PIECE_SIZE);



    switch (id) {
    case 0: case 7:
        type  = TYPE::ROOK;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/br.png";
        break;
    case 1: case 6:
        type  = TYPE::KNIGHT;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/bn.png";
        break;
    case 2: case 5:
        type  = TYPE::BISHOP;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/bb.png";
        break;
    case 3:
        type  = TYPE::QUEEN;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/bq.png";
        break;
    case 4:
        type  = TYPE::KING;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/bk.png";
        break;

    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        type  = TYPE::PAWN;
        color = COLOR::BLACK;
        pAddress = ":/pieces/chessPieces/chess/bp.png";
        break;
    case 56: case 63:
        type  = TYPE::ROOK;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wr.png";
        break;
    case 57: case 62:
        type  = TYPE::KNIGHT;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wn.png";
        break;
    case 58: case 61:
        type  = TYPE::BISHOP;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wb.png";
        break;
    case 59:
        type  = TYPE::QUEEN;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wq.png";
        break;
    case 60:
        type  = TYPE::KING;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wk.png";
        break;
    case 48: case 49: case 50: case 51: case 52: case 53: case 54: case 55:
        type  = TYPE::PAWN;
        color = COLOR::WHITE;
        pAddress = ":/pieces/chessPieces/chess/wp.png";
        break;
    default:
        type = TYPE::EMPTY;
        color = COLOR::WHITE;
        break;
    }

    imageAddress = pAddress;

    if (imageAddress != "") {
        QPixmap piecesImage(imageAddress);
        this->setPixmap(piecesImage.scaled(100, 100, Qt::KeepAspectRatio));
    }
}

void ChessPieces::swapChessInfo(ChessPieces &rhs) {
    // 三角交換
    QPixmap tmp = rhs.pixmap(Qt::ReturnByValue);
    rhs.setPixmap(this->pixmap(Qt::ReturnByValue));
    this->setPixmap(tmp);

    std::swap(type, rhs.type);
    std::swap(color, rhs.color);
    std::swap(imageAddress, rhs.imageAddress);
}

void swap(ChessPieces& lhs, ChessPieces& rhs) {
	lhs.swapChessInfo(rhs);
}

// a1 -> Position{7, 0}
Position posFromAlgebraic(std::string notation)
{
    Position pos;
    pos.row = 8 - (notation.at(1) - '0');
    pos.col = notation.at(0) - 'a';
    return pos;
}

void ChessPieces::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << pos.row << ',' << pos.col << "click" << (int)type << (int)color << imageAddress;
        emit clicked(pos);

    }
}


