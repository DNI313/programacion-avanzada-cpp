#include <gtest/gtest.h>

#include "bodega.h"

// Pruebas de la bodega. Escribe una prueba por TODO, cada una con su
// propia Bodega. Los nombres de las pruebas ya estan elegidos: dicen que
// se verifica, para que el reporte de GoogleTest se lea solo.
//
// Compilar:  g++ -std=c++20 -Wall -Wextra -g ejercicio2_test_bodega.cpp -lgtest_main -lgtest -pthread -o bin/test_bodega
// Ejecutar:  ./bin/test_bodega

// Dos ingresos del mismo codigo se suman, no se reemplazan.
TEST(Bodega, IngresarDosVecesAcumula) {
    Bodega bodega;

    bodega.ingresar(305, 10);
    bodega.ingresar(305, 5);

    EXPECT_EQ(bodega.stockDe(305), 15);
}

// Retirar exactamente todo el stock es valido y deja el codigo en cero.
TEST(Bodega, RetirarTodoElStockLoDejaEnCero) {
    Bodega bodega;
    bodega.ingresar(210, 5);

    EXPECT_NO_THROW(bodega.retirar(210, 5));

    EXPECT_EQ(bodega.stockDe(210), 0);
}

// Un retiro que no alcanza se rechaza y la bodega queda como estaba.
TEST(Bodega, RetirarMasDelStockLanzaYNoCambiaNada) {
    Bodega bodega;
    bodega.ingresar(120, 2);

    EXPECT_THROW(bodega.retirar(120, 5), std::runtime_error);

    EXPECT_EQ(bodega.stockDe(120), 2);
}

// Una cantidad no positiva es un error aunque el codigo exista.
TEST(Bodega, CantidadCeroLanza) {
    Bodega bodega;
    bodega.ingresar(305, 10);

    EXPECT_THROW(bodega.retirar(305, 0), std::invalid_argument);
}

// Retirar de un codigo desconocido se rechaza sin crearlo: consultarlo
// despues tiene que seguir lanzando out_of_range.
TEST(Bodega, CodigoDesconocidoLanzaYNoSeCrea) {
    Bodega bodega;
    bodega.ingresar(305, 10);

    EXPECT_THROW(bodega.retirar(999, 1), std::out_of_range);

    EXPECT_THROW(bodega.stockDe(999), std::out_of_range);
}