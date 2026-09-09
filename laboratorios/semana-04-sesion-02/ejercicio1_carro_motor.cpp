#include <iostream>

class Vehiculo {
    private:
        double velocidadKmh;
        double kilometraje;
    public:
        Vehiculo() {
            velocidadKmh = 0;
            kilometraje = 0;
        }

        bool setVelocidadKmh(double nuevaVelocidad) {
            if (nuevaVelocidad < 0) { return false; }
            velocidadKmh = nuevaVelocidad;
            return true;
        }

        bool setKilometraje(double nuevoKilometraje) {
            if (nuevoKilometraje < 0) { return false; }
            kilometraje = nuevoKilometraje;
            return true;
        }

        void describir() {
            std::cout << "Vehiculo a " << velocidadKmh << " km/h, " << kilometraje << " km recorridos" << std::endl;
        }
};

class Motor {
    private:
        int potenciaHP;
        bool encendido;
    public:
        Motor() {
            potenciaHP = 0;
            encendido = false;
        }

        bool setPotenciaHP(int nuevaPotencia) {
            if (nuevaPotencia <= 0) { return false; }
            potenciaHP = nuevaPotencia;
            return true;
        }

        void encender() {
            encendido = true;
            std::cout << "Motor de " << potenciaHP << " HP encendido" << std::endl;
        }

        void apagar() {
            encendido = false;
        }

        bool estaEncendido() {
            return encendido;
        }
};

class Carro: public Vehiculo {
    private:
        Motor motor;

    public:
        Carro() {
            motor = Motor();
        }

        bool arrancar(int potenciaHP) {
            if (!motor.setPotenciaHP(potenciaHP)) {
                return false;
            }

            motor.encender();

            if (motor.estaEncendido()) {
                std::cout << "Carro arrancado" << std::endl;
                return true;
            } else {
                std::cout << "No se pudo arrancar el carro" << std::endl;
                return false;
            }
        }

        void tocarBocina() {
            std::cout << "Piii!" << std::endl;
        }
};

int main() {
    Carro c;
    c.setVelocidadKmh(120);
    c.setKilometraje(45000);
    c.describir();
    c.arrancar(150);
    c.tocarBocina();
    return 0;
}
