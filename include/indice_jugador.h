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
#endif // INDICE_JUGADOR_H_INCLUDED
