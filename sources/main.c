#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/entidades.h"
#include "../include/interfaz_usuario.h"
#include "../include/tablero.h"
#include "../include/logica_juego.h"

int main()
{
    tJuego juego;
    int ret;

    ret = inicializarJuego(&juego);
    
    while (juego.corriendo)
    {
        ret = procesarJuego(&juego);
    }
    
    mostrarError(ret);
    
    return ret;
}
