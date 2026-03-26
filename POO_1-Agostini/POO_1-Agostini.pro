QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HistorialRepository.cpp \
    HistorialWidget.cpp \
    LoginWidget.cpp \
    MainWidget.cpp \
    SessionManager.cpp \
    TPEditorDialog.cpp \
    TPRepository.cpp \
    TPRowWidget.cpp \
    TrabajoPractico.cpp \
    Usuario.cpp \
    UsuarioRepository.cpp \
    main.cpp \


HEADERS += \
    Usuario.h\
    TrabajoPractico.h\
    AccionHistorial.h\
    UsuarioRepository.h\
    TPRepository.h\
    HistorialRepository.h\
    SessionManager.h\
    LoginWidget.h\
    MainWidget.h\
    TPEditorDialog.h\
    HistorialWidget.h\
    TPRowWidget.h\

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
