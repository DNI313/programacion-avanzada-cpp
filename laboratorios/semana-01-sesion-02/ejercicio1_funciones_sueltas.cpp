#include <iostream>

double area(double base, double altura){
    return base * altura;
}

int main(){

    double base = 10.0;
    double altura = 5.0l;

    std::cout << "Area rect 1: " << area(base, altura) << std::endl;
    std::cout << "Area rect 2: " << area(5.5, 7) << std::endl;
    std::cout << "Area rect 3: " << area(2.3, 7.8) << std::endl;



    return 0; // si llega aca, no hay error
}