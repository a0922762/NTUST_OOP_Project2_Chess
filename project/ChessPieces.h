/*****************************************************************//**
 * File: ChessPieces.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-05
 * Editor: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Update Date: 2023-05-06
 * Description: Header file for ChessPieces.cpp
 *********************************************************************/
#pragma once
#include <QLabel>
#include <QPixmap>

#include <vector>
#include <string>
#include <iostream>

#include "common.h"

#define INIT_PIECE_SIZE 80


enum class COLOR { WHITE, BLACK };
enum class TYPE { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
// 下方flag分別對應：白方王翼入堡、白方后翼入堡、黑方王翼入堡、黑方后翼入堡
enum class CASTLING { WHITE_K = 0b0001, WHITE_Q = 0b0010, BLACK_k = 0b0100, BLACK_q = 0b1000};
Position posFromAlgebraic(std::string notation);

/**
 * @brief ChessPieces (ViewManager)
 * @details 負責管理每一格的顯示
 */
class ChessPieces : public QLabel
{
	Q_OBJECT
private:
	Position pos;
	TYPE type;
	COLOR color;
    QString imageAddress;
public:
	explicit ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent = nullptr);
    ChessPieces(int row, int col, QWidget* parent = nullptr);

    void setPos(Position pos) { this->pos = pos; }
    void swapChessInfo(ChessPieces& rhs);
    void setEmpty() { setType(TYPE::EMPTY, COLOR::WHITE); }
    void setType(TYPE type, COLOR color);

	bool isWhite() const { return color == COLOR::WHITE; }
	bool isBlack() const { return color == COLOR::BLACK; }
	int getRow() const { return pos.row; }
	int getCol() const { return pos.col; }
	Position& getPos() { return pos; }
    const QString& getImgAddr() const { return this->imageAddress; }
	const Position& getPos() const { return pos; }
	TYPE getType() const { return type; }

	// 當滑鼠點擊時觸發
    void mousePressEvent(QMouseEvent* event) override;

	friend void swap(ChessPieces& lhs, ChessPieces& rhs);
signals:
	void clicked(Position pos); 
};
