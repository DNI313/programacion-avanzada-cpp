#pragma once

#include <stdexcept>
#include <vector>

// Promedio movil de las lecturas del sensor de temperatura del
// invernadero: el promedio de cada grupo de `ventana` lecturas seguidas.
// Con 6 lecturas y ventana 3 hay 4 promedios: lecturas 0 a 2, 1 a 3, 2 a 4
// y 3 a 5.

std::vector<double> promedioMovil(const std::vector<double>& lecturas, int ventana) {
    if (ventana <= 0) {
        throw std::invalid_argument("La ventana debe ser mayor que cero");
    }

    std::vector<double> promedios;
    const auto tamanoVentana = static_cast<std::vector<double>::size_type>(ventana);
    if (tamanoVentana > lecturas.size()) {
        return promedios;
    }

    for (std::vector<double>::size_type inicio = 0;
         inicio <= lecturas.size() - tamanoVentana;
         ++inicio) {
        double suma = 0.0;
        for (std::vector<double>::size_type j = 0; j < tamanoVentana; ++j) {
            suma = suma + lecturas[inicio + j];
        }
        promedios.push_back(suma / ventana);
    }
    return promedios;
}
