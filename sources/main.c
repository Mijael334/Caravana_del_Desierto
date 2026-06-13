#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/entidades.h"
#include "../include/interfaz_usuario.h"
#include "../include/tablero.h"

int main()
{
    srand(time(NULL));

    char seleccion;
    tConfig configuracion;
    tLista tablero;
    tBandido *lista_bandidos = NULL;
    crearLista(&tablero);

    // Llamar a la carga del ABB desde el archivo binario (Integrante 3).

    // Mostrar menú y pedir nombre (Integrante 4).

    // Cargar configuración y generar lista circular (Integrante 1 y 3).

    // Bucle de juego:

    // Tirar dado y elegir dirección (Integrante 2).

    // Encolar movimientos (Integrante 1).

    // Desencolar y actualizar posiciones (Integrante 1 y 2).

    // Refrescar pantalla (Integrante 4).

    // Guardar resultados y mostrar historial (Integrante 3 y 4).

    return 0;
}
