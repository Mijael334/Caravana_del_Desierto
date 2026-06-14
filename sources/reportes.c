#include "../include/reportes.h"

int registrarPartidaEnArchivo(const tJuego *juego, const char *resultado)
{
    FILE *arch;
    tReportePartida reporte;
    if(abrir_archivo(&arch, NOM_ARCH_PARTIDAS, "a+b"))
    {
        return ERROR_ARCHIVO_PARTIDAS;
    }
    fseek(arch, 0L, SEEK_END);
    reporte.idPartida = (ftell(arch) / sizeof(tReportePartida)) + 1;
    reporte.usuario = juego->usuario;
    reporte.datosPartida = juego->partida.jugador.estadoEnPartida;
    reporte.cantMovimientos = juego->partida.cantMovsAdelante + juego->partida.cantMovsAtras;
    strcpy(reporte.resultado, resultado);
    fwrite(&reporte, sizeof(tReportePartida), 1, arch);
    fclose(arch);

    return TODO_OK;
}

int cmpPuntos(const void *a, const void *b)
{
    const tRanking *r1 = (const tRanking *)a;
    const tRanking *r2 = (const tRanking *)b;
    return r1->puntos - r2->puntos;
}

void imprimirRanking(const void *d)
{
    const tRanking *r = (const tRanking *)d;
    printf("     %-20s %-20s %10u pts\n", r->usuario.username, r->usuario.nickname, r->puntos);
}

void mostrarRankingJugadores(tListaSE *rankingJugadores)
{
    FILE *arch;
    tUsuario usuario;
    vaciarLista(rankingJugadores);
    if(abrir_archivo(&arch, NOM_ARCH_USUARIOS, "rb"))
    {
        return;
    }
    while(fread(&usuario, sizeof(tUsuario), 1, arch))
    {
        insertarEnOrdenLista(rankingJugadores, &usuario, sizeof(tUsuario), cmpPuntos);
    }
    fclose(arch);
    if(listaVacia(rankingJugadores))
    {
        printf("Rankig vacio\n");
    }
    else
    {
        mostrarLista(rankingJugadores, imprimirRanking);
    }
    vaciarLista(rankingJugadores);
}
