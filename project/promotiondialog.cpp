
#include "promotiondialog.h"
#include <QDebug>

TYPE PromotionDialog::showDialog(COLOR color)
{
    QScopedPointer<PromotionDialog> w(new PromotionDialog(color));
    w->exec();
    qDebug() << (w->choose == TYPE::KNIGHT ? "Knight chosen." :
                 w->choose == TYPE::BISHOP ? "Bishop chosen." :
                 w->choose == TYPE::ROOK ?   "Rook chosen." :
                 w->choose == TYPE::QUEEN ?  "Queen chosen." :
                                             "There is a bug.");
    return w->choose;
}

PromotionDialog::PromotionDialog(COLOR color, QWidget *parent)
    : QDialog(parent), ui(new Ui::PromotionDialog), choose(TYPE::EMPTY)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowCloseButtonHint, false);
    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    // palette
    QPalette myPalette;
    myPalette.setColor(this->backgroundRole(), QColor(71, 68, 59));
    this->setPalette(myPalette);

    // button style
    this->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background-color: rgba(0,0,0,0);"
        "  padding: 6px;"
        "}"
        "QPushButton:hover {"
        "  border: 3px solid rgb(145, 145, 135);"
        "  border-radius: 6px;"
        "}"
        );

    if (color == COLOR::BLACK) {
        ui->stackedWidget->setCurrentIndex(1);
        connect(ui->bn, &QPushButton::clicked, this, [this]() { choose = TYPE::KNIGHT; this->accept(); });
        connect(ui->bb, &QPushButton::clicked, this, [this]() { choose = TYPE::BISHOP; this->accept(); });
        connect(ui->br, &QPushButton::clicked, this, [this]() { choose = TYPE::ROOK; this->accept(); });
        connect(ui->bq, &QPushButton::clicked, this, [this]() { choose = TYPE::QUEEN; this->accept(); });
    }
    else {
        ui->stackedWidget->setCurrentIndex(0);
        connect(ui->wn, &QPushButton::clicked, this, [this]() { choose = TYPE::KNIGHT; this->accept(); });
        connect(ui->wb, &QPushButton::clicked, this, [this]() { choose = TYPE::BISHOP; this->accept(); });
        connect(ui->wr, &QPushButton::clicked, this, [this]() { choose = TYPE::ROOK; this->accept(); });
        connect(ui->wq, &QPushButton::clicked, this, [this]() { choose = TYPE::QUEEN; this->accept(); });
    }
}
