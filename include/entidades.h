#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include "reportes.h"

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
    char nombre [TAM_MAX_NOM + 1];
    unsigned puntos;
    tEstadoJugador estadoEnPartida;
} tJugador;


void inicializarJugador (tJugador* jugador, const char* nombre, unsigned puntos, unsigned cantVidas);
int crearBandidos (tBandido** bandidos, unsigned cantBandidos, unsigned cantCasilleros);
int generarRandomUniforme (int max_valor);
int cmpUsuario (const void* a, const void* b);
int agregarUsuarioEnArchivo(const tUsuario *usuario, const char *nombreArchivo, unsigned *posOut);
int leerUsuarioDeArchivo(tUsuario *usuario, unsigned pos, const char *nombreArchivo);

#endif // ENTIDADES_H_INCLUDED
