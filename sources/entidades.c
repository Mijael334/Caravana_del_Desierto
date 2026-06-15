#include "../include/entidades.h"
#include "../include/gestion_archivos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void inicializarJugador (tJugador* jugador, const char* nombre, unsigned puntos, unsigned cantVidas)
{
    jugador->id = ID_JUGADOR;
    strcpy(jugador->nombre, nombre);
    jugador->puntos = puntos;

    jugador->estadoEnPartida.afectadoPorTormenta = FALSO;
    jugador->estadoEnPartida.posEnRuta = 1;
    jugador->estadoEnPartida.protegido = FALSO;
    jugador->estadoEnPartida.puntos = 0;
    jugador->estadoEnPartida.vidas = cantVidas;
}

int crearBandidos (tBandido** bandidos, unsigned cantBandidos, unsigned cantCasilleros)
{
    unsigned i;
    tBandido bandido;

    bandido.id = INICIO_ID_BANDIDOS;
    bandido.vivo = VIVO;

    *bandidos = malloc(sizeof(tBandido) * cantBandidos);

    if(*bandidos == NULL)
        return ERROR_MEM;

    for (i = 0; i < cantBandidos; i++)
    {
        bandido.id++;
        bandido.posEnRuta = generarRandomUniforme(cantCasilleros);

        memcpy(*bandidos + i, &bandido, sizeof(bandido));
    }
    
    return TODO_OK;
} 

int generarRandomUniforme (int max_valor)
{
    int limite = RAND_MAX - (RAND_MAX % max_valor); //para que todos los numeros tengan las mismas prob de salir
    int random;

    do
    {
        random = rand();
    } while (random >= limite);

    return (random % max_valor) + 1;
}


int agregarUsuarioEnArchivo(const tUsuario *usuario, const char *nombreArchivo, unsigned *posOut)
{
    FILE *arch;
    long offset;
 
    arch = fopen(nombreArchivo, "ab");
    if(arch == NULL)
        return ERROR_ARCHIVO_USUARIOS;
 
    offset = ftell(arch);
    if(offset < 0)
    {
        fclose(arch);
        return ERROR_ARCHIVO_USUARIOS;
    }
 
    fwrite(usuario, sizeof(tUsuario), 1, arch);
    fclose(arch);
 
    *posOut = (offset / sizeof(tUsuario));
    return TODO_OK;
}
 
int leerUsuarioDeArchivo(tUsuario *usuario, unsigned pos, const char *nombreArchivo)
{
    FILE *arch;
 
    arch = fopen(nombreArchivo, "rb");
    if(arch == NULL)
        return ERROR_ARCHIVO_USUARIOS;
 
    fseek(arch, pos * sizeof(tUsuario), SEEK_SET);
    if(fread(usuario, sizeof(tUsuario), 1, arch) != 1)
    {
        fclose(arch);
        return ERROR_ARCHIVO_USUARIOS;
    }
 
    fclose(arch);
    return TODO_OK;
}