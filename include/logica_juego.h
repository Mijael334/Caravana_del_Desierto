#ifndef LOGICA_JUEGO_H_INCLUDED
#define LOGICA_JUEGO_H_INCLUDED

#include "../include/entidades.h"
#include "../include/cola_dinamica.h"
#include "../include/lista_doble_circular.h"
#include "../include/gestion_archivos.h"

#define MAX_DADO 6


#define DIR_ADELANTE 'F'
#define DIR_ATRAS 'B'

#define PUNTOS_PREMIO 1


typedef enum
{
    EVENTO_VACIO = 0,
    EVENTO_OASIS,
    EVENTO_TORMENTA,
    EVENTO_VIDA_EXTRA,
    EVENTO_INICIO,
    EVENTO_SALIDA,
    EVENTO_PREMIO,
} tTipoEvento;

typedef enum
{
    ESTADO_MENU = 0,
    ESTADO_PARTIDA,
    ESTADO_PUNTAJE_PARTIDA,
    ESTADO_RANKING,
    ESTADO_SALIR,
} tEstadoJuego;

typedef struct
{
    unsigned numeroCasillero;
    int jugadorAca;
    int cantBandidos;
    tTipoEvento evento;
} tCasillero;

typedef struct
{
    unsigned cantPasos;
    char direccion;
    int id;
} tMovimiento;

typedef struct
{
    tJugador jugador;
    tBandido* bandidos;
    unsigned cantCasilleros;
    unsigned puntosEnPartida;
    int cantMovsAdelante;
    int cantMovsAtras;
    tCola movimientos;
    tLista ruta;
} tPartida;

typedef struct
{
    int corriendo;
    tEstadoJuego estadoJuego;
    tUsuario usuario;
    tPartida partida;
    tConfig configPartida;
    tLista rankingJugadores;
} tJuego;

int inicializarJuego (tJuego* juego);
int procesarJuego (tJuego* juego);
void ingresarNombreJugador (char* nombre);


int procesarPartida (tJuego* juego);
int actualizarEstadoPartida (tJugador* jugador, tBandido* bandidos, unsigned cantBandidos,tLista* ruta, unsigned cantCasilleros, tEstadoJuego* estadoJuego);
void finalizarPartida (tJuego* juego);
void eliminarBandido(tBandido* bandido, tLista* ruta, unsigned cantCasilleros);

int buscarJugadorEnRanking (tLista* rankingJugadores, const char* nombreABusc, int (*cmp) (const void*, const void*));

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones);
void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion, unsigned posJugador);
void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones);

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones);
void moverBandido(tBandido *bandido, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverBandidoEnRuta (tBandido* bandido, const tMovimiento* mov, tLista* ruta, unsigned cantPosiciones);
void moverJugador(tEstadoJugador *jugador, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverJugadorEnRuta (tEstadoJugador* jugador, tLista* ruta);

#endif // LOGICA_JUEGO_H_INCLUDED
