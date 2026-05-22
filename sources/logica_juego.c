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

        ingresarNombreJugador(juego->jugador.nombre);

        //pos = buscarJugadorEnRanking (ranking ,&juego->ranking, &juego->jugador);

        if(pos != NO_ENCONTRADO)
        {
            printf("Es usted el jugador %s Y/N:", juego->jugador.nombre);
            scanf (" %c", &aceptar);
            fflush(stdin);

            if(aceptar == 'N')
                printf("El nombre %s ya esta en uso.\n", juego->jugador.nombre);
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

/*
    logica de movimiento del jugador:

    - no puede retroceder si no hay lugar, lo que fuerza dir hacia adelante
    - puede rebotar al final del tablero
    - si esta afectado por la tormenta pierde un turno

    logica de movimiento de bandido:

    - atraviesa el inicio y final como circulo hacia atras y adelante
    - solo se mueve si esta vivo
*/

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

void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion, unsigned posJugador)
{
    // si no hay lugar para retroceder, fuerza direccion adelante
    if(direccion == DIR_ATRAS && pasos >= posJugador)
        direccion = DIR_ADELANTE;

    tMovimiento mov = {pasos, direccion, ID_JUGADOR};
    ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
}

void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones)
{
    int i;
    unsigned pasos;
    char dir;

    for(i = 0; i < cantBandidos; i++)
    {
        if((bandidos+i)->vivo == VIVO)
        {
            pasos = generarRandomUniforme(MAX_DADO);
            dir = calcularDireccionBandido((bandidos+i)->posEnRuta, posJugador, cantPosiciones);

            tMovimiento mov = {pasos, dir, (bandidos+i)->id};
            ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
        }
    }
}

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones)
{
    tMovimiento mov;
    int i;

    while(!colaVaciaDin(cola))
    {
        sacarDeColaDin(cola, &mov, sizeof(tMovimiento));

        if(mov.id == ID_JUGADOR)
        {
            if(jugador->afectadoPorTormenta)
                jugador->afectadoPorTormenta = 0; // saco el efecto de tormenta
            else
                moverJugador(jugador, mov.cantPasos, mov.direccion, cantPosiciones);
        }
        else
        {
            for(i = 0; i < cantBandidos; i++)
                if((bandidos+i)->id == mov.id && (bandidos+i)->vivo == VIVO)
                    moverBandido(bandidos + i, mov.cantPasos, mov.direccion, cantPosiciones);
        }
    }
}

void moverJugador(tEstadoJugador *jugador, unsigned pasos, char direccion, unsigned cantPosiciones)
{
    if(direccion == DIR_ADELANTE)
    {
        unsigned nuevaPos = jugador->posEnRuta + pasos;
        if(nuevaPos > cantPosiciones)
            nuevaPos = 2*cantPosiciones - nuevaPos; // rebote al final
        jugador->posEnRuta = nuevaPos;
    }
    else
    {
        jugador->posEnRuta -= pasos;
    }
}

void moverBandido(tBandido *bandido, unsigned pasos, char direccion, unsigned cantPosiciones)
{
    if(direccion == DIR_ADELANTE)
    {
        bandido->posEnRuta += pasos;
        if(bandido->posEnRuta > cantPosiciones)
            bandido->posEnRuta -= cantPosiciones; // atraviesa la salida
    }
    else
    {
        if(pasos >= bandido->posEnRuta)
            bandido->posEnRuta = cantPosiciones - (pasos - bandido->posEnRuta); // atraviesa entrada
        else
            bandido->posEnRuta -= pasos;
    }
}

/*
    faltaria hacer:

    - ver si bandido==jugador -> restar vida y matar bandido y mover jugador al inicio
    - aplicar efectos, vida, puntos, tormenta, etc
    - ver si gano con jugador en posicion de salida o perdio con vida==0

*/

int cmpRanking (const void* a, const void* b)
{
    const tRanking* r1 = (const tRanking*) a;
    const tRanking* r2 = (const tRanking*) b;

    return strcmp(r1->nombre, r2->nombre);
}
