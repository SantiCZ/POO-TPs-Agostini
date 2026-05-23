# Collaborative Canvas – Qt Application

## Estructura del proyecto

```
collaborative_canvas/
├── collaborative_canvas.pro
├── main.cpp
├── DrawingModel.h / .cpp      # Modelo: almacenamiento de trazos
├── CanvasView.h / .cpp        # Vista: renderizado con paintEvent
├── SyncManager.h / .cpp       # Sincronización con VPS
├── MainWindow.h / .cpp        # Ventana principal + toolbar Metro
└── server/
    ├── server.js              # Backend Node.js/Express
    └── package.json
```

## Compilación (Qt Client)

Requisitos: Qt 5.15+ o Qt 6.x, módulos core gui widgets network, C++17

```bash
cd collaborative_canvas/
qmake collaborative_canvas.pro
make
```

## Servidor VPS

```bash
cd collaborative_canvas/server/
npm install
node server.js        # Puerto 8080 por defecto
```

Endpoints:
  GET    /drawing  → recuperar trazos
  POST   /drawing  → merge & guardar trazos
  DELETE /drawing  → limpiar canvas
  GET    /health   → estado del servidor

## Controles

| Acción              | Control                      |
|---------------------|------------------------------|
| Dibujar             | Click izquierdo + arrastrar  |
| Borrar              | Click derecho + arrastrar    |
| Cambiar color       | Teclas 1 al 9                |
| Cambiar grosor      | Rueda del mouse              |
| Guardar al servidor | Botón GUARDAR                |

Paleta: tecla 1 = RGB(192,19,76) · tecla 9 = RGB(24,233,199), interpolación lineal.

## Arquitectura

- DrawingModel: almacena QVector<Stroke>, interpolación Catmull-Rom, merge incremental por UUID
- CanvasView: QWidget con paintEvent, doble capa (canvas + overlay en curso)
- SyncManager: QNetworkAccessManager, poll automático cada 5s, merge sin pérdida

## Estrategia de merge (sin pérdida)

Cada trazo tiene UUID único. El servidor y el cliente sólo agregan trazos
cuyo ID no conocen → nunca se sobreescribe ni pierde información de ningún usuario.
