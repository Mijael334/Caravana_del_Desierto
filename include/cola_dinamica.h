#ifndef COLA_DINAMICA_H_INCLUDED
#define COLA_DINAMICA_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

#define ERROR_MEM -1
#define ERROR_VACIO -2
#define OK 1


typedef struct sNodo
{
    void *info;
    unsigned tam;
    struct sNodo *sig;
}tNodo;

typedef struct{
   tNodo *pri;
   tNodo *ult;
}tCola;

void crearColaDin(tCola *p);

int colaVaciaDin(const tCola *p);

int ponerEnColaDin(tCola *p, const void *d, unsigned tam);
int verPrimeroColaDin(const tCola *p, void *d, unsigned tam);
int sacarDeColaDin(tCola *p, void *d, unsigned tam);

void vaciarColaDin(tCola* p);

#endif // COLA_DINAMICA_H_INCLUDED
