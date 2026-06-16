#ifndef INTERFAZ_USUARIO_H_INCLUDED
#define INTERFAZ_USUARIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/arbol.h"
#include "../include/indice_jugador.h"
#include "../include/tablero.h"
#include "../include/logica_juego.h"

#define TITULO_JUEGO "CARAVANA DEL DESIERTO"

#define TAM_TITULO 50

#define TECLA_ESPACIO 32
#define DIFERENCIA_MAYUS_Y_MINUS ('a' - 'A')

#define TIEMPO_ANIMACION_JUGADOR_MS 150
#define TIEMPO_ANIMACION_BANDIDO_MS 50

typedef enum
{
    OP_MENU_NUEVA_PARTIDA = 0,
    OP_MENU_RANKING,
    OP_MENU_PARTIDAS,
    OP_MENU_SALIR,
    CANT_OP_MENU,
} tOpcionMenu;



void mostrarBienvenida(const char *nickname);
 
int seleccionarOpcionMenu(const char *titulo, const char *opciones[], int cantOpciones);
int solicitarNombreUsuario(char *nombre, int tamMaxNombre, const tArbolBinBusq *arbolIndice, unsigned *indiceReg);
void leerNicknamePorTeclado(char *nickname, int tamMaxNickname);
void mostrarEstadoPartida(const tPartida *partida);
void mostrarMensajeEvento(tEventoTurno evento);
void mostrarTableroEsperandoTurno(tLista *ruta, const tPartida *partida);
int pedirDireccionJugador(tLista *ruta, const tPartida *partida, int cantPasos);

void animarMovimientoJugador(tPartida *partida, unsigned pasos, char direccion);
void animarMovimientoBandido(tPartida *partida,tBandido *bandido, unsigned pasos, char direccion);

#endif // INTERFAZ_USUARIO_H_INCLUDED
