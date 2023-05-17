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

class ChessBoard;

/**
 * @brief GameManager
 * @details 負責特殊規則的判斷、盤面載入、和判斷遊戲狀態
 */
class GameManager {
public:
    enum class State { PLAYING, WHITE_WIN, BLACK_WIN, DRAW};

private:

public:
    // 1. 若走出入堡，移動城堡。 2. 依移動更新castlingFlag
    static void updateForCastling(ChessPieces* board[8][8], int& castlingFlag, Position pre, Position post);
    // 檢查chess可否升變
    static void checkForPromotion(ChessPieces* chess);

    // 依盤面資訊生成FEN
    static QString toFEN(const ChessPieces* pieces[8][8], COLOR moveTeam, int castlingFlag, Position enPassant, int halfmove, int fullmove);
    // 從FEN載入盤面
    static void load(QString FEN, ChessPieces* pieces[8][8], COLOR& moveTeam, int& castlingFlag, Position& enPassant, int& halfmove, int& fullmove);

    // 在走子前，先計算敵方勢力範圍和判斷勝負
    // Pre: board->currentTeam為行動方，敵方為currentTeam的另一方
    static void drawTerritoryAndUpdateState(ChessBoard* board);
};
