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

#define MAX_TEXTO_MENU 35


char menu(const char matriz_menu[][MAX_TEXTO_MENU], const char *titulo);
char opcion(const char matriz_menu[][MAX_TEXTO_MENU], const char *titulo, const char *mensaje);
 
int seleccionarOpcionMenu(const char *titulo, const char *opciones[], int cantOpciones);
int solicitarNombreUsuario(char *nombre, int tamMaxNombre, const tArbolBinBusq *arbolIndice, unsigned *indiceReg);
void leerNicknamePorTeclado(char *nickname, int tamMaxNickname);
void mostrarEstadoPartida(const tPartida *partida);
void mostrarMensajeEvento(tEventoTurno evento);
void mostrarTableroEsperandoTurno(tLista *ruta, const tPartida *partida);
int pedirDireccionJugador(tLista *ruta, const tPartida *partida, int cantPasos);

#endif // INTERFAZ_USUARIO_H_INCLUDED
