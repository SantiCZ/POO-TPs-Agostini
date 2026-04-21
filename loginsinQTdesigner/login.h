#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
class LoginWindow : public QWidget {
public:
    LoginWindow() {
        userInput = new QLineEdit(this);
        passInput = new QLineEdit(this);
        loginButton = new QPushButton("iniciar sesion", this);
        userInput->setPlaceholderText("usuario");
        passInput->setPlaceholderText("contrasena");
        passInput->setEchoMode(QLineEdit::Password);
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(userInput);
        layout->addWidget(passInput);
        layout->addWidget(loginButton);
        setLayout(layout);
        setWindowTitle("login");
        connect(loginButton, &QPushButton::clicked, this, &LoginWindow::verificarLogin);
    }
private:
    QLineEdit *userInput;
    QLineEdit *passInput;
    QPushButton *loginButton;
    void verificarLogin() {
        QString user = userInput->text();
        QString pass = passInput->text();
        if (user == "admin" && pass == "1234") {
            QMessageBox::information(this, "login", "acceso correcto");
        } else {
            QMessageBox::warning(this, "login", "usuario incorrecto");
        }
    }
};

#endif