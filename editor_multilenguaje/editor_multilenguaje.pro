QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = editor_multilenguaje
TEMPLATE = app

SOURCES += \
    main.cpp \
    pantalla.cpp \
    login.cpp \
    editorprincipal.cpp \
    modobloqueado.cpp \
    validadorcpp.cpp \
    validadorpython.cpp \
    validadorjava.cpp \
    panelcurriculum.cpp \
    gestorlog.cpp \
    gestorconfig.cpp \
    exportadorjpg.cpp

HEADERS += \
    pantalla.h \
    login.h \
    editorprincipal.h \
    modobloqueado.h \
    validadorsintaxis.h \
    validadorcpp.h \
    validadorpython.h \
    validadorjava.h \
    panelcurriculum.h \
    gestorlog.h \
    gestorconfig.h \
    exportadorjpg.h

RESOURCES += recursos.qrc