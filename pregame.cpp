
#include "pregame.h"
#include "ui_pregame.h"
#include <QCloseEvent>
#include <QDebug>


PreGame::PreGame(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreGame)
{
    ui->setupUi(this);
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


