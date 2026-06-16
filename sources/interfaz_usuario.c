#include "../include/interfaz_usuario.h"
#include <conio.h>
#include <windows.h>

void mostrarBienvenida(const char *nickname)
{
    system("CLS");
    printf("\n\n   === %s ===\n\n", TITULO_JUEGO);
    printf("   Bienvenido de vuelta, %s!\n\n", nickname);
    printf("   Presione [ESPACIO] para continuar...\n");
    while(getch() != TECLA_ESPACIO);
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
            tecla += DIFERENCIA_MAYUS_Y_MINUS;

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
            case TECLA_ESPACIO:
                confirmado = VERDADERO;
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
    printf("   Ingrese su username: ");
    fflush(stdin);
    fgets(nombre, tamMaxNombre, stdin);
    len = strlen(nombre);
    if(len > 0 && nombre[len - 1] == '\n')
        nombre[len - 1] = '\0';
}

int nombreExisteEnIndice(const char *nombre, const tArbolBinBusq *arbolIndice, unsigned *indiceRegOut)
{
    tIndice ind;
    int resultado;
    strcpy(ind.clave.nombre, nombre);
    resultado = buscarEnArbolBinBusq(arbolIndice, &ind, sizeof(tIndice), cmpClaveIndice);
    if(resultado == CLAVE_ENCONTRADA && indiceRegOut != NULL)
        *indiceRegOut = ind.indiceRegistro;
    return resultado;
}

int confirmarUsername(const char *nombre)
{
    char titulo[TAM_TITULO];
    const char *opciones[] = {"SI", "NO"};
    sprintf(titulo, "Username \"%s\" ya registrado. Sos vos?", nombre);
    return seleccionarOpcionMenu(titulo, opciones, 2) == 0;
}

int solicitarNombreUsuario(char *nombre, int tamMaxNombre, const tArbolBinBusq *arbolIndice, unsigned *indiceRegOut)
{
    int existe = CLAVE_NO_ENCONTRADA;
    int listo = 0;

    while(!listo)
    {
        leerNombrePorTeclado(nombre, tamMaxNombre);

        existe = nombreExisteEnIndice(nombre, arbolIndice, indiceRegOut);

        if(existe == CLAVE_NO_ENCONTRADA)
            listo = 1;
        else if(confirmarUsername(nombre))
            listo = 1;
    }

    return existe;
}

void leerNicknamePorTeclado(char *nickname, int tamMaxNickname)
{
    int len;
    system("CLS");
    printf("\n\n   === CARAVANA DEL DESIERTO ===\n\n");
    printf("   Ingrese su nickname: ");
    fflush(stdin);
    fgets(nickname, tamMaxNickname, stdin);
    len = strlen(nickname);
    if(len > 0 && nickname[len - 1] == '\n')
        nickname[len - 1] = '\0';
}

void mostrarEstadoPartida(const tPartida *partida)
{
    printf("   Vidas: %u   |   Puntos: %u", partida->jugador.estadoEnPartida.vidas, partida->jugador.estadoEnPartida.puntos);

    if(partida->ultimosPasos == 0)
        printf("   |   --\n\n");
    else
        printf("   |   Ultimo movimiento: %c%u\n\n", partida->ultimaDireccion, partida->ultimosPasos);
}

void mostrarMensajeEvento(tEventoTurno evento)
{
    switch(evento)
    {
        case EVENTO_TURNO_INICIO:
            printf("   >> Comienza la partida!\n");
            break;
        case EVENTO_TURNO_NADA:
            printf("   >> Estas en un desierto, no sucede nada.\n");
            break;
        case EVENTO_TURNO_OASIS:
            printf("   >> Caiste en un oasis, Estas protegido por este turno.\n");
            break;
        case EVENTO_TURNO_PREMIO:
            printf("   >> Caiste en un premio, +%d puntos.\n", PUNTOS_PREMIO);
            break;
        case EVENTO_TURNO_VIDA_EXTRA:
            printf("   >> Caiste en una vida extra, Obtenes +1 de vida.\n");
            break;
        case EVENTO_TURNO_TORMENTA:
            printf("   >> Caiste en una tormenta, Perdes el proximo turno.\n");
            break;
        case EVENTO_TURNO_BANDIDO:
            printf("   >> Te ataco un bandido, Perdiste 1 vida y volviste al inicio.\n");
            break;
        case EVENTO_TURNO_MUERTE:
            printf("   >> Los bandidos te mataron. Fin de la partida.\n");
            break;
        case EVENTO_TURNO_VICTORIA:
            printf("   >> Llegaste a la salida, Victoria!\n");
            break;
        case EVENTO_TURNO_TORMENTA_BLOQUEADA:
            printf("   >> Caiste en una tormenta pero el oasis te protegio!\n");
            break;
        case EVENTO_TURNO_BANDIDO_BLOQUEADO:
            printf("   >> Un bandido te ataco pero el oasis te protegio!\n");
            break;
        case EVENTO_TURNO_TURNO_PERDIDO:
            printf("   >> Caiste en una tormenta, Perdes el turno.\n");
            break;
    }
}

int pedirDireccionJugador(tLista *ruta, const tPartida *partida, int cantPasos)
{
    const char *opciones[] = {"ADELANTE", "ATRAS"};
    int seleccion = 0, confirmado = 0, i;
    char tecla;
    int puedeRetroceder = (cantPasos < (int)partida->jugador.estadoEnPartida.posEnRuta);
    int cantOpciones = puedeRetroceder ? 2 : 1;

    while(!confirmado)
    {
        system("CLS");
        mostrarEstadoPartida(partida);
        renderizar_tablero(ruta, stdout);
        printf("\n   Tiraste un %d! Elija direccion:\n\n", cantPasos);
        for(i = 0; i < cantOpciones; i++)
        {
            if(i == seleccion)
                printf("        [ %s ]\n", opciones[i]);
            else
                printf("          %s  \n", opciones[i]);
        }
        if(!puedeRetroceder)
            printf("\n   (No hay espacio para retroceder)\n");
        printf("\n   [A/S] Mover    [ESPACIO] Confirmar\n");

        tecla = getch();
        if(tecla >= 'A' && tecla <= 'Z')
            tecla += DIFERENCIA_MAYUS_Y_MINUS;

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
            case TECLA_ESPACIO:
                confirmado = VERDADERO;
                break;
        }
    }

    return seleccion;
}

void mostrarTableroEsperandoTurno(tLista *ruta, const tPartida *partida)
{
    int listo = FALSO;
    char tecla;

    while(!listo)
    {
        system("CLS");
        mostrarEstadoPartida(partida);
        renderizar_tablero(ruta, stdout);

        if(partida->eventoPrevio != EVENTO_TURNO_NADA)
            mostrarMensajeEvento(partida->eventoPrevio); 

        mostrarMensajeEvento(partida->ultimoEvento);
        printf("   Toca [ESPACIO] para tirar el dado...\n");

        tecla = getch();
        if(tecla == TECLA_ESPACIO)
            listo = VERDADERO;
    }
}

void animarMovimientoJugador(tPartida *partida, unsigned pasos, char direccion)
{
    unsigned i;
    char dirActual = direccion;

    for(i = 0; i < pasos; i++)
    {
        moverJugadorEnRuta(&partida->jugador.estadoEnPartida, &partida->ruta);

        if(dirActual == DIR_ADELANTE && partida->jugador.estadoEnPartida.posEnRuta == partida->cantCasilleros)
            dirActual = DIR_ATRAS;

        if(dirActual == DIR_ADELANTE)
            partida->jugador.estadoEnPartida.posEnRuta++;
        else
            partida->jugador.estadoEnPartida.posEnRuta--;

        moverJugadorEnRuta(&partida->jugador.estadoEnPartida, &partida->ruta);

        system("CLS");
        mostrarEstadoPartida(partida);
        renderizar_tablero(&partida->ruta, stdout);
        Sleep(TIEMPO_ANIMACION_JUGADOR_MS);
    }
}

void animarMovimientoBandido(tPartida *partida,tBandido *bandido, unsigned pasos, char direccion)
{
    unsigned i;
    tCasillero clave, *casillero;

    for(i = 0; i < pasos; i++)
    {
        clave.numeroCasillero = bandido->posEnRuta;
        casillero = (tCasillero*) buscarElemPorClaveLista(&partida->ruta, &clave, cmpCasillero);
        casillero->cantBandidos--;

        if(direccion == DIR_ADELANTE)
        {
            bandido->posEnRuta++;
            if(bandido->posEnRuta > partida->cantCasilleros)
                bandido->posEnRuta = 1;
        }
        else
        {
            if(bandido->posEnRuta == 1)
                bandido->posEnRuta = partida->cantCasilleros;
            else
                bandido->posEnRuta--;
        }

        clave.numeroCasillero = bandido->posEnRuta;
        casillero = (tCasillero*) buscarElemPorClaveLista(&partida->ruta, &clave, cmpCasillero);
        casillero->cantBandidos++;

        system("CLS");
        mostrarEstadoPartida(partida);
        renderizar_tablero(&partida->ruta, stdout);
        Sleep(TIEMPO_ANIMACION_BANDIDO_MS);
    }
}
