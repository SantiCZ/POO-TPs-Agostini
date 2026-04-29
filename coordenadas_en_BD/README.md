# Ejercicio 09 — Coordenadas en base de datos
## Dibujo a mano alzada con persistencia SQLite y login

---

## Estructura del proyecto

```
ejercicio09/
├── ejercicio09.pro          ← Archivo de proyecto Qt
├── src/
│   ├── main.cpp             ← Punto de entrada
│   ├── database.h / .cpp    ← Gestor de SQLite (usuarios, trazos, logs)
│   ├── logger.h / .cpp      ← Logger de archivo de texto
│   ├── pintura.h / .cpp     ← Widget de dibujo (derivado de QWidget)
│   ├── loginwindow.h / .cpp ← Ventana de login
│   └── mainwindow.h / .cpp  ← Ventana principal
├── ui/
│   ├── loginwindow.ui       ← Diseño del login (Qt Designer)
│   └── mainwindow.ui        ← Diseño de la ventana principal
└── resources/
    └── resources.qrc
```

---

## Requisitos

- Qt 5.12+ o Qt 6.x
- Módulos: `core`, `gui`, `widgets`, `sql`
- Driver SQLite (incluido en Qt por defecto: `QSQLITE`)
- Compilador C++17 (MSVC, GCC o Clang)

---

## Compilar y ejecutar

### Desde Qt Creator
1. Abrir `ejercicio09.pro`
2. Configurar el kit de compilación
3. Build → Run (Ctrl+R)

### Desde línea de comandos
```bash
cd ejercicio09
qmake ejercicio09.pro
make          # Linux/macOS
# o
nmake         # Windows con MSVC
./ejercicio09 # Linux/macOS
ejercicio09.exe # Windows
```

---

## Usuarios de prueba (cargados automáticamente)

| Usuario | Contraseña |
|---------|------------|
| admin   | admin123   |
| user    | user123    |
| pepe    | pepe456    |

> Las contraseñas **nunca** se almacenan en texto plano.
> Se guarda únicamente el hash SHA-256.

---

## Controles del lienzo

| Control | Acción |
|---------|--------|
| Clic + arrastrar | Dibujar trazo |
| Rueda del mouse ↑ | Aumentar grosor del pincel |
| Rueda del mouse ↓ | Disminuir grosor del pincel |
| `R` | Color Rojo |
| `G` | Color Verde |
| `B` | Color Azul |
| `K` | Color Negro |
| `W` | Borrador (blanco) |
| `Ctrl+Z` | Deshacer (hasta 10 trazos) |
| `Escape` | Borrar todo el lienzo |

---

## Base de datos (ejercicio09.db)

### Tabla `usuarios`
```sql
CREATE TABLE usuarios (
    id       INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    pw_hash  TEXT NOT NULL,        -- SHA-256, nunca texto plano
    activo   INTEGER DEFAULT 1,
    creado   TEXT DEFAULT (datetime('now'))
);
```

### Tabla `trazos_segmentos`
```sql
CREATE TABLE trazos_segmentos (
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    color_r INTEGER NOT NULL,
    color_g INTEGER NOT NULL,
    color_b INTEGER NOT NULL,
    grosor  INTEGER NOT NULL DEFAULT 3,
    creado  TEXT DEFAULT (datetime('now'))
);
```

### Tabla `trazos_puntos`
```sql
CREATE TABLE trazos_puntos (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    segmento_id INTEGER REFERENCES trazos_segmentos(id) ON DELETE CASCADE,
    orden       INTEGER NOT NULL,  -- posición en el trazo
    x           INTEGER NOT NULL,
    y           INTEGER NOT NULL
);
```

### Tabla `logs_acceso`
```sql
CREATE TABLE logs_acceso (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    usuario   TEXT NOT NULL,
    evento    TEXT NOT NULL,       -- 'LOGIN_EXITOSO', 'LOGIN_FALLIDO (intento N)'
    exitoso   INTEGER DEFAULT 0,
    timestamp TEXT DEFAULT (datetime('now'))
);
```

> Se puede administrar con **SQLiteStudio**, **DB Browser for SQLite** o cualquier
> herramienta compatible con SQLite.

---

## Archivos generados en tiempo de ejecución

| Archivo | Contenido |
|---------|-----------|
| `ejercicio09.db` | Base de datos SQLite con usuarios, trazos y logs |
| `accesos.log`    | Log de texto con accesos exitosos y fallidos (sin contraseñas) |

---

## Seguridad implementada

- ✅ Contraseñas almacenadas como hash SHA-256 (nunca en texto plano)
- ✅ Logs de acceso exitoso y fallido en BD y en archivo
- ✅ Bloqueo tras 5 intentos fallidos consecutivos
- ✅ Animación de sacudida visual en credenciales incorrectas
- ✅ ON DELETE CASCADE para integridad referencial en puntos de trazos

---

## Diagrama de clases (simplificado)

```
QWidget
  └── Pintura          ← Widget de dibujo principal
        ├── usa Database
        └── emite: colorCambiado, grosorCambiado, statusMessage

QDialog
  └── LoginWindow      ← Login construido con Qt Designer
        └── usa Database, Logger

QMainWindow
  └── MainWindow       ← Ventana principal
        ├── contiene Pintura (widget central)
        └── usa Database

Database (QObject)     ← Acceso a SQLite
Logger (Singleton)     ← Log en archivo de texto
```
