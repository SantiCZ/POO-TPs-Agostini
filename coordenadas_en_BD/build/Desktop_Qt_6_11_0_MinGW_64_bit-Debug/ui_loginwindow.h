/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTitulo;
    QLabel *lblSubtitulo;
    QSpacerItem *spacer1;
    QLabel *lblUsuario;
    QLineEdit *leUsuario;
    QLabel *lblPassword;
    QLineEdit *lePassword;
    QLabel *lblError;
    QSpacerItem *spacer2;
    QPushButton *btnLogin;
    QPushButton *btnCancelar;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(380, 340);
        LoginWindow->setModal(true);
        verticalLayout = new QVBoxLayout(LoginWindow);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(32, 32, 32, 28);
        lblTitulo = new QLabel(LoginWindow);
        lblTitulo->setObjectName("lblTitulo");
        lblTitulo->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblTitulo);

        lblSubtitulo = new QLabel(LoginWindow);
        lblSubtitulo->setObjectName("lblSubtitulo");
        lblSubtitulo->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblSubtitulo);

        spacer1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(spacer1);

        lblUsuario = new QLabel(LoginWindow);
        lblUsuario->setObjectName("lblUsuario");

        verticalLayout->addWidget(lblUsuario);

        leUsuario = new QLineEdit(LoginWindow);
        leUsuario->setObjectName("leUsuario");
        leUsuario->setMaxLength(64);

        verticalLayout->addWidget(leUsuario);

        lblPassword = new QLabel(LoginWindow);
        lblPassword->setObjectName("lblPassword");

        verticalLayout->addWidget(lblPassword);

        lePassword = new QLineEdit(LoginWindow);
        lePassword->setObjectName("lePassword");
        lePassword->setEchoMode(QLineEdit::Password);
        lePassword->setMaxLength(128);

        verticalLayout->addWidget(lePassword);

        lblError = new QLabel(LoginWindow);
        lblError->setObjectName("lblError");
        lblError->setAlignment(Qt::AlignCenter);
        lblError->setWordWrap(true);

        verticalLayout->addWidget(lblError);

        spacer2 = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(spacer2);

        btnLogin = new QPushButton(LoginWindow);
        btnLogin->setObjectName("btnLogin");

        verticalLayout->addWidget(btnLogin);

        btnCancelar = new QPushButton(LoginWindow);
        btnCancelar->setObjectName("btnCancelar");

        verticalLayout->addWidget(btnCancelar);


        retranslateUi(LoginWindow);

        btnLogin->setDefault(true);


        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        lblTitulo->setText(QCoreApplication::translate("LoginWindow", "\360\237\216\250 Pintura", nullptr));
        lblSubtitulo->setText(QCoreApplication::translate("LoginWindow", "Ingrese sus credenciales para continuar", nullptr));
        lblUsuario->setText(QCoreApplication::translate("LoginWindow", "Usuario", nullptr));
        leUsuario->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Ingrese su usuario", nullptr));
        lblPassword->setText(QCoreApplication::translate("LoginWindow", "Contrase\303\261a", nullptr));
        lePassword->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Ingrese su contrase\303\261a", nullptr));
        lblError->setText(QString());
        btnLogin->setText(QCoreApplication::translate("LoginWindow", "Iniciar Sesi\303\263n", nullptr));
        btnCancelar->setText(QCoreApplication::translate("LoginWindow", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
