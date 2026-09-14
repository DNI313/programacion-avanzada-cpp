#include <iostream>
using namespace std;

int main(){
    int numero = 10;
    int *ptr_puntero = &numero;

    std::cout << "Direccion de memoria de numero: " << ptr_puntero << std::endl;
    std::cout <<"Valor de memoria desde puntero: " << *ptr_puntero << std::endl;
    std::cout << "Valor memoria desde la variable: " << &ptr_puntero << std::endl;

    int notas [5] = {80, 90, 60, 50, 71};
    int *ptr_notas = notas;
    
    for (int i = 0; i < 5; i++){
        std::cout << "Valor de memoria desde puntero: " << *(ptr_notas + i) << std::endl;
    }
    
    return 0;


}