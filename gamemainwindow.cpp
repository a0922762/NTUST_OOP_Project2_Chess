
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"
#include "pregame.h"
#include "timedisplay.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

// Intend: set up ui
GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::GameMainWindow)
    , pregameDialog(new PreGame(this))
{
    ui->setupUi(this);
    connect(pregameDialog, &PreGame::startButtonClicked, this, &GameMainWindow::startGame);
    connect(ui->actionNew_Game, &QAction::triggered, this, &GameMainWindow::newGame);
    connect(ui->actionSurrender, &QAction::triggered, this, [this]() {
        // if white's turn
        this->gameOver(GameManager::State::BLACK_WIN);
        // else
        // this->gameOver(GameManager::State::WHITE_WIN);
    });
    connect(ui->actionPause, &QAction::triggered, this, &GameMainWindow::pause);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->white_TimeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::BLACK_WIN); });
    connect(ui->black_timeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::WHITE_WIN); });
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

    // if white first {
    ui->white_TimeLabel->start();
    this->updateInfo(COLOR::WHITE);
    // } else {
    // ui->black_timeLabel->start();
    // this->updateInfo(COLOR::Black);
    // }

    // enable all action
    QList<QAction*> actionList = this->findChildren<QAction*>(QRegularExpression("^action"));
    for (auto it = actionList.begin(); it != actionList.end(); ++it) {
        qDebug() << *it << "Enabled";
        (*it)->setEnabled(true);
    }
}

void GameMainWindow::gameOver(GameManager::State state)
{
    ui->white_TimeLabel->stop();
    ui->black_timeLabel->stop();
    QString message = (state == GameManager::State::BLACK_WIN ? "<b>Black</b> Wins!!!!" :
                       state == GameManager::State::WHITE_WIN ? "<b>White</b> Wins!!!!" :
                                                                "Draw");
    QMessageBox::information(this, "Game Over", message);
    ui->actionPause->setDisabled(true);
    ui->actionSurrender->setDisabled(true);
}

void GameMainWindow::updateInfo(COLOR color)
{
    if (color == COLOR::WHITE) {
        ui->black_timeLabel->stop();
        ui->white_TimeLabel->start();
        ui->turnLabel->setText("White's Turn");
        ui->turnLabel->setStyleSheet("background-color: white;" "color: black;");
    }
    else {
        ui->white_TimeLabel->stop();
        ui->black_timeLabel->start();
        ui->turnLabel->setText("Black's Turn");
        ui->turnLabel->setStyleSheet("background-color: black;" "color: white;");
    }
}

void GameMainWindow::pause()
{
    ui->black_timeLabel->stop();
    ui->white_TimeLabel->stop();
    QMessageBox::information(this, "Paused", "遊戲暫停");
    // if white's turn
    ui->white_TimeLabel->start();
    // else
    // ui->black_timeLabel->start();
}

void GameMainWindow::newGame()
{
    ui->white_TimeLabel->stop();
    ui->black_timeLabel->stop();
    pregameDialog->setResult(QDialog::Rejected);
    pregameDialog->open();
}

