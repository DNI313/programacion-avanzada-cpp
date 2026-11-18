# Semana 13, Sesión 2: Concurrencia II: atomics y patrones seguros

## Objetivo de la sesión

Distinguir cuándo un tipo atómico reemplaza correctamente a un mutex y cuándo no, implementando un contador concurrente con std::atomic.

## Antes de empezar

**Lectura previa:** ["Beyond the Mutex: A Practical Guide to High-Performance Concurrency in C++", Mansi Maheshwari](https://medium.com/@mansi_maheshwari/beyond-the-mutex-a-practical-guide-to-high-performance-concurrency-in-c-a73a0755293e)

**Referencia:** [std::atomic (cppreference)](https://en.cppreference.com/w/cpp/atomic/atomic.html)

Según el artículo, ¿qué cuesta un mutex, y qué alternativas propone? ¿En qué casos dice que conviene seguir con el mutex?

Según el artículo, un mutex cuesta por la contención: hilos bloqueados esperando, cambios de contexto y pérdida de paralelismo. Como alternativas propone tipos atómicos (`std::atomic`) y estructuras sin candados (lock-free). El mutex conviene cuando la sección crítica protege varios datos o varios pasos que tienen que ser una unidad, porque un atómico solo cubre una operación sobre un dato.

En la sesión pasada te pregunté: con 16 hilos y un millón de visitas cada uno, ¿dónde pasa el tiempo el programa? ¿Hace falta un candado completo para sumar 1 a un número?

El tiempo se va en pelear por el candado, no en sumar. No hace falta un candado completo para sumar 1: basta una operación atómica de lee-suma-escribe.

Entra a esta carpeta y crea la carpeta de los ejecutables:

```
cd laboratorios/semana-13-sesion-02
mkdir -p bin
```

## Ejercicio 1: el contador con std::atomic

Archivos: [`ejercicio1_contador_atomico.cpp`](./ejercicio1_contador_atomico.cpp), con dos `TODO`, y [`ejercicio1_contador_candado.cpp`](./ejercicio1_contador_candado.cpp), la versión con mutex de la sesión pasada, completa, con cuatro puertas. Compila los dos y corre cada uno con `time` antes de cambiar nada.

**Respuesta 1 (después del TODO 1):** `total` ya es un `std::atomic<int>`, y `registrar` sigue diciendo `total = total + 1;`. ¿Qué crees que imprime?

Imprime un número menor a 4000000 y distinto en cada corrida: `total` es atómico, pero `total = total + 1` son dos operaciones atómicas separadas (una lectura y una asignación), y entre ellas otro hilo puede cambiar el valor.

**Respuesta 2 (después del TODO 2):** con `total++`, ¿qué imprime? ¿Por qué `total++` sí funciona y `total = total + 1` no, si `total` es atómico en los dos casos?

Con `total++` imprime siempre 4000000. `total++` es una sola operación atómica de lee-suma-escribe (`fetch_add`), indivisible. `total = total + 1` es una lectura atómica, una suma, y luego una escritura atómica: cada parte es atómica, pero la secuencia no, y se pierden incrementos.

**Respuesta 3:** compara el tiempo de la versión atómica con el de la versión con mutex. ¿Cuál es más rápida, y por cuánto?

Medido con `time` (cuatro puertas): atómica 0.116 s reales contra 0.460 s con mutex, unas 4 veces más rápida. El mutex además gasta mucho tiempo de sistema (0.82 s de `sys`) por los bloqueos.

**La versión completa, tal como la resuelvo yo:**

Cambio 1: `std::atomic<int> total;`. Cambio 2: `total++;` en `registrar()`. Con solo el cambio 1 la salida es menor a 4000000 y distinta cada vez. Con ambos: `Visitas registradas: 4000000`, `real 0m0.116s` (atómico) contra `real 0m0.460s` (mutex).

## Durante los ConcepTests

**ConcepTest 1. Tu voto, antes de discutir en pareja** (A, B, C o D):

_(tu voto)_

**Tu razonamiento:**

_(tu respuesta)_

**Después de discutir en pareja, ¿cambiaste de voto? ¿Por qué?**

_(tu respuesta)_

**ConcepTest 2. Tu voto, antes de discutir en pareja** (A, B, C o D):

_(tu voto)_

**Tu razonamiento:**

_(tu respuesta)_

## Ejercicio 2: la tarjeta del comedor sin mutex

Archivo: [`ejercicio2_tarjeta_atomica.cpp`](./ejercicio2_tarjeta_atomica.cpp). Solo o en pareja.

Alguien cambió el mutex de la tarjeta por `std::atomic<int>`. `cobrarIngenuo` es ese cambio, y cada una de sus líneas es atómica.

1. Corre el archivo tal cual tres veces. ¿Cuántas tarjetas quedaron en negativo con `cobrarIngenuo`? Córrelo también con `-fsanitize=thread`: ¿reporta algo?

Con `cobrarIngenuo`, tres corridas dejaron 576, 38 y 217 tarjetas en negativo (cambia de una corrida a otra). ThreadSanitizer no reporta nada, porque no hay data race: todos los accesos son atómicos.

2. Escribe `cobrar` con `compare_exchange_weak`, siguiendo los pasos del `TODO`. Corre diez veces y con ThreadSanitizer.
3. Responde: ¿por qué `cobrarIngenuo` deja tarjetas en negativo si cada línea es atómica? ¿Por qué ThreadSanitizer no lo detecta?

`cobrarIngenuo` deja tarjetas en negativo porque pregunta y resta son dos operaciones atómicas separadas: dos cajas ven `saldo >= monto` con el último centavo y las dos restan. Cada línea es atómica, pero la decisión completa no. ThreadSanitizer no lo detecta porque busca data races (accesos no sincronizados), y aquí todos los accesos están sincronizados: es un error lógico (una condición de carrera sin data race), no un acceso indebido a memoria.

4. Responde: en `cobrar`, el saldo se cobra con `compare_exchange_weak`, y después se suma 1 a `aceptados` con otra operación. Si otro hilo leyera `saldo` y `aceptados` justo entre las dos, ¿qué vería? ¿Es un problema para este programa? ¿Para cuál sí lo sería?

Entre `compare_exchange_weak` y `aceptados++` otro hilo vería el saldo ya descontado pero `aceptados` todavía sin sumar: una foto inconsistente. En este programa no es problema, porque los totales solo se leen en `main` después de `join()`. Sí lo sería en un programa que muestre o use saldo y aceptados en vivo, o que necesite la invariante «saldo + aceptados = constante» en todo momento (por ejemplo un reporte o una auditoría): ahí habría que usar un mutex o juntar los dos datos en uno solo.

## Ejercicio 3: el centro de impresión

Archivo: [`ejercicio3_centro_impresion.cpp`](./ejercicio3_centro_impresion.cpp). Solo o en pareja.

Cuatro computadoras envían trabajos al mismo tiempo, y cada estudiante tiene una cuota de 100 páginas. El encabezado describe la clase `CentroDeImpresion`; la escribes tú. El `main` ya está escrito. Mientras no escribas la clase, el archivo no compila.

1. Escribe `CentroDeImpresion` y corre el programa diez veces y con ThreadSanitizer: la salida tiene que ser siempre la del encabezado.
2. Completa la tabla con tu decisión para cada dato de la clase:

| Dato | ¿`std::atomic` o protegido por `std::mutex`? | Por qué |
|---|---|---|
| Si el centro está cerrado | `std::atomic<bool>` | Es un solo valor independiente, que solo se lee o se escribe entero; no depende de otros datos. |
| Trabajos aceptados y rechazados | `std::atomic<int>` | Son contadores independientes que solo se incrementan; `++` es una sola operación atómica. |
| Páginas totales del centro | `std::atomic<int>` | Es un acumulador independiente: `+=` es atómico y no hay ninguna condición que revisar. |
| Páginas de cada estudiante (el `map`) | `std::mutex` | Es una estructura compuesta, y revisar la cuota y sumar tiene que ser una unidad; además `[]` puede insertar y modificar el `map`. |

3. Responde: ¿por qué las páginas de cada estudiante no pueden ser un `std::map<int, std::atomic<int>>` sin mutex, aunque cada valor sea atómico? Piensa en la cuota y en lo que hace `[]` sobre un `map` cuando la clave no existe.

No pueden ser un `std::map<int, std::atomic<int>>` sin mutex por dos razones. Primera, la cuota: revisar «actuales + paginas > 100» y luego sumar son dos pasos, y aunque cada valor sea atómico, dos hilos pueden pasar la revisión a la vez y dejar al estudiante sobre la cuota (como en `cobrarIngenuo`). Segunda, `[]` sobre una clave que no existe inserta un elemento, y modificar la estructura del `map` desde varios hilos es una data race: los atómicos protegen el valor, no al `map`.

## Qué construcciones nuevas usa este laboratorio

`#include <atomic>`, `std::atomic<int>` y `std::atomic<bool>` como atributos, asignados en el constructor. Operaciones atómicas: leer (`int actual = saldo;`), asignar, `++`, `+=`, `-=`, `fetch_add`, y `compare_exchange_weak(esperado, nuevo)`. Medir el tiempo de un programa con `time`.

## Antes de la próxima sesión

**Lectura:** ["Why C++ Still Dominates High-Frequency Trading (And It's Not Just Speed)", Rishi Jain](https://medium.com/@beingrishijain/why-cpp-still-dominates-high-frequency-trading-and-its-not-just-speed-ad15d6a1a256)

Hoy la versión atómica del contador fue varias veces más rápida que la del mutex. En un sistema que compra y vende acciones, una diferencia de microsegundos decide quién llega primero. ¿Qué características de C++ que vimos en el semestre crees que importan en ese tipo de sistema, además de la velocidad?

En HFT importan, además de la velocidad: el control determinista de la memoria (sin recolector de basura que pause en momentos inesperados), RAII para liberar recursos sin costo oculto, abstracciones de costo cero (templates, `inline`), el control sobre el diseño de datos en memoria y el caché, y las herramientas de concurrencia que vimos (atomics, mutex), que permiten una latencia predecible.
