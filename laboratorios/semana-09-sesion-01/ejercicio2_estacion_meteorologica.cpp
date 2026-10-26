#include <iostream>
#include <memory>
#include <utility>

// Salida esperada:
// Medidores conectados: 3
// Temperatura: 21.5 C
// Presion: 1013.2 hPa
// Viento: 12.4 km/h
// Agregar un cuarto medidor: rechazado

class IMedidor {
    public:
        virtual double leerValor() const = 0;
        virtual const char* nombreMagnitud() const = 0;
        virtual const char* unidad() const = 0;
        virtual ~IMedidor() = default;
};

class Termometro : public IMedidor {
    private:
        double valor;
    public:
        explicit Termometro(double nuevoValor) : valor(nuevoValor) {}

        double leerValor() const override {
            return valor;
        }

        const char* nombreMagnitud() const override {
            return "Temperatura";
        }

        const char* unidad() const override {
            return "C";
        }
};

class Barometro : public IMedidor {
    private:
        double valor;
    public:
        explicit Barometro(double nuevoValor) : valor(nuevoValor) {}

        double leerValor() const override {
            return valor;
        }

        const char* nombreMagnitud() const override {
            return "Presion";
        }

        const char* unidad() const override {
            return "hPa";
        }
};

class Anemometro : public IMedidor {
    private:
        double valor;
    public:
        explicit Anemometro(double nuevoValor) : valor(nuevoValor) {}

        double leerValor() const override {
            return valor;
        }

        const char* nombreMagnitud() const override {
            return "Viento";
        }

        const char* unidad() const override {
            return "km/h";
        }
};

class EstacionMeteorologica {
    private:
        std::unique_ptr<IMedidor> medidores[3];
        int cantidad;
        int capacidad;
    public:
            EstacionMeteorologica() : cantidad(0), capacidad(3) {}

            bool agregar(std::unique_ptr<IMedidor> nuevo) {
                if (cantidad >= capacidad) {
                    return false;
                }

                medidores[cantidad] = std::move(nuevo);
                ++cantidad;
                return true;
        }

            int getCantidad() const {
            return cantidad;
        }

            void reportar() const {
                for (int i = 0; i < cantidad; ++i) {
                    std::cout << medidores[i]->nombreMagnitud() << ": "
                              << medidores[i]->leerValor() << " "
                              << medidores[i]->unidad() << std::endl;
                }
            }
};

int main() {
    EstacionMeteorologica estacion;
    estacion.agregar(std::make_unique<Termometro>(21.5));
    estacion.agregar(std::make_unique<Barometro>(1013.2));

    std::unique_ptr<IMedidor> viento = std::make_unique<Anemometro>(12.4);
    estacion.agregar(std::move(viento));

    std::cout << "Medidores conectados: " << estacion.getCantidad() << std::endl;
    estacion.reportar();

    bool aceptado = estacion.agregar(std::make_unique<Termometro>(30.0));
    std::cout << "Agregar un cuarto medidor: ";
    if (aceptado) {
        std::cout << "aceptado" << std::endl;
    } else {
        std::cout << "rechazado" << std::endl;
    }

    return 0;
}
