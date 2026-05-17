#include "../include/logica_juego.h"
#include "../include/gestion_archivos.h"
#include <stdio.h>
#include <stdlib.h>


int inicializarJuego (tJuego* juego)
{
    FILE* archConfig;
    //crearLista(&juego->rankingJugadores);

    abrir_archivo(&archConfig, NOM_ARCH_CONFIG, "rt");

    //crearPartida()

    //cargarRankingDeJugadores


    juego->corriendo = 1;
}


/// @brief genera un numero random y le saca el modulo de 6 (MAX_DADO)
/// @return devuelve un valor entre 1 y 6
int generarValorDado ()
{
    int limite = RAND_MAX - (RAND_MAX % MAX_DADO); //para que todos los numeros tengan las mismas prob de salir
    int random;

    do
    {
        random = rand();
    } while (random >= limite);

    return (random % MAX_DADO) + 1;
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
                    // mueve bandido sabiendo sus pasos y direccion
        }
    }
}


