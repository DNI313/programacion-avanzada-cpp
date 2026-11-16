#include <iostream>
#include <mutex>
#include <thread>

// Ejercicio 1: el contador de visitas de la biblioteca, desde dos puertas.
//
// Cada puerta tiene un sensor que llama registrar() una vez por persona.
// Las dos puertas trabajan al mismo tiempo, cada una en su propio hilo.
// Hoy entran 1000000 personas por cada puerta.
//
// Compilar:  g++ -std=c++20 -Wall -Wextra -g ejercicio1_contador_visitas.cpp -o bin/ejercicio1
// Ejecutar:  ./bin/ejercicio1
// Con ThreadSanitizer:
//   g++ -std=c++20 -Wall -Wextra -g -fsanitize=thread ejercicio1_contador_visitas.cpp -o bin/ejercicio1_tsan
//
// Salida esperada (cuando el contador es correcto):
// Visitas registradas: 2000000

class ContadorDeVisitas {
private:
    int total = 0;              // dato compartido entre los hilos
    mutable std::mutex m;       // candado que protege a total

public:
    void registrar() {
        std::lock_guard<std::mutex> lock(m);   // bloquea; se suelta solo al salir
        total++;                               // sección crítica
    }

    int getTotal() const {
        std::lock_guard<std::mutex> lock(m);   // también protege la lectura
        return total;
    }
};

// Cada "puerta" registra una cantidad de visitas
void puerta(ContadorDeVisitas* contador, int visitas) {
    for (int i = 0; i < visitas; i++) {
        contador->registrar();
    }
}

int main() {
    ContadorDeVisitas contador;

    std::thread t1(puerta, &contador, 1000000);   // hilo 1: 1,000,000 visitas
    std::thread t2(puerta, &contador, 1000000);   // hilo 2: 1,000,000 visitas

    t1.join();   // espera a que termine t1
    t2.join();   // espera a que termine t2

    // Siempre debe imprimir 2000000
    std::cout << "Visitas registradas: " << contador.getTotal() << std::endl;
}