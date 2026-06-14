#ifndef INDICE_JUGADOR_H_INCLUDED
#define INDICE_JUGADOR_H_INCLUDED


#include "gestion_archivos.h"

#define TAM_MAX_NOM 20

#define CLAVE_ENCONTRADA 1
#define CLAVE_NO_ENCONTRADA 0

typedef struct
{
    char nombre[TAM_MAX_NOM];
}tClave;

typedef struct
{
    tClave clave;
    unsigned indiceRegistro;
}tIndice;

int cmpClaveIndice(const void *a, const void *b);
int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *arch, tArbolBinBusq *pa, unsigned tam, int limiteInferior, int limiteSuperior);
int prepararLimitesCargaArbol(FILE *archivoIndice, tArbolBinBusq *pa, unsigned tam);
int cargarIndiceDesdeArchivo(tArbolBinBusq *pa, const char *nombreArchivoIndice, unsigned tam);

int indexarArchivoUsuariosOrdenado(tArbolBinBusq *pa, const char *nombreArchivoUsuarios);
void cargarArchivoOrdenadoEnIndiceBalanceado(tArbolBinBusq *pa, FILE *arch, int inicio, int fin);
int crearArchivoIndiceOrdenado(tArbolBinBusq *pa, const char *nombreArchivo);
#endif // INDICE_JUGADOR_H_INCLUDED
