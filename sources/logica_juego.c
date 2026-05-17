#include "../include/logica_juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int inicializarJuego (tJuego* juego)
{
    char aceptar;

    srand(time(NULL));

    crearLista(&juego->rankingJugadores);

    lectura_de_configuracion(NOM_ARCH_CONFIG, &juego->configPartida);

    //cargarRankingDeJugadores(tLista* &juego->ranking)
    
    do 
    {
        int pos;
        aceptar = 'Y';

        ingresarNombreJugador(&juego->jugador.nombre);

        //pos = buscarJugadorEnRanking (ranking ,&juego->ranking, &juego->jugador);

        if(pos != NO_ENCONTRADO)
        {
            printf("Es usted el jugador %s Y/N:", juego->jugador.nombre);
            scanf (" %c", &aceptar);
            fflush(stdin);

            if(aceptar == 'N')
                printf("El nombre %s ya esta en uso.\n");
        }

    } while (aceptar != 'Y');
    
    juego->corriendo = 1;
}

void ingresarNombreJugador (char* nombre)
{
    printf("Ingrese su nombre: ");
    fgets(nombre, MAX_NOMBRE + 1, stdin);
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


int cmpRanking (const void* a, const void* b)
{
    const tRanking* r1 = (const tRanking*) a;
    const tRanking* r2 = (const tRanking*) b;

    return strcmp(r1->nombre, r2->nombre);
}