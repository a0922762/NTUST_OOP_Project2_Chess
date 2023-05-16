
#include "pregame.h"
#include "ui_pregame.h"
#include <QCloseEvent>
#include <QDebug>
#include <QTabBar>
#include <QMessageBox>
#include <QRegularExpressionValidator>

PreGame::PreGame(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreGame)
    , FEN_validator(QRegularExpression("^[rnbqkpRNBQKP12345678/]+ [wb] (-|[KQkq]+) (-|[abcdefgh][12345678]) (\\d+) (\\d+)$"))
{
    ui->setupUi(this);
    connect(ui->startGame, &QPushButton::clicked, this, &PreGame::sendSetting);

    this->setFont(QFont("cascadia mono"));

    // 整體
    QPalette myPalette;
    myPalette.setColor(QPalette::Window, QColor(71, 68, 59));
    this->setPalette(myPalette);

    // tab內
    myPalette.setColor(QPalette::Window, QColor(92, 88, 78));
    myPalette.setColor(QPalette::WindowText, QColor(209, 202, 180));
    myPalette.setColor(QPalette::HighlightedText, QColor(255, 0, 0));
    myPalette.setColor(QPalette::Highlight, QColor(255, 255, 255));
    ui->tabWidget->setPalette(myPalette);
    ui->tabWidget->setDocumentMode(true); // 不要顯示邊框
    ui->tabWidget->tabBar()->setStyleSheet(
        "QTabBar::tab {background-color: rgb(133, 126, 114); color: rgb(209, 202, 180);}"
        "QTabBar::tab:selected {background-color: rgb(92, 88, 78); color: rgb(209, 202, 180);}"
        "QTabBar::tab:hover {border-bottom: 4px solid rgb(71, 68, 59);}"
        );
    // 讓tab內自動套上背景顏色
    ui->timeTab->setAutoFillBackground(true);
    ui->boardTab->setAutoFillBackground(true);

    // validator
    // FEN格式： <棋子擺放> <移動方> <可否入堡> <過路兵的目標> <halfmove clock> <fullmove number>
    // 為了方便輸入，所以對輸入用的validator做了一些調整
    ui->FEN_edit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^[rnbqkpRNBQKP12345678/]+ [wb]? (-|[KQkq]+)? (-|[abcdefgh][12345678]?)? (\\d*) (\\d*)$")
        ));
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

// Intend: 按下Esc則關閉程式
void PreGame::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Escape)
        qApp->quit();
}

// Intend: 將使用者輸入的設定送出（emit）
// Pre: 按下 startGame
void PreGame::sendSetting()
{
    SettingProtocol setting;

    // 設定棋盤類型
    if (ui->regular_radio->isChecked()) {
        setting.FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }
    else if (ui->regularBlack_radio->isChecked()) {
        setting.FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1";
    }
    else {
        setting.FEN = ui->FEN_edit->text();
        int pos = 0;
        if (FEN_validator.validate(setting.FEN, pos) != QValidator::Acceptable) {
            QString regex = FEN_validator.regularExpression().pattern();
            QMessageBox::warning(this, "FEN load failed",
                                 "The FEN code doesn't match the regular expression: " + regex);
            return;
        }
    }

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


