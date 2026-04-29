#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "admindb.h"
#include "logger.h"

namespace Ui { class LoginWindow; }

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(AdminDB *adminDB, QWidget *parent = nullptr);
    ~LoginWindow();

    QString usuarioLogueado() const { return m_usuario; }

private slots:
    void on_btnLogin_clicked();
    void on_btnCancelar_clicked();
    void on_lePassword_returnPressed();

private:
    bool intentarLogin();
    void mostrarError(const QString &msg);
    void sacudirVentana();

    Ui::LoginWindow *ui;
    AdminDB         *m_adminDB;
    QString          m_usuario;
    int              m_intentosFallidos = 0;
    static constexpr int MAX_INTENTOS = 5;
};

#endif // LOGINWINDOW_H
