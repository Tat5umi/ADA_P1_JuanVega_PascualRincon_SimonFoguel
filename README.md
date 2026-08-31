# EDA_P1_Foguel_Rincon

## Práctica 1 de Estructuras de Datos y Algoritmos.

Pilas y colas en sistemas reales: undo/redo de un editor de código y control de tráfico en un firewall. Implementación desde cero y análisis formal de complejidad.

## Integrantes

- Simón Foguel Giraldo
- Pascual Rincón Cardona

## Descripción

El proyecto resuelve dos problemas sobre estructuras unidimensionales implementadas desde cero, sin usar std::stack, std::queue, std::deque ni std::list.

Problema 1 — Sistema de undo/redo: Simula un editor de texto que registra cada operación de edición (INSERT, DELETE, REPLACE) y permite deshacerlas y rehacerlas. Usa dos pilas: una de deshacer y una de rehacer. Toda edición nueva invalida el historial de rehacer.

Problema 2 — Búfer y limitador de tasa de un firewall: Simula la recepción de paquetes en un búfer circular de capacidad fija, con descarte por saturación, más un limitador por ventana deslizante que rechaza un paquete si en los últimos T segundos ya llegaron L o más.

Ambas estructuras están implementadas en dos representaciones distintas (arreglo dinámico y lista enlazada) sobre la misma interfaz, para contrastar el TAD con sus implementaciones concretas.

## Estructura del repositorio


EDA_P1_PascualRincon_SimonFoguel/
├── README.md
├── data_eventos/          Generador y datos sintéticos del Problema 1
│   ├── generador_eventos.cpp
│   └── eventos_n{1000,10000,100000,1000000}.txt
├── data_buffer/           Generador y datos sintéticos del Problema 2
│   ├── generator.cpp
│   ├── generate_all.sh
│   ├── parameters.txt
│   └── paquetes_n{1000,10000,100000,1000000}.txt
├── src/
│   ├── Vector.hpp             Arreglo dinámico propio (base de la pila sobre arreglo)
│   ├── List.hpp               Lista enlazada propia (base de la pila y cola sobre lista)
│   ├── stack_array.hpp        Pila sobre arreglo dinámico
│   ├── stack_list.hpp         Pila sobre lista enlazada
│   ├── queue_circular.hpp     Cola circular sobre arreglo de capacidad fija
│   ├── queue_list.hpp         Cola sobre lista enlazada
│   ├── undoredo.hpp           Lógica completa del Problema 1
│   ├── buffer.cpp             Lógica y ejecución del Problema 2
│   ├── main.cpp               Ejecutable del Problema 1
│   └── experimentos.cpp       Experimentación del Problema 1
├── tests/                 Un archivo por cada caso de la Sección 11
└── results/               Salidas de experimentación y de los casos de prueba


Todas las estructuras son plantillas de C++ (template<typename T>), por lo que se implementan enteramente en archivos .hpp y no tienen .cpp asociado. Esta es la práctica estándar del lenguaje: el compilador necesita ver la definición completa en cada unidad de traducción que instancie la plantilla.

## Requisitos

- Compilador con soporte de C++17 (probado con g++ 13 en Linux y g++ 16 en MinGW-w64 sobre Windows)
- Python 3 con pandas y matplotlib, únicamente para regenerar las gráficas del Problema 2

## Compilación

El proyecto produce tres ejecutables independientes. No usar src/*.cpp con comodín**: main.cpp, experimentos.cpp y buffer.cpp tienen cada uno su propia función main, y compilarlos juntos falla con símbolo duplicado.

bash
g++ -std=c++17 -O2 -o undoredo     src/main.cpp
g++ -std=c++17 -O2 -o experimentos src/experimentos.cpp
g++ -std=c++17 -O2 -o buffer       src/buffer.cpp

bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined -g -o undoredo src/main.cpp


## Ejecución

Problema 1

Recibe la ruta del archivo de eventos como argumento:

bash
./undoredo tests/caso1_secuencia_mixta.txt

Imprime el estado final del documento, los errores de lectura, el log de operaciones y el resumen con el tamaño de ambas pilas y el conteo de UNDO/REDO efectivos contra no-op.

La salida normal va a stdout y los mensajes de error a stderr, de modo que se puede redirigir a un archivo sin contaminarlo:

bash
./undoredo tests/caso1_secuencia_mixta.txt > results/caso1.txt


Formato del archivo de entrada. Un evento por línea. Las líneas vacías y las que empiezan con # se ignoran.

EDIT INSERT <pos> <texto>
EDIT DELETE <pos> <cuantos>
EDIT REPLACE <pos> <cuantos> <texto>
UNDO
REDO

Los comandos y los tipos van en mayúsculas. En INSERT y REPLACE, el texto es todo el resto de la línea, así que puede contener espacios.

Problema 2

bash
cd src && ./buffer

Lee los parámetros C, T, L y R desde data_buffer/parameters.txt (una configuración por línea) y el archivo de paquetes desde data_buffer/.

## Reproducción de los experimentos

Datos del Problema 1 — semilla 6012009

La semilla corresponde a la fecha 06/01/2009 y está fija en data_eventos/generador_eventos.cpp.

bash
cd data_eventos
g++ -std=c++17 -O2 -o gen generador_eventos.cpp
./gen

Genera los cuatro archivos de eventos. La distribución es 60% INSERT, 15% DELETE, 15% UNDO y 10% REDO.

El generador simula las dos pilas del motor mientras produce los eventos, guardando únicamente el delta de longitud de cada operación. Sin esa simulación, los UNDO y REDO desplazan el largo estimado del documento y cerca de la mitad de las posiciones generadas caen fuera de rango. Con ella, el motor procesa los cuatro archivos sin una sola posición inválida.

Los cuatro archivos son prefijos exactos unos de otros: los mil eventos de eventos_n1000.txt son idénticos a los primeros mil de eventos_n1000000.txt, lo que permite comparar tamaños sin la variación de secuencias distintas.

Datos del Problema 2 — semilla 42

La semilla corresponde a un numero al azar

bash
cd data_buffer
./generate_all.sh

## Medición

bash
./experimentos > results/experimentos_p1.txt

El ejecutable debe correrse desde el directorio donde estén los archivos eventos_n*.txt. Procesa los cuatro tamaños con cinco repeticiones cada uno y reporta media y desviación estándar muestral en milisegundos.

Reloj utilizado. Se mide con std::chrono::steady_clock y no con high_resolution_clock, que es el que sugiere el enunciado. La razón es que en libstdc++ high_resolution_clock es un alias de system_clock, cuyo is_steady es falso: es el reloj de pared y puede saltar hacia atrás o hacia adelante si el sistema lo ajusta durante la medición. steady_clock es monótono y tiene la misma resolución de un nanosegundo.

La lectura del archivo queda fuera del cronómetro. Incluirla mediría el disco, que domina el tiempo por órdenes de magnitud sobre las operaciones de pila. El objeto Motor se construye dentro del bucle de repeticiones, para que cada corrida parta de un documento vacío y pilas vacías, pero antes de arrancar el reloj, para no medir su construcción.

## Resumen de resultados — Problema 1

Cuatro tamaños, cinco repeticiones cada uno, en milisegundos.

| n | pila | media | desv. est. | efectivos | no-op |
|---|---|---|---|---|---|
| 1 000 | arreglo | 0.07 | 0.04 | 178 | 79 |
| 1 000 | lista | 0.07 | 0.01 | 178 | 79 |
| 10 000 | arreglo | 1.00 | 0.13 | 1 643 | 823 |
| 10 000 | lista | 0.82 | 0.03 | 1 643 | 823 |
| 100 000 | arreglo | 13.41 | 1.57 | 16 422 | 8 545 |
| 100 000 | lista | 13.21 | 1.08 | 16 422 | 8 545 |
| 1 000 000 | arreglo | 1 908.43 | 228.06 | 164 875 | 84 945 |
| 1 000 000 | lista | 1 678.49 | 16.92 | 164 875 | 84 945 |

Tres observaciones que el informe desarrolla en detalle:

El crecimiento es superlineal en el número de eventos. De 100 000 a 1 000 000 el tamaño se multiplica por diez y el tiempo por más de ciento cuarenta. La causa no son las pilas, que son O(1) por operación, sino el documento: se representa con std::string y cada edición desplaza la cola del texto, de modo que Modificar es Θ(L) con L el largo del documento. La complejidad total del procesamiento es entonces O(n·L) y no O(n).

Las dos representaciones de pila no se distinguen en esta medición. Con el documento dominando el tiempo, la diferencia entre arreglo y lista queda por debajo del ruido. Medidas de forma aislada, 600 000 operaciones de apilado y desapilado toman unos 82 ms en la versión de arreglo y unos 44 ms en la de lista, menos del 8% del tiempo total de la corrida grande.

La desviación estándar del arreglo es más de diez veces la de la lista en el tamaño mayor. Esa varianza es la firma del crecimiento de capacidad por duplicación: las corridas en las que toca copiar el bloque completo tardan sensiblemente más que las demás. Es evidencia medida del comportamiento amortizado.

Casos de prueba

Los siete casos obligatorios de la Sección 11 están en tests/, uno por archivo, en el formato de entrada del Problema 1. Cada archivo lleva comentarios que describen qué verifica.

| Archivo | Verifica |
| caso1_secuencia_mixta.txt | INSERT, DELETE y REPLACE combinados con UNDO y REDO |
| caso2_undo_sin_ediciones.txt | UNDO sobre pila vacía: no-op reportado, sin caída |
| caso3_edicion_undo_redo.txt | Una edición, deshacer y rehacer |
| caso4_edicion_tras_undo.txt | Una edición posterior a un UNDO vacía la pila Redo |
| caso5_n_ediciones_n_undo.txt | N ediciones seguidas de N deshacer consecutivos |
| caso6_redo_de_mas.txt | Más REDO que elementos disponibles |
| caso7_crecimiento_capacidad.txt | Veinte ediciones fuerzan el redimensionamiento del arreglo |

## Decisiones de diseño documentadas

- Deltas en vez de instantáneas. Cada registro del historial guarda solo el fragmento destruido y el que quedó en su lugar, con costo espacial proporcional al cambio y no al documento. Guardar una copia completa por operación costaría Θ(n·L).
- Primitiva única de edición. INSERT, DELETE y REPLACE son casos particulares de una sola operación Modificar(pos, cuantosQuitar, textoNuevo), de modo que el motor no ramifica por tipo de edición en ningún punto.
- Edición nula. Una edición que no quita ni agrega nada no se aplica, no se apila y no invalida el historial de rehacer; queda registrada en el log.
- Cola llena contra cola vacía. La cola circular mantiene un contador explícito de elementos, lo que permite distinguir ambos casos sin sacrificar una celda ni usar un indicador aparte.
- Manejo de errores. Un archivo inexistente aborta la ejecución; una línea mal formada o un evento con posición fuera de rango se registran y el procesamiento continúa. Ninguna condición límite produce una caída.
