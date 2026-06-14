#include "../include/reportes.h"

int registrarPartidaEnArchivo(tReportePartida *reporte)
{
    FILE *arch;
    if(abrir_archivo(&arch, NOM_ARCH_PARTIDAS, "a+b"))
    {
        return ERROR_ARCHIVO_PARTIDAS;
    }
    fseek(arch, 0L, SEEK_END);
    reporte->idPartida = (ftell(arch) / sizeof(tReportePartida)) + 1;
    fwrite(reporte, sizeof(tReportePartida), 1, arch);
    fclose(arch);

    return TODO_OK;
}

int cmpPuntos(const void *a, const void *b)
{
    const tRanking *r1 = (const tRanking *)a;
    const tRanking *r2 = (const tRanking *)b;
    return r1->puntos - r2->puntos;
}

void imprimirRegistroRanking(const void *d)
{
    const tRanking *r = (const tRanking *)d;
    printf(" \t%s\t\t%s\t\t%d\n", r->usuario.username, r->usuario.nickname, r->puntos);
}

void mostrarRanking(const tListaSE *ranking, void (*mostrarRegistro)(const void *))
{
    system("CLS");
    printf("\n=================== RANKING ===================\n\n");

    if(listaVacia(ranking))
    {
        printf("  Ranking vacio.\n");
    }
    else
    {
        printf(" \tNickname\t\tUsername\t\tPuntos\n");
        printf("  ---------------------------------------------\n");
        mostrarLista(ranking, mostrarRegistro);
    }

    printf("\n===============================================\n");
    system("pause");
}
