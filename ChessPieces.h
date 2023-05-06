#pragma once
/*****************************************************************//**
 * File: ChessPieces.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-05
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-05
 * Description: Header file for ChessPieces.cpp
 *********************************************************************/

enum class COLOR { White, Black };
enum class TYPE {Pawn, Rook, Knight, Bishop, Queen, King };
//垃圾程式 待更改
class ChessPieces
{
public:
	int row, col;
	int id;
	TYPE type;
	COLOR color;
	bool dead;

public:
	ChessPieces(int id);

	bool isWhite() const { return color == COLOR::White; }
	bool isBlack() const { return color == COLOR::Black; }
	int getRow() const { return row; }
	int getCol() const { return col; }

	void move(int row, int col) { this->row = row; this->col = col; }
	bool isDead() const { return dead; }

	
};
