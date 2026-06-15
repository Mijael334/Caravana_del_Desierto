#ifndef LOGICA_JUEGO_H_INCLUDED
#define LOGICA_JUEGO_H_INCLUDED

#include "entidades.h"
#include "cola_dinamica.h"
#include "lista_doble_circular.h"
#include "lista_enlazada.h"
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
    EVENTO_TURNO_TORMENTA_BLOQUEADA,
    EVENTO_TURNO_BANDIDO_BLOQUEADO,
    EVENTO_TURNO_TURNO_PERDIDO,   
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
    tCola registroMovimientos; 
    tLista ruta;
    tEventoTurno ultimoEvento;
    tEventoTurno eventoPrevio;
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
    tListaSE listaRankingJugadores;
    tArbolBinBusq arbolIndUsuarios;
    FILE* archPartidas;
} tJuego;

int inicializarJuego (tJuego* juego);
int procesarJuego (tJuego* juego);

int procesarMenu(tJuego* juego);
int procesarPartida (tJuego* juego);
int procesarPuntajePartida(const tUsuario* usuario, tPartida* partida, tEstadoJuego* estadoJuego, FILE* archPartidas);
int procesarRanking(tJuego* juego);

int actualizarEstadoPartida(tPartida* partida, unsigned cantBandidos, tEstadoJuego* estadoJuego);
void finalizarPartida (tJuego* juego);
void eliminarBandido(tBandido* bandido, tLista* ruta, unsigned cantCasilleros);

int buscarJugadorEnRanking (tLista* rankingJugadores, const char* nombreABusc, int (*cmp) (const void*, const void*));

void desencolarMovimientos(tPartida *partida, unsigned cantBandidos);
void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion, unsigned posJugador);
void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones);

void moverBandido(tBandido *bandido, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverBandidoEnRuta (tBandido* bandido, const tMovimiento* mov, tLista* ruta, unsigned cantPosiciones);
void moverJugador(tEstadoJugador *jugador, unsigned pasos, char direccion, unsigned cantPosiciones);
void moverJugadorEnRuta (tEstadoJugador* jugador, tLista* ruta);

void limpiarJuego (tJuego* juego);

void mostrarError (int err);

#endif // LOGICA_JUEGO_H_INCLUDED
