# t-rex extremo — qt/c++

juego inspirado en el clasico t-rex de google chrome
implementado en qt y c++

## estructura del proyecto

```text
TP10-T-rex/
├── main.cpp          — punto de entrada crea qapplication y manager
├── manager.h/cpp     — clase manager administra ventanas y conexiones principales
├── gamewindow.h/cpp  — ventana principal logica y renderizado
├── trex.h/cpp        — clase trex dinosaurio del jugador
├── pajaro.h/cpp      — clase pajaro cada pajaro con su propio qtimer
├── trex_game.pro     — archivo de proyecto qmake
```

## clases y responsabilidades

### manager

- administra conexiones principales y visualizacion de ventanas
- instancia y muestra `gamewindow`

### gamewindow

- contiene el `qtimer` principal de 16 ms
- este timer:
  - mueve los cactus
  - desplaza el fondo
  - detecta colisiones
  - controla la generacion de cactus

- contiene el `qtimer` de pajaros de 5000 ms
- llama a `slot_spawnpajaro()`

- contiene el `qtimer` de dificultad de 3000 ms
- aumenta velocidades progresivamente

- usa `qtimer::singleshot()` para generar pajaros sorpresa

- dibuja todo usando `qpainter`
- no utiliza imagenes externas

### trex

- hereda de `qobject`
- tiene un `qtimer` de gravedad de 16 ms para el salto
- tiene un `qtimer` de animacion de 120 ms para alternar frames de patas
- expone `boundingrect()` para detectar colisiones

### pajaro

- hereda de `qobject`

- cada instancia tiene:
  - un `qtimer` de movimiento de 16 ms
  - un `qtimer` de animacion de alas de 150 ms

- tipos disponibles:
  - `alto`
  - `medio`
  - `bajo`

- emite la senal `moved()` al moverse
- esto solicita el repintado de la ventana

- `setspeed()` permite actualizar velocidad desde afuera

## controles

| tecla              | accion        |
|--------------------|---------------|
| espacio            | saltar        |
| flecha abajo       | agacharse     |
| flecha derecha     | adelantarse   |
| flecha izquierda   | frenarse      |
| espacio o enter    | reiniciar     |

## compilar con qmake

```bash
qmake TP10-T-rex.pro
make

# windows con msvc
nmake

# windows con mingw
mingw32-make
```


## requisitos

- qt 5.x o qt 6.x
- c++11 o superior
- qmake o cmake 3.16+