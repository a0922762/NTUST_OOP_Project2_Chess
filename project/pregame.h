/*****************************************************************//**
 * File: pregame.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: this is prgame window 
 *********************************************************************/
#ifndef PREGAME_H
#define PREGAME_H

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include <QRegularExpressionValidator>
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
    // 若點右上角的X關閉視窗，則離開遊戲
    void closeEvent(QCloseEvent *) override;
    // 按下ESC，等同按下右上角的X
    void keyPressEvent(QKeyEvent*) override;

private slots:
    // 統整使用者的設定，並透過signal送出
    void sendSetting();

public slots:
    // 開啟視窗，預設result為Rejected
    void open() override { setResult(QDialog::Rejected); QDialog::open(); }

signals:
    void startButtonClicked(SettingProtocol setting);

private:
    Ui::PreGame *ui;
    QRegularExpressionValidator FEN_validator;
};

#endif // PREGAME_H
