#include <iostream>

class Animal {
    private:
        int edad;
        double peso;
    public:
        
        Animal() {
            edad = 0;
            peso = 0;
        }

        Animal(int edad_inicial, double peso_inicial){
            if (!setEdad(edad_inicial)) { edad = 0; }
            if (!setPeso(peso_inicial)) { peso = 0.0; }
        }

<<<<<<< HEAD
    bool setEdad(double nuevaEdad) {
        if (nuevaEdad < 0) {
            // TODO: si nuevaEdad es negativa, devuelve false sin modificar edad.
            return false;
        }
        // Si no, asigna edad = nuevaEdad y devuelve true.
        edad = nuevaEdad;
        return true;
    }

    bool setPesoKg(double nuevoPeso) {
        // TODO: mismo patron que setEdad(), pero para pesoKg (debe ser mayor que 0).
        if (nuevoPeso <= 0) {
            return false;
        }
        pesoKg = nuevoPeso;
        return true;
    }
=======
        bool setEdad(double nueva_edad){
            if(nueva_edad < 0) { return false; }
            edad = nueva_edad;
            return true;
        }

        bool setPeso(double nuevo_peso){
            if(nuevo_peso <= 0) { return false; }
            peso = nuevo_peso;
            return true;
        }
>>>>>>> 90fbb9ebee6bf1a2548f0f6706eb3d0f1ca11491

        double getPeso(){ return peso; }
        int getEdad(){ return edad; }

<<<<<<< HEAD
    void describir() {
        // TODO: imprime "Animal de " + edad + " anios, " + pesoKg + " kg"
        std::cout << "Animal de " << edad << " años, " << pesoKg << " kg" << std::endl;
    }
=======
        void describir(){
            std::cout << "Animal de: " << edad << " anios, " << peso << " kg" << std::endl;
        }

};
class Perro: public Animal {
    public:
        void ladrar() {
            std::cout << "Guau!" << std::endl;
        }
>>>>>>> 90fbb9ebee6bf1a2548f0f6706eb3d0f1ca11491
};

class Gato: public Animal {
    public:
        void maullar() {
            std::cout << "Miuau!" << std::endl;
        }

};


int main(){

    Animal a;
    a.describir();

    Perro p;
    p.setEdad(4);
    p.setPeso(10);

    p.describir();
    p.ladrar();

    Gato g;
    g.setEdad(3);
    g.setPeso(3);
    
    g.describir();
    g.maullar();

    return 0;
}