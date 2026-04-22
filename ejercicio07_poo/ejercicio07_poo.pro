QT += core gui widgets network

CONFIG += c++17

TARGET = ejercicio07
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/pantalla.cpp \
    src/login.cpp \
    src/ventana.cpp \
    src/clima.cpp \
    src/configmanager.cpp \
    src/logger.cpp \
    src/networkmanager.cpp

HEADERS += \
    src/pantalla.h \
    src/login.h \
    src/ventana.h \
    src/clima.h \
    src/configmanager.h \
    src/logger.h \
    src/networkmanager.h

DISTFILES += \
    config/config.ini

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
