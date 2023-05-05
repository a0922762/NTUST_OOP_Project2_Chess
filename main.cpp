
#include "gamemainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameMainWindow* w = new GameMainWindow;
    w->show();
    return a.exec();
}
