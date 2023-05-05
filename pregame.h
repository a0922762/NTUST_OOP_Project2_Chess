
#ifndef PREGAME_H
#define PREGAME_H

#include <QWidget>
#include <QDialog>



QT_BEGIN_NAMESPACE
namespace Ui { class PreGame; }
QT_END_NAMESPACE

class PreGame : public QDialog

{
    Q_OBJECT

public:
    explicit PreGame(QWidget *parent = nullptr);
    ~PreGame();
    virtual void closeEvent(QCloseEvent *) override;

public slots:

signals:
    void startButtonClicked(QString FEN);

private:
    Ui::PreGame *ui;
};

#endif // PREGAME_H
