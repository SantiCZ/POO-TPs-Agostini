#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include "TrabajoPractico.h"
#include "TPRepository.h"
#include "HistorialRepository.h"

// Pantalla de alta/edición de un TP (sin QDialog, usa QWidget puro)
class TPEditorDialog : public QWidget {
    Q_OBJECT
public:
    explicit TPEditorDialog(TPRepository*       tpRepo,
                            HistorialRepository* histRepo,
                            QWidget*            parent = nullptr);

    // Configura el editor. Si id está vacío → nuevo TP. Si no → edición.
    void cargarTP(const QString& usuario, const QString& id = QString());

signals:
    void guardadoExitoso();
    void cancelado();

private slots:
    void onGuardar();
    void onGuardarNotas();
    void onCancelar();

private:
    TPRepository*        m_tpRepo;
    HistorialRepository* m_histRepo;
    QString              m_usuario;
    QString              m_editingId;   // vacío si es nuevo

    QLabel*    m_lblTitulo;
    QLineEdit* m_txtNombre;
    QLineEdit* m_txtDescripcion;
    QComboBox* m_cmbEstado;
    QComboBox* m_cmbPrioridad;
    QDateEdit* m_dateEntrega;
    QTextEdit* m_txtNotas;
    QLabel*    m_lblNotasSaved;
    QLabel*    m_lblError;

    void buildUI();
};
