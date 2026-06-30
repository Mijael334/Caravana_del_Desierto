#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/logica_juego.h"
#include "../include/testing.h"

int main()
{
    tJuego juego;
    int ret;
//testingCrearArchivos(&juego);
    ret = inicializarJuego(&juego);
    
    while (juego.corriendo)
        ret = procesarJuego(&juego);
    
    mostrarError(ret);
    
    limpiarJuego(&juego);

    return ret;
}
