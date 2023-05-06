
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"
#include "pregame.h"
#include "timedisplay.h"
#include <QDebug>

// Intend: set up ui
GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::GameMainWindow)
    , pregameDialog(new PreGame(this))
{
    ui->setupUi(this);
    connect(pregameDialog, &PreGame::startButtonClicked, this, &GameMainWindow::startGame);
}

// Intent: destruct the object
GameMainWindow::~GameMainWindow()
{
    delete ui;
    delete pregameDialog;
}

// Intent: show window and dialog
void GameMainWindow::show()
{
    QMainWindow::show();
    pregameDialog->open();
}

// Intend: 從PreGame Dialog接收setting並進行遊戲初始化
// Pre: PreGame Dialog的startGame被按下
// Post: 關掉Dialog，如果初始化成功；跳出警告，如果失敗
void GameMainWindow::startGame(SettingProtocol setting)
{
    // 對棋盤初始化
    qDebug() << "FEN: " << setting.FEN;
    // if (fail)
    //    QMessageDialog
    //    return;

    qDebug() << setting.initTime << setting.timeLimitType;
    // 一開始的時間
    ui->white_TimeLabel->setTime(setting.initTime);
    ui->black_timeLabel->setTime(setting.initTime);
    // 計時類型
    timeLimitType = setting.timeLimitType;
    // 調整timeLabel的類型
    switch (timeLimitType) {
    case SettingProtocol::NO_LIMIT:
        // 為兩個玩家分別計時
        ui->white_TimeLabel->setType(TimeDisplay::TIMING);
        ui->black_timeLabel->setType(TimeDisplay::TIMING);
        break;
    case SettingProtocol::LIMIT_PER_PLAYER:
        // 用兩個時鐘分別倒數
        ui->white_TimeLabel->setType(TimeDisplay::COUNTDOWN);
        ui->black_timeLabel->setType(TimeDisplay::COUNTDOWN);
        break;
    }

    // close dialog
    QDialog* senderDialog = qobject_cast<QDialog*>(QObject::sender());
    if (senderDialog != nullptr)
        senderDialog->accept();

    // start timer
    ui->white_TimeLabel->start();
}

