#include "../include/logica_juego.h"
#include "../include/indice_jugador.h"
#include "../include/interfaz_usuario.h"
#include "../include/gestion_archivos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"

int inicializarJuego (tJuego* juego)
{
    int ret;

    srand((unsigned)time(NULL));

    juego->corriendo = FALSO;

    juego->partida.bandidos = NULL;
    crearColaDin(&juego->partida.movimientos);
    crearArbolBinBusq(&juego->arbolIndUsuarios);
    crearLista(&juego->listaRankingJugadores);

    ret = indexarArchivoUsuariosOrdenado(&juego->arbolIndUsuarios, NOM_ARCH_INDICE_USUARIOS);

    if(ret == ERROR_ARCHIVO_INDICE)
        return ret;

    ret = lectura_de_configuracion(NOM_ARCH_CONFIG, &juego->configPartida);

    if(ret != TODO_OK)
        return ret;

    ret = crearBandidos(&juego->partida.bandidos, juego->configPartida.bandidos_max, juego->configPartida.cant_posiciones);

    if(ret != TODO_OK)
        return ret;

    juego->corriendo = VERDADERO;
    juego->estadoJuego = ESTADO_MENU;

    return TODO_OK;
}

int procesarJuego (tJuego* juego)
{
    int ret;

    switch (juego->estadoJuego)
    {
    case ESTADO_MENU:
        ret = procesarMenu(juego);
        break;
    case ESTADO_PARTIDA:
        ret = procesarPartida(juego);
        break;
    case ESTADO_PUNTAJE_PARTIDA:
        ret = procesarPuntajePartida(&juego->partida, &juego->estadoJuego);
        break;
    case ESTADO_RANKING:
        //mostrar ranking
    default:
        break;
    }

    if(juego->estadoJuego == ESTADO_SALIR)
        juego->corriendo = FALSO;

    return ret;
}

int procesarMenu(tJuego* juego)
{
    int seleccion, ret = TODO_OK;
    int encontradoEnIndice;
    const char *opciones[] = {"Comenzar Nueva Partida",
                              "Ver Ranking",
                              "Salir del juego"};

    encontradoEnIndice = solicitarNombreUsuario(juego->usuario.nombre, MAX_NOMBRE + 1, &juego->arbolIndUsuarios);

    if(encontradoEnIndice == CLAVE_NO_ENCONTRADA)
    {
    }

    do
    {
        seleccion = seleccionarOpcionMenu(TITULO_JUEGO, opciones, 3);

        switch(seleccion)
        {
            case 0:
                ret = crear_tablero_circular(&juego->partida.ruta, &juego->configPartida, juego->partida.bandidos);

                if(ret == TODO_OK)
                    guardar_tablero_en_archivo(&juego->partida.ruta, juego->configPartida.cant_posiciones);
                else
                    juego->corriendo = FALSO;

                break;
            case 1:
                system("CLS");
                printf("\n--- RANKING ---\n");
                system("PAUSE");
                break;
            case 2:
                printf("\nSaliendo del programa\n");
                juego->estadoJuego = ESTADO_SALIR;
                break;
        }
    } while(seleccion != 2);

    return ret;
}

int procesarPartida(tJuego* juego)
{
    int cantPasos = generarRandomUniforme(MAX_DADO);
    char dirMovimiento; //= calcularDireccion (); se falta la funcion para pedir la dir, hay que ver si lo tiene que escribir o lo hacemos directo con presionar la tecla

    encolarMovimientoJugador(&juego->partida.movimientos, cantPasos, dirMovimiento, juego->partida.jugador.estadoEnPartida.posEnRuta);

    encolarMovimientosBandidos (&juego->partida.movimientos, juego->partida.bandidos, juego->configPartida.bandidos_max, juego->partida.jugador.estadoEnPartida.posEnRuta,
                                 juego->partida.cantCasilleros);

    desencolarMovimientos(&juego->partida.movimientos, juego->partida.bandidos, juego->configPartida.bandidos_max, &juego->partida.jugador.estadoEnPartida,
                          &juego->partida.ruta, juego->partida.cantCasilleros);

    actualizarEstadoPartida(&juego->partida.jugador, juego->partida.bandidos, juego->configPartida.bandidos_max, &juego->partida.ruta,
                          juego->partida.cantCasilleros, &juego->estadoJuego);

    if(juego->estadoJuego == ESTADO_PUNTAJE_PARTIDA)
        finalizarPartida(juego);


    return TODO_OK;
}

int procesarPuntajePartida(const tPartida* partida, tEstadoJuego* estadoJuego)
{
    printf("\n=============PARTIDA FINALIZADA=============\n");
    printf("Resultado: %s", partida->jugador.estadoEnPartida.vidas != 0 ? MSJ_RESULTADO_VICTORIA : MSJ_RESULTADO_DERROTA );
    printf("Jugador: %s", partida->jugador.nombre);
    printf("puntos: %d\t\tVidas Restantes: %d", partida->jugador.estadoEnPartida.puntos, partida->jugador.estadoEnPartida.vidas);
    printf("Forward: %d casillas\t\tBackward: %d casillas", partida->cantMovsAdelante, partida->cantMovsAtras);

    //aca iria un fwrite con todos estos datos en el registro de partida

    //aca debe pedir enter para seguir

    *estadoJuego = ESTADO_MENU;

    return TODO_OK;
}

int actualizarEstadoPartida (tJugador* jugador, tBandido* bandidos, unsigned cantBandidos,tLista* ruta, unsigned cantCasilleros, tEstadoJuego* estadoJuego)
{
    tCasillero casilleroNum, *casillero;

    casilleroNum.numeroCasillero = jugador->estadoEnPartida.posEnRuta;

    casillero = buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);

    if(jugador->estadoEnPartida.protegido == VERDADERO)
        jugador->estadoEnPartida.protegido = FALSO;

    if(jugador->estadoEnPartida.afectadoPorTormenta == VERDADERO)
        jugador->estadoEnPartida.afectadoPorTormenta = FALSO;

    switch (casillero->evento)
    {
        case EVENTO_OASIS: jugador->estadoEnPartida.protegido = VERDADERO;
            break;
        case EVENTO_PREMIO: jugador->estadoEnPartida.puntos += PUNTOS_PREMIO;
            break;
        case EVENTO_VIDA_EXTRA: jugador->estadoEnPartida.vidas++;
            break;
        case EVENTO_TORMENTA: jugador->estadoEnPartida.afectadoPorTormenta = VERDADERO;
            break;
        case EVENTO_SALIDA: *estadoJuego = ESTADO_PUNTAJE_PARTIDA;
            break;
        default:
            break;
    }

    if(*estadoJuego == ESTADO_PUNTAJE_PARTIDA)
        return TODO_OK;


    if(casillero->evento == EVENTO_VACIO && casillero->cantBandidos == 0)
        return TODO_OK;

    if(casillero->cantBandidos > 0)
    {
        tBandido* bandido = NULL;
        unsigned posJugador = jugador->estadoEnPartida.posEnRuta, i = 0;

        if(jugador->estadoEnPartida.protegido == FALSO)
        {
            //mueve al jugador al inicio
            moverJugadorEnRuta(&jugador->estadoEnPartida, ruta);
            jugador->estadoEnPartida.posEnRuta = 1;
            moverJugadorEnRuta(&jugador->estadoEnPartida, ruta);

            jugador->estadoEnPartida.vidas--;

            if(jugador->estadoEnPartida.vidas == 0)
                *estadoJuego = ESTADO_PUNTAJE_PARTIDA;
        }

        //busca al bandido en base a la posicion
        while (bandido == NULL && i < cantBandidos)
        {
            if((bandidos + i)->posEnRuta == posJugador && (bandidos + i)->vivo == VIVO)
                bandido = bandidos + i;
            else
                i++;
        }

        eliminarBandido(bandido, ruta, cantCasilleros);
    }

    return TODO_OK;
}

void finalizarPartida (tJuego* juego)
{
    free(juego->partida.bandidos);
    vaciarColaDin(&juego->partida.movimientos);
    liberarLista(&juego->partida.ruta);

    juego->usuario.puntos += juego->partida.jugador.puntos;
}

void eliminarBandido(tBandido* bandido, tLista* ruta, unsigned cantCasilleros)
{
    tCasillero *casillero, casilleroNum;

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos--;

    bandido->vivo = MUERTO;
}


void ingresarNombreJugador (char* nombre)
{
    printf("Ingrese su nombre: ");
    fgets(nombre, MAX_NOMBRE + 1, stdin);
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

void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *ruta, unsigned cantPosiciones)
{
    tMovimiento mov;
    int i;

    while(!colaVaciaDin(cola))
    {
        sacarDeColaDin(cola, &mov, sizeof(tMovimiento));

        if(mov.id == ID_JUGADOR)
        {
            if(jugador->afectadoPorTormenta == FALSO)
            {
                moverJugadorEnRuta(jugador, ruta);
                moverJugador(jugador, mov.cantPasos, mov.direccion, cantPosiciones);
                moverJugadorEnRuta(jugador, ruta);
            }
        }
        else
        {
            for(i = 0; i < cantBandidos; i++)
            {
                if((bandidos+i)->id == mov.id && (bandidos+i)->vivo == VIVO)
                    moverBandidoEnRuta(bandidos+i, &mov, ruta, cantPosiciones);
            }
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

void moverJugadorEnRuta (tEstadoJugador* jugador, tLista* ruta)
{
    tCasillero* casillero, casilleroNum;

    casilleroNum.numeroCasillero = jugador->posEnRuta;
    casillero = (tCasillero*) buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);

    if(casillero)
        casillero->jugadorAca = !casillero->jugadorAca;
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

void moverBandidoEnRuta (tBandido* bandido, const tMovimiento* mov, tLista* ruta, unsigned cantPosiciones)
{
    tCasillero* casillero, casilleroNum;

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = (tCasillero*) buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos--;

    moverBandido(bandido, mov->cantPasos, mov->direccion, cantPosiciones);

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = (tCasillero*) buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos++;
}

void limpiarJuego (tJuego* juego)
{
    vaciarColaDin(&juego->partida.movimientos);
    liberarLista(&juego->listaRankingJugadores);
    eliminarArbol(&juego->arbolIndUsuarios);
    free(juego->partida.bandidos);
}

void mostrarError (int err)
{
    switch (err)
    {
    case TODO_OK:
        break;
    case ERROR_MEM: puts("HUBO UN ERROR AL RESERVAR MEMORIA.");
        break;
    case ERROR_ARCHIVO_CONFIG: puts("HUBO UN ERROR AL ABRIR EL ARCHVOS DE CONFIGURACION.");
        break;
    case ERROR_ARCHIVO_INDICE: puts("HUBO UN ERROR AL ABRIR EL ARCHIVO DE INDICE DE USUARIOS.");
        break;
    case ERROR_ARCHIVO_USUARIOS: puts("HUBO UN ERROR AL ABRIR EL ARCHIVO DE USUARIOS.");
        break;
    default: puts("ERROR DESCONOCIDO.");
        break;
    }
}