
#include "pregame.h"
#include "ui_pregame.h"


PreGame::PreGame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PreGame)
{
    ui->setupUi(this);
}

PreGame::~PreGame()
{
    delete ui;
}


