#include "punto2.h"

// Hago una funcion auxiliar para buscar si el prefijo y el caracter ya los tengo en el diccionario.
int buscarEnDiccionario(ParLZ78* diccionario, int tamañoDict, int prefijo, char c) {
    // Recorro desde 1 porque la posicion 0 es para la frase vacia
    for (int i = 1; i < tamañoDict; i++) {
        // Si el indice y el caracter coinciden con lo que busco, entonces es porque lo encontre
        if (diccionario[i].indice == prefijo && diccionario[i].caracter == c) {
            return i; // Retorno la posicion donde lo halle
        }
    }
    return 0; // Si termino el ciclo y no lo encuentro, devuelvo 0
}

ParLZ78* comprimirLZ78(const char* texto, int longitud, int& tamañoSalida) {
    // Creo mi diccionario con memoria dinamica usando new, con un tamaño fijo
    int capacidadDict = 1000;
    ParLZ78* diccionario = new ParLZ78[capacidadDict];
    int tamañoDict = 1; // Ya cuento la posicion 0
    diccionario[0] = {0, '\0'}; // La frase 0 es la cadena vacia

    // Creo el arreglo donde voy a guardar los pares que voy haciendo
    // En el peor caso, no comprime nada y ocupare la misma longitud del texto
    ParLZ78* salida = new ParLZ78[longitud];
    tamañoSalida = 0; // Llevo el conteo de los pares emitidos

    int prefijoActual = 0; // Siempre arranco buscando desde la raiz (0)

    // Recorro todo el texto caracter por caracter
    for (int i = 0; i < longitud; i++) {
        char c = texto[i];

        // Busco si la combinacion que llevo ya existe en mi diccionario
        int indiceEncontrado = buscarEnDiccionario(diccionario, tamañoDict, prefijoActual, c);

        if (indiceEncontrado != 0) {
            // Si ya existe, simplemente me muevo a ese indice y continuo
            prefijoActual = indiceEncontrado;
        } else {
            // Si no existe, emito el par actual y lo guardo en mi salida
            salida[tamañoSalida].indice = prefijoActual;
            salida[tamañoSalida].caracter = c;
            tamañoSalida++;

            // Tambien agrego esta nueva combinacion a mi diccionario
            diccionario[tamañoDict].indice = prefijoActual;
            diccionario[tamañoDict].caracter = c;
            tamañoDict++;

            // Reseteo el prefijo a 0 para empezar a buscar la siguiente frase desde cero
            prefijoActual = 0;
        }
    }

    // Si al terminar el texto me quedo un prefijo sin emitir, lo guardo con caracter nulo
    if (prefijoActual != 0) {
        salida[tamañoSalida].indice = prefijoActual;
        salida[tamañoSalida].caracter = '\0';
        tamañoSalida++;
    }

    // libero la memoria de mi diccionario temporal con delete[] porque ya no lo necesito
    delete[] diccionario;
    return salida; // Devuelvo mi arreglo de pares al main
}

char* descomprimirLZ78(ParLZ78* pares, int numPares, int& longitudSalida) {
    // Para descomprimir, reconstruyo mi diccionario igualito que en la compresion
    int capacidadDict = 1000;
    ParLZ78* diccionario = new ParLZ78[capacidadDict];
    int tamañoDict = 1;
    diccionario[0] = {0, '\0'};

    // Creo un arreglo de caracteres lo suficientemente grande para meter el texto final
    char* textoSalida = new char[10000];
    longitudSalida = 0; // Para saber cuanto texto llevo descomprimido

    // Recorro todos los pares que recibi
    for (int i = 0; i < numPares; i++) {
        int prefijo = pares[i].indice;
        char c = pares[i].caracter;

        // Guardo esta nueva frase reconstruida en mi diccionario
        if (prefijo != 0 || c != '\0') {
            diccionario[tamañoDict].indice = prefijo;
            diccionario[tamañoDict].caracter = c;
            tamañoDict++;
        }

        // Para saber cuanto mide la frase, navego hacia atras guiandome por los prefijos
        int tempPrefijo = tamañoDict - 1;
        int longitudFrase = 0;

        while (tempPrefijo != 0) {
            longitudFrase++; // Cuento cuantos saltos doy
            tempPrefijo = diccionario[tempPrefijo].indice;
        }

        // Ahora vuelvo a navegar hacia atras, pero esta vez copiando las letras
        tempPrefijo = tamañoDict - 1;

        // Me ubico exactamente al final de donde debe ir esta frase en mi texto de salida
        int posicion = longitudSalida + longitudFrase - 1;

        while (tempPrefijo != 0) {
            textoSalida[posicion] = diccionario[tempPrefijo].caracter; // Copio el caracter
            tempPrefijo = diccionario[tempPrefijo].indice;             // Salto al anterior
            posicion--;                                                // Me muevo hacia atras
        }

        // Sumo la longitud de esta frase a mi conteo total del texto
        longitudSalida += longitudFrase;
    }

    // Libero mi diccionario temporal
    delete[] diccionario;
    // Pongo un caracter nulo al final del texto por seguridad
    textoSalida[longitudSalida] = '\0';
    return textoSalida;
}