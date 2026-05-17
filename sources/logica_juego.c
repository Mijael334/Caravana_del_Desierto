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
