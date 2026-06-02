#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../include/logica_juego.h"
#include "../include/lista_doble_circular.h"
#include "../include/gestion_archivos.h"

#define MAX_SIMBOLOS 7

int criterio_tormenta_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_bandido_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_tormenta_emergencia(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_bandido_emergencia(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
tNodoL *buscar_nodo_en_posicion(tLista *lista, int posicion);
tNodoL *buscar_nodo_seguro(tLista *lista, int cant_posiciones, int (*es_ideal)(const tCasillero *, const tCasillero *, const tCasillero *), int (*es_emergencia)(const tCasillero *, const tCasillero *, const tCasillero *));
int configuracion_valida(const tConfig *configuracion);
void distribuir_evento_aleatorio(tLista *lista, int cant_posiciones, int cantidad_maxima, tTipoEvento tipo_evento);
void ubicar_tormenta_segura(tLista *lista, int cant_posiciones);
void ubicar_bandido_seguro(tLista *lista, int cant_posiciones, tBandido *bandido, int id_bandido);
int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos);
void renderizar_tablero(tLista *lista, int cant_posiciones, FILE *destino);
void guardar_tablero_en_archivo(tLista *lista, int cant_posiciones);

int cmpCasillero (const void* a, const void* b);
int cmpCasilleroEvento (const void* a, const void* b);

#endif // TABLERO_H_INCLUDED
