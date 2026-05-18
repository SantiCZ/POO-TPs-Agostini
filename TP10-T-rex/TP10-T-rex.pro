QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET   = TP10-T.rex
TEMPLATE = app

SOURCES += \
    main.cpp        \
    manager.cpp     \
    gamewindow.cpp  \
    trex.cpp        \
    pajaro.cpp

HEADERS += \
    manager.h       \
    gamewindow.h    \
    pajaro.h \
    trex.h          \
    pajaro.h

#
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target