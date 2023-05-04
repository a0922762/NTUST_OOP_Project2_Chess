
#ifndef PREGAME_H
#define PREGAME_H

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class PreGame; }
QT_END_NAMESPACE

class PreGame : public QWidget

{
    Q_OBJECT

public:
    PreGame(QWidget *parent = nullptr);
    ~PreGame();

private:
    Ui::PreGame *ui;
};

#endif // PREGAME_H
