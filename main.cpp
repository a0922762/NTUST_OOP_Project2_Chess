
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
