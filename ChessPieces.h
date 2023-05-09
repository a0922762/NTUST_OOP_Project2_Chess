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

#include <memory>
#include <vector>
#include <string>
#include "common.h"

enum class COLOR { WHITE, BLACK };
enum class TYPE { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
// 下方flag分別對應：白方王翼入堡、白方后翼入堡、黑方王翼入堡、黑方后翼入堡
enum class CASTLING { WHITE_K = 0b0001, WHITE_Q = 0b0010, BLACK_k = 0b0100, BLACK_q = 0b1000};
Position posFromAlgebraic(std::string notation);
bool isOnBoard(const Position& pos);

struct BoardInfo;

class ChessPieces : public QLabel
{
	Q_OBJECT
public:
	Position pos;
	TYPE type;
	COLOR color;
public:
	explicit ChessPieces(const Position& pos, TYPE type, COLOR color, QWidget* parent = nullptr);
	ChessPieces(const Position& pos, TYPE type, COLOR color);
    ChessPieces(int row, int col, QWidget* parent = nullptr);

	bool isWhite() const { return color == COLOR::WHITE; }
	bool isBlack() const { return color == COLOR::BLACK; }
	int getRow() const { return pos.row; }
	int getCol() const { return pos.col; }
	Position& getPos() { return pos; }
	const Position& getPos() const { return pos; }

	void move(int row, int col) { this->pos.row = row; this->pos.col = col; }

    // 渲染
    void render();  // minimum size & sizePolicy & background role
    void resizeEvent(QResizeEvent*) override;  // resize to equal height and width
    void paintEvent(QPaintEvent*) override;  // svg image

	// 將勢力範圍記錄在 info.enemyTerritory
	void drawTerritory(BoardInfo& info);
	// 將可能走法存在 info.possibleMove[pos.row][pos.col]
	void listPossibleMove(BoardInfo& info);

	// 當滑鼠點擊時觸發
    void mousePressEvent(QMouseEvent* event) override;

signals:
	// 參1 = 點擊的棋子, 參2 = 是否為第一次點擊 (第一次啟用/第二次取消)
	void clicked(Position pos);
};

struct BoardInfo {
	COLOR currentTeam; // 移動方
	std::unique_ptr<ChessPieces> board[8][8]; // 棋盤&棋子
	std::vector<Position> possibleMove[8][8]; // 每一格可能的走法
	struct {
		bool isDonamited;
		bool isAttackingKing;
	} enemyTerritory[8][8];  // 敵方勢力範圍
	int numOfChecking = 0;  // 有幾個「實際的」敵方棋子可以攻擊到國王
	Position EnPassant = { -1, -1 };  // 可吃過路兵的位置
	int castlingFlag = 0;  // 可否入堡的旗標
};
