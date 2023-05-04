
#include "pregame.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PreGame w;
    w.show();
    return a.exec();
}
