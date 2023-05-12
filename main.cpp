
#include "gamemainwindow.h"

#include <QApplication>
#include <ChessBoard.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameMainWindow* w = new GameMainWindow;
    ChessBoard* board = new ChessBoard(nullptr);

    board->show();

    return a.exec();
}
