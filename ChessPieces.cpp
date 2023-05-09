#include "ChessPieces.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QPalette>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QPainter>

ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent)
    : QLabel(parent), pos(pos), type(type), color(color)
{

}

ChessPieces::ChessPieces(const Position& pos, TYPE type, COLOR color)
    : pos(pos), type(type), color(color)
{

}

ChessPieces::ChessPieces(int row, int col, QWidget* parent) : QLabel(parent){
    pos = Position{ row, col };
    int id = row * 8 + col;

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
    render();
}

void ChessPieces::render()
{
    int height = QGuiApplication::primaryScreen()->availableGeometry().height();
    height = (height / 2.0) / 8.0;   // 依螢幕大小調整
    this->setMinimumSize(height, height);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    // row + col 為 奇數：深色，偶數：淺
    this->setBackgroundRole((pos.row + pos.col) % 2 ? QPalette::AlternateBase : QPalette::Base);
    // 使用palette的顏色來填滿
    this->setAutoFillBackground(true);
}

void ChessPieces::resizeEvent(QResizeEvent *event)
{
    int h = event->size().height();
    if (h == event->oldSize().height()) {
        event->ignore();
    } else if (h != event->size().width()) {
        setMinimumWidth(h);
    }
    else {
        event->accept();
    }
}

void ChessPieces::paintEvent(QPaintEvent *)
{
    if (type != TYPE::EMPTY) {
        QString pAddress;
        pAddress.reserve(43);
        pAddress = (color == COLOR::WHITE ? ":/pieces/chessPieces_svg/w_" : ":/pieces/chessPieces_svg/b_");
        pAddress += (type == TYPE::PAWN ?  "pawn_" :
                    type == TYPE::BISHOP ? "bishop_":
                    type == TYPE::ROOK ?   "rook_" :
                    type == TYPE::KNIGHT ? "knight_":
                    type == TYPE::QUEEN ?  "queen_":
                                           "king_");
        pAddress += ".svg";
        QPainter painter(this);
        QSvgRenderer renderer(pAddress);
        renderer.render(&painter);
    }
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
        qDebug() << pos.row << ',' << pos.col << "click";
        emit clicked(pos);

    }
}
