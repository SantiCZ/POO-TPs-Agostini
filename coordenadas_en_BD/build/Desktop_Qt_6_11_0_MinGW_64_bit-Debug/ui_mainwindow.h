/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionColorRojo;
    QAction *actionColorVerde;
    QAction *actionColorAzul;
    QAction *actionColorNegro;
    QAction *actionBorrador;
    QAction *actionDeshacer;
    QAction *actionBorrarTodo;
    QAction *actionAcercaDe;
    QWidget *centralwidget;
    QMenuBar *menuBar;
    QMenu *menuArchivo;
    QMenu *menuEditar;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 720);
        actionColorRojo = new QAction(MainWindow);
        actionColorRojo->setObjectName("actionColorRojo");
        actionColorVerde = new QAction(MainWindow);
        actionColorVerde->setObjectName("actionColorVerde");
        actionColorAzul = new QAction(MainWindow);
        actionColorAzul->setObjectName("actionColorAzul");
        actionColorNegro = new QAction(MainWindow);
        actionColorNegro->setObjectName("actionColorNegro");
        actionBorrador = new QAction(MainWindow);
        actionBorrador->setObjectName("actionBorrador");
        actionDeshacer = new QAction(MainWindow);
        actionDeshacer->setObjectName("actionDeshacer");
        actionBorrarTodo = new QAction(MainWindow);
        actionBorrarTodo->setObjectName("actionBorrarTodo");
        actionAcercaDe = new QAction(MainWindow);
        actionAcercaDe->setObjectName("actionAcercaDe");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1024, 26));
        menuArchivo = new QMenu(menuBar);
        menuArchivo->setObjectName("menuArchivo");
        menuEditar = new QMenu(menuBar);
        menuEditar->setObjectName("menuEditar");
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuArchivo->menuAction());
        menuBar->addAction(menuEditar->menuAction());
        menuArchivo->addAction(actionBorrarTodo);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionAcercaDe);
        menuEditar->addAction(actionDeshacer);
        toolBar->addAction(actionColorNegro);
        toolBar->addAction(actionColorRojo);
        toolBar->addAction(actionColorVerde);
        toolBar->addAction(actionColorAzul);
        toolBar->addAction(actionBorrador);
        toolBar->addSeparator();
        toolBar->addAction(actionDeshacer);
        toolBar->addAction(actionBorrarTodo);
        toolBar->addSeparator();
        toolBar->addAction(actionAcercaDe);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Pintura", nullptr));
        actionColorRojo->setText(QCoreApplication::translate("MainWindow", "\360\237\224\264 Rojo (R)", nullptr));
#if QT_CONFIG(tooltip)
        actionColorRojo->setToolTip(QCoreApplication::translate("MainWindow", "Color Rojo (tecla R)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionColorRojo->setShortcut(QCoreApplication::translate("MainWindow", "R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionColorVerde->setText(QCoreApplication::translate("MainWindow", "\360\237\237\242 Verde (G)", nullptr));
#if QT_CONFIG(tooltip)
        actionColorVerde->setToolTip(QCoreApplication::translate("MainWindow", "Color Verde (tecla G)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionColorVerde->setShortcut(QCoreApplication::translate("MainWindow", "G", nullptr));
#endif // QT_CONFIG(shortcut)
        actionColorAzul->setText(QCoreApplication::translate("MainWindow", "\360\237\224\265 Azul (B)", nullptr));
#if QT_CONFIG(tooltip)
        actionColorAzul->setToolTip(QCoreApplication::translate("MainWindow", "Color Azul (tecla B)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionColorAzul->setShortcut(QCoreApplication::translate("MainWindow", "B", nullptr));
#endif // QT_CONFIG(shortcut)
        actionColorNegro->setText(QCoreApplication::translate("MainWindow", "\342\232\253 Negro (K)", nullptr));
#if QT_CONFIG(tooltip)
        actionColorNegro->setToolTip(QCoreApplication::translate("MainWindow", "Color Negro (tecla K)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionColorNegro->setShortcut(QCoreApplication::translate("MainWindow", "K", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBorrador->setText(QCoreApplication::translate("MainWindow", "\342\254\234 Borrador (W)", nullptr));
#if QT_CONFIG(tooltip)
        actionBorrador->setToolTip(QCoreApplication::translate("MainWindow", "Borrador \342\200\224 color blanco (tecla W)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionBorrador->setShortcut(QCoreApplication::translate("MainWindow", "W", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDeshacer->setText(QCoreApplication::translate("MainWindow", "\342\206\251 Deshacer", nullptr));
#if QT_CONFIG(tooltip)
        actionDeshacer->setToolTip(QCoreApplication::translate("MainWindow", "Deshacer \303\272ltimo trazo (Ctrl+Z)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionDeshacer->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBorrarTodo->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221 Borrar todo", nullptr));
#if QT_CONFIG(tooltip)
        actionBorrarTodo->setToolTip(QCoreApplication::translate("MainWindow", "Borrar todo el lienzo (Escape)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionBorrarTodo->setShortcut(QCoreApplication::translate("MainWindow", "Escape", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAcercaDe->setText(QCoreApplication::translate("MainWindow", "\342\204\271 Acerca de", nullptr));
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
        menuEditar->setTitle(QCoreApplication::translate("MainWindow", "Editar", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Herramientas", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
