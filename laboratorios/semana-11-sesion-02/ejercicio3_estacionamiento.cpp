#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>

// Salida esperada:
// Placa 4521: puesto 0
// Placa 7788: puesto 1
// Placa 3011: puesto 2
// Placa 9090: sin puesto, estacionamiento lleno
// Placa 7788: error, placa ya registrada
// Placa 7788 esta en el puesto 1
// Placa 5555 no esta en el estacionamiento
// Placa 4521 sale del puesto 0
// Placa 9090: puesto 0
// Placa 4521 no esta en el estacionamiento

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

    // Devuelve el puesto asignado, o std::nullopt si no hay puestos libres
    // (situacion esperada: un estacionamiento puede llenarse).
    // Lanza std::invalid_argument si la placa ya esta adentro (error de
    // quien llama: una placa no puede entrar dos veces).
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

    // Devuelve el puesto de la placa, o std::nullopt si no esta adentro
    // (consultar por una placa ausente es una pregunta legitima).
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

// Registra la llegada de una placa e imprime el resultado.
void registrarLlegada(Estacionamiento& estacionamiento, int placa) {
    try {
        std::optional<int> puesto = estacionamiento.asignarPuesto(placa);
        if (puesto) {
            std::cout << "Placa " << placa << ": puesto " << *puesto << std::endl;
        } else {
            std::cout << "Placa " << placa << ": sin puesto, estacionamiento lleno" << std::endl;
        }
    } catch (const std::invalid_argument& error) {
        std::cout << "Placa " << placa << ": error, " << error.what() << std::endl;
    }
}

// Libera la placa e imprime el resultado.
void registrarSalida(Estacionamiento& estacionamiento, int placa) {
    std::optional<int> liberado = estacionamiento.liberar(placa);
    if (liberado) {
        std::cout << "Placa " << placa << " sale del puesto " << *liberado << std::endl;
    } else {
        std::cout << "Placa " << placa << " no esta en el estacionamiento" << std::endl;
    }
}

int main() {
    Estacionamiento estacionamiento(3);

    int llegadas[5] = {4521, 7788, 3011, 9090, 7788};
    for (int i = 0; i < 5; i++) {
        registrarLlegada(estacionamiento, llegadas[i]);
    }

    int consultas[2] = {7788, 5555};
    for (int i = 0; i < 2; i++) {
        std::optional<int> puesto = estacionamiento.puestoDe(consultas[i]);
        if (puesto) {
            std::cout << "Placa " << consultas[i] << " esta en el puesto " << *puesto << std::endl;
        } else {
            std::cout << "Placa " << consultas[i] << " no esta en el estacionamiento" << std::endl;
        }
    }

    registrarSalida(estacionamiento, 4521);

    registrarLlegada(estacionamiento, 9090);

    registrarSalida(estacionamiento, 4521);

    return 0;
}