#include "punto4.h"

// Incluyo las librerias estandar que necesito para consola, archivos, strings y excepciones
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

// Incluyo los encabezados de mis puntos anteriores para poder llamarlos dentro del flujo
#include "punto1.h"
#include "punto2.h"
#include "punto3.h"

using namespace std;

namespace punto4 {

void ejecutarPractica() {
    // Encierro todo el proceso en un bloque try para capturar cualquier error inesperado
    try {
        cout << "--- SISTEMA INTEGRADO ---" << endl;

        // 1. Abrir y leer el archivo de entrada
        // Abro 'entrada.txt' en modo binario para evitar alteraciones en caracteres especiales o saltos de linea
        ifstream archivoLector("entrada.txt", ios::binary);

        // Valido que el archivo realmente haya abierto. Si no existe, lanzo una excepcion
        if (!archivoLector.is_open()) {
            throw runtime_error("Error: No se pudo abrir entrada.txt");
        }

        // Leo todo el contenido del archivo de un solo golpe usando iteradores y lo guardo en mi string
        string textoOriginal((istreambuf_iterator<char>(archivoLector)), istreambuf_iterator<char>());
        archivoLector.close(); // Cierro el archivo apenas termino la lectura por buena practica

        // Le pido al usuario que escoja que algoritmo de compresion desea probar
        cout << "Seleccione compresion (1 para RLE, 2 para LZ78): ";
        int opcion;
        cin >> opcion;

        // Pido la cantidad de posiciones a rotar los bits para el modulo de encriptacion
        cout << "Ingrese n para rotacion de bits (1-7): ";
        int n;
        cin >> n;

        // Pido la clave para el proceso XOR
        cout << "Ingrese clave K para XOR (numero 0-255): ";
        int claveInt;
        cin >> claveInt;

        // Cambio el entero que digito el usuario a un byte sin signo (unsigned char)
        unsigned char claveK = static_cast<unsigned char>(claveInt);

        if (opcion == 1) {
            // 1. Comprimir usando el punto1
            // Llamo a mi funcion de compresion del punto1
            string comprimido = punto1::comprimir(textoOriginal);

            // Obtengo un puntero de bytes al contenido de la cadena comprimida para poder encriptarla
            unsigned char* datosMemoria = reinterpret_cast<unsigned char*>(&comprimido[0]);
            size_t tamanoBytes = comprimido.length(); // Calculo la cantidad de bytes a procesar

            // 2. Encriptar y Desencriptar usando el punto3
            // Aplico la rotacion y el XOR sobre los bytes comprimidos
            punto3::encriptar(datosMemoria, tamanoBytes, n, claveK);

            // Revierto la encriptacion usando la misma clave y rotacion inversa para recuperar la data
            punto3::desencriptar(datosMemoria, tamanoBytes, n, claveK);

            // 3. Descomprimir usando el punto1
            // Retorno la cadena a su estado de texto original
            string descomprimido = punto1::descomprimir(comprimido);

            // 4. Guardar archivo de salida
            // Creo el archivo 'salida_rle.txt' y escribo el resultado obtenido
            ofstream archivoEscritor("salida_rle.txt");
            archivoEscritor << descomprimido;
            archivoEscritor.close();

            // Verifico que todo el proceso haya sido correcto comparando con la cadena inicial
            if (textoOriginal == descomprimido) {
                cout << "Proceso RLE Exitoso." << endl;
            } else {
                cout << "Error: El texto RLE no coincide." << endl;
            }

        } else if (opcion == 2) {
            int numPares = 0; // Variable donde la funcion de compresion me devolvera el tamano de la salida

            // 1. Comprimir usando el punto2
            // Paso el texto original convertido a puntero de caracteres y obtengo el arreglo dinamico de estructuras ParLZ78
            punto2::ParLZ78* comprimido = punto2::comprimir(textoOriginal.c_str(), textoOriginal.length(), numPares);

            // Reinterpreto el arreglo de estructuras como un bloque de bytes para que el punto3 pueda trabajar sobre el
            unsigned char* datosMemoria = reinterpret_cast<unsigned char*>(comprimido);

            // Calculo los bytes totales multiplicando la cantidad de pares por lo que mide cada estructura
            size_t tamanoBytes = numPares * sizeof(punto2::ParLZ78);

            // 2. Encriptar y Desencriptar usando el punto3
            // Encripto la memoria del LZ78
            punto3::encriptar(datosMemoria, tamanoBytes, n, claveK);

            // Desencripto la memoria para retornar al arreglo de pares original
            punto3::desencriptar(datosMemoria, tamanoBytes, n, claveK);

            // 3. Descomprimir usando el punto2
            int longFinal = 0; // Variable que me almacenara la longitud final del texto reconstruido
            char* textoDescomprimido = punto2::descomprimir(comprimido, numPares, longFinal);

            // Convierto la cadena de char* a un std::string para facilitar la comparacion y guardado
            string resultadoStr(textoDescomprimido, longFinal);

            // 4. Guardar archivo de salida
            // Creo el archivo de resultado para LZ78
            ofstream archivoEscritor("salida_lz78.txt");
            archivoEscritor << resultadoStr;
            archivoEscritor.close();

            // Verifico que el texto reconstruido sea exacto al original
            if (textoOriginal == resultadoStr) {
                cout << "Proceso LZ78 Exitoso." << endl;
            } else {
                cout << "Error: El texto LZ78 no coincide." << endl;
            }

            // Liberacion de memoria dinamica
            // Liberar los arreglos creados con 'new' en el punto2 para no generar fugas de memoria
            delete[] comprimido;
            delete[] textoDescomprimido;

        } else {
            cout << "Opcion invalida." << endl;
        }

    } catch (const exception& e) {
        // Capturo cualquier excepcion lanzada por los modulos o por fallas de archivo y muestro el mensaje
        cout << "\nExcepcion capturada: " << e.what() << endl;
    }
}

}