#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QStackedLayout>
#include "UsuarioRepository.h"

class LoginWidget : public QWidget {
    Q_OBJECT
public:
    explicit LoginWidget(UsuarioRepository* userRepo, QWidget* parent = nullptr);

signals:
    void loginExitoso(const QString& username);

private slots:
    void onLogin();
    void onRegistrar();
    void onIrARegistro();
    void onIrALogin();

private:
    UsuarioRepository* m_userRepo;

    QWidget*        m_loginPanel;
    QLineEdit*      m_loginUser;
    QLineEdit*      m_loginPass;
    QLabel*         m_loginError;

    QWidget*        m_regPanel;
    QLineEdit*      m_regUser;
    QLineEdit*      m_regPass;
    QLineEdit*      m_regPass2;
    QLabel*         m_regError;

    QStackedLayout* m_stack;

    QWidget* buildLoginPanel();
    QWidget* buildRegisterPanel();
};
