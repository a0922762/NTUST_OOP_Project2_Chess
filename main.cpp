/*****************************************************************//**
 * File: main.cpp
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: the enter point of the program
 *********************************************************************/
#include "gamemainwindow.h"

#include <QApplication>
#include <ChessBoard.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameMainWindow* w = new GameMainWindow;
    w->show();
//    ChessBoard* board = new ChessBoard(nullptr);

//    w->show();

    return a.exec();
}
