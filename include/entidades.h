#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#define MAX_NOMBRE 10

#define JUGADOR  'J'
#define BANDIDO  'B'
#define PREMIO  'P'
#define OASIS  'O'
#define TORMENTA  'T'
#define VIDA_EXTRA  'V'
#define INICIO  'I'
#define SALIDA  'S'
#define VACIO  '.'


typedef struct
{

    unsigned vidas;
    int protegido;
    unsigned puntos;
    unsigned posEnRuta;
} tEstado_Jugador;

typedef struct
{
    unsigned id;
    char nombre [MAX_NOMBRE + 1];
    unsigned puntos;
    tEstado_Jugador estadoPartida;
} tJugador;

#endif // ENTIDADES_H_INCLUDED
