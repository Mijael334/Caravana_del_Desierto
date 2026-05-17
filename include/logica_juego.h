#ifndef LOGICA_JUEGO_H_INCLUDED
#define LOGICA_JUEGO_H_INCLUDED

#include "../include/entidades.h"
#include "../include/cola_dinamica.h"
#include "../include/lista_doble_circular.h"
#include "../include/gestion_archivos.h"

#define MAX_DADO 6

#define DIR_ADELANTE 'F'
#define DIR_ATRAS 'B'


typedef enum
{
    VACIO = 0,
    OASIS,
    TORMENTA,
    VIDA_EXTRA,
    INICIO,
    SALIDA,
    PREMIO,
} tTipoEvento;

typedef struct 
{
    unsigned numeroCasillero;
    int jugadorAca;
    int cantBandidos;
    tTipoEvento evento;
}tCasillero;

typedef struct 
{
    unsigned cantPasos;
    char direccion;
    int id;
} tMovimiento;


typedef struct 
{
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
    tJugador jugador;
    tPartida partida;
    tConfig configPartida;
    tLista rankingJugadores;
} tJuego;

int inicializarJuego (tJuego* juego);
void ingresarNombreJugador (char* nombre);


int buscarJugadorEnRanking (tLista* rankingJugadores, const char* nombreABusc, int (*cmp) (const void*, const void*));

int generarRandomUniforme (int max_valor);


int cmpRanking (const void* a, const void* b);

#endif // LOGICA_JUEGO_H_INCLUDED
