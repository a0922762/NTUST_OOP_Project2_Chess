
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

    // palette
    QPalette myPalette;
    myPalette.setColor(this->backgroundRole(), QColor(71, 68, 59));
    myPalette.setColor(QPalette::WindowText, QColor(209, 202, 180));
    this->setPalette(myPalette);

    // 設置棋盤旁的label
    QFont labelFont("impact", 12, 5);
    auto idxOnBorder = [](int idx) { return idx == 0 || idx == 9; };
    int fixedSize = 20;
    for (int r = 0; r <=9; ++r) {
        for (int c = 0; c <=9; ++c) {
            if (!idxOnBorder(r) && !idxOnBorder(c))
                continue;

            QLabel* p = qobject_cast<QLabel*>(ui->boardLayout->itemAtPosition(r, c)->widget());
            if (idxOnBorder(r))
                p->setFixedHeight(fixedSize);
            if (idxOnBorder(c))
                p->setFixedWidth(fixedSize);

            p->setFont(labelFont);
            p->setAlignment(Qt::AlignCenter);
        }
    }


    connect(pregameDialog, &PreGame::startButtonClicked, this, &GameMainWindow::startGame);
    connect(ui->actionNew_Game, &QAction::triggered, this, &GameMainWindow::newGame);
    connect(ui->actionSurrender, &QAction::triggered, this, [this]() {
        if (this->ui->chessBoard->getTurn() == COLOR::WHITE)
            this->gameOver(GameManager::State::BLACK_WIN);
        else
            this->gameOver(GameManager::State::WHITE_WIN);
    });
    connect(ui->actionPause, &QAction::triggered, this, &GameMainWindow::pause);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->white_TimeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::BLACK_WIN); });
    connect(ui->black_timeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::WHITE_WIN); });
    connect(ui->chessBoard, &ChessBoard::changedTurnSignal, this, &GameMainWindow::updateInfo);
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

    this->updateInfo(ui->chessBoard->getTurn());

    // enable all action
    QList<QAction*> actionList = this->findChildren<QAction*>(QRegularExpression("^action"));
    for (auto it = actionList.begin(); it != actionList.end(); ++it) {
        qDebug() << *it << "Enabled";
        (*it)->setEnabled(true);
    }
}

void GameMainWindow::gameOver(GameManager::State state)
{
    if (state == GameManager::State::PLAYING)
        return;

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
        ui->turnLabel->setStyleSheet("background-color: white; color: black; font: bold 35px alef;");
        ui->roundLCD->display(ui->roundLCD->value() + 1);
    }
    else {
        ui->white_TimeLabel->stop();
        ui->black_timeLabel->start();
        ui->turnLabel->setText("Black's Turn");
        ui->turnLabel->setStyleSheet("background-color: black; color: white; font: bold 35px alef;");
    }
}

void GameMainWindow::pause()
{
    ui->black_timeLabel->stop();
    ui->white_TimeLabel->stop();
    QMessageBox::information(this, "Paused", "遊戲暫停");
    if (ui->chessBoard->getTurn() == COLOR::WHITE)
        ui->white_TimeLabel->start();
    else
        ui->black_timeLabel->start();
}

void GameMainWindow::newGame()
{
    ui->white_TimeLabel->stop();
    ui->black_timeLabel->stop();
    pregameDialog->open();
}

