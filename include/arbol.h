#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATO_NO_ENCONTRADO 0
#define DATO_ENCONTRADO 1
#define DATO_DUPLICADO -1
#define SIN_MEMORIA -2
#define TODO_OK 0

typedef struct sNodoArbol
{
    void *info;
    unsigned tamInfo;
    struct sNodoArbol *der,
                      *izq;
}tNodoArbol;
typedef tNodoArbol *tArbolBinBusq;

void crearArbolBinBusq(tArbolBinBusq *pt);
int insertarArbolBinBusq(tArbolBinBusq *pt, const void *d, unsigned cantbyte, int(*cmp)(const void *a1, const void *a2));
int buscarEnArbolBinBusq(const tArbolBinBusq *pt, void *d, unsigned cantbyte,int(*cmp)(const void *a1, const void *a2));
void recorrerArbolInnorden(const tArbolBinBusq *pt, unsigned n, void *params, void (*accion)(void *info , unsigned tam, unsigned m, void *param));
void eliminarArbol(tArbolBinBusq *pt);


#endif // ARBOL_H_INCLUDED
