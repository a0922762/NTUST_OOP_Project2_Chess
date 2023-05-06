
#ifndef SETTINGPROTOCOL_H
#define SETTINGPROTOCOL_H
#include <QString>
#include <QTime>

/**
 * @brief 定義 PreGame 和 GameMainWindow 間傳遞資料的方式
 */
struct SettingProtocol {
    QString FEN;
    enum TimeLimitType_t {
        NO_LIMIT, LIMIT_PER_PLAYER
    } timeLimitType;
    QTime initTime;
};

#endif // SETTINGPROTOCOL_H
