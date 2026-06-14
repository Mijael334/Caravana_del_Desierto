#include "../include/interfaz_usuario.h"
#include <conio.h>

char opcion(const char matriz_menu[][MAX_TEXTO_MENU], const char *titulo, const char *mensaje)
{
    char opc;
    int i;
    system("CLS");
    printf("\n\n %s \n", titulo);
    for(i = 1; i <= strlen(matriz_menu[0]); i++)
    {
        printf("\n %c - %s", matriz_menu[0][i - 1], matriz_menu[i]);
    }
    printf("\n\n%s: ", mensaje);
    fflush(stdin);
    scanf("%c", &opc);
    return toupper(opc);
}
 
void dibujarOpcionesMenu(const char *titulo, const char *opciones[], int cantOpciones, int seleccion)
{
    int i;
    system("CLS");
    printf("\n\n   === %s ===\n\n", titulo);
    for(i = 0; i < cantOpciones; i++)
    {
        if(i == seleccion)
            printf("        [ %s ]\n", opciones[i]);
        else
            printf("          %s  \n", opciones[i]);
    }
    printf("\n   [W/S] Mover    [ESPACIO] Confirmar\n");
}
 
int seleccionarOpcionMenu(const char *titulo, const char *opciones[], int cantOpciones)
{
    int seleccion = 0, confirmado = 0;
    char tecla;
 
    while(!confirmado)
    {
        dibujarOpcionesMenu(titulo, opciones, cantOpciones, seleccion);
        tecla = getch();
        if(tecla >= 'A' && tecla <= 'Z')
            tecla += 32;
 
        switch(tecla)
        {
            case 'w':
            case 'a':
                if(seleccion > 0)
                    seleccion--;
                break;
            case 's':
            case 'd':
                if(seleccion < cantOpciones - 1)
                    seleccion++;
                break;
            case 32:
                confirmado = 1;
                break;
        }
    }
 
    return seleccion;
}
 
void leerNombrePorTeclado(char *nombre, int tamMaxNombre)
{
    int len;
    system("CLS");
    printf("\n\n   === CARAVANA DEL DESIERTO ===\n\n");
    printf("   Ingrese su nombre: ");
    fflush(stdin);
    fgets(nombre, tamMaxNombre, stdin);
    len = strlen(nombre);
    if(len > 0 && nombre[len - 1] == '\n')
        nombre[len - 1] = '\0';
}
 
int nombreExisteEnIndice(const char *nombre, const tArbolBinBusq *arbolIndice)
{
    tIndice ind;
    strcpy(ind.clave.nombre, nombre);
    return busquedaIndexada(arbolIndice, &ind, sizeof(tIndice), cmpClaveIndice);
}
 
int confirmarEsUsted(const char *nombre)
{
    char titulo[80];
    const char *opciones[] = {"SI", "NO"};
    sprintf(titulo, "Nombre \"%s\" ya registrado. Es usted?", nombre);
    return seleccionarOpcionMenu(titulo, opciones, 2) == 0;
}
 
int solicitarNombreUsuario(char *nombre, int tamMaxNombre, const tArbolBinBusq *arbolIndice)
{
    int existe = CLAVE_NO_ENCONTRADA;
    int listo = 0;
 
    while(!listo)
    {
        leerNombrePorTeclado(nombre, tamMaxNombre);

        existe = nombreExisteEnIndice(nombre, arbolIndice);
 
        if(existe == CLAVE_NO_ENCONTRADA)
            listo = 1;
        else if(confirmarEsUsted(nombre))
            listo = 1;
    }
 
    return existe;
}

int pedirDireccionJugador(tLista *ruta, int cantPasos)
{
    const char *opciones[] = {"ADELANTE", "ATRAS"};
    int seleccion = 0, confirmado = 0, i;
    char tecla;

    while(!confirmado)
    {
        system("CLS");
        renderizar_tablero(ruta, stdout);
        printf("\n   Tiraste un %d! Elija direccion:\n\n", cantPasos);
        for(i = 0; i < 2; i++)
        {
            if(i == seleccion)
                printf("        [ %s ]\n", opciones[i]);
            else
                printf("          %s  \n", opciones[i]);
        }
        printf("\n   [W/A/S/D] Mover    [ESPACIO] Confirmar\n");

        tecla = getch();
        if(tecla >= 'A' && tecla <= 'Z')
            tecla += 32;

        switch(tecla)
        {
            case 'w':
            case 'a':
                if(seleccion > 0)
                    seleccion--;
                break;
            case 's':
            case 'd':
                if(seleccion < 1)
                    seleccion++;
                break;
            case 32:
                confirmado = 1;
                break;
        }
    }

    return seleccion;
}

void mostrarTableroEsperandoTurno(tLista *ruta)
{
    int listo = 0;
    char tecla;

    while(!listo)
    {
        system("CLS");
        renderizar_tablero(ruta, stdout);
        printf("\n   Se termino el turno.\n");
        printf("   Toca [ESPACIO] para tirar el dado...\n");

        tecla = getch();
        if(tecla == 32)
            listo = 1;
    }
}