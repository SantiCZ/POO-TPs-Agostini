#include "ventana.h"
#include "configmanager.h"
#include "logger.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QResizeEvent>
#include <QApplication>
#include <QScreen>
#include <QProgressBar>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QFont>

Ventana::Ventana(NetworkManager* nm, QWidget* parent)
    : Pantalla(parent)
    , m_nm(nm)
    , m_replyImagen(nullptr)
    , m_lblFondo(nullptr)
    , m_lblCargando(nullptr)
    , m_progressBar(nullptr)
    , m_scroll(nullptr)
    , m_contenidoCV(nullptr)
    , m_imagenCargada(false)
{
}

void Ventana::inicializar()
{
    construirUI();
    aplicarEstilos();
    Logger::instancia().info("Ventana: Inicializada");
    emit pantallaLista();
}

void Ventana::actualizarUI()
{
    if (m_imagenCargada) aplicarFondo();
}

void Ventana::construirUI()
{
    setWindowTitle("Sistema de Gestión — Principal");
    setWindowState(Qt::WindowMaximized);

    // Fondo base oscuro
    QVBoxLayout* layPrincipal = new QVBoxLayout(this);
    layPrincipal->setContentsMargins(0, 0, 0, 0);
    layPrincipal->setSpacing(0);

    // Label de fondo — ocupa toda la ventana
    m_lblFondo = new QLabel(this);
    m_lblFondo->setObjectName("lblFondo");
    m_lblFondo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_lblFondo->setAlignment(Qt::AlignCenter);
    m_lblFondo->setScaledContents(false);

    // Overlay semitransparente sobre el fondo
    QWidget* overlay = new QWidget(m_lblFondo);
    overlay->setObjectName("overlay");
    overlay->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* layOverlay = new QVBoxLayout(overlay);
    layOverlay->setContentsMargins(0, 0, 0, 0);

    // Barra superior con loader
    m_lblCargando = new QLabel("⏳ Descargando imagen de fondo...");
    m_lblCargando->setObjectName("lblCargando");
    m_lblCargando->setAlignment(Qt::AlignCenter);

    m_progressBar = new QProgressBar();
    m_progressBar->setObjectName("progressBar");
    m_progressBar->setMaximumHeight(3);
    m_progressBar->setTextVisible(false);
    m_progressBar->setRange(0, 0); // indeterminado hasta tener total

    QWidget* barraLoader = new QWidget();
    barraLoader->setObjectName("barraLoader");
    QVBoxLayout* layLoader = new QVBoxLayout(barraLoader);
    layLoader->addWidget(m_lblCargando);
    layLoader->addWidget(m_progressBar);

    // ScrollArea con el CV
    m_scroll = new QScrollArea();
    m_scroll->setObjectName("scrollCV");
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_contenidoCV = crearSeccionCV();
    m_scroll->setWidget(m_contenidoCV);

    layOverlay->addWidget(barraLoader);
    layOverlay->addWidget(m_scroll, 1);

    layPrincipal->addWidget(m_lblFondo);

    // El overlay se posiciona sobre el fondo
    overlay->setGeometry(m_lblFondo->rect());
    connect(m_lblFondo, &QLabel::linkActivated, this, [](const QString&){});
}

void Ventana::resizeEvent(QResizeEvent* event)
{
    Pantalla::resizeEvent(event);

    if (m_lblFondo) {
        m_lblFondo->setGeometry(rect());

        // Redimensionar el overlay hijo
        if (m_lblFondo->children().count() > 0) {
            QWidget* overlay = qobject_cast<QWidget*>(m_lblFondo->children().first());
            if (overlay) overlay->setGeometry(m_lblFondo->rect());
        }
    }

    if (m_imagenCargada) aplicarFondo();
}

void Ventana::descargarImagenFondo()
{
    ConfigManager& cfg = ConfigManager::instancia();
    QString cachePath  = cfg.imagenCachePath();

    // Verificar cache local primero
    if (QFileInfo::exists(cachePath)) {
        Logger::instancia().info("Ventana: Imagen encontrada en cache -> " + cachePath);
        m_pixmapFondo = QPixmap(cachePath);
        if (!m_pixmapFondo.isNull()) {
            m_imagenCargada = true;
            m_lblCargando->setText("✓ Imagen cargada desde cache");
            m_progressBar->setVisible(false);
            aplicarFondo();
            emit imagenLista();
            return;
        }
    }

    // Descargar desde URL
    QString urlStr = cfg.imagenUrl();
    if (urlStr.isEmpty()) {
        Logger::instancia().advertencia("Ventana: URL de imagen no configurada");
        m_imagenCargada = true;
        m_lblCargando->setVisible(false);
        m_progressBar->setVisible(false);
        emit imagenLista();
        return;
    }

    Logger::instancia().info("Ventana: Descargando imagen desde -> " + urlStr);
    m_replyImagen = m_nm->get(QUrl(urlStr));

    // Connect para progreso de descarga
    connect(m_replyImagen, &QNetworkReply::downloadProgress,
            this,           &Ventana::onProgresoDescarga);

    // Connect para finalización
    connect(m_replyImagen, &QNetworkReply::finished,
            this, [this]() { onImagenDescargada(m_replyImagen); });
}

void Ventana::onProgresoDescarga(qint64 recibidos, qint64 total)
{
    if (total > 0) {
        m_progressBar->setRange(0, static_cast<int>(total));
        m_progressBar->setValue(static_cast<int>(recibidos));
        m_lblCargando->setText(QString("⏬ Descargando imagen... %1 / %2 KB")
                               .arg(recibidos / 1024)
                               .arg(total / 1024));
    }
}

void Ventana::onImagenDescargada(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Logger::instancia().error("Ventana: Error descargando imagen -> " + reply->errorString());
        m_lblCargando->setText("⚠ No se pudo descargar la imagen de fondo.");
        m_progressBar->setVisible(false);
        m_imagenCargada = true;
        emit imagenLista();
        reply->deleteLater();
        return;
    }

    QByteArray datos = reply->readAll();
    Logger::instancia().info(QString("Ventana: Imagen descargada (%1 KB)").arg(datos.size() / 1024));

    m_pixmapFondo = QPixmap();
    if (!m_pixmapFondo.loadFromData(datos)) {
        Logger::instancia().error("Ventana: No se pudo cargar la imagen descargada");
        m_lblCargando->setText("⚠ Imagen inválida.");
        m_imagenCargada = true;
        emit imagenLista();
        reply->deleteLater();
        return;
    }

    // Guardar en cache
    QString cachePath = ConfigManager::instancia().imagenCachePath();
    QDir().mkpath(QFileInfo(cachePath).absolutePath());
    QFile f(cachePath);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(datos);
        f.close();
        Logger::instancia().info("Ventana: Imagen guardada en cache -> " + cachePath);
    }

    m_lblCargando->setVisible(false);
    m_progressBar->setVisible(false);
    m_imagenCargada = true;
    aplicarFondo();
    emit imagenLista();

    reply->deleteLater();
}

void Ventana::aplicarFondo()
{
    if (m_pixmapFondo.isNull() || !m_lblFondo) return;

    // Escalar manteniendo relación de aspecto sin deformación
    QPixmap escalado = m_pixmapFondo.scaled(
        m_lblFondo->size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
    );
    m_lblFondo->setPixmap(escalado);
}

void Ventana::aplicarEstilos()
{
    setStyleSheet(R"(
        Ventana {
            background-color: #0a0a0f;
        }

        #lblFondo {
            background-color: #111827;
        }

        #overlay {
            background: transparent;
        }

        #barraLoader {
            background: rgba(0, 0, 0, 0.75);
            padding: 8px 20px;
        }

        #lblCargando {
            color: #94a3b8;
            font-size: 13px;
        }

        #progressBar {
            background: #1e293b;
            border: none;
            border-radius: 2px;
        }

        #progressBar::chunk {
            background: #3b82f6;
            border-radius: 2px;
        }

        #scrollCV {
            background: transparent;
        }

        #scrollCV QScrollBar:vertical {
            background: rgba(0, 0, 0, 0.3);
            width: 6px;
            border-radius: 3px;
        }

        #scrollCV QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 0.3);
            border-radius: 3px;
        }

        /* ── CV Cards ─────────────────────────────────────── */

        #cardPerfil {
            background: rgba(15, 23, 42, 0.92);
            border-radius: 20px;
            border: 1px solid rgba(255, 255, 255, 0.08);
        }

        #lblNombre {
            font-family: "Georgia", serif;
            font-size: 32px;
            font-weight: bold;
            color: #f8fafc;
        }

        #lblRol {
            font-size: 16px;
            color: #3b82f6;
            font-weight: 600;
        }

        #lblUbicacion {
            font-size: 13px;
            color: #64748b;
        }

        #lblBio {
            font-size: 14px;
            color: #94a3b8;
            line-height: 1.6;
        }

        #lblAvatar {
            border-radius: 55px;
            border: 3px solid #3b82f6;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1e3a5f, stop:1 #1e293b);
            color: #60a5fa;
            font-size: 48px;
        }

        #cardSeccion {
            background: rgba(15, 23, 42, 0.88);
            border-radius: 16px;
            border: 1px solid rgba(255, 255, 255, 0.06);
        }

        #lblSeccionTitulo {
            font-size: 11px;
            color: #475569;
            font-weight: bold;
            letter-spacing: 2px;
        }

        #cardExp {
            background: rgba(30, 41, 59, 0.6);
            border-radius: 12px;
            border-left: 3px solid #3b82f6;
        }

        #lblExpTitulo {
            font-size: 16px;
            font-weight: bold;
            color: #f1f5f9;
        }

        #lblExpEmpresa {
            font-size: 13px;
            color: #3b82f6;
            font-weight: 600;
        }

        #lblExpPeriodo {
            font-size: 12px;
            color: #64748b;
        }

        #lblExpDesc {
            font-size: 13px;
            color: #94a3b8;
            line-height: 1.5;
        }

        #badge {
            background: rgba(59, 130, 246, 0.15);
            color: #60a5fa;
            border: 1px solid rgba(59, 130, 246, 0.3);
            border-radius: 20px;
            font-size: 11px;
            padding: 3px 10px;
        }

        #badgeSkill {
            background: rgba(16, 185, 129, 0.12);
            color: #34d399;
            border: 1px solid rgba(16, 185, 129, 0.25);
            border-radius: 20px;
            font-size: 11px;
            padding: 3px 10px;
        }

        #statNum {
            font-size: 24px;
            font-weight: bold;
            color: #f1f5f9;
        }

        #statLabel {
            font-size: 11px;
            color: #64748b;
        }
    )");
}

QWidget* Ventana::crearSeccionCV()
{
    QWidget* contenedor = new QWidget();
    contenedor->setObjectName("contenedorCV");
    contenedor->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* lay = new QVBoxLayout(contenedor);
    lay->setContentsMargins(60, 40, 60, 60);
    lay->setSpacing(20);
    lay->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    // ── CARD PERFIL ──────────────────────────────────────────────────────
    QFrame* cardPerfil = new QFrame();
    cardPerfil->setObjectName("cardPerfil");
    cardPerfil->setMaximumWidth(900);
    QHBoxLayout* layPerfil = new QHBoxLayout(cardPerfil);
    layPerfil->setContentsMargins(36, 36, 36, 36);
    layPerfil->setSpacing(30);

    // Avatar con iniciales
    QLabel* lblAvatar = new QLabel("JG");
    lblAvatar->setObjectName("lblAvatar");
    lblAvatar->setFixedSize(110, 110);
    lblAvatar->setAlignment(Qt::AlignCenter);
    lblAvatar->setFont(QFont("Georgia", 28, QFont::Bold));

    // Info personal
    QVBoxLayout* layInfo = new QVBoxLayout();
    layInfo->setSpacing(4);

    QLabel* lblNombre = new QLabel("Juan García");
    lblNombre->setObjectName("lblNombre");

    QLabel* lblRol = new QLabel("Desarrollador de Software — Qt / C++ / Sistemas Embebidos");
    lblRol->setObjectName("lblRol");

    QLabel* lblUbi = new QLabel("📍 Córdoba, Argentina  ·  🎓 UTN FRC  ·  ✉ juan.garcia@email.com");
    lblUbi->setObjectName("lblUbicacion");

    QLabel* lblBio = new QLabel(
        "Desarrollador con sólida formación en C++ moderno y Qt Framework, especializado en "
        "aplicaciones de escritorio, sistemas embebidos e integración de APIs REST. "
        "Apasionado por el diseño de software limpio, patrones de diseño y la experiencia de usuario. "
        "Busco oportunidades donde combinar ingeniería de software con sistemas de tiempo real.");
    lblBio->setObjectName("lblBio");
    lblBio->setWordWrap(true);
    lblBio->setMaximumWidth(680);

    // Stats horizontales
    QHBoxLayout* layStats = new QHBoxLayout();
    layStats->setSpacing(30);
    auto crearStat = [](const QString& num, const QString& label) -> QWidget* {
        QWidget* w = new QWidget();
        QVBoxLayout* l = new QVBoxLayout(w);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(0);
        QLabel* n = new QLabel(num); n->setObjectName("statNum"); n->setAlignment(Qt::AlignCenter);
        QLabel* lbl = new QLabel(label); lbl->setObjectName("statLabel"); lbl->setAlignment(Qt::AlignCenter);
        l->addWidget(n); l->addWidget(lbl);
        return w;
    };
    layStats->addWidget(crearStat("3+", "Años exp."));
    layStats->addWidget(crearStat("12+", "Proyectos"));
    layStats->addWidget(crearStat("Qt 6", "Framework"));
    layStats->addStretch();

    layInfo->addWidget(lblNombre);
    layInfo->addWidget(lblRol);
    layInfo->addSpacing(4);
    layInfo->addWidget(lblUbi);
    layInfo->addSpacing(8);
    layInfo->addWidget(lblBio);
    layInfo->addSpacing(12);
    layInfo->addLayout(layStats);

    layPerfil->addWidget(lblAvatar, 0, Qt::AlignTop);
    layPerfil->addLayout(layInfo, 1);

    QGraphicsDropShadowEffect* s1 = new QGraphicsDropShadowEffect();
    s1->setBlurRadius(40); s1->setOffset(0, 8); s1->setColor(QColor(0, 0, 0, 80));
    cardPerfil->setGraphicsEffect(s1);
    lay->addWidget(cardPerfil, 0, Qt::AlignHCenter);

    // ── EXPERIENCIA ──────────────────────────────────────────────────────
    QFrame* cardExp = new QFrame();
    cardExp->setObjectName("cardSeccion");
    cardExp->setMaximumWidth(900);
    QVBoxLayout* layExp = new QVBoxLayout(cardExp);
    layExp->setContentsMargins(36, 28, 36, 28);
    layExp->setSpacing(16);

    QLabel* tituloExp = new QLabel("EXPERIENCIA");
    tituloExp->setObjectName("lblSeccionTitulo");
    layExp->addWidget(tituloExp);

    layExp->addWidget(crearTarjetaExp(
        "Desarrollador Qt Senior",
        "TechSoft Córdoba S.A.",
        "Mar 2023 — Presente",
        "Desarrollo de aplicaciones de escritorio multiplataforma con Qt 6 y C++17. "
        "Integración con APIs REST, manejo de QNetworkAccessManager, diseño de interfaces "
        "con QML y widgets. Implementación de patrones MVC y arquitectura por capas.",
        "Qt 6 · C++17 · QML · REST APIs · CMake · Git"
    ));

    layExp->addWidget(crearTarjetaExp(
        "Desarrollador Sistemas Embebidos",
        "AutoControl S.R.L.",
        "Ago 2022 — Feb 2023",
        "Programación de microcontroladores ARM Cortex-M con FreeRTOS. "
        "Comunicación UART, SPI, I2C y CAN Bus. Desarrollo de HMI con Qt Embedded "
        "para paneles industriales táctiles.",
        "C · C++ · FreeRTOS · Qt Embedded · ARM · CAN Bus"
    ));

    layExp->addWidget(crearTarjetaExp(
        "Pasante — Desarrollo de Software",
        "Universidad Tecnológica Nacional FRC",
        "Mar 2021 — Jul 2022",
        "Asistencia en proyectos de investigación en sistemas de control. "
        "Desarrollo de herramientas de análisis de señales en Python y C++. "
        "Documentación técnica y presentación de resultados.",
        "C++ · Python · Qt · MATLAB · LaTeX"
    ));

    QGraphicsDropShadowEffect* s2 = new QGraphicsDropShadowEffect();
    s2->setBlurRadius(30); s2->setOffset(0, 4); s2->setColor(QColor(0, 0, 0, 60));
    cardExp->setGraphicsEffect(s2);
    lay->addWidget(cardExp, 0, Qt::AlignHCenter);

    // ── EDUCACIÓN + SKILLS (lado a lado) ─────────────────────────────────
    QHBoxLayout* layFilaInf = new QHBoxLayout();
    layFilaInf->setSpacing(20);
    layFilaInf->setAlignment(Qt::AlignHCenter);

    // Educación
    QFrame* cardEdu = new QFrame();
    cardEdu->setObjectName("cardSeccion");
    cardEdu->setMaximumWidth(430);
    cardEdu->setMinimumWidth(300);
    QVBoxLayout* layEdu = new QVBoxLayout(cardEdu);
    layEdu->setContentsMargins(30, 24, 30, 24);
    layEdu->setSpacing(12);

    QLabel* tituloEdu = new QLabel("EDUCACIÓN");
    tituloEdu->setObjectName("lblSeccionTitulo");
    layEdu->addWidget(tituloEdu);

    auto agregarEdu = [&](const QString& titulo, const QString& inst, const QString& per) {
        QFrame* f = new QFrame(); f->setObjectName("cardExp");
        QVBoxLayout* l = new QVBoxLayout(f); l->setContentsMargins(16, 12, 16, 12); l->setSpacing(2);
        QLabel* t = new QLabel(titulo); t->setObjectName("lblExpTitulo"); t->setWordWrap(true);
        QLabel* i = new QLabel(inst);   i->setObjectName("lblExpEmpresa");
        QLabel* p = new QLabel(per);    p->setObjectName("lblExpPeriodo");
        l->addWidget(t); l->addWidget(i); l->addWidget(p);
        layEdu->addWidget(f);
    };

    agregarEdu("Ingeniería en Sistemas de Información",
               "UTN — Facultad Regional Córdoba",
               "2019 — 2024  ·  Promedio: 8.2");
    agregarEdu("Técnico en Electrónica",
               "IPET N° 70 — Córdoba",
               "2013 — 2018");

    QGraphicsDropShadowEffect* s3 = new QGraphicsDropShadowEffect();
    s3->setBlurRadius(25); s3->setOffset(0, 4); s3->setColor(QColor(0, 0, 0, 50));
    cardEdu->setGraphicsEffect(s3);

    // Skills
    QFrame* cardSkills = new QFrame();
    cardSkills->setObjectName("cardSeccion");
    cardSkills->setMaximumWidth(430);
    cardSkills->setMinimumWidth(300);
    QVBoxLayout* laySkills = new QVBoxLayout(cardSkills);
    laySkills->setContentsMargins(30, 24, 30, 24);
    laySkills->setSpacing(12);

    QLabel* tituloSk = new QLabel("HABILIDADES");
    tituloSk->setObjectName("lblSeccionTitulo");
    laySkills->addWidget(tituloSk);

    QStringList skillsLenguajes = {"C++17", "C", "Python", "SQL", "QML", "Bash"};
    QStringList skillsFram      = {"Qt 6", "STL", "Boost", "OpenCV", "FreeRTOS"};
    QStringList skillsHerr      = {"Git", "CMake", "GDB", "Valgrind", "Docker", "Linux"};

    auto agregarGrupo = [&](const QString& grupo, const QStringList& items) {
        QLabel* g = new QLabel(grupo);
        g->setObjectName("lblExpPeriodo");
        g->setStyleSheet("color: #475569; font-size: 11px; font-weight: bold;");
        laySkills->addWidget(g);
        QWidget* row = new QWidget();
        QHBoxLayout* l = new QHBoxLayout(row);
        l->setContentsMargins(0, 0, 0, 0); l->setSpacing(6);
        for (auto& s : items) {
            QLabel* b = new QLabel(s);
            b->setObjectName(grupo == "Lenguajes" ? "badge" : "badgeSkill");
            l->addWidget(b);
        }
        l->addStretch();
        laySkills->addWidget(row);
    };

    agregarGrupo("Lenguajes", skillsLenguajes);
    agregarGrupo("Frameworks", skillsFram);
    agregarGrupo("Herramientas", skillsHerr);

    QGraphicsDropShadowEffect* s4 = new QGraphicsDropShadowEffect();
    s4->setBlurRadius(25); s4->setOffset(0, 4); s4->setColor(QColor(0, 0, 0, 50));
    cardSkills->setGraphicsEffect(s4);

    layFilaInf->addWidget(cardEdu);
    layFilaInf->addWidget(cardSkills);

    QWidget* filaInfW = new QWidget();
    filaInfW->setAttribute(Qt::WA_TranslucentBackground);
    filaInfW->setMaximumWidth(900);
    filaInfW->setLayout(layFilaInf);
    lay->addWidget(filaInfW, 0, Qt::AlignHCenter);

    return contenedor;
}

QWidget* Ventana::crearTarjetaExp(const QString& titulo, const QString& empresa,
                                   const QString& periodo, const QString& descripcion,
                                   const QString& tecnologias)
{
    QFrame* card = new QFrame();
    card->setObjectName("cardExp");
    QVBoxLayout* lay = new QVBoxLayout(card);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(4);

    QLabel* lblTit = new QLabel(titulo);       lblTit->setObjectName("lblExpTitulo");
    QLabel* lblEmp = new QLabel(empresa);      lblEmp->setObjectName("lblExpEmpresa");
    QLabel* lblPer = new QLabel(periodo);      lblPer->setObjectName("lblExpPeriodo");
    QLabel* lblDesc = new QLabel(descripcion); lblDesc->setObjectName("lblExpDesc");
    lblDesc->setWordWrap(true);

    lay->addWidget(lblTit);
    lay->addWidget(lblEmp);
    lay->addWidget(lblPer);
    lay->addSpacing(6);
    lay->addWidget(lblDesc);
    lay->addSpacing(8);

    // Badges de tecnologías
    QWidget* rowBadges = new QWidget();
    QHBoxLayout* layB = new QHBoxLayout(rowBadges);
    layB->setContentsMargins(0, 0, 0, 0); layB->setSpacing(6);
    for (const QString& tec : tecnologias.split(" · ")) {
        layB->addWidget(crearBadge(tec));
    }
    layB->addStretch();
    lay->addWidget(rowBadges);

    return card;
}

QWidget* Ventana::crearBadge(const QString& texto)
{
    QLabel* badge = new QLabel(texto);
    badge->setObjectName("badge");
    return badge;
}
