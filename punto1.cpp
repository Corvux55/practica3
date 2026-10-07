#include "punto1.h"
#include <stdexcept>
#include <cctype>

namespace punto1 {
std::string comprimir(const std::string &texto){
    if (texto.empty()){
        throw std::invalid_argument("punto1::comprimir: texto vacio");
    }
    std::string comprimido; //acumulador de salida
    const std::size_t n=texto.length();//size_t porque es lo que devuelve length()
    std::size_t i=0; //posicion actual
    while (i<n){
        const char actual = texto[i];
        std::size_t contador =1; // ya contamos el de la posicion i
        while (i + contador < n&& texto[i + contador]== actual){ //verificamos limite antes de acceder
            ++contador;
        }
        comprimido += std::to_string(contador);//cantidad como texto
        comprimido += actual; //caracter que se repite
        i+= contador;// saltar el bloque ya procesado
    }
    return comprimido;
}
std::string descomprimir(const std::string &comprimido){
    if (comprimido.empty()){
        throw std::invalid_argument("punto1::descomprimir: texto vacio");
    }
    std::string descomprimido;
    const std::size_t n=comprimido.length();
    std::size_t i=0;
    while (i <n){
        if (!std::isdigit(static_cast<unsigned char>(comprimido[i]))){ //error si no es digito
            throw std::runtime_error("punto1::descormprimir: se esperaba digito");
        }
        std::string numStr; //acomula los digitos de la cantidad
        while(i<n && std::isdigit(static_cast<unsigned char>(comprimido[i]))){ //lee todos los digitos
            numStr += comprimido[i];
            ++i;
        }
        if (i >= n){//debe existir el caracter
            throw std::runtime_error("punto1::descomprimir: falta caracter");
        }
        const int cantidad = std::stoi(numStr); //texto a numero
        const char caracter = comprimido[i];//lee caracter
        ++i;
        for(int k=0; k<cantidad;++k){ //expande
            descomprimido += caracter;
        }
    }
    return descomprimido;
}
}