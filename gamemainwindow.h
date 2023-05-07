
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
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

public slots:
    void show();
    void startGame(SettingProtocol setting);
    // 遊戲結束，state->結果
    void gameOver(GameManager::State state);
    // 將回合轉成某一隊，並設定訊息
    void updateInfo(COLOR color);
    void pause();
    void newGame();

signals:

private:
    Ui::GameMainWindow* ui;
    PreGame* pregameDialog;
    SettingProtocol::TimeLimitType_t timeLimitType;
};

#endif // GAMEMAINWINDOW_H
