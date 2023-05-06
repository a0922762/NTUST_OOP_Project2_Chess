//垃圾程式 待更改
#pragma once

#include "ChessPieces.h"
#include <utility>

using namespace std;

class BasePlayer{
private:
	static ChessPieces chessPieces[32];
	COLOR color;


public:
	pair<int, int> getCanMove(int id, int row, int col /*, Board information*/);
	bool tryMove(int id, int row, int col /*, Board information*/);


	bool attack(int id1, int id2);
	bool move(int id, int row, int col);



};
