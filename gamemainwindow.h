/*****************************************************************//**
 * File: gamemainwindow.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: main game window 
 *********************************************************************/
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "SettingProtocol.h"
#include "GameManager.h"

// pre declaration
class PreGame;

QT_BEGIN_NAMESPACE
namespace Ui { class GameMainWindow; }
QT_END_NAMESPACE

/**
 * @brief 主要的遊玩介面
 * @details
 * 顯示（show）時，會跳出PreGame視窗供玩家輸入設定。
 * startGame slot用來接收玩家的設定，並初始化遊戲。
 */
class GameMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameMainWindow(QWidget *parent = nullptr);
    ~GameMainWindow();

    // 關閉主視窗，一併關閉所有視窗
    void closeEvent(QCloseEvent*) override { qApp->quit(); }

public slots:
    // 一併顯示pregame
    void show();
    // 開始遊戲，setting為玩家輸入的設定
    void startGame(SettingProtocol setting);
    // 遊戲結束，state->結果
    void gameOver(GameManager::State state);
    // 將回合轉成某一隊，並設定訊息
    void updateInfo(COLOR color);
    // 暫停
    void pause();
    // 停止遊戲+重新輸入遊戲設定
    void newGame();
    // 在新視窗顯示盤面FEN
    void showFEN();

signals:

private:
    Ui::GameMainWindow* ui;
    PreGame* pregameDialog;
    SettingProtocol::TimeLimitType_t timeLimitType;
};

#endif // GAMEMAINWINDOW_H
