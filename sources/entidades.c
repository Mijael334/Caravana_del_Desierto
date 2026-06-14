#include "../include/entidades.h"
#include <string.h>
#include <stdlib.h>

void inicializarJugador (tJugador* jugador, const char* nombre, unsigned puntos, unsigned cantVidas)
{
    jugador->id = ID_JUGADOR;
    strcpy(jugador->nombre, nombre);
    jugador->puntos = puntos;

    jugador->estadoEnPartida.afectadoPorTormenta = FALSO;
    jugador->estadoEnPartida.posEnRuta = 1;
    jugador->estadoEnPartida.protegido = FALSO;
    jugador->estadoEnPartida.puntos = 0;
    jugador->estadoEnPartida.vidas = cantVidas;
}

int crearBandidos (tBandido** bandidos, unsigned cantBandidos, unsigned cantCasilleros)
{
    unsigned i;
    tBandido bandido;

    bandido.id = INICIO_ID_BANDIDOS;
    bandido.vivo = VIVO;

    *bandidos = malloc(sizeof(tBandido) * cantBandidos);

    if(*bandidos == NULL)
        return ERROR_MEM;

    for (i = 0; i < cantBandidos; i++)
    {
        bandido.id = bandido.id + i;
        bandido.posEnRuta = generarRandomUniforme(cantCasilleros);

        memcpy(*bandidos + i, &bandido, sizeof(bandido));
    }
    
    return TODO_OK;
} 

int generarRandomUniforme (int max_valor)
{
    int limite = RAND_MAX - (RAND_MAX % max_valor); //para que todos los numeros tengan las mismas prob de salir
    int random;

    do
    {
        random = rand();
    } while (random >= limite);

    return (random % max_valor) + 1;
}

int cmpUsuario (const void* a, const void* b)
{
    const tUsuario* u1 = (const tUsuario*) a;
    const tUsuario* u2 = (const tUsuario*) b;

    return strcmp(u1->nombre, u2->nombre);
}