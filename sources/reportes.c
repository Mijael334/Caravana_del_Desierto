#include "../include/reportes.h"
#include "../include/interfaz_usuario.h"
#include <conio.h>

int cmpUsuario (const void* a, const void* b)
{
    const tUsuario* u1 = (const tUsuario*) a;
    const tUsuario* u2 = (const tUsuario*) b;

    return strcmp(u1->username, u2->username);
}

int cmpUsuarioRanking(const void* a, const void* b)
{
    const tRanking* r1 = (const tRanking*)a;
    const tRanking* r2 = (const tRanking*)b;

    return strcmp(r1->usuario.username, r2->usuario.username);
}

int acumularPuntos(void** info, unsigned* tamInfo, const void* d, unsigned tam)
{
    tRanking* rankOrig = (tRanking*) *info;
    const tRanking* rankNue = (const tRanking*)d;

    rankOrig->puntos += rankNue->puntos;

    return OK;
}

int cargarRankingDesdeArch(tListaSE* listaRanking, FILE* arch)
{
    tReportePartida reporte;
    tRanking jugador;
    int ret;

    fflush(arch);
    fseek(arch, 0, SEEK_SET);

    while(fread(&reporte, sizeof(tReportePartida), 1, arch))
    {
        strcpy(jugador.usuario.username, reporte.usuario.username);
        strcpy(jugador.usuario.nickname, reporte.usuario.nickname);
        jugador.puntos = reporte.puntosObtenidos;
        ret = insertarEnOrdenLista(listaRanking, &jugador, sizeof(tRanking), cmpUsuarioRanking, acumularPuntos);
        
        if(ret != OK && ret!=CLA_DUP)
            return ERROR_MEM;
    }

    ordenarLista(listaRanking, cmpPuntos);

    return TODO_OK;
}

int registrarPartidaEnArchivo(tReportePartida *reporte, FILE* arch)
{
    fflush(arch);
    fseek(arch, 0L, SEEK_END);
    reporte->idPartida = (ftell(arch) / sizeof(tReportePartida)) + 1;
    fwrite(reporte, sizeof(tReportePartida), 1, arch);

    return TODO_OK;
}

int cmpPuntos(const void *a, const void *b)
{
    const tRanking *r1 = (const tRanking *)a;
    const tRanking *r2 = (const tRanking *)b;
    return r2->puntos - r1->puntos;
}

void imprimirRegistroRanking(const void *d, void* paramExtra)
{
    int* contador = (int*) paramExtra;
    const tRanking *r = (const tRanking *)d;

    if(*contador < TAM_RANKING)
    {
        printf("  #%-4d %-20s %-20s %-10u\n", (*contador) + 1, r->usuario.username, r->usuario.nickname, r->puntos);
        (*contador)++;
    }
}

void mostrarRanking(const tListaSE *ranking, void (*mostrarRegistro)(const void *, void* ))
{
    int contador = 0;
    system("CLS");
    printf("\n=============================== RANKING ===============================\n\n");

    if(listaVacia(ranking))
    {
        printf("  Ranking vacio.\n");
    }
    else
    {
        printf("  %-5s %-20s %-20s %-10s\n", "TOP","Username", "NICKNAME", "PUNTOS");
        printf("  ---------------------------------------------------------------------\n");
        mostrarLista(ranking, imprimirRegistroRanking, &contador);
    }

    printf("\n=======================================================================\n");

    printf("\n   Presione [ESPACIO] para continuar...\n");
    while(getch() != TECLA_ESPACIO);
}
