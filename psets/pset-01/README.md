# PSet 1: Fundamentos de POO

## Contexto

En las últimas dos sesiones pasamos de `struct` a `class`, agregamos encapsulación con `public`/`private`, getters, setters que validan un invariante, y por último constructores y destructores. Este PSet es tu turno de resolver, solo, 10 ejercicios progresivos que practican exactamente esos conceptos: cada uno construye sobre el anterior, y el último los integra todos.

No necesitas nada de sesiones posteriores a esta: todo lo que resuelves aquí sale de lo que ya viste hasta Semana 2, Sesión 2.

## Ejercicios

1. **`ejercicio1_circulo.cpp`**: `struct` con un método que calcula un valor a partir de un atributo.
2. **`ejercicio2_contador_regresivo.cpp`**: un método que reasigna un atributo del propio `struct`, con una condición.
3. **`ejercicio3_punto_cartesiano.cpp`**: un método que recibe otro `struct` del mismo tipo como parámetro por valor.
4. **`ejercicio4_termostato.cpp`**: primer paso a `class`, atributo privado con un getter.
5. **`ejercicio5_bateria.cpp`**: setter que devuelve `bool` y valida un invariante de un solo atributo.
6. **`ejercicio6_horario_atencion.cpp`**: setter que valida un invariante conjunto entre dos atributos.
7. **`ejercicio7_cronometro.cpp`**: constructor simple, que asigna un valor recibido directamente.
8. **`ejercicio8_extintor.cpp`**: constructor con valor por defecto ante un dato inválido.
9. **`ejercicio9_vela_encendida.cpp`**: constructor y destructor en la misma clase.
10. **`ejercicio10_reserva_sala.cpp`**: integrador. Combina el invariante conjunto, el constructor con valor por defecto y el destructor, y verifica el orden de destrucción LIFO con tres objetos.

Cada archivo tiene, en su encabezado, la salida exacta que debe producir cuando esté completo. Resuélvelos en orden: cada uno da por sentado que el anterior ya quedó claro.

## Compilar y ejecutar

```
g++ -std=c++20 -Wall -Wextra -g ejercicio1_circulo.cpp -o bin/ejercicio1
./bin/ejercicio1
```

Repite el mismo patrón con cada ejercicio, cambiando el nombre del archivo y del binario.

## Formato y dónde entregar

Sube tus 10 archivos `.cpp` completos a tu fork del repositorio, en esta misma carpeta (`psets/pset-01/`). Pull Request al repositorio del curso antes de la fecha de entrega.

## Fecha de entrega

Semana 4, Sesión 2.

## Qué reviso

- Que cada programa compile sin advertencias (`-Wall -Wextra`) y produzca exactamente la salida esperada del encabezado.
- Que los atributos que deben ser privados lo sean, y que cada método use el mecanismo correcto para el concepto de esa sesión (getter, setter con `bool`, constructor, destructor, según el ejercicio).
- Que el invariante conjunto del Ejercicio 6 y el Ejercicio 10 se valide junto, en un solo método, no atributo por atributo por separado.
- Que no uses `const`, paso por referencia (`&`), listas de inicialización de miembros, punteros o `std::string`: no está mal si los usas bien, pero ninguno es necesario aquí, y si tu solución los necesita para funcionar es señal de que algo se resolvió con una construcción que no hemos visto todavía.

## Una recomendación

El Ejercicio 10 no te pide nada nuevo: es el Ejercicio 6, el Ejercicio 8 y el Ejercicio 9 trabajando juntos en una sola clase. Si te cuesta, vuelve primero a esos tres antes de pelear con el ejercicio integrador directamente.
