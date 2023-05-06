//垃圾程式 待更改
#pragma once

#include "ChessPieces.h"
#include <utility>

using namespace std;

class BasePlayer{
private:
	std::shared_ptr<BoardInfo> info;
	COLOR color;


public:
	BasePlayer(std::shared_ptr<BoardInfo> info, COLOR color) : info(info), color(color) {}

	pair<int, int> getCanMove(int row, int col /*, Board information*/);
	bool tryMove(int row, int col /*, Board information*/);


	bool attack(int id1, int id2);
	bool move(int id, int row, int col);



};
