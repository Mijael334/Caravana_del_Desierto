#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../include/logica_juego.h"
#include "../include/lista_doble_circular.h"
#include "../include/gestion_archivos.h"

#define MAX_SIMBOLOS 7

int configuracion_valida(const tConfig *configuracion);
int criterio_tormenta_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_bandido_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_casillero_libre(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
void distribuir_elemento(tLista *lista, int cant_posiciones, int cant_maxima, int (*criterio)(const tCasillero *, const tCasillero *, const tCasillero *), tTipoEvento tipo_evento, int es_bandido, tBandido *bandido);
int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos);
void renderizar_tablero(tLista *lista, int cant_posiciones, FILE *destino);
void guardar_tablero_en_archivo(tLista *lista, int cant_posiciones);
int cmpCasillero (const void* a, const void* b);
int cmpCasilleroEvento (const void* a, const void* b);

#endif // TABLERO_H_INCLUDED
