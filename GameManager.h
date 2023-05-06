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
    std::shared_ptr<BoardInfo> info;
    State state;

public:
    GameManager(std::shared_ptr<BoardInfo> info) :info(info), state(State::PLAYING) {}

    void load(std::string FEN);
    // 為下一回合的行動方計算所有可行走法+更新state
    void nextTurn();
    State getState() const { return state; }
};