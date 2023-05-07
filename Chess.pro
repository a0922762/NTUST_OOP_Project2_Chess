QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

win32-msvc: QMAKE_CXXFLAGS += /utf-8

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamemainwindow.cpp \
    main.cpp \
    pregame.cpp \
    timedisplay.cpp

HEADERS += \
    ChessPieces.h \
    GameManager.h \
    SettingProtocol.h \
    gamemainwindow.h \
    pregame.h \
    timedisplay.h

FORMS += \
    gamemainwindow.ui \
    pregame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
