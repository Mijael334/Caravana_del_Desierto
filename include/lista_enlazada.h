#ifndef LISTA_ENLAZADA_H_INCLUDED
#define LISTA_ENLAZADA_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

#define ERROR_MEM -1
#define ERROR_VACIO -2
#define CLA_DUP -3
#define OK 1

typedef struct sNodoLista
{
    void *info;
    unsigned tamInfo;
    struct sNodoLista *sig;
}tNodoLista;

typedef tNodoLista *tListaSE;

void crearListaSimple(tListaSE *pl);
int listaVacia(const tListaSE *pl);
int insertarEnOrdenLista(tListaSE *pl, const void *d, unsigned tam, int (*comparar)(const void *, const void *), int (*acum)(void** , unsigned* , const void*, unsigned));
void vaciarLista(tListaSE *pl);
void mostrarLista(const tListaSE *pl, void (*mostrar)(const void *, void*), void* paramExtra);
void ordenarLista (tListaSE* pl, int (*cmp)(const void*, const void*));
tListaSE* _buscarMenor (tListaSE* pl, int (*cmp)(const void*, const void*));

#endif // LISTA_ENLAZADA_H_INCLUDED
