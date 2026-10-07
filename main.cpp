#include <iostream>
#include "punto1.h"
#include <exception>

int main(){
    try {
    std:: string original;
    std::cout << "ingrese la cadena a comprimir";
    std::cin >>original;
    if(original.empty()){
        std::cerr <<"No ingreso ninguna cadena \n";
        return 1;
    }
    const std::string comprimido = punto1::comprimir(original);
    const std::string recuperado = punto1::descomprimir(comprimido);

    std::cout<<"Original: " <<original<<"\n";
    std::cout<<"Comprimido: "<<comprimido<<"\n";
    std::cout<<"Recuperado: "<<recuperado<<"\n";

    std::cout <<(original ==recuperado? "[OK] coincide.\n":"[ERROR] No coincide.\n");
}
catch(const std::exception& e){
    std::cerr << "Excwpcion: "<<e.what()<<"\n";
    return 1;
}
return 0;
}
