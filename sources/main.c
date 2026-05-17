#include <stdio.h>
#include <stdlib.h>

#include "../include/interfaz_usuario.h"

int main()
{
    printf("Hello world!\n");
    const char menu_principal[][MAX_TEXTO_MENU] = {"123",
                                                   "Comenzar Nueva Partida",
                                                   "Ver Ranking",
                                                   "Salir del juego"};
    const char *titulo = "=== CARAVANA DEL DESIERTO ===";
    char seleccion;

    // Llamar a la carga del ABB desde el archivo binario (Integrante 3).

    // Mostrar menú y pedir nombre (Integrante 4).

    do{
        seleccion = menu(menu_principal, titulo);
        switch(seleccion){
    case '1':
        system("CLS");
        printf("\n--- NUEVA PARTIDA ---\n");
        system("PAUSE");
        break;
    case '2':
        system("CLS");
        printf("\n--- RANKING ---\n");
        system("PAUSE");
        break;
    case '3':
        printf("\nSaliendo del programa\n");
        break;
        }
    }while(seleccion != '3');

    // Cargar configuración y generar lista circular (Integrante 1 y 3).

    // Bucle de juego:

    // Tirar dado y elegir dirección (Integrante 2).

    // Encolar movimientos (Integrante 1).

    // Desencolar y actualizar posiciones (Integrante 1 y 2).

    // Refrescar pantalla (Integrante 4).

    // Guardar resultados y mostrar historial (Integrante 3 y 4).

    return 0;
}
