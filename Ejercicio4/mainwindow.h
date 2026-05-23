#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void solicitarDatos();
    void procesarRespuesta(QNetworkReply *reply);

private:
    void enviarOrden(QString urlStr);

    QNetworkAccessManager *manager;
    QTimer *timer;
    QHBoxLayout *layoutTablero;
};

#endif // MAINWINDOW_H