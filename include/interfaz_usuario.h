#ifndef INTERFAZ_USUARIO_H_INCLUDED
#define INTERFAZ_USUARIO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/arbol.h"
#include "../include/indice_jugador.h"


#define TITULO_JUEGO "CARAVANA DEL DESIERTO"
#define MAX_TEXTO_MENU 35


char menu(const char matriz_menu[][MAX_TEXTO_MENU], const char *titulo);
char opcion(const char matriz_menu[][MAX_TEXTO_MENU], const char *titulo, const char *mensaje);
 
int seleccionarOpcionMenu(const char *titulo, const char *opciones[], int cantOpciones);
int solicitarNombreUsuario(char *nombre, int tamMaxNombre, const tArbolBinBusq *arbolIndice);

#endif // INTERFAZ_USUARIO_H_INCLUDED
