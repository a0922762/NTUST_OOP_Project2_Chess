#pragma once
/*****************************************************************//**
 * File: ChessPieces.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-05
 * Editor: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Update Date: 2023-05-06
 * Description: Header file for ChessPieces.cpp
 *********************************************************************/
#include <QLabel>
#include <QPixmap>

#include <memory>
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

struct BoardInfo;

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
    void setEmpty() { type = TYPE::EMPTY; imageAddress = ""; }

	bool isWhite() const { return color == COLOR::WHITE; }
	bool isBlack() const { return color == COLOR::BLACK; }
	int getRow() const { return pos.row; }
	int getCol() const { return pos.col; }
	Position& getPos() { return pos; }
    const QPixmap* getImage() const { return this->pixmap(); }
    const QString& getImgAddr() const { return this->imageAddress; }
	const Position& getPos() const { return pos; }

	

	TYPE getType() const { return type; }

	// 當滑鼠點擊時觸發
    void mousePressEvent(QMouseEvent* event) override;

	friend void swap(ChessPieces& lhs, ChessPieces& rhs);
signals:
	void clicked(Position pos); 
};

//struct BoardInfo {
//	COLOR currentTeam; // 移動方
//	std::unique_ptr<ChessPieces> board[8][8]; // 棋盤&棋子
//	std::vector<Position> possibleMove[8][8]; // 每一格可能的走法
//	struct {
//		bool isDonamited;
//		bool isAttackingKing;
//	} enemyTerritory[8][8];  // 敵方勢力範圍
//	int numOfChecking = 0;  // 有幾個「實際的」敵方棋子可以攻擊到國王
//	Position EnPassant = { -1, -1 };  // 可吃過路兵的位置
//	int castlingFlag = 0;  // 可否入堡的旗標
//};
