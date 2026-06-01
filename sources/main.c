#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/entidades.h"
#include "../include/interfaz_usuario.h"
#include "../include/tablero.h"

int main()
{
    srand(time(NULL));
      
    const char menu_principal[][MAX_TEXTO_MENU] = {"123",
                                                   "Comenzar Nueva Partida",
                                                   "Ver Ranking",
                                                   "Salir del juego"};
    const char *titulo = "=== CARAVANA DEL DESIERTO ===";
    char seleccion;
    tConfig configuracion;
    tLista tablero;
    tBandido *lista_bandidos = NULL;

    // Llamar a la carga del ABB desde el archivo binario (Integrante 3).

    // Mostrar menú y pedir nombre (Integrante 4).

    do{
        seleccion = menu(menu_principal, titulo);
        switch(seleccion){
    case '1':
        system("CLS");
        printf("\n=== NUEVA PARTIDA ===\n");
        lectura_de_configuracion(NOM_ARCH_CONFIG, &configuracion);
        lista_bandidos = (tBandido *)malloc(configuracion.bandidos_max * sizeof(tBandido));
        if(lista_bandidos == NULL)
        {
            printf("No hay memoria suficiente para los bandidos\n");
        }
        if(crear_tablero_circular(&tablero, &configuracion, lista_bandidos))
        {
            printf("Tablero generado exitosamente\n");
            printf("=== VISUALIZACION EN PANTALLA ===\n");
            renderizar_tablero(&tablero, configuracion.cant_posiciones, stdout);
            printf("=================================\n");
            guardar_tablero_en_archivo(&tablero, configuracion.cant_posiciones);
        }
        free(lista_bandidos);
        lista_bandidos = NULL;
        printf("\n");
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
