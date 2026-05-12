#ifndef LOGICA_JUEGO_H_INCLUDED
#define LOGICA_JUEGO_H_INCLUDED

#include "../include/entidades.h"

#define MAX_DADO 6

typedef struct 
{
    int corriendo;
    tJugador jugador;
    //tPartida partida; tengo que pensar que datos va a tener
}tJuego;


int generarValorDado ();

#endif // LOGICA_JUEGO_H_INCLUDED
