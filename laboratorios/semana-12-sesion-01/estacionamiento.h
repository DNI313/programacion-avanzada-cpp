#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

class Estacionamiento {
private:
    int totalPuestos;
    std::vector<bool> ocupado;
    std::map<int, int> puestoDePlaca;

public:
    Estacionamiento(int puestos) {
        totalPuestos = puestos;
        for (int i = 0; i < puestos; i++) {
            ocupado.push_back(false);
        }
    }

    // Devuelve el puesto asignado, o std::nullopt si no hay puestos libres.
    // Lanza std::invalid_argument si la placa ya esta adentro.
    std::optional<int> asignarPuesto(int placa) {
        if (puestoDePlaca.count(placa) == 1) {
            throw std::invalid_argument("placa ya registrada");
        }
        for (int i = 0; i < totalPuestos; i++) {
            if (!ocupado[i]) {
                ocupado[i] = true;
                puestoDePlaca[placa] = i;
                return i;
            }
        }
        return std::nullopt;
    }

    // Devuelve el puesto de la placa, o std::nullopt si no esta adentro.
    std::optional<int> puestoDe(int placa) {
        auto it = puestoDePlaca.find(placa);
        if (it == puestoDePlaca.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    // Libera el puesto de la placa y lo devuelve, o std::nullopt si no estaba.
    std::optional<int> liberar(int placa) {
        std::optional<int> puesto = puestoDe(placa);
        if (!puesto) {
            return std::nullopt;
        }
        ocupado[*puesto] = false;
        puestoDePlaca.erase(placa);
        return puesto;
    }
};