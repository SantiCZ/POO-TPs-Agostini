#ifndef EDITORPRINCIPAL_H
#define EDITORPRINCIPAL_H

#include "pantalla.h"
#include "validadorsintaxis.h"
#include "panelcurriculum.h"
#include "exportadorjpg.h"

#include <QPlainTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <QTextBlock>
#include <QTextEdit>
#include <QTimer>

// ============================================================
// EditorDeTexto: subclase de QPlainTextEdit con override de eventos.
// Se comunica con EditorPrincipal mediante signals/slots.
// ============================================================
class EditorDeTexto : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditorDeTexto(QWidget *parent = nullptr);

signals:
    void lineaAbandonada(int numeroLinea, const QString &contenidoLinea);
    void teclaPresionada(int key, Qt::KeyboardModifiers mods);
    void mousePresionado(const QPoint &pos, Qt::MouseButton btn);

protected:
    void keyPressEvent(QKeyEvent *event)     override;
    void mousePressEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event)    override;
    void focusOutEvent(QFocusEvent *event)   override;

private:
    int m_ultimaLinea;
};

// ============================================================
// EditorPrincipal: pantalla principal del editor multilenguaje
// ============================================================
class EditorPrincipal : public Pantalla
{
    Q_OBJECT

public:
    explicit EditorPrincipal(QWidget *parent = nullptr);
    ~EditorPrincipal() override;

    // -- Implementación de la interfaz Pantalla --
    void inicializarUI()     override;
    void conectarEventos()   override;
    void cargarDatos()       override;
    bool validarEstado()     override;
    void registrarEvento(const QString &descripcion) override;
    QString nombrePantalla() const override;

protected:
    void keyPressEvent(QKeyEvent *event)     override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event)    override;
    void closeEvent(QCloseEvent *event)      override;
    void focusInEvent(QFocusEvent *event)    override;
    void focusOutEvent(QFocusEvent *event)   override;

private slots:
    void cambiarLenguaje(const QString &lenguaje);
    void validarLinea(int numeroLinea, const QString &contenido);
    void exportarJpg();
    void manejarTecla(int key, Qt::KeyboardModifiers mods);
    void manejarMouse(const QPoint &pos, Qt::MouseButton btn);
    void actualizarBarraEstado();
    void nuevoArchivo();
    void limpiarErrores();

private:
    void crearValidador(const QString &lenguaje);
    void marcarLineaError(int numeroLinea, const QString &mensaje);
    void limpiarLineaError(int numeroLinea);

    // Widgets principales
    EditorDeTexto   *m_editor;
    QComboBox       *m_cmbLenguaje;
    QPushButton     *m_btnExportar;
    QPushButton     *m_btnNuevo;
    QPushButton     *m_btnLimpiarErrores;
    QLabel          *m_lblLenguaje;
    QLabel          *m_lblDiagnostico;
    QLabel          *m_lblEstado;
    PanelCurriculum *m_panelCV;
    QSplitter       *m_splitter;

    // Layouts
    QVBoxLayout     *m_layoutPrincipal;
    QHBoxLayout     *m_layoutToolbar;

    // Validación polimórfica
    ValidadorSintaxis *m_validador;
    ExportadorJpg      m_exportador;

    // Mapa de errores por número de línea
    QMap<int, QString> m_errores;

    QString m_lenguajeActual;
    bool    m_modificado;
};

#endif // EDITORPRINCIPAL_H
