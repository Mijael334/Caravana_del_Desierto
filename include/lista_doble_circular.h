#ifndef LISTA_DOBLE_CIRCULAR_H_INCLUDED
#define LISTA_DOBLE_CIRCULAR_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

#define OK 1
#define ERROR_MEM -1
#define ERROR_VACIA -2
#define ERROR_POS -3
#define NO_ENCONTRADO -5

typedef struct sNodoL {
    void *info;
    unsigned tam;
    struct sNodoL *sig;
    struct sNodoL *ant;
}tNodoL;

typedef tNodoL* tLista;

void crearLista(tLista *p);
void liberarLista(tLista *p);

int insFinLista(tLista *p, void *info, unsigned tam);
int insPriLista(tLista *p, void *info, unsigned tam);

//void recorrerLista(const tLista *p, void (*procesar)(void*));
void recorrerLista(const tLista *p, void (*procesar)(void *, FILE *), FILE *pf);

void* buscarElemPorClaveLista(const tLista *p, const void *clave, int (*cmp)(const void*, const void*));

#endif // LISTA_DOBLE_CIRCULAR_H_INCLUDED
