
#include "pregame.h"
#include "ui_pregame.h"
#include <QCloseEvent>
#include <QDebug>


PreGame::PreGame(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreGame)
{
    ui->setupUi(this);
    connect(ui->startGame, &QPushButton::clicked, this, &PreGame::sendSetting);
}

PreGame::~PreGame()
{
    delete ui;
}

// Intend: 若用戶直接把它關掉，結束程式
void PreGame::closeEvent(QCloseEvent * e)
{
    qDebug() << this << "closed with result: " << this->result();
    if (this->result() != QDialog::Accepted) {
        qApp->quit();
    }
    e->accept();
}

// Intend: 將使用者輸入的設定送出（emit）
// Pre: 按下 startGame
void PreGame::sendSetting()
{
    SettingProtocol setting;

    // 設定棋盤類型
    setting.FEN = (ui->regular_radio->isChecked() ? "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
                                                  : ui->FEN_edit->text());

    // 設定計時類型
    if (ui->noTimeLimit_radio->isChecked()) {
        setting.timeLimitType = SettingProtocol::NO_LIMIT;
        setting.initTime = QTime(0 , 0, 0);
    }
    else {
        setting.timeLimitType = SettingProtocol::LIMIT_PER_PLAYER;
        setting.initTime = QTime(ui->hour->value(), ui->minute->value(), ui->second->value());
    }

    emit startButtonClicked(setting);
}


