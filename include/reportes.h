#ifndef REPORTES_H_INCLUDED
#define REPORTES_H_INCLUDED

#include "../include/logica_juego.h"
#include "../include/lista_enlazada.h"

typedef struct
{
    unsigned idPartida;
    tUsuario usuario;
    tEstadoJugador datosPartida;
    unsigned cantMovimientos;
    char resultado[10];
}tReportePartida;

void registrarPartidaEnArchivo(const tJuego *juego, const char *resultado);
void mostrarRankingJugadores(tLista *rankingJugadores);
void imprimirRanking(const void *d);
int cmpPuntos(const void *a, const void *b);

#endif // REPORTES_H_INCLUDED
