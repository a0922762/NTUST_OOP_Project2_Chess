
#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include "ChessPieces.h"
#include "ui_promotiondialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class PromotionDialog; }
QT_END_NAMESPACE

/**
 * @brief 升變用Dialog
 * @details
 * 升變時會顯示，供玩家選擇升變成的目標
 */
class PromotionDialog : public QDialog
{
    Q_OBJECT
    Ui::PromotionDialog* ui;
    TYPE choose;

    // 建構子，color為顏色
    PromotionDialog(COLOR color, QWidget* parent = nullptr);

public:
    // Intend: 顯示Dialog，供玩家選擇
    // Pre: color - 移動方（依此改變顯示的棋子顏色）
    // Post: 選擇以TYPE回傳
    static TYPE showDialog(COLOR color);

    // 忽略按鍵
    void keyPressEvent(QKeyEvent* e) { e->ignore(); }
};

#endif // PROMOTIONDIALOG_H
