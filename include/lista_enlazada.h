#ifndef LISTA_ENLAZADA_H_INCLUDED
#define LISTA_ENLAZADA_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

#define ERROR_MEM -1
#define ERROR_VACIO -2
#define OK 1

typedef struct sNodoLista
{
    void *info;
    unsigned tamInfo;
    struct sNodoLista *sig;
}tNodoLista;

typedef tNodoLista *tLista;

void crearListaSimple(tLista *pl);
int listaVacia(const tLista *pl);
int insertarEnOrdenLista(tLista *pl, const void *d, unsigned tam, int (*comparar)(const void *, const void *));
void vaciarLista(tLista *pl);
void mostrarLista(const tLista *pl, void (*mostrar)(const void *));

#endif // LISTA_ENLAZADA_H_INCLUDED
