#ifndef PUNTO2_H
#define PUNTO2_H

// Creo la estructura basica para representar el par (i, c) que me pide el pdf.
// Ojo, no puedo usar clases raras, solo una estructura simple.
struct ParLZ78 {
    int indice;    // Indice de la frase anterior
    char caracter; // Caracter que extiende la frase
};

// Defino mis funciones principales para comprimir y descomprimir.
// Debo usar punteros crudos porque el pdf me prohibe usar std::vector o std::string aqui.
ParLZ78* comprimirLZ78(const char* texto, int longitud, int& tamañoSalida);
char* descomprimirLZ78(ParLZ78* pares, int numPares, int& longitudSalida);

#endif // PUNTO2_H
