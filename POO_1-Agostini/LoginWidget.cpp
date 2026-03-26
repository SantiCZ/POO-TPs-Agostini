#include "LoginWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

LoginWidget::LoginWidget(UsuarioRepository* userRepo, QWidget* parent)
    : QWidget(parent), m_userRepo(userRepo) {
    setWindowTitle("Planificador de TPs");
    setFixedSize(360, 330);

    m_stack = new QStackedLayout(this);
    m_stack->addWidget(buildLoginPanel());
    m_stack->addWidget(buildRegisterPanel());
    m_stack->setCurrentIndex(0);
}

QWidget* LoginWidget::buildLoginPanel() {
    m_loginPanel = new QWidget;
    auto* lay = new QVBoxLayout(m_loginPanel);
    lay->setContentsMargins(36, 32, 36, 32);
    lay->setSpacing(8);

    auto* title = new QLabel("Planificador de TPs");
    title->setStyleSheet("font-size:20px; font-weight:bold; margin-bottom:4px;");
    lay->addWidget(title);

    auto* hint = new QLabel("Prueba: admin/admin  ·  alumno/1234");
    hint->setStyleSheet("color:#999; font-size:11px;");
    lay->addWidget(hint);
    lay->addSpacing(8);

    lay->addWidget(new QLabel("Usuario:"));
    m_loginUser = new QLineEdit;
    m_loginUser->setPlaceholderText("nombre de usuario");
    lay->addWidget(m_loginUser);

    lay->addWidget(new QLabel("Contraseña:"));
    m_loginPass = new QLineEdit;
    m_loginPass->setPlaceholderText("contraseña");
    m_loginPass->setEchoMode(QLineEdit::Password);
    lay->addWidget(m_loginPass);

    m_loginError = new QLabel;
    m_loginError->setStyleSheet("color:#c0392b; font-size:12px;");
    m_loginError->setWordWrap(true);
    lay->addWidget(m_loginError);
    lay->addSpacing(4);

    auto* btnLay = new QHBoxLayout;
    auto* btnEntrar = new QPushButton("Ingresar");
    auto* btnReg    = new QPushButton("Registrarse");
    btnEntrar->setDefault(true);
    btnLay->addWidget(btnEntrar);
    btnLay->addWidget(btnReg);
    lay->addLayout(btnLay);

    connect(btnEntrar,  &QPushButton::clicked,      this, &LoginWidget::onLogin);
    connect(btnReg,     &QPushButton::clicked,      this, &LoginWidget::onIrARegistro);
    connect(m_loginPass,&QLineEdit::returnPressed,  this, &LoginWidget::onLogin);
    return m_loginPanel;
}

QWidget* LoginWidget::buildRegisterPanel() {
    m_regPanel = new QWidget;
    auto* lay = new QVBoxLayout(m_regPanel);
    lay->setContentsMargins(36, 32, 36, 32);
    lay->setSpacing(8);

    auto* title = new QLabel("Crear cuenta");
    title->setStyleSheet("font-size:20px; font-weight:bold; margin-bottom:8px;");
    lay->addWidget(title);

    lay->addWidget(new QLabel("Usuario:"));
    m_regUser = new QLineEdit; m_regUser->setPlaceholderText("nombre de usuario");
    lay->addWidget(m_regUser);

    lay->addWidget(new QLabel("Contraseña (mín. 4 caracteres):"));
    m_regPass = new QLineEdit; m_regPass->setEchoMode(QLineEdit::Password);
    lay->addWidget(m_regPass);

    lay->addWidget(new QLabel("Repetir contraseña:"));
    m_regPass2 = new QLineEdit; m_regPass2->setEchoMode(QLineEdit::Password);
    lay->addWidget(m_regPass2);

    m_regError = new QLabel;
    m_regError->setStyleSheet("color:#c0392b; font-size:12px;");
    m_regError->setWordWrap(true);
    lay->addWidget(m_regError);
    lay->addSpacing(4);

    auto* btnLay  = new QHBoxLayout;
    auto* btnCrear = new QPushButton("Crear cuenta");
    auto* btnVolver= new QPushButton("Volver");
    btnLay->addWidget(btnCrear);
    btnLay->addWidget(btnVolver);
    lay->addLayout(btnLay);

    connect(btnCrear,  &QPushButton::clicked, this, &LoginWidget::onRegistrar);
    connect(btnVolver, &QPushButton::clicked, this, &LoginWidget::onIrALogin);
    return m_regPanel;
}

void LoginWidget::onLogin() {
    m_loginError->setStyleSheet("color:#c0392b; font-size:12px;");
    m_loginError->clear();
    QString user = m_loginUser->text().trimmed();
    QString pass = m_loginPass->text();
    if (user.isEmpty() || pass.isEmpty()) { m_loginError->setText("Completá todos los campos."); return; }
    if (!m_userRepo->validarCredenciales(user, pass)) { m_loginError->setText("Usuario o contraseña incorrectos."); return; }
    m_loginPass->clear();
    emit loginExitoso(user);
}

void LoginWidget::onRegistrar() {
    m_regError->clear();
    QString user  = m_regUser->text().trimmed();
    QString pass  = m_regPass->text();
    QString pass2 = m_regPass2->text();
    if (user.isEmpty() || pass.isEmpty())  { m_regError->setText("Completá todos los campos."); return; }
    if (pass.length() < 4)                 { m_regError->setText("Contraseña demasiado corta (mín. 4)."); return; }
    if (pass != pass2)                     { m_regError->setText("Las contraseñas no coinciden."); return; }
    if (m_userRepo->existe(user))          { m_regError->setText("Ese usuario ya existe."); return; }
    m_userRepo->agregar({user, pass});
    m_loginUser->setText(user);
    m_loginPass->clear();
    m_regUser->clear(); m_regPass->clear(); m_regPass2->clear();
    m_loginError->setStyleSheet("color:#27ae60; font-size:12px;");
    m_loginError->setText("Cuenta creada. Podés iniciar sesión.");
    m_stack->setCurrentIndex(0);
}

void LoginWidget::onIrARegistro() { m_regError->clear(); m_stack->setCurrentIndex(1); }
void LoginWidget::onIrALogin()    { m_loginError->setStyleSheet("color:#c0392b; font-size:12px;"); m_loginError->clear(); m_stack->setCurrentIndex(0); }
