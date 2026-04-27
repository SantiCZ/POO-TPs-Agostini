#include "panelcurriculum.h"
#include <QFrame>
#include <QScrollArea>

PanelCurriculum::PanelCurriculum(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(260);
    setStyleSheet(R"(
        QWidget {
            background-color: #13131f;
        }
    )");
    construirPanel();
}

void PanelCurriculum::construirPanel()
{
    // Área con scroll para el CV
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("background: transparent; border: none;");

    QWidget *contenedor = new QWidget();
    contenedor->setStyleSheet("background: transparent;");
    m_layout = new QVBoxLayout(contenedor);
    m_layout->setContentsMargins(16, 20, 16, 20);
    m_layout->setSpacing(16);

    // ---- Foto de perfil (círculo con inicial) ----
    QLabel *foto = new QLabel(contenedor);
    foto->setFixedSize(90, 90);
    foto->setAlignment(Qt::AlignCenter);
    foto->setStyleSheet(R"(
        background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
            stop:0 #4040cc, stop:1 #8030ff);
        border-radius: 45px;
        color: white;
        font-size: 36px;
        font-weight: bold;
        font-family: 'Segoe UI';
    )");
    foto->setText("A");

    QHBoxLayout *hFoto = new QHBoxLayout();
    hFoto->addStretch();
    hFoto->addWidget(foto);
    hFoto->addStretch();
    m_layout->addLayout(hFoto);

    // ---- Nombre y título ----
    auto mkLabel = [&](const QString &texto, const QString &estilo) -> QLabel* {
        QLabel *l = new QLabel(texto, contenedor);
        l->setWordWrap(true);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(estilo);
        return l;
    };

    m_layout->addWidget(mkLabel("Admin Developer",
                                "color:#e0e0ff; font-size:16px; font-weight:bold; font-family:'Segoe UI';"));
    m_layout->addWidget(mkLabel("Desarrollador de Software | C++ · Python · Java",
                                "color:#8080bb; font-size:12px; font-family:'Segoe UI';"));

    // ---- Separador ----
    auto sep = [&]() -> QFrame* {
        QFrame *f = new QFrame(contenedor);
        f->setFrameShape(QFrame::HLine);
        f->setStyleSheet("color: #2a2a4a;");
        return f;
    };
    m_layout->addWidget(sep());

    // ---- Descripción ----
    m_layout->addWidget(mkLabel("Acerca de mí",
                                "color:#9090cc; font-size:13px; font-weight:bold; font-family:'Segoe UI';"));
    m_layout->addWidget(mkLabel(
        "Apasionado por el desarrollo de software multiplataforma. "
        "Especializado en aplicaciones de escritorio con Qt/C++ y sistemas "
        "con arquitectura orientada a objetos. Comprometido con el código "
        "limpio y el aprendizaje continuo.",
        "color:#a0a0cc; font-size:12px; font-family:'Segoe UI'; line-height:1.5;"));

    m_layout->addWidget(sep());

    // ---- Habilidades ----
    m_layout->addWidget(mkLabel("Habilidades",
                                "color:#9090cc; font-size:13px; font-weight:bold; font-family:'Segoe UI';"));

    struct Skill { QString nombre; int nivel; };
    QList<Skill> skills = {
        {"C++ / Qt", 90}, {"Python", 80}, {"Java", 75},
        {"POO / Patrones", 85}, {"Git", 78}, {"Linux", 70}
    };

    for (const Skill &sk : skills) {
        QLabel *lblSkill = new QLabel(sk.nombre, contenedor);
        lblSkill->setStyleSheet("color:#c0c0ee; font-size:12px; font-family:'Segoe UI';");

        QFrame *barraFondo = new QFrame(contenedor);
        barraFondo->setFixedHeight(7);
        barraFondo->setStyleSheet("background:#1e1e38; border-radius:3px;");

        QFrame *barraRelleno = new QFrame(barraFondo);
        barraRelleno->setFixedHeight(7);
        int ancho = static_cast<int>((228.0 * sk.nivel) / 100.0);
        barraRelleno->setFixedWidth(ancho);
        barraRelleno->setStyleSheet(
            "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
            "stop:0 #4040cc, stop:1 #8030ff); border-radius:3px;");

        m_layout->addWidget(lblSkill);
        m_layout->addWidget(barraFondo);
    }

    m_layout->addWidget(sep());

    // ---- Educación ----
    m_layout->addWidget(mkLabel("Educación",
                                "color:#9090cc; font-size:13px; font-weight:bold; font-family:'Segoe UI';"));
    m_layout->addWidget(mkLabel("📚 Tecnicatura en Programación",
                                "color:#c0c0ee; font-size:12px; font-family:'Segoe UI';"));
    m_layout->addWidget(mkLabel("Universidad Nacional de Córdoba · 2023–Presente",
                                "color:#7070aa; font-size:11px; font-family:'Segoe UI';"));

    m_layout->addWidget(sep());

    // ---- Contacto ----
    m_layout->addWidget(mkLabel("Contacto",
                                "color:#9090cc; font-size:13px; font-weight:bold; font-family:'Segoe UI';"));

    auto contactRow = [&](const QString &icono, const QString &dato) {
        QLabel *l = new QLabel(icono + "  " + dato, contenedor);
        l->setStyleSheet("color:#a0a0cc; font-size:12px; font-family:'Segoe UI';");
        l->setWordWrap(true);
        m_layout->addWidget(l);
    };

    contactRow("📧", "admin@editorml.com");
    contactRow("🌐", "github.com/admindev");
    contactRow("📍", "Córdoba, Argentina");
    contactRow("💼", "linkedin.com/in/admindev");

    m_layout->addStretch();
    contenedor->setLayout(m_layout);

    scroll->setWidget(contenedor);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scroll);
    setLayout(mainLayout);
}
