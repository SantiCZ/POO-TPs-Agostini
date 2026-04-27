/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QLabel *label_2;
    QLabel *lclave;
    QSpacerItem *horizontalSpacer;
    QLineEdit *leclave;
    QLineEdit *leusuario;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(474, 317);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setObjectName("gridLayout_2");
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName("pushButton");

        gridLayout_2->addWidget(pushButton, 2, 1, 1, 1);

        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        lclave = new QLabel(Widget);
        lclave->setObjectName("lclave");

        gridLayout_2->addWidget(lclave, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 2, 1, 1);

        leclave = new QLineEdit(Widget);
        leclave->setObjectName("leclave");
        leclave->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_2->addWidget(leclave, 1, 1, 1, 2);

        leusuario = new QLineEdit(Widget);
        leusuario->setObjectName("leusuario");

        gridLayout_2->addWidget(leusuario, 0, 1, 1, 2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "clave", nullptr));
        lclave->setText(QCoreApplication::translate("Widget", "usuario", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
