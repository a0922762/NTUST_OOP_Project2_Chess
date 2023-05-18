/*****************************************************************//**
 * File: timedisplay.h
 * Author: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Create Date: 2023-05-17
 * Editor: TENG-FENG YOU (vic.feng1234@gmail.com)
 * Update Date: 2023-05-17
 * Description: this is a time display
 *********************************************************************/
#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

/**
 * @brief 顯示+計時用的QLabel
 * @details
 * 該class有兩種計時模式倒數（COUNTDOWN）、和計時（TIMING），定義在enum TimeDisplay::Type中。
 * 倒數模式會從目前時間（setTime設定的值）倒數，直到時間為`00:00:00`，結束時發出timeout的訊號。
 * 計時模式則是從目前時間開始計時。
 */
class TimeDisplay : public QLabel
{
    Q_OBJECT
public:
    enum Type {COUNTDOWN /** 倒數 */, TIMING /** 計時 */};

public:
    TimeDisplay(QWidget* parent = nullptr, TimeDisplay::Type timerType = TimeDisplay::TIMING);
    void setTime(const QTime& rhs) { currentTime = rhs; updateText(); }
    void setType(TimeDisplay::Type type) { timerType = type; }

public slots:
    void start() { timeCounter.start(); }
    void stop() { timeCounter.stop(); }
    void updateText() { QLabel::setText(currentTime.toString("HH:mm:ss"));}

private slots:
    void update();

signals:
    void timeout();

private:
    QTime currentTime;
    QTimer timeCounter;
    Type timerType;
};

#endif // TIMEDISPLAY_H
