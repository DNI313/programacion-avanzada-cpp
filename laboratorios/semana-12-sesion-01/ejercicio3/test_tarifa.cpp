#include <gtest/gtest.h>
#include <stdexcept>

#include "tarifa.h"

// Tarifa del estacionamiento de visitas de la universidad. Las cuatro
// carpetas version_a a version_d tienen cada una su tarifa.h con una
// implementacion distinta de la misma funcion:
//
//     int tarifaEnCentavos(int minutos)
//
// Lo que tiene que cumplir:
// - Con minutos negativos lanza std::invalid_argument.
// - Hasta 15 minutos, inclusive, no se cobra.
// - Desde el minuto 16, se cobran 50 centavos por cada hora o fraccion,
//   contadas desde el minuto 0: 16 minutos son una hora, 60 minutos son
//   una hora, 61 minutos son dos horas.
// - Ninguna estadia cuesta mas de 500 centavos.
//
// Escribe aqui tus pruebas a partir de esta descripcion, antes de abrir
// cualquier tarifa.h. Compila el mismo archivo contra cada version
// cambiando la carpeta de -I:
//
//     g++ -std=c++20 -Wall -Wextra -g -I ejercicio3/version_a ejercicio3/test_tarifa.cpp -lgtest_main -lgtest -pthread -o bin/test_tarifa_a
//     ./bin/test_tarifa_a

TEST(TarifaTest, MinutosNegativosLanzanInvalidArgument) {
    EXPECT_THROW(tarifaEnCentavos(-1), std::invalid_argument);
}

TEST(TarifaTest, HastaQuinceMinutosNoSeCobra) {
    for (int minutos : {0, 1, 14, 15}) {
        SCOPED_TRACE(minutos);
        EXPECT_EQ(tarifaEnCentavos(minutos), 0);
    }
}

TEST(TarifaTest, CobraPorCadaHoraOFraccionDesdeElMinutoCero) {
    const int casos[][2] = {
        {16, 50},
        {59, 50},
        {60, 50},
        {61, 100},
        {119, 100},
        {120, 100},
        {121, 150},
    };

    for (const auto& caso : casos) {
        SCOPED_TRACE(caso[0]);
        EXPECT_EQ(tarifaEnCentavos(caso[0]), caso[1]);
    }
}

TEST(TarifaTest, NuncaSuperaQuinientosCentavos) {
    EXPECT_EQ(tarifaEnCentavos(600), 500);
    EXPECT_EQ(tarifaEnCentavos(601), 500);
    EXPECT_EQ(tarifaEnCentavos(1200), 500);
}
