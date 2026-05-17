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

char calcularDireccionBandido(unsigned posBandido, unsigned posJugador, unsigned cantPosiciones)
{
    unsigned distanciaAdelante, distanciaAtras;

    if(posJugador >= posBandido)
        distanciaAdelante = posJugador - posBandido;
    else
        distanciaAdelante = cantPosiciones - posBandido + posJugador;

    distanciaAtras = cantPosiciones - distanciaAdelante;

    if(distanciaAdelante <= distanciaAtras)
        return DIR_ADELANTE;
    else
        return DIR_ATRAS;
}

void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion)
{
    tMovimiento mov = {pasos, direccion, -1};
    ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
}

void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones)
{
    int i=0,pasos;
    char dir;
    for(i=0; i < cantBandidos; i++)
    {
        pasos = generarValorDado();
        dir = calcularDireccionBandido(bandidos[i].posEnRuta, posJugador, cantPosiciones);
        tMovimiento mov = {pasos, dir, bandidos[i].id};
        ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
    }
}

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos,
                                   tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones)
{
    tMovimiento mov;
    int i;

    while(!colaVaciaDin(cola))
    {
        sacarDeColaDin(cola,&mov,sizeof(tMovimiento));

        if(mov.id== ID_JUGADOR)
        {
            // mueve jugador
        }
        else
        {
            for(i=0;i<cantBandidos;i++)
                if(bandidos[i].id == mov.id && bandidos[i].vivo == VIVO)
                {
                    // mueve bandido sabiendo sus pasos y direccion
                }
        }
    }
}

int cmpRanking (const void* a, const void* b)
{
    const tRanking* r1 = (const tRanking*) a;
    const tRanking* r2 = (const tRanking*) b;

    return strcmp(r1->nombre, r2->nombre);
}
