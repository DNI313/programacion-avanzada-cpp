#include <gtest/gtest.h>

#include "estacionamiento.h"

// Pruebas del estacionamiento de la Semana 11. Cada TEST es una prueba
// independiente: crea su propio estacionamiento, hace algo y verifica el
// resultado. El primero ya esta escrito como modelo.
//
// Compilar:  g++ -std=c++20 -Wall -Wextra -g ejercicio1_test_estacionamiento.cpp -lgtest_main -lgtest -pthread -o bin/test_estacionamiento
// Ejecutar:  ./bin/test_estacionamiento

TEST(Estacionamiento, PrimeraPlacaRecibePuestoCero) {
    Estacionamiento estacionamiento(3);

    std::optional<int> puesto = estacionamiento.asignarPuesto(4521);

    ASSERT_TRUE(puesto.has_value());
    EXPECT_EQ(*puesto, 0);
}

// Con 3 puestos, tres placas distintas entran; la cuarta recibe un optional
// vacio porque ya no hay lugar.
TEST(Estacionamiento, LlenoDevuelveVacio) {
    Estacionamiento estacionamiento(3);

    estacionamiento.asignarPuesto(4521);
    estacionamiento.asignarPuesto(7788);
    estacionamiento.asignarPuesto(3011);
    std::optional<int> cuarta = estacionamiento.asignarPuesto(9090);

    EXPECT_FALSE(cuarta.has_value());
}

// Una placa que ya esta adentro no puede entrar otra vez: es un error de
// quien llama, asi que se lanza una excepcion.
TEST(Estacionamiento, PlacaRepetidaLanza) {
    Estacionamiento estacionamiento(3);

    estacionamiento.asignarPuesto(123);

    EXPECT_THROW(estacionamiento.asignarPuesto(123), std::invalid_argument);
}

// Al salir una placa, su puesto queda libre y la siguiente placa lo usa.
TEST(Estacionamiento, PuestoLiberadoSeReusa) {
    Estacionamiento estacionamiento(3);

    estacionamiento.asignarPuesto(4521);  // puesto 0
    estacionamiento.asignarPuesto(7788);  // puesto 1
    estacionamiento.asignarPuesto(3011);  // puesto 2
    estacionamiento.liberar(7788);

    std::optional<int> nueva = estacionamiento.asignarPuesto(9090);

    ASSERT_TRUE(nueva.has_value());
    EXPECT_EQ(*nueva, 1);
}