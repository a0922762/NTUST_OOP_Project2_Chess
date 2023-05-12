#pragma once
#include <iostream>

using namespace std;

class Position {
public:
    int row;
    int col;

    bool operator==(const Position pos) { return this->row == pos.row && this->col == pos.col;}


};

\
