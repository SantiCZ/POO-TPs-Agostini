#include "mainwindow.h"
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QUrl>
#include <QScrollArea>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Kanban Lolo Pro - Dashboard");
    resize(1200, 800);

    this->setStyleSheet("QWidget#mainRoot { background-color: #1e1e2f; }");
    setObjectName("mainRoot");

    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border: none; background-color: #1e1e2f; }");

    QWidget *contenido = new QWidget();
    layoutTablero = new QHBoxLayout(contenido);
    layoutTablero->setContentsMargins(20, 20, 20, 20);
    layoutTablero->setSpacing(25);

    scroll->setWidget(contenido);

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(scroll);

    manager = new QNetworkAccessManager(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::solicitarDatos);
    timer->start(3000);

    solicitarDatos();
}

void MainWindow::enviarOrden(QString urlStr) {
    QNetworkRequest request((QUrl(urlStr)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager->post(request, QByteArray());
}

void MainWindow::solicitarDatos() {
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://161.97.92.143:8001/tablero")));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            procesarRespuesta(reply);
        }
        reply->deleteLater();
    });
}

void limpiarLayoutInterno(QLayout *layout) {
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        } else if (QLayout *child = item->layout()) {
            limpiarLayoutInterno(child);
        }
        delete item;
    }
}

void MainWindow::procesarRespuesta(QNetworkReply *reply) {
    limpiarLayoutInterno(layoutTablero);

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = doc.object();
    QJsonArray columnas = root["columnas"].toArray();
    QJsonArray tarjetas = root["tarjetas"].toArray();

    for (int i = 0; i < columnas.size(); ++i) {
        QJsonObject col = columnas[i].toObject();
        int idCol = col["id"].toInt();
        QString nombreCol = col["name"].toString();

        QVBoxLayout *colLayout = new QVBoxLayout();

        QLabel *header = new QLabel(nombreCol.toUpper());
        header->setAlignment(Qt::AlignCenter);
        header->setStyleSheet("font-weight: bold; color: #a2a2c2; font-size: 18px; padding: 10px;");
        colLayout->addWidget(header);

        QPushButton *btnNew = new QPushButton("+ AGREGAR TAREA");
        btnNew->setCursor(Qt::PointingHandCursor);
        btnNew->setStyleSheet(
            "QPushButton { background-color: #3d3d5c; color: white; border-radius: 8px; padding: 8px; font-weight: bold; border: 1px solid #555; }"
            "QPushButton:hover { background-color: #4d4d7c; }"
            );
        colLayout->addWidget(btnNew);

        connect(btnNew, &QPushButton::clicked, [this, idCol]() {
            timer->stop();
            bool ok;
            QString txt = QInputDialog::getText(this, "Nueva", "Título:", QLineEdit::Normal, "", &ok);
            if (ok && !txt.isEmpty()) {
                enviarOrden(QString("http://161.97.92.143:8001/nueva_tarea?titulo=%1&columna_id=%2").arg(txt).arg(idCol));
            }
            timer->start(3000);
        });

        QString cardBgColor = "#ffffff";
        if (nombreCol.toLower().contains("pendiente")) cardBgColor = "#ff7675";
        else if (nombreCol.toLower().contains("proceso")) cardBgColor = "#ffeaa7";
        else if (nombreCol.toLower().contains("hecho")) cardBgColor = "#55efc4";

        for (const QJsonValue &vTar : tarjetas) {
            QJsonObject tar = vTar.toObject();
            if (tar["column_id"].toInt() == idCol) {
                int idTar = tar["id"].toInt();

                QWidget *card = new QWidget();
                card->setStyleSheet(QString("QWidget { background-color: %1; border-radius: 12px; margin-bottom: 10px; }").arg(cardBgColor));

                QVBoxLayout *cardInner = new QVBoxLayout(card);
                cardInner->setContentsMargins(15, 15, 15, 15);

                QLabel *titleLabel = new QLabel(tar["title"].toString());
                titleLabel->setAlignment(Qt::AlignCenter);
                titleLabel->setWordWrap(true);
                titleLabel->setStyleSheet("color: #2c3e50; font-weight: bold; font-size: 15px; background: transparent;");
                cardInner->addWidget(titleLabel);

                QHBoxLayout *btnRow = new QHBoxLayout();
                btnRow->setSpacing(10);
                btnRow->setAlignment(Qt::AlignCenter);

                QString styleBotones =
                    "QPushButton { "
                    "  background-color: white; "
                    "  color: black; "
                    "  border-radius: 10px; "
                    "  font-weight: bold; "
                    "  font-size: 16px; "
                    "  border: none; "
                    "} "
                    "QPushButton:hover { background-color: #f0f0f0; }";

                if (i < columnas.size() - 1) {
                    int idSiguiente = columnas[i+1].toObject()["id"].toInt();
                    QPushButton *m = new QPushButton("➜");
                    m->setFixedSize(55, 30);
                    m->setCursor(Qt::PointingHandCursor);
                    m->setStyleSheet(styleBotones);
                    btnRow->addWidget(m);
                    connect(m, &QPushButton::clicked, [this, idTar, idSiguiente]() {
                        enviarOrden(QString("http://161.97.92.143:8001/mover_tarjeta?tarjeta_id=%1&nueva_columna_id=%2").arg(idTar).arg(idSiguiente));
                    });
                }

                QPushButton *b = new QPushButton("✕");
                b->setFixedSize(55, 30);
                b->setCursor(Qt::PointingHandCursor);
                b->setStyleSheet(styleBotones);
                btnRow->addWidget(b);
                connect(b, &QPushButton::clicked, [this, idTar]() {
                    enviarOrden(QString("http://161.97.92.143:8001/borrar_tarjeta?tarjeta_id=%1").arg(idTar));
                });

                cardInner->addLayout(btnRow);
                colLayout->addWidget(card);
            }
        }
        colLayout->addStretch();
        layoutTablero->addLayout(colLayout);
    }
}