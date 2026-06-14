#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include "utilidades.h"

#define MAX_NOMBRE 10

#define ID_JUGADOR  1
#define INICIO_ID_BANDIDOS 2

#define VIVO 1
#define MUERTO 0

typedef struct
{
    unsigned id;
    unsigned posEnRuta;
    int vivo;
} tBandido;

typedef struct
{
    unsigned vidas;
    int protegido;
    int afectadoPorTormenta;
    unsigned puntos;
    unsigned posEnRuta;
} tEstadoJugador;

typedef struct
{
    unsigned id;
    char nombre [MAX_NOMBRE + 1];
    unsigned puntos;
    tEstadoJugador estadoEnPartida;
} tJugador;

typedef struct
{
    char username [MAX_NOMBRE + 1];
    char nickname [MAX_NOMBRE + 1];
} tUsuario;

void inicializarJugador (tJugador* jugador, const char* nombre, unsigned puntos, unsigned cantVidas);
int crearBandidos (tBandido** bandidos, unsigned cantBandidos, unsigned cantCasilleros);
int generarRandomUniforme (int max_valor);
int cmpUsuario (const void* a, const void* b);

#endif // ENTIDADES_H_INCLUDED
