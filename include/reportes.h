#ifndef REPORTES_H_INCLUDED
#define REPORTES_H_INCLUDED

#include "lista_enlazada.h"
#include "gestion_archivos.h"


#define TAM_RESULTADO 9

#define TAM_RANKING 10

typedef struct
{
    char username [TAM_MAX_NOM + 1];
    char nickname [TAM_MAX_NOM + 1];
} tUsuario;

typedef struct
{
    unsigned idPartida;
    tUsuario usuario;
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

int cargarRankingDesdeArch(tListaSE* listaRanking, FILE* arch);
int registrarPartidaEnArchivo(tReportePartida *reporte, FILE* arch);
int acumularPuntos(void** info, unsigned* tamInfo, const void* d, unsigned tam);
int cmpUsuario (const void* a, const void* b);
void mostrarRanking(const tListaSE *ranking, void (*mostrarRegistro)(const void *, void* ));
void imprimirRegistroRanking(const void *d, void* paramExtra);
int cmpPuntos(const void *a, const void *b);

#endif // REPORTES_H_INCLUDED
