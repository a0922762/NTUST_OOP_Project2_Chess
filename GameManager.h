#pragma once
/*****************************************************************//**
 * File: GameManager.h
 * Author: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Create Date: 2023-05-06
 * Editor: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Update Date: 2023-05-06
 * Description: Header file for GameManager.cpp
 *********************************************************************/
#include "ChessPieces.h"

class GameManager {
public:
    enum class State { PLAYING, WHITE_WIN, BLACK_WIN, DRAW};

private:

public:
    static void checkForPromotion(ChessPieces* chess);

    static void load(QString FEN, ChessPieces* pieces[8][8], COLOR& moveTeam, int& castlingFlag, Position& enPassant, int& halfmove, int& fullmove);
};
