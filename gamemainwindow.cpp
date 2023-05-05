
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"
#include "pregame.h"

// Intend: set up ui
GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui(new Ui::GameMainWindow)
    , pregameDialog(new PreGame(this))
{
    ui->setupUi(this);
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

