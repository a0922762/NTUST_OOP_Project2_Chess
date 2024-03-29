/*****************************************************************//**
 * File: common.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: common class 
 *********************************************************************/
#pragma once
#include <iostream>

using namespace std;

class Position {
public:
    int row;
    int col;

    bool operator==(const Position pos) const { return this->row == pos.row && this->col == pos.col;}
    Position& operator+=(const Position pos) { row += pos.row; col += pos.col; return *this; }
    Position operator+(const Position pos) const { return Position(*this) += pos; }
};


