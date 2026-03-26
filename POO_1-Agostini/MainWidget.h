#pragma once
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include "TPRepository.h"
#include "HistorialRepository.h"
#include "SessionManager.h"

class MainWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainWidget(TPRepository*       tpRepo,
                        HistorialRepository* histRepo,
                        SessionManager*     session,
                        QWidget*            parent = nullptr);

    void setUsuario(const QString& username);

signals:
    void logoutRequested();
    void nuevoTPRequested();
    void editarTPRequested(const QString& id);
    void historialRequested();

public slots:
    void refrescarGrilla();
    void onSessionTick(int remaining);

private slots:
    void onNuevo();
    void onLogout();
    void onHistorial();
    void onFiltroChanged();
    void onEditarTP(const QString& id);
    void onEliminarTP(const QString& id);

private:
    TPRepository*        m_tpRepo;
    HistorialRepository* m_histRepo;
    SessionManager*      m_session;
    QString              m_usuario;

    QLabel*      m_lblBienvenida;
    QLabel*      m_lblTimer;
    QComboBox*   m_cmbEstado;
    QComboBox*   m_cmbPrioridad;
    QLineEdit*   m_txtBuscar;
    QWidget*     m_gridContainer;
    QGridLayout* m_grid;
    QScrollArea* m_scroll;
    QLabel*      m_lblVacio;

    void buildUI();
    void limpiarGrilla();
};
