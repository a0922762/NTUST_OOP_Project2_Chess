/*****************************************************************//**
 * File: gamemainwindow.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: this is a game main window implention
 *********************************************************************/
#include "gamemainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QClipboard>
#include "ui_gamemainwindow.h"
#include "pregame.h"
#include "timedisplay.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

// Intend: constructor
// Pre: none
// Post: ui is set up
GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::GameMainWindow)
    , pregameDialog(new PreGame(this))
{
    ui->setupUi(this);
    ui->checkedLabel->hide();

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

    // connenct signal and slot
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
    connect(ui->actionto_FEN, &QAction::triggered, this, &GameMainWindow::showFEN);
    connect(ui->actionUndo, &QAction::triggered, ui->chessBoard, &ChessBoard::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->chessBoard, &ChessBoard::redo);
    connect(ui->white_TimeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::BLACK_WIN); });
    connect(ui->black_timeLabel, &TimeDisplay::timeout, this, [this]() { this->gameOver(GameManager::State::WHITE_WIN); });
    connect(ui->chessBoard, &ChessBoard::changedTurnSignal, this, &GameMainWindow::updateInfo);
    connect(ui->chessBoard, &ChessBoard::gameOver, this, &GameMainWindow::gameOver);
}

// Intent: destruct the object
// Pre: none
// Post: ui is deleted
GameMainWindow::~GameMainWindow()
{
    delete ui;
    delete pregameDialog;
}

// Intent: show window and dialog
// Pre: none
// Post: window and dialog are shown
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
    try {
        ui->chessBoard->load(setting.FEN);
    }
    catch (QString& str) {
        QMessageBox::warning(this, "FEN load failed", str);
        return;
    }

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

    // enable all action
    QList<QAction*> actionList = this->findChildren<QAction*>(QRegularExpression("^action"));
    for (auto it = actionList.begin(); it != actionList.end(); ++it) {
        (*it)->setEnabled(true);
    }
}

// Intend: 輸出遊戲結束的訊息
// Pre: none
// Post: 輸出遊戲結束的訊息
void GameMainWindow::gameOver(GameManager::State state)
{

    if (state == GameManager::State::PLAYING) {
        ui->actionPause->setDisabled(false);
        ui->actionSurrender->setDisabled(false);
        return;
    }

    ui->chessBoard->setState(state);
    ui->white_TimeLabel->stop();
    ui->black_timeLabel->stop();
    QString message = (state == GameManager::State::BLACK_WIN ? "<b>Black</b> Wins!!!!" :
                       state == GameManager::State::WHITE_WIN ? "<b>White</b> Wins!!!!" :
                                                                "Draw");
    QMessageBox::information(this, "Game Over", message);
    ui->actionPause->setDisabled(true);
    ui->actionSurrender->setDisabled(true);
}

// Intend: 將Info顯示為color那方
// Pre: none
// Post: Info顯示為color那方
void GameMainWindow::updateInfo(COLOR color)
{
    if (color == COLOR::WHITE) {
        ui->black_timeLabel->stop();
        ui->white_TimeLabel->start();
        ui->turnLabel->setText("White's Turn");
        ui->turnLabel->setStyleSheet("background-color: white; color: black; font: bold 35px alef;");
    }
    else {
        ui->white_TimeLabel->stop();
        ui->black_timeLabel->start();
        ui->turnLabel->setText("Black's Turn");
        ui->turnLabel->setStyleSheet("background-color: black; color: white; font: bold 35px alef;");
    }
    ui->roundLCD->display(ui->chessBoard->getFullMove());
    ui->halfmoveLCD->display(ui->chessBoard->getHalfMove());
    ui->checkedLabel->setVisible(ui->chessBoard->isChecked());
}

// Intend: 暫停遊戲
// Pre: none
// Post: 暫停遊戲
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

// Intend: 重新顯示pregame並重新開始
// Pre: none
// Post: 重新顯示pregame並重新開始
void GameMainWindow::newGame()
{
    ui->white_TimeLabel->stop();
    ui->black_timeLabel->stop();
    pregameDialog->open();
}

// Intend: 顯示一個視窗，上面有盤面的FEN
// Pre: none
// Post: 顯示一個視窗，上面有盤面的FEN
void GameMainWindow::showFEN()
{
    QWidget* w = new QWidget;
    w->setMinimumWidth(700);
    w->setWindowTitle("FEN");
    w->setPalette(this->palette());
    w->setFont(QFont("cascadia mono"));
    QHBoxLayout* v = new QHBoxLayout(w);

    v->addWidget(new QLabel("FEN: "));
    QLineEdit* line = new QLineEdit;
    line->setReadOnly(true);
    line->setText(ui->chessBoard->getCurrentFEN());
    v->addWidget(line);
    QPushButton* clip = new QPushButton;
    clip->setIcon(QIcon(":/chessPieces/clipboard.png"));
    connect(clip, &QPushButton::clicked, line, [line]() {
        QApplication::clipboard()->setText(line->text());
        line->setSelection(0, line->text().size());
    });
    v->addWidget(clip);

    w->show();
    w->setAttribute(Qt::WA_DeleteOnClose);
}

