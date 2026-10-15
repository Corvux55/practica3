#include "punto4.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

// Incluyo las cabeceras de mis puntos respetando sus nombres exactos
#include "punto1.h"
#include "punto2.h"
#include "punto3.h"

using namespace std;

namespace punto4 {

void ejecutarPractica() {
    try {
        cout << "--- SISTEMA INTEGRADO (PUNTO 4) ---" << endl;

        // 1. Abrir y leer el archivo de entrada
        ifstream archivoLector("entrada.txt", ios::binary);
        if (!archivoLector.is_open()) {
            throw runtime_error("Error: No se pudo abrir entrada.txt");
        }

        string textoOriginal((istreambuf_iterator<char>(archivoLector)), istreambuf_iterator<char>());
        archivoLector.close();

        cout << "Seleccione compresion (1 para RLE, 2 para LZ78): ";
        int opcion;
        cin >> opcion;

        cout << "Ingrese n para rotacion de bits (1-7): ";
        int n;
        cin >> n;

        cout << "Ingrese clave K para XOR (numero 0-255): ";
        int claveInt;
        cin >> claveInt;
        unsigned char claveK = static_cast<unsigned char>(claveInt);

        if (opcion == 1) {
            // 1. Comprimir usando el punto1
            string comprimido = punto1::comprimir(textoOriginal);

            unsigned char* datosMemoria = reinterpret_cast<unsigned char*>(&comprimido[0]);
            size_t tamanoBytes = comprimido.length();

            // 2. Encriptar y Desencriptar usando el punto3
            punto3::encriptar(datosMemoria, tamanoBytes, n, claveK);
            punto3::desencriptar(datosMemoria, tamanoBytes, n, claveK);

            // 3. Descomprimir usando el punto1
            string descomprimido = punto1::descomprimir(comprimido);

            // 4. Guardar archivo de salida
            ofstream archivoEscritor("salida_rle.txt");
            archivoEscritor << descomprimido;
            archivoEscritor.close();

            if (textoOriginal == descomprimido) {
                cout << "Proceso RLE Exitoso." << endl;
            } else {
                cout << "Error: El texto RLE no coincide." << endl;
            }

        } else if (opcion == 2) {
            int numPares = 0;

            // 1. Comprimir usando el punto2
            punto2::ParLZ78* comprimido = punto2::comprimir(textoOriginal.c_str(), textoOriginal.length(), numPares);

            unsigned char* datosMemoria = reinterpret_cast<unsigned char*>(comprimido);
            size_t tamanoBytes = numPares * sizeof(punto2::ParLZ78);

            // 2. Encriptar y Desencriptar usando el punto3
            punto3::encriptar(datosMemoria, tamanoBytes, n, claveK);
            punto3::desencriptar(datosMemoria, tamanoBytes, n, claveK);

            // 3. Descomprimir usando el punto2
            int longFinal = 0;
            char* textoDescomprimido = punto2::descomprimir(comprimido, numPares, longFinal);

            string resultadoStr(textoDescomprimido, longFinal);

            // 4. Guardar archivo de salida
            ofstream archivoEscritor("salida_lz78.txt");
            archivoEscritor << resultadoStr;
            archivoEscritor.close();

            if (textoOriginal == resultadoStr) {
                cout << "Proceso LZ78 Exitoso." << endl;
            } else {
                cout << "Error: El texto LZ78 no coincide." << endl;
            }

            // Liberacion de memoria dinamica asignada
            delete[] comprimido;
            delete[] textoDescomprimido;

        } else {
            cout << "Opcion invalida." << endl;
        }

    } catch (const exception& e) {
        cout << "\nExcepcion capturada: " << e.what() << endl;
    }
}

}