#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#define MAX_NOMBRE 10

#define ID_JUGADOR  1
#define INICIO_ID_BANDIDOS 2

#define VIVO 1
#define MUERTO 0

#define FALSO 0
#define VERDADERO 1

#define TODO_OK 0
#define ERR_MEM 1


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
    char nombre [MAX_NOMBRE + 1];
    unsigned puntos;
} tUsuario;

void inicializarJugador (tJugador* jugador, const char* nombre, unsigned puntos, unsigned cantVidas);
int crearBandidos (tBandido** bandidos, unsigned cantBandidos, unsigned cantCasilleros);
int generarRandomUniforme (int max_valor);
int cmpUsuario (const void* a, const void* b);

#endif // ENTIDADES_H_INCLUDED
