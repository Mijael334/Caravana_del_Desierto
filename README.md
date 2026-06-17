# Caravana del Desierto


Trabajo Práctico de Algoritmos y Estructuras de Datos - UNLaM 2026 C1.


## Integrantes


- Acuña, Lucas Daniel
- Zambrana Bernal, Mijael
- Ramirez, Kevin
- Portilla Martinez, Jeremias


**Comisión:** 03-3600  - **Grupo:** 5


## Cómo compilar


Proyecto desarrollado en C con Code::Blocks / MinGW sobre Windows. Usa `conio.h` y `windows.h` para entrada de teclado y temporización, por lo que no compila directamente en Linux/Mac sin adaptación.


Para compilar desde Code::Blocks abrir el `.cbp` y compilar normal.


Se espera encontrar en la carpeta `docs/` el archivo `config.txt`. El resto de archivos (`usuarios.dat`, `partidas.dat`, `indice.idx`, `caravana.txt`) se generan en tiempo de ejecución.


**Primera ejecución:** si no existen los archivos de persistencia (`usuarios.dat`, `indice.idx`, `partidas.dat`), el juego los crea vacíos al iniciar.


## Uso


Al iniciar pide un username. Si ya existe, lo reconoce y muestra el nickname asociado. Si no, pide el nickname y lo registra.


Desde el menú principal se puede:
- Comenzar nueva partida
- Ver ranking de jugadores
- Ver historial de partidas
- Salir del juego


Controles durante la partida: `[ESPACIO]` tira el dado, `W`/`S` navegan opciones, `[X]` abandona la partida en curso.


## Estructura


```
sources/   código fuente
include/   headers
docs/      config.txt y archivos generados en runtime
```


Módulos:
- `main` - punto de entrada
- `logica_juego` - máquina de estados y lógica de turnos
- `tablero` - generación y renderizado del tablero
- `entidades` - jugador, bandidos, usuario
- `interfaz_usuario` - menús, animaciones, lectura de teclado
- `gestion_archivos` - apertura, lectura de config, persistencia binaria
- `reportes` - partidas y ranking
- `indice_jugador` - índice de usuarios sobre ABB
- `arbol`, `lista_doble_circular`, `lista_enlazada`, `cola_dinamica` - TDAs


## Configuración


`docs/config.txt` con los parámetros iniciales:


```
cantidad_posiciones: 25
vidas_inicio: 3
maximo_bandidos: 2
maximo_premios: 3
maximo_vidas_extra: 1
maximo_oasis: 2
maximo_tormentas: 3
```


Donde:
- `cantidad_posiciones`: largo total del recorrido, incluyendo inicio y salida.
- `vidas_inicio`: vidas con las que arranca el jugador.
- `maximo_bandidos`: cantidad de bandidos sobre el tablero.
- `maximo_premios`: premios (+1 punto cada uno).
- `maximo_vidas_extra`: vidas extra distribuidas en el recorrido.
- `maximo_oasis`: oasis que otorgan protección por un turno.
- `maximo_tormentas`: tormentas que hacen perder el turno siguiente.


## Notación del tablero


Los símbolos básicos son los que define la consigna: `I`, `S`, `P`, `V`, `O`, `T`, `B`, `J`, `.` para casillero vacío.


Para combinaciones de elementos en un mismo casillero usamos `[ ]` agrupando los símbolos, separados por espacio. El símbolo del jugador (`J`) siempre va al final.


| Caso | Representación |
|---|---|
| Jugador solo | `[J]` |
| Jugador en oasis | `[O J]` |
| Jugador en tormenta | `[T J]` |
| Jugador con bandido | `[B J]` |
| Jugador con N bandidos | `[B B ... J]` |
| Bandido solo | `[B]` |
| N bandidos | `[B B B]` |
| Bandido sobre evento | `[O B]`, `[T B]`, `[P B]`, etc |


Eventos sin ocupantes se imprimen sin corchetes: `P`, `T`, `O`, `V`, `.`.


## Decisiones de diseño


**Ruta del desierto:** lista doble circular. Cada nodo es un `tCasillero` con número de posición, evento, flag de jugador y contador de bandidos.


**Bandidos:** se almacenan en un vector dinámico aparte de la lista. Cada bandido conserva su posición en la ruta. El casillero solo mantiene un contador de bandidos presentes. Esto desacopla la cantidad de bandidos del recorrido de la lista.


**Cola de movimientos:** una sola cola encola tanto el movimiento del jugador como el de cada bandido vivo en cada turno. Se desencola y se aplica uno por uno. El tipo `tMovimiento` incluye un `id` para discriminar al jugador (`ID_JUGADOR = 1`) de los bandidos (id ≥ 2).


**Movimiento del jugador:**
- En cada turno se tira un dado entre 1 y 6.
- El jugador elige adelante o atrás. Si no hay espacio para retroceder, la UI solo ofrece la opción adelante.
- Rebote en la salida: si los pasos hacen sobrepasar la ciudad refugio, rebota.


**Movimiento de bandidos:** cada bandido elige la dirección de menor distancia circular hacia el jugador, y tira su propio dado. Pueden atravesar la entrada y la salida (lista circular).


**Lógica de tormenta:** al caer en T, el jugador queda marcado con un flag `afectadoPorTormenta`. En el siguiente turno se detecta el flag al inicio, se apaga, y el jugador no tira dado. Los bandidos sí se mueven durante el turno perdido. Si el jugador estaba protegido por un oasis al pisar T, la tormenta no activa el flag.


**Oasis:** activa una protección que cubre el siguiente turno contra bandidos y tormentas. Se consume al moverse fuera del oasis.


**Bandidos eliminados:** cuando un bandido captura al jugador, el jugador pierde una vida y vuelve al inicio, y el bandido se elimina de la ruta (`vivo = MUERTO`). Esto reduce la dificultad progresivamente.


**Solucionabilidad del tablero:** garantizada por construcción. Los bandidos no se colocan en el inicio ni en la salida (`criterio_bandido_ideal`), las tormentas no son consecutivas (`criterio_tormenta_ideal`), y la combinación de elección de dirección + vidas iniciales + oasis hace que siempre exista un camino a la salida.


**Persistencia de usuarios:**
- `usuarios.dat`: archivo binario con registros `tUsuario` en orden de alta.
- `indice.idx`: archivo binario con los nodos del ABB grabados en inorden (queda ordenado por username).
- Al iniciar el juego se carga el `.idx` reconstruyendo un ABB balanceado: se lee el archivo por mitades recursivas y se inserta cada registro en el árbol. Esto produce un árbol balanceado.
- Cada alta de usuario actualiza el árbol en memoria y regraba el `.idx` completo. Mantiene la invariante de archivo ordenado.


**Persistencia de partidas:** `partidas.dat` con registros `tReportePartida` (id, usuario, puntos, vidas restantes, casillas adelante, casillas atrás, resultado). El id se calcula al momento de grabar como `ftell / sizeof + 1`.


**Ranking:** se construye en runtime leyendo `partidas.dat`, acumulando puntos por usuario en una lista simple, y ordenándola por puntos descendente. Se muestra el top 10 con su posición (#1, #2, ...).


## Funcionalidades adicionales


- **Sistema de username + nickname:** el username es único y se usa como clave del índice. El nickname es el nombre que se muestra en pantalla y en el ranking.
- **Mensaje de bienvenida:** cuando un usuario existente vuelve a jugar, se muestra "Bienvenido de vuelta, \<nickname>" antes del menú.
- **Título del menú con nickname:** el menú principal muestra el nickname del usuario activo.
- **Menú con flechas:** navegación con W/S y selección con ESPACIO en vez de tipear opciones.
- **Animación de movimientos:** el jugador y los bandidos se ven moverse casillero por casillero en pantalla.
- **Confirmación de salida:** pregunta antes de cerrar el juego.
- **Confirmación de username:** si el username ya existe, pregunta si es el mismo usuario o no, para evitar pisar la sesión de otro.
- **Historial de partidas:** opción del menú para ver todas las partidas registradas en `partidas.dat` (id, usuario, puntos, vidas, movimientos, resultado).
- **Abandonar partida:** durante una partida en curso, presionar [X] permite abandonarla. Pide confirmación y, si se acepta, vuelve al menú sin registrar la partida en `partidas.dat`.


## Registro de movimientos


Al finalizar la partida se muestra el listado de movimientos del jugador en formato `FX` / `BX` (Forward / Backward), separados por coma, en el orden en que se realizaron.


Ejemplo: `F3, B2, F5, F1, F4`.
