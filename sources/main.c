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
    if(ret != TODO_OK)
    {
        printf("Error al inicializar el juego (codigo %d)\n", ret);
        system("PAUSE");
        return ret;
    }

    procesarMenu(&juego);

    return 0;
}
