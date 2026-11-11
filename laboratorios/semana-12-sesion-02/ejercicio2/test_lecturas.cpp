#include <gtest/gtest.h>

#include <vector>

#include "lecturas.h"

TEST(PromedioMovil, PrimerPromedioDeTres) {
    std::vector<double> lecturas = {18.0, 19.0, 20.0, 21.0, 22.0, 23.0};

    std::vector<double> promedios = promedioMovil(lecturas, 3);

    ASSERT_GE(promedios.size(), 1);
    EXPECT_NEAR(promedios[0], 19.0, 0.001);
}

TEST(PromedioMovil, VentanaDeUnoDevuelveLasMismasLecturas) {
    std::vector<double> lecturas = {18.5, 30.0, 12.25};

    std::vector<double> promedios = promedioMovil(lecturas, 1);

    ASSERT_EQ(promedios.size(), 3);
    EXPECT_NEAR(promedios[2], 12.25, 0.001);
}

TEST(PromedioMovil, CantidadDePromedios) {
    std::vector<double> lecturas = {18.0, 19.0, 20.0, 21.0, 22.0, 23.0};

    std::vector<double> promedios = promedioMovil(lecturas, 3);

    ASSERT_EQ(promedios.size(), 4);
    EXPECT_NEAR(promedios[0], 19.0, 0.001);
    EXPECT_NEAR(promedios[1], 20.0, 0.001);
    EXPECT_NEAR(promedios[2], 21.0, 0.001);
    EXPECT_NEAR(promedios[3], 22.0, 0.001);
}

TEST(PromedioMovil, VentanaMayorQueLecturasDevuelveVacio) {
    std::vector<double> lecturas = {18.0, 19.0};

    EXPECT_TRUE(promedioMovil(lecturas, 3).empty());
}

TEST(PromedioMovil, VentanaCeroLanza) {
    std::vector<double> lecturas = {18.0, 19.0};

    EXPECT_THROW(promedioMovil(lecturas, 0), std::invalid_argument);
}
