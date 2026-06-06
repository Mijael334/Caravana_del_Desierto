#ifndef INDICE_JUGADOR_H_INCLUDED
#define INDICE_JUGADOR_H_INCLUDED

#include "arbol.h"
#define TAM_MAX_NOM 20
typedef struct
{
    char nombre[TAM_MAX_NOM];
}tClave;

typedef struct
{
    tClave clave;
    unsigned indiceRegistro;
}tIndice;

int compararClaveIndice(const void *a, const void *b);
int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *arch, tArbolBinBusq *pa, unsigned tam, int limiteInferior, int limiteSuperior);
int prepararLimitesCargaArbol(FILE *archivoIndice, tArbolBinBusq *pa, unsigned tam);
int cargarIndiceDesdeArchivo(tArbolBinBusq *pa, const char *nombreArchivoIndice, unsigned tam);

#endif // INDICE_JUGADOR_H_INCLUDED
