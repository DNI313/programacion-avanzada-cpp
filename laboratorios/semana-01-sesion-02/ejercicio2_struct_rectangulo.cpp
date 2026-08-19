#include <iostream>

struct Rectangulo {
    double base; // atributos
    double altura; // atributos

    double area(){ // comportamiento -> metodo
        return base * altura;
    }
};

int main(){

    Rectangulo rect1;
    Rectangulo rect2;
    Rectangulo rect3;

    rect1.altura = 5;
    rect1.base = 10.4;

//  std::cout << "Area rect 1: " << area(base, altura) << std::endl;
    std::cout << "Area rect 1: " << rect1.area() << std::endl;


    return 0;
}