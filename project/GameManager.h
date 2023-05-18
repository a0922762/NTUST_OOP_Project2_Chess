#pragma once
/*****************************************************************//**
 * File: GameManager.h
 * Author: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Create Date: 2023-05-06
 * Editor: BO-YUAN JEANG (jeangbyj@gmail.com)
 * Update Date: 2023-05-17
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
    // 「我方」、「敵方」的概念皆相對於currentTeam
    // 畫勢力範圍，標記「可攻擊的空格」、「被保護的敵方棋子」、「被check的我方國王」
    // 若為bishop、rook、queen額外標記「被pin住的我方棋子」
    static void drawTerritory(ChessBoard* board, Position pos);
    static void drawTerritoryPawn(ChessBoard* board, Position pos);
    static void drawTerritoryKnight(ChessBoard* board, Position pos);
    static void drawTerritoryKing(ChessBoard* board, Position pos);
    static void drawTerritoryQueen(ChessBoard* board, Position pos);
    static void drawTerritoryRook(ChessBoard* board, Position pos);
    static void drawTerritoryBishop(ChessBoard* board, Position pos);
    // helper function without checking for pinned，pos為可攻擊到的點
    static void drawHelperWithoutPinned(ChessBoard* board, Position pos, Position from);
    // helper function that checks for pinned，from為bishop、rook、queen的位置，delta為朝哪個方向
    // 想法為一個方向、一個方向檢查
    static void drawHelperWithPinned(ChessBoard* board, Position from, Position delta);

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
