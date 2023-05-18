
#include "timedisplay.h"

// Intend: construct
TimeDisplay::TimeDisplay(QWidget* parent, TimeDisplay::Type timerType)
    : QLabel(parent), currentTime(0, 0, 0), timeCounter(this), timerType(timerType)
{
    this->updateText();
    timeCounter.setInterval(1000);
    QObject::connect(&timeCounter, &QTimer::timeout, this, &TimeDisplay::update);
}

// Intend: 更新顯示的時間
void TimeDisplay::update()
{
    if (timerType == COUNTDOWN) {
        currentTime = currentTime.addSecs(-1);
        this->updateText();
        if (currentTime == QTime(0, 0, 0)) {  // 若倒數結束
            emit timeout();
            stop();
        }
    }
    else {
        currentTime = currentTime.addSecs(1);
        this->updateText();
    }
}

