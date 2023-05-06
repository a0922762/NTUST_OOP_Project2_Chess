
#ifndef PREGAME_H
#define PREGAME_H

#include <QWidget>
#include <QDialog>
#include "SettingProtocol.h"



QT_BEGIN_NAMESPACE
namespace Ui { class PreGame; }
QT_END_NAMESPACE


/**
 * @brief 在遊戲前跳出的Dialog，供玩家設定
 * @details
 * 可以設定棋盤的佈局，和計時方式。
 * 若玩家直接關掉該視窗，則視為離開遊戲。
 * 按下start game時，發出startButtonClicked的訊號
 */
class PreGame : public QDialog

{
    Q_OBJECT

public:
    explicit PreGame(QWidget *parent = nullptr);
    ~PreGame();
    virtual void closeEvent(QCloseEvent *) override;

private slots:
    void sendSetting();

signals:
    void startButtonClicked(SettingProtocol setting);

private:
    Ui::PreGame *ui;
};

#endif // PREGAME_H
