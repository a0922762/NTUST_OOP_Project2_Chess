
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>

class PreGame;

QT_BEGIN_NAMESPACE
namespace Ui { class GameMainWindow; }
QT_END_NAMESPACE

class GameMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameMainWindow(QWidget *parent = nullptr);
    ~GameMainWindow();

public slots:
    void show();

signals:

private:
    Ui::GameMainWindow* ui;
    PreGame* pregameDialog;
};

#endif // GAMEMAINWINDOW_H
