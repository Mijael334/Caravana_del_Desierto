#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#define MAX_NOMBRE 10

#define JUGADOR  'J'
#define BANDIDO  'B'


typedef struct 
{
    unsigned id;
    unsigned posEnRuta;
} tBandido;


typedef struct
{
    unsigned vidas;
    int protegido;
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
