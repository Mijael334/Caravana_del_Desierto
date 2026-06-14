#ifndef REPORTES_H_INCLUDED
#define REPORTES_H_INCLUDED

#include "utilidades.h"
#include "lista_enlazada.h"
#include "gestion_archivos.h"


#define TAM_RESULTADO 9

typedef struct
{
    char username [TAM_MAX_NOM + 1];
    char nickname [TAM_MAX_NOM + 1];
} tUsuario;

typedef struct
{
    unsigned idPartida;
    tUsuario usuario;
    unsigned cantMovimientos;
    unsigned puntosObtenidos;
    unsigned vidasRestantes;
    unsigned forwardCasillas;
    unsigned backwardCasillas;
    char resultado[TAM_RESULTADO];
}tReportePartida;

typedef struct
{
    tUsuario usuario;
    unsigned puntos;
} tRanking;

int registrarPartidaEnArchivo(tReportePartida *reporte);
void mostrarRanking(const tListaSE *ranking, void (*mostrarRegistro)(const void *));
void imprimirRegistroRanking(const void *d);
int cmpPuntos(const void *a, const void *b);

#endif // REPORTES_H_INCLUDED
