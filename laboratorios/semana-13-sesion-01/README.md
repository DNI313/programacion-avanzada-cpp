# Semana 13, Sesión 1: Concurrencia I: threads, mutex, condiciones de carrera

## Objetivo de la sesión

Identificar una condición de carrera en código concurrente y protegerla con un mutex.

## Antes de empezar

**Lectura previa:** ["Race Condition: The Invisible Bug That Breaks Your Code", Varbolean](https://medium.com/@varboleansh/race-condition-c5d038870f83)

**Referencia:** [std::mutex (cppreference)](https://en.cppreference.com/w/cpp/thread/mutex.html)

Según el artículo, ¿qué es una condición de carrera, y por qué es tan difícil de encontrar probando el programa?

Una condición de carrera ocurre cuando el resultado depende del orden en que los hilos se intercalan al acceder a un dato compartido y al menos uno lo modifica. Es difícil de encontrar porque solo falla con ciertos intercalados, que dependen del planificador, la carga y el hardware: probando casi siempre sale bien, y al depurar o agregar prints se cambian los tiempos y desaparece.

En la sesión pasada te pregunté por un programa que falla una de cada mil ejecuciones con los mismos datos. ¿Qué tendría que estar pasando? ¿Te serviría gdb para encontrarlo?

Tendría que haber un dato compartido sin protección, de modo que el error solo aparece cuando los hilos se intercalan de una forma poco probable (una de mil corridas). gdb sirve poco: detenerse en un breakpoint o ir paso a paso altera los tiempos y suele ocultar el error, y no hay un punto fijo donde buscarlo. Para esto sirve más ThreadSanitizer, que detecta el acceso sin sincronizar aunque esa corrida salga bien.

Entra a esta carpeta y crea la carpeta de los ejecutables:

```
cd laboratorios/semana-13-sesion-01
mkdir -p bin
```

Los tres programas usan `<thread>` y `<mutex>`, que ya vienen con el compilador. Cada archivo trae su comando de compilación en el encabezado, y el de ThreadSanitizer (`-fsanitize=thread`), el sanitizer de la Semana 12 para hilos.

## Ejercicio 1: el contador de visitas de la biblioteca

Archivo: [`ejercicio1_contador_visitas.cpp`](./ejercicio1_contador_visitas.cpp). Compílalo y córrelo una vez antes de cambiar nada: las dos puertas registran una después de la otra, y la cuenta da bien.

**Respuesta 1 (antes del TODO 1):** con las dos puertas en dos hilos, al mismo tiempo, cada una registra un millón de visitas. ¿Qué número crees que imprime? ¿El mismo cada vez?

No imprime 2000000: imprime un número menor, y distinto en cada corrida, porque se pierden visitas.

**Respuesta 2 (después del TODO 1):** corre el programa cinco veces y anota los cinco resultados. `total = total + 1` es una sola línea. ¿Por qué se pierden visitas?

Cinco corridas sin mutex (medidas): 1122612, 1343404, 1105807, 1100198 y 1606840. `total = total + 1` parece una línea pero son tres pasos: leer, sumar, escribir. Si los dos hilos leen el mismo valor antes de que alguno escriba, los dos escriben valor+1 y una visita se pierde.

**Respuesta 3 (después del TODO 2):** con el `lock_guard`, ¿qué imprime? ¿Qué reporta ThreadSanitizer antes y después del mutex?

Con el `lock_guard` imprime siempre 2000000. ThreadSanitizer sin mutex reporta `WARNING: ThreadSanitizer: data race`; con el mutex no reporta nada.

**La versión completa, tal como la resuelvo yo:**

Dos hilos `std::thread t1(puerta, &contador, 1000000)` y `t2`, con `join()` de ambos. `registrar()` toma un `std::lock_guard<std::mutex>` y hace `total++`; `getTotal()` también toma el candado. Resultado: `Visitas registradas: 2000000` siempre. ThreadSanitizer: sin mutex reporta data race; con mutex, ningún reporte.

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

## Ejercicio 2: la tarjeta del comedor

Archivo: [`ejercicio2_tarjeta_comedor.cpp`](./ejercicio2_tarjeta_comedor.cpp). Solo o en pareja.

Dos cajas cobran de la misma tarjeta prepago al mismo tiempo. El encabezado dice qué tiene que quedar al final.

1. Completa el TODO 1 (los dos hilos), sin mutex todavía. Corre tres veces y anota el saldo final y los cobros.

Sin mutex (tres corridas): saldo final 4915, 11293 y 30927; aceptados 94857, 89127 y 67897; rechazados 0. Las cuentas están mal: los hilos se pisan al restar y al contar, y el saldo ni siquiera llega a 0.

2. Agrega el mutex (TODO 2), pero toma el candado solo alrededor de la resta y de cada contador, dentro de los dos lados del `if`, no alrededor de la pregunta `saldo >= monto`. Corre diez veces. ¿Algún saldo final fue negativo? ¿Por qué puede pasar, si la resta está protegida?

Con el candado solo alrededor de la resta, en una de diez corridas el saldo final fue -1. Sí puede pasar: dos cajas preguntan `saldo >= monto` a la vez, las dos ven que queda 1 centavo, y cada una resta después, por lo que se cobra dos veces el último centavo. La resta está protegida, pero la decisión (pregunta + cambio) no es una unidad indivisible.

3. Completa el TODO 3: un solo `lock_guard` al inicio de `cobrar`, que proteja la pregunta y el cambio juntos. Corre diez veces y con ThreadSanitizer.

Con un solo `lock_guard` al inicio de `cobrar`, la pregunta y el cambio son una sección crítica. En todas las corridas: saldo 0, aceptados 100000, rechazados 20000, y ThreadSanitizer no reporta nada.

Responde: `getSaldo()` no toma el candado. ¿Es un problema en este programa? ¿En qué programa sí lo sería?

En este programa no es problema: `getSaldo()` solo se llama en `main` después de `join()`, cuando ya no hay hilos escribiendo. Sí lo sería si otro hilo leyera el saldo mientras las cajas siguen cobrando (por ejemplo, una pantalla que muestra el saldo en vivo): sería una carrera de datos y podría leer un valor a medias o inconsistente.

## Ejercicio 3: la taquilla del concierto

Archivo: [`ejercicio3_taquilla.cpp`](./ejercicio3_taquilla.cpp). Solo o en pareja.

Cinco vendedores venden los asientos del mismo teatro al mismo tiempo. El encabezado describe la clase `Taquilla`; esta vez no hay código de partida para la clase: la escribes tú. El `main` ya está escrito y verifica las reglas al final. Mientras no escribas la clase, el archivo no compila.

1. Escribe `Taquilla` y corre el programa diez veces: las tres líneas finales tienen que salir siempre iguales a las del encabezado.
2. Córrelo con ThreadSanitizer: no tiene que reportar nada.
3. Responde: ¿qué partes de `Taquilla` son sección crítica? ¿Por qué `reservarPar` no puede llamar dos veces a `reservar`?

Son sección crítica: el vector `duenos` y el contador `cantidadVendidos`, es decir, todo lo que lee o escribe `reservar`, `reservarPar`, `duenoDe` y `vendidos`. `reservarPar` no puede llamar dos veces a `reservar` porque cada llamada toma y suelta el candado: entre las dos otro vendedor podría tomar el segundo asiento y el par quedaría a medias. Hay que revisar y reservar los dos asientos bajo un mismo candado.

4. Responde: un compañero propone poner un solo mutex en `main`, alrededor de los cinco `std::thread`. ¿Funcionaría? ¿Qué se perdería?

Funcionaría en el sentido de ser correcto, pero se perdería toda la concurrencia: un mutex alrededor de los cinco `std::thread` hace que se ejecuten uno tras otro (en la práctica, secuencial). Además el candado tiene que estar dentro de `Taquilla`, que es quien conoce el dato que protege; así cualquier cliente de la clase queda a salvo, no solo este `main`.

## Qué construcciones nuevas usa este laboratorio

`#include <thread>` y `std::thread hilo(funcion, argumento1, argumento2)`, con argumentos por valor o como puntero; `join()`. `#include <mutex>`, `std::mutex` como atributo, y `std::lock_guard<std::mutex> guardia(candado);`, que toma el candado al crearse y lo suelta al salir del bloque. Compilar con `-fsanitize=thread`.

## Antes de la próxima sesión

**Lectura:** ["Beyond the Mutex: A Practical Guide to High-Performance Concurrency in C++", Mansi Maheshwari](https://medium.com/@mansi_maheshwari/beyond-the-mutex-a-practical-guide-to-high-performance-concurrency-in-c-a73a0755293e)

Hoy el mutex arregló el contador de visitas. Imagina que, en vez de dos puertas, son 16 hilos y cada uno registra un millón de visitas. Cada `registrar` toma y suelta el candado. ¿Dónde pasa el tiempo el programa? ¿Hace falta un candado completo para sumar 1 a un número?

Cada `registrar` toma y suelta el candado, y con 16 hilos el tiempo se va en esperar el candado (contención, cambios de contexto), no en sumar. No hace falta un candado completo para sumar 1: una operación atómica (`std::atomic`), como se ve en la sesión 2.
