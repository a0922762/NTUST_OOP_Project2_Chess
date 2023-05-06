#pragma once
/*****************************************************************//**
 * File: ChessPieces.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-05
 * Editor: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Update Date: 2023-05-06
 * Description: Header file for ChessPieces.cpp
 *********************************************************************/
#include <memory>
#include <vector>
#include <string>

enum class COLOR { White, Black };
enum class TYPE { Pawn, Rook, Knight, Bishop, Queen, King };
// 下方flag分別對應：白方王翼入堡、白方后翼入堡、黑方王翼入堡、黑方后翼入堡
enum class CASTLING { WHITE_K = 0b0001, WHITE_Q = 0b0010, BLACK_k = 0b0100, BLACK_q = 0b1000};
struct Position { int row; int col; };
Position posFromAlgebraic(std::string notation);
bool isOnBoard(const Position& pos);
//垃圾程式 待更改

struct BoardInfo;

class ChessPieces
{
public:
	Position pos;
	TYPE type;
	COLOR color;

public:
	ChessPieces(const Position& pos, TYPE type, COLOR color);

	bool isWhite() const { return color == COLOR::White; }
	bool isBlack() const { return color == COLOR::Black; }
	int getRow() const { return pos.row; }
	int getCol() const { return pos.col; }
	Position& getPos() { return pos; }
	const Position& getPos() const { return pos; }

	void move(int row, int col) { this->pos.row = row; this->pos.col = col; }

	// 將勢力範圍記錄在 info.enemyTerritory
	void drawTerritory(BoardInfo& info);
	// 將可能走法存在 info.possibleMove[pos.row][pos.col]
	void listPossibleMove(BoardInfo& info);
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