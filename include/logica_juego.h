#ifndef LOGICA_JUEGO_H_INCLUDED
#define LOGICA_JUEGO_H_INCLUDED

#include "entidades.h"
#include "cola_dinamica.h"
#include "lista_doble_circular.h"
#include "gestion_archivos.h"
#include "tablero.h"

#define MSJ_RESULTADO_VICTORIA "VICTORIA"
#define MSJ_RESULTADO_DERROTA "DERROTA"

#define MAX_DADO 6

#define DIR_ADELANTE 'F'
#define DIR_ATRAS 'B'

#define PUNTOS_PREMIO 1

typedef enum
{
    ESTADO_MENU = 0,
    ESTADO_PARTIDA,
    ESTADO_PUNTAJE_PARTIDA,
    ESTADO_RANKING,
    ESTADO_SALIR,
} tEstadoJuego;

typedef enum
{
    EVENTO_TURNO_INICIO = 0,
    EVENTO_TURNO_NADA,
    EVENTO_TURNO_OASIS,
    EVENTO_TURNO_PREMIO,
    EVENTO_TURNO_VIDA_EXTRA,
    EVENTO_TURNO_TORMENTA,
    EVENTO_TURNO_BANDIDO,
    EVENTO_TURNO_MUERTE,
    EVENTO_TURNO_VICTORIA,
} tEventoTurno;

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
    tEventoTurno ultimoEvento;
    unsigned ultimosPasos;
    char ultimaDireccion;
} tPartida;

typedef struct
{
    int corriendo;
    tEstadoJuego estadoJuego;
    tUsuario usuario;
    tPartida partida;
    tConfig configPartida;
    tLista listaRankingJugadores;
    tArbolBinBusq arbolIndUsuarios;
} tJuego;

int inicializarJuego (tJuego* juego);
int procesarJuego (tJuego* juego);

int procesarMenu(tJuego* juego);
int procesarPartida (tJuego* juego);
int procesarPuntajePartida(const tUsuario* usuario, const tPartida* partida, tEstadoJuego* estadoJuego);

int actualizarEstadoPartida(tPartida* partida, unsigned cantBandidos, tEstadoJuego* estadoJuego);
void finalizarPartida (tJuego* juego);
void eliminarBandido(tBandido* bandido, tLista* ruta, unsigned cantCasilleros);

int buscarJugadorEnRanking (tLista* rankingJugadores, const char* nombreABusc, int (*cmp) (const void*, const void*));

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones, const tPartida *partida);
void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion, unsigned posJugador);
void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones);

void moverBandido(tBandido *bandido, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverBandidoEnRuta (tBandido* bandido, const tMovimiento* mov, tLista* ruta, unsigned cantPosiciones);
void moverJugador(tEstadoJugador *jugador, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverJugadorEnRuta (tEstadoJugador* jugador, tLista* ruta);

void limpiarJuego (tJuego* juego);

void mostrarError (int err);

#endif // LOGICA_JUEGO_H_INCLUDED
