QT += core gui widgets sql

CONFIG += c++17

QMAKE_CXXFLAGS += -Wall -Wextra

TARGET   = coordenadas_en_BD
TEMPLATE = app

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/admindb.cpp \
    src/mainwindow.cpp \
    src/loginwindow.cpp \
    src/pintura.cpp \
    src/logger.cpp

HEADERS += \
    src/admindb.h \
    src/mainwindow.h \
    src/loginwindow.h \
    src/pintura.h \
    src/logger.h

FORMS += \
    ui/loginwindow.ui \
    ui/mainwindow.ui



CONFIG += console

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target