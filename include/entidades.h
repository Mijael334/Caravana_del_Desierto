#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

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
    unsigned puntos;
    char nombre [MAX_NOMBRE + 1];
} tRanking;


#endif // ENTIDADES_H_INCLUDED
