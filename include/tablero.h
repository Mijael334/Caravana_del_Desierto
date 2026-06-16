#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#include "entidades.h"
#include "lista_doble_circular.h"
#include "gestion_archivos.h"

#define MAX_SIMBOLOS 7

#define CANT_MIN_CASILLEROS 2

// Límites de la ruta 
#define MIN_POSICIONES 15
#define MAX_POSICIONES 100

#define ERROR_CONFIG_INAVLIDA -10

// Porcentajes máximos sugeridos para la distribución de elementos 
#define PCT_MAX_BANDIDOS   12/100   // 12% del tablero
#define PCT_MAX_PREMIOS    20/100   // 20% del tablero
#define PCT_MAX_OASIS      10/100   // 10% del tablero
#define PCT_MAX_TORMENTAS  12/100   // 12% del tablero
#define PCT_MAX_VIDAS_EXT  10/100   // 5% del tablero

// Límite fijo para la configuración inicial de vidas
#define MIN_VIDAS_INICIO   1
#define MAX_VIDAS_INICIO   5


typedef enum
{
    EVENTO_VACIO = 0,
    EVENTO_OASIS,
    EVENTO_TORMENTA,
    EVENTO_VIDA_EXTRA,
    EVENTO_INICIO,
    EVENTO_SALIDA,
    EVENTO_PREMIO,
} tTipoEvento;

typedef struct
{
    unsigned numeroCasillero;
    int jugadorAca;
    int cantBandidos;
    tTipoEvento evento;
} tCasillero;


int configuracion_valida(const tConfig *configuracion);
int criterio_tormenta_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_bandido_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
int criterio_casillero_libre(const tCasillero *act, const tCasillero *ant, const tCasillero *sig);
void distribuir_elemento(tLista *lista, int cant_posiciones, int cant_maxima, int (*criterio)(const tCasillero *, const tCasillero *, const tCasillero *), tTipoEvento tipo_evento, int es_bandido, tBandido *bandido);
int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos);
void renderizar_tablero(const tLista *lista, FILE *destino);
void guardar_tablero_en_archivo(tLista *lista);
int cmpCasillero (const void* a, const void* b);
int cmpCasilleroEvento (const void* a, const void* b);
void imprimirCasillero(void *info, FILE *destino);
void vaciar_datos_casillero(void *dato, FILE *pf);

#endif // TABLERO_H_INCLUDED
