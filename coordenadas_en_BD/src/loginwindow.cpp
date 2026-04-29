#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>

LoginWindow::LoginWindow(AdminDB *adminDB, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
    , m_adminDB(adminDB)
{
    ui->setupUi(this);
    setWindowTitle("coordenadas_en_BD — Login");
    setFixedSize(sizeHint());

    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->lblError->setVisible(false);
    ui->lblError->setStyleSheet("color: #d32f2f; font-weight: bold;");

    setStyleSheet(R"(
        QDialog { background-color: #1e1e2e; }
        QLabel  { color: #cdd6f4; font-size: 13px; }
        QLabel#lblTitulo    { font-size: 22px; font-weight: bold; color: #89dceb; }
        QLabel#lblSubtitulo { font-size: 11px; color: #6c7086; }
        QLineEdit {
            background-color: #313244; color: #cdd6f4;
            border: 1px solid #45475a; border-radius: 6px;
            padding: 8px 12px; font-size: 13px;
        }
        QLineEdit:focus { border-color: #89dceb; }
        QPushButton#btnLogin {
            background-color: #89dceb; color: #1e1e2e;
            border: none; border-radius: 6px;
            padding: 9px 0; font-size: 14px; font-weight: bold;
        }
        QPushButton#btnLogin:hover   { background-color: #74c7ec; }
        QPushButton#btnLogin:pressed { background-color: #89b4fa; }
        QPushButton#btnCancelar {
            background-color: transparent; color: #6c7086;
            border: 1px solid #45475a; border-radius: 6px; padding: 9px 0;
        }
        QPushButton#btnCancelar:hover { color: #cdd6f4; border-color: #6c7086; }
    )");

    ui->leUsuario->setFocus();
}

LoginWindow::~LoginWindow() { delete ui; }

// ─────────────────────────────────────────────────────────────────
bool LoginWindow::intentarLogin()
{
    QString usuario = ui->leUsuario->text().trimmed();
    QString clave   = ui->lePassword->text();

    if (usuario.isEmpty() || clave.isEmpty()) {
        mostrarError("Complete usuario y contrasena");
        return false;
    }

    // hashear sha-256 — nunca comparamos la clave en texto plano
    QString claveHash = QCryptographicHash::hash(
                            clave.toUtf8(), QCryptographicHash::Sha256
                            ).toHex();

    bool valido = m_adminDB->validarUsuario(usuario, claveHash);

    if (valido) {
        m_usuario = usuario;

        // registrar acceso exitoso (sin la clave)
        m_adminDB->registrar("LOGIN_EXITOSO usuario='" + usuario + "'");
        Logger::instance().info(usuario, "Login exitoso");

        return true;

    } else {
        m_intentosFallidos++;

        // registrar intento fallido (solo usuario, sin clave)
        m_adminDB->registrar(
            QString("LOGIN_FALLIDO usuario='%1' intento=%2")
                .arg(usuario).arg(m_intentosFallidos)
            );
        Logger::instance().warn(usuario,
                                QString("Login fallido — intento %1").arg(m_intentosFallidos));

        if (m_intentosFallidos >= MAX_INTENTOS) {
            mostrarError("Demasiados intentos. Cerrando.");
            Logger::instance().error(usuario, "Cuenta bloqueada por exceso de intentos");
            QTimer::singleShot(1500, this, &LoginWindow::reject);
        } else {
            mostrarError(
                QString("Usuario o contrasena incorrectos (%1/%2 intentos)")
                    .arg(m_intentosFallidos).arg(MAX_INTENTOS)
                );
            sacudirVentana();
        }
        return false;
    }
}

// ─────────────────────────────────────────────────────────────────
void LoginWindow::mostrarError(const QString &msg)
{
    ui->lblError->setText(msg);
    ui->lblError->setVisible(true);
    ui->lePassword->clear();
    ui->lePassword->setFocus();
}

void LoginWindow::sacudirVentana()
{
    QPoint origen = pos();
    auto *anim = new QSequentialAnimationGroup(this);
    for (int i = 0; i < 5; ++i) {
        auto *r = new QPropertyAnimation(this, "pos");
        r->setDuration(30);
        r->setStartValue(origen);
        r->setEndValue(origen + QPoint(10, 0));
        auto *l = new QPropertyAnimation(this, "pos");
        l->setDuration(30);
        l->setStartValue(origen + QPoint(10, 0));
        l->setEndValue(origen - QPoint(10, 0));
        anim->addAnimation(r);
        anim->addAnimation(l);
    }
    auto *reset = new QPropertyAnimation(this, "pos");
    reset->setDuration(30);
    reset->setStartValue(pos());
    reset->setEndValue(origen);
    anim->addAnimation(reset);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::on_btnLogin_clicked()       { if (intentarLogin()) accept(); }
void LoginWindow::on_btnCancelar_clicked()    { reject(); }
void LoginWindow::on_lePassword_returnPressed(){ on_btnLogin_clicked(); }