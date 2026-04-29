#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pintura.h"
#include "admindb.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AdminDB *adminDB, const QString &usuario, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onColorCambiado(const QColor &c);
    void onGrosorCambiado(int g);
    void onStatusMessage(const QString &msg);

    void on_actionColorRojo_triggered();
    void on_actionColorVerde_triggered();
    void on_actionColorAzul_triggered();
    void on_actionColorNegro_triggered();
    void on_actionBorrador_triggered();
    void on_actionDeshacer_triggered();
    void on_actionBorrarTodo_triggered();
    void on_actionAcercaDe_triggered();

private:
    void configurarEstilo();

    Ui::MainWindow *ui;
    AdminDB        *m_adminDB;
    Pintura        *m_pintura;
    QString         m_usuario;
};

#endif // MAINWINDOW_H
