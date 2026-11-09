#pragma once

#include <map>
#include <stdexcept>

// La bodega de la Semana 11, Sesion 1. Desde entonces, alguien del equipo
// la reordeno "para que se lea mejor", y nadie volvio a correr el main de
// esa sesion. Lo que tiene que cumplir sigue siendo lo mismo:
//
// - ingresar suma la cantidad al stock del codigo (lo crea si no existe).
// - retirar resta la cantidad, o lanza:
//   - std::invalid_argument("cantidad no positiva") si cantidad <= 0;
//   - std::out_of_range("codigo desconocido") si el codigo no existe;
//   - std::runtime_error("stock insuficiente") si no alcanza.
//   Ningun retiro rechazado cambia la bodega.
// - stockDe devuelve el stock del codigo, o lanza std::out_of_range si el
//   codigo no existe.

class Bodega {
private:
    std::map<int, int> stock;

public:
    void ingresar(int codigo, int cantidad) {
        stock[codigo] = stock[codigo] + cantidad;
    }

    void retirar(int codigo, int cantidad) {
        if (cantidad <= 0) {
            throw std::invalid_argument("cantidad no positiva");
        }
        // find no inserta nada: consultar un codigo desconocido no debe
        // crearlo (stock[codigo] lo creaba con valor 0).
        std::map<int, int>::iterator it = stock.find(codigo);
        if (it == stock.end()) {
            throw std::out_of_range("codigo desconocido");
        }
        if (it->second < cantidad) {
            throw std::runtime_error("stock insuficiente");
        }
        it->second = it->second - cantidad;
    }

    int stockDe(int codigo) {
        return stock.at(codigo);
    }
};