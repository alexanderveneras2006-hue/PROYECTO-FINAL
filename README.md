# Tic Tac Toe Final

Juego de Tic Tac Toe avanzado en C++, desarrollado como proyecto final con funcionalidades mejoradas y una experiencia de usuario interactiva.

## 📌 Descripción general

`TicTacToeFinal.cpp` es un juego de Tic Tac Toe (también conocido como "3 en raya") que permite jugar en modo:
- Jugador vs Jugador (PvP)
- Jugador vs Computadora (PvC)

Además de los modos clásicos, el juego soporta tableros de tamaño variable desde `2x2` hasta `9x9`, un sistema de deshacer movimiento (`undo`), y un historial persistente de partidas guardadas en un archivo.

## 🚀 Características principales

- Tablero dinámico configurable por el usuario.
- Modo PvP y PvC.
- Detección de victoria para:
  - filas
  - columnas
  - diagonales
- Reglas extendidas para tableros grandes:
  - en tableros de `4x4` o mayores, la condición de victoria es lograr `4 en línea`.
- Comando `U` o `u` para deshacer el último movimiento.
- Historial de partidas registrado automáticamente en `historial.txt`.
- Presentación en consola con colores ANSI en terminales compatibles.

## 🧠 Cómo funciona el código

El proyecto está contenido en un solo archivo fuente: `TicTacToeFinal.cpp`.

### Estructura del programa

- **Variables globales y macros de color**
  - Se define `jugador_actual` para almacenar el nombre del jugador.
  - Se emplean códigos ANSI (`VERDE`, `MAGENTA`, `ROJO`, etc.) para mejorar la visualización.

- **Funciones clave**
  - `Limpiar()`: limpia la pantalla según la plataforma (`cls` en Windows, `clear` en otros sistemas).
  - `centrar()`: centra texto en la pantalla para una presentación más agradable.
  - `LeerEntero()`: lee y valida entradas numéricas robustamente.
  - `RegistrarResultado()`: guarda datos de la partida en `historial.txt`.
  - `VerHistorial()`: muestra el historial cargado desde el archivo.
  - `Instrucciones()`: despliega las reglas del juego según el tamaño del tablero y el modo.
  - `Ganador()`: verifica si hay un ganador usando subfunciones de fila, columna y diagonal.
  - `ImprimirTablero()`: dibuja el tablero actual en consola.
  - `AsignarCasilla()`: coloca la marca de un jugador en la casilla seleccionada.
  - `MovimientoHumano()`: lee la jugada del jugador humano y maneja el comando `undo`.
  - `MovimientoComputadora()`: implementa una IA básica que intenta ganar, bloquear y luego elegir una casilla aleatoria.
  - `Juego()`: controla el flujo de una partida completa.
  - `main()`: muestra el menú principal y gestiona las opciones del usuario.

### Arquitectura del proyecto

El programa está implementado en un único archivo fuente: `TicTacToeFinal.cpp`. Aunque el código es monolítico, está organizado en capas claras:

- Interfaz de usuario y presentación.
- Lógica de juego y validaciones.
- Gestión de historial y persistencia.

La entrada del usuario siempre pasa por funciones específicas de lectura y validación antes de modificar el estado del tablero, lo que evita corrupción de datos y mantiene la experiencia estable.

#### Flujo principal

1. `main()`
   - Inicializa la semilla aleatoria.
   - Solicita el nombre del jugador.
   - Muestra el menú principal con opciones de juego, historial y salida.
   - Envía el control a `Juego()` según el modo elegido.

2. `Juego(int n, bool modoComputadora)`
   - Limpia la pantalla y muestra las instrucciones.
   - Crea un tablero dinámico de tamaño `n x n` usando `vector<vector<char>>`.
   - Alterna los turnos entre `X` y `O`.
   - Usa una pila (`stack<Movimiento>`) para almacenar el historial de movimientos y permitir undo.
   - Controla el ciclo de juego hasta que se detecta un ganador o un empate.
   - Al finalizar, registra el resultado y muestra el historial.

3. `MovimientoHumano()` / `MovimientoComputadora()`
   - `MovimientoHumano()` lee entrada de fila y columna; valida rango y disponibilidad.
   - Permite deshacer con `U` o `u`.
   - `MovimientoComputadora()` implementa estrategia en tres pasos:
     1. Intenta ganar instantáneamente.
     2. Bloquea al jugador si puede ganar en el siguiente turno.
     3. Selecciona una casilla vacía aleatoria.

4. `Ganador()` y sus subfunciones
   - Para tableros pequeños, verifica líneas completas de símbolos.
   - Para tableros de 4x4 o mayores, busca 4 símbolos consecutivos en filas, columnas y diagonales.

5. Persistencia
   - `RegistrarResultado()` crea o actualiza `historial.txt` con la partida.
   - `VerHistorial()` lee `historial.txt` y muestra los registros guardados.

### Detalle de módulos y responsabilidades

- `Limpiar()`
  - Aísla la compatibilidad del sistema operativo.

- `centrar()`
  - Mejora el diseño de los menús y mensajes en consola.

- `LeerEntero()`
  - Garantiza entradas válidas para números, evitando que `cin` quede en estado de error.

- `RegistrarResultado()`
  - Calcula puntos basados en eficiencia.
  - Usa memoria dinámica temporal para simular punteros y refuerza el concepto de punteros en C++.

- `VerHistorial()`
  - Lee líneas del archivo en un buffer dinámico.
  - Presenta el historial con formato y color.

- `Instrucciones()`
  - Ajusta los mensajes según el tamaño del tablero y el modo de juego.

- `GanadorFila()`, `GanadorColumna()`, `GanadorDiagonal()`
  - Soportan tableros tradicionales de 2x2 y 3x3.

- `GanadorRow4()`, `GanadorCol4()`, `GanadorDiag4()`
  - Soportan la condición de 4 en línea en tableros 4x4 o mayores.

- `ImprimirTablero()`
  - Dibuja bordes y celdas en la consola usando `|` y `---`.

- `AsignarCasilla()`
  - Traduce coordenadas de usuario (1-based) a índices de vector (0-based).

- `PuedeGanar()`
  - Simula un movimiento para la IA y comprueba si produce victoria.

- `MovimientoHumano()`
  - Permite comando `undo` para restaurar el estado anterior.

- `MovimientoComputadora()`
  - Emplea una estrategia de búsqueda básica con prioridad alta para ganar o bloquear.

### Lógica de condiciones de victoria

- En tableros de 2x2 y 3x3, el ganador se define por una fila, columna o diagonal completa.
- En tableros de 4x4 o mayores, la victoria requiere 4 símbolos consecutivos en cualquier fila, columna o diagonal.
- El código divide esta verificación en funciones especializadas para mantener claridad y permitir crecimiento futuro.

### Detalles de implementación

- El tablero se representa como `vector<vector<char>>`, lo cual facilita la manipulación dinámica del tamaño.
- El comando `undo` reutiliza una pila de movimientos con coordenadas y jugador.
- El historial se guarda con datos de jugador, tamaño de tablero, ganador, puntos y máximo teórico.
- Los colores ANSI se aplican directamente con macros para hacer la interfaz más atractiva.

### Seguridad y robustez

- Validación de entrada en todas las opciones de menú.
- Manejo de errores de archivo en lectura/escritura de historial.
- Control de índices válida antes de asignar una casilla.
- Reinicio seguro del juego y retorno al menú principal sin reiniciar el programa.

### Uso recomendado

- Ejecutar en una terminal compatible con ANSI para ver colores.
- Elegir tableros de mayor tamaño para un reto más complejo.
- Usar `U` para practicar tácticas con deshacer y experimentar con estrategias.

### Conclusión

Este proyecto es una implementación completa de Tic Tac Toe en C++ con:
- interfaz de texto atractiva
- juego variable de tamaño
- IA creada con heurísticas básicas
- historial persistente
- funcionalidad undo
- estructura clara de funciones

El README ahora describe el proyecto con precisión técnica y detalle completo, sin diagramas.

## 🛠️ Requisitos

- Compilador C++ compatible con C++11 o superior.
- No requiere librerías externas, solo la STL estándar.

## 💻 Compilación

Abre una terminal y ejecuta:

```bash
cd "c:\Users\User\Desktop\PROYECTO FINAL TIC TAC TOE"
g++ TicTacToeFinal.cpp -o TicTacToeFinal
```

## ▶️ Ejecución

En Windows CMD o PowerShell:

```powershell
TicTacToeFinal.exe
```

En terminal compatible con ANSI:

```bash
./TicTacToeFinal
```

## 🎮 Uso del juego

1. Ejecuta el programa.
2. Ingresa tu nombre.
3. Selecciona una opción del menú:
   - `1` Jugar PvP
   - `2` Jugar PvC
   - `3` Ver historial
   - `4` Salir
4. Ingresa el tamaño de tablero entre `2` y `9`.
5. Durante la partida, ingresa:
   - fila y columna para colocar tu marca
   - `U` o `u` para deshacer el último movimiento

## 📂 Archivos generados

- `TicTacToeFinal.cpp` — archivo fuente.
- `historial.txt` — archivo de texto donde se guardan los resultados de las partidas.

## 👨‍💻 Autor

- Cristian Guaman

> Proyecto final de Tic Tac Toe en C++ con modos de juego mejorados, historial persistente y soporte para tableros variables.
