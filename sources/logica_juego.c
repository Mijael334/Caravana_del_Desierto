#include "../include/logica_juego.h"
#include "../include/indice_jugador.h"
#include "../include/interfaz_usuario.h"
#include "../include/gestion_archivos.h"
#include "../include/reportes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "time.h"

int inicializarJuego(tJuego *juego)
{
    int ret;

    srand((unsigned)time(NULL));

    juego->corriendo = FALSO;

    juego->usuario.username[0] = '\0';
    juego->usuario.nickname[0] = '\0';
    juego->partida.bandidos = NULL;
    crearColaDin(&juego->partida.movimientos);
    crearColaDin(&juego->partida.registroMovimientos); 
    crearArbolBinBusq(&juego->arbolIndUsuarios);
    crearListaSimple(&juego->listaRankingJugadores);
    crearLista(&juego->partida.ruta);

    ret = indexarArchivoUsuariosOrdenado(&juego->arbolIndUsuarios, NOM_ARCH_INDICE_USUARIOS);

    if(ret == ERROR_ARCHIVO_INDICE)
        return ret;

    ret = lectura_de_configuracion(NOM_ARCH_CONFIG, &juego->configPartida);

    if (ret != TODO_OK)
        return ret;

    ret = crearBandidos(&juego->partida.bandidos, juego->configPartida.bandidos_max, juego->configPartida.cant_posiciones);

    if (ret != TODO_OK)
        return ret;

    juego->corriendo = VERDADERO;
    juego->estadoJuego = ESTADO_MENU;

    return TODO_OK;
}

int procesarJuego(tJuego *juego)
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
        ret = procesarPuntajePartida(&juego->usuario, &juego->partida, &juego->estadoJuego);
        break;
    case ESTADO_RANKING:
        // mostrar ranking
    default:
        break;
    }

    if(juego->estadoJuego == ESTADO_SALIR)
        juego->corriendo = FALSO;

    return ret;
}

int procesarMenu(tJuego *juego)
{
    int seleccion, ret = TODO_OK;
    int encontradoEnIndice;
    unsigned indiceReg = 0;
    unsigned posNueva;
    const char *opciones[] = {"Comenzar Nueva Partida", "Ver Ranking", "Salir del juego"};
    const char *opcSalir[] = {"NO, seguir jugando", "SI, salir"};

    if(juego->usuario.username[0] == '\0')
    {
        encontradoEnIndice = solicitarNombreUsuario(juego->usuario.username, TAM_MAX_NOM + 1, &juego->arbolIndUsuarios, &indiceReg);


        if(encontradoEnIndice == CLAVE_ENCONTRADA)
        {
            leerUsuarioDeArchivo(&juego->usuario, indiceReg, NOM_ARCH_USUARIOS);
        }
        else
        {
            leerNicknamePorTeclado(juego->usuario.nickname, TAM_MAX_NOM + 1);
            agregarUsuarioEnArchivo(&juego->usuario, NOM_ARCH_USUARIOS, &posNueva);
            registrarNuevoUsuarioEnIndice(&juego->arbolIndUsuarios, juego->usuario.username, posNueva, NOM_ARCH_INDICE_USUARIOS);
        }
    }

    seleccion = seleccionarOpcionMenu(TITULO_JUEGO, opciones, 3);

    switch (seleccion)
    {
        case 0:
            ret = crear_tablero_circular(&juego->partida.ruta, &juego->configPartida, juego->partida.bandidos);

            if(ret == TODO_OK)
            {
                inicializarJugador(&juego->partida.jugador, juego->usuario.nickname, juego->partida.jugador.puntos, juego->configPartida.vidas_incio);
                juego->partida.cantCasilleros = juego->configPartida.cant_posiciones;
                juego->partida.cantMovsAdelante = juego->partida.cantMovsAtras = 0;
                juego->partida.puntosEnPartida = 0;
                juego->partida.ultimoEvento = EVENTO_TURNO_INICIO;
                juego->partida.ultimosPasos = 0;
                juego->partida.ultimaDireccion = '-';
                vaciarColaDin(&juego->partida.registroMovimientos);     

                guardar_tablero_en_archivo(&juego->partida.ruta);
                juego->estadoJuego = ESTADO_PARTIDA;
            }
            else
                juego->corriendo = FALSO;

            break;
        case 1:
            system("CLS");
            printf("\n--- RANKING ---\n");
            system("PAUSE");
            break;
        case 2:
            if(seleccionarOpcionMenu("Seguro que querés salir?", opcSalir, 2) == 1)
                juego->estadoJuego = ESTADO_SALIR;
            break;
    }


    return ret;
}

int procesarPartida(tJuego *juego)
{
    int cantPasos;
    char dirMovimiento;

    mostrarTableroEsperandoTurno(&juego->partida.ruta, &juego->partida);

    if(juego->partida.jugador.estadoEnPartida.afectadoPorTormenta == FALSO)
    {
        cantPasos = generarRandomUniforme(MAX_DADO);
        dirMovimiento = (pedirDireccionJugador(&juego->partida.ruta, &juego->partida, cantPasos) == 0) ? DIR_ADELANTE : DIR_ATRAS;

        juego->partida.ultimosPasos = cantPasos;
        juego->partida.ultimaDireccion = dirMovimiento;

        tMovimiento registro;
        registro.cantPasos = cantPasos;
        registro.direccion = dirMovimiento;
        registro.id = ID_JUGADOR;
        ponerEnColaDin(&juego->partida.registroMovimientos, &registro, sizeof(tMovimiento));

        encolarMovimientoJugador(&juego->partida.movimientos, cantPasos, dirMovimiento, juego->partida.jugador.estadoEnPartida.posEnRuta);
    }

    encolarMovimientosBandidos(&juego->partida.movimientos, juego->partida.bandidos, juego->configPartida.bandidos_max, juego->partida.jugador.estadoEnPartida.posEnRuta,
                               juego->partida.cantCasilleros);

    desencolarMovimientos(&juego->partida, juego->configPartida.bandidos_max);

    actualizarEstadoPartida(&juego->partida, juego->configPartida.bandidos_max, &juego->estadoJuego);

    renderizar_tablero(&juego->partida.ruta, stdout);

    if (juego->estadoJuego == ESTADO_PUNTAJE_PARTIDA)
    {
        system("CLS");
        mostrarEstadoPartida(&juego->partida);
        renderizar_tablero(&juego->partida.ruta, stdout);
        mostrarMensajeEvento(juego->partida.ultimoEvento);
        printf("\n   Presione [ESPACIO] para continuar...\n");
        while(getch() != 32);
        finalizarPartida(juego);
    }

    return TODO_OK;
}


int procesarPuntajePartida(const tUsuario* usuario, tPartida* partida, tEstadoJuego* estadoJuego)
{
    tReportePartida reporte;
    tMovimiento mov;
    int primero = 1;

    strcpy(reporte.resultado,  partida->jugador.estadoEnPartida.vidas != 0 ? MSJ_RESULTADO_VICTORIA : MSJ_RESULTADO_DERROTA);
    strcpy(reporte.usuario.username, usuario->username);
    strcpy(reporte.usuario.nickname, usuario->nickname);
    reporte.puntosObtenidos = partida->jugador.estadoEnPartida.puntos;
    reporte.vidasRestantes = partida->jugador.estadoEnPartida.vidas;
    reporte.forwardCasillas = partida->cantMovsAdelante;
    reporte.backwardCasillas = partida->cantMovsAtras;

    system("CLS");
    printf("\n=============PARTIDA FINALIZADA=============\n\n");
    printf("Resultado: %s\n\n", partida->jugador.estadoEnPartida.vidas != 0 ? MSJ_RESULTADO_VICTORIA : MSJ_RESULTADO_DERROTA );
    printf("Username: %s\t\t\tNickname:%s\n\n", reporte.usuario.username, reporte.usuario.nickname);
    printf("Puntos: %d\t\t\tVidas Restantes: %d\n\n", reporte.puntosObtenidos, reporte.vidasRestantes);
    printf("Forward: %d casillas\t\tBackward: %d casillas\n\n", reporte.forwardCasillas, reporte.backwardCasillas);

    printf("Movimientos realizados: \n");
    while(!colaVaciaDin(&partida->registroMovimientos))
    {
        sacarDeColaDin(&partida->registroMovimientos, &mov, sizeof(tMovimiento));
        if(!primero)
            printf(", ");
        printf("%c%u", mov.direccion, mov.cantPasos);
        primero = 0;
    }
    printf("\n");

    registrarPartidaEnArchivo(&reporte);

    system("pause");

    *estadoJuego = ESTADO_MENU;

    return TODO_OK;
}

int actualizarEstadoPartida(tPartida* partida, unsigned cantBandidos, tEstadoJuego* estadoJuego)
{
    tCasillero casilleroNum, *casillero;
    int aplicadoEfectoAhora = FALSO;

    casilleroNum.numeroCasillero = partida->jugador.estadoEnPartida.posEnRuta;

    casillero = buscarElemPorClaveLista(&partida->ruta, &casilleroNum, cmpCasillero);
    partida->ultimoEvento = EVENTO_TURNO_NADA;
    
    
    if (partida->jugador.estadoEnPartida.protegido == VERDADERO && !aplicadoEfectoAhora)
        partida->jugador.estadoEnPartida.protegido = FALSO;

    switch (casillero->evento)
    {
    case EVENTO_OASIS:
        partida->jugador.estadoEnPartida.protegido = VERDADERO;
        partida->ultimoEvento = EVENTO_TURNO_OASIS;  
        aplicadoEfectoAhora = VERDADERO;
        break;
    case EVENTO_PREMIO:
        partida->jugador.estadoEnPartida.puntos += PUNTOS_PREMIO;
        partida->ultimoEvento = EVENTO_TURNO_PREMIO; 
        break;
    case EVENTO_VIDA_EXTRA:
        partida->jugador.estadoEnPartida.vidas++;
        partida->ultimoEvento = EVENTO_TURNO_VIDA_EXTRA; 
        break;
    case EVENTO_TORMENTA:
        if(partida->jugador.estadoEnPartida.afectadoPorTormenta == FALSO)
        {
            partida->jugador.estadoEnPartida.afectadoPorTormenta = VERDADERO;
            partida->ultimoEvento = EVENTO_TURNO_TORMENTA;  
            aplicadoEfectoAhora = VERDADERO;
        }
        else
            partida->jugador.estadoEnPartida.afectadoPorTormenta = FALSO;
        break;
    case EVENTO_SALIDA:
        *estadoJuego = ESTADO_PUNTAJE_PARTIDA;
        partida->ultimoEvento = EVENTO_TURNO_VICTORIA;
        break;
    default:
        break;
    }

    if(*estadoJuego == ESTADO_PUNTAJE_PARTIDA)
        return TODO_OK;

    if (casillero->evento == EVENTO_VACIO && casillero->cantBandidos == 0)
        return TODO_OK;

    if (casillero->cantBandidos > 0)
    {
        tBandido *bandido = NULL;
        unsigned posJugador = partida->jugador.estadoEnPartida.posEnRuta, i = 0;

        if (partida->jugador.estadoEnPartida.protegido == FALSO)
        {
            // mueve al jugador al inicio
            moverJugadorEnRuta(&partida->jugador.estadoEnPartida, &partida->ruta);
            partida->jugador.estadoEnPartida.posEnRuta = 1;
            moverJugadorEnRuta(&partida->jugador.estadoEnPartida, &partida->ruta);

            partida->jugador.estadoEnPartida.vidas--;

            if (partida->jugador.estadoEnPartida.vidas == 0)
            {
                *estadoJuego = ESTADO_PUNTAJE_PARTIDA;
                partida->ultimoEvento = EVENTO_TURNO_MUERTE;
            }
            else
            {
                partida->ultimoEvento = EVENTO_TURNO_BANDIDO;  
            }
            
        }

        // busca al bandido en base a la posicion
        while (bandido == NULL && i < cantBandidos)
        {
            if ((partida->bandidos + i)->posEnRuta == posJugador && (partida->bandidos + i)->vivo == VIVO)
                bandido = partida->bandidos + i;
            else
                i++;
        }

        eliminarBandido(bandido, &partida->ruta, partida->cantCasilleros);
    }

    return TODO_OK;
}

void finalizarPartida(tJuego *juego)
{
    vaciarColaDin(&juego->partida.movimientos);

    recorrerLista(&juego->partida.ruta, vaciar_datos_casillero, NULL);

    juego->partida.jugador.puntos += juego->partida.jugador.estadoEnPartida.puntos;
}

void eliminarBandido(tBandido *bandido, tLista *ruta, unsigned cantCasilleros)
{
    tCasillero *casillero, casilleroNum;

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos--;

    bandido->vivo = MUERTO;
}

void ingresarNombreJugador(char *nombre)
{
    printf("Ingrese su nombre: ");
    fgets(nombre, TAM_MAX_NOM + 1, stdin);
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

    if (posJugador >= posBandido)
        distanciaAdelante = posJugador - posBandido;
    else
        distanciaAdelante = cantPosiciones - posBandido + posJugador;

    distanciaAtras = cantPosiciones - distanciaAdelante;

    if (distanciaAdelante <= distanciaAtras)
        return DIR_ADELANTE;
    else
        return DIR_ATRAS;
}

void encolarMovimientoJugador(tCola *cola, unsigned pasos, char direccion, unsigned posJugador)
{
    // si no hay lugar para retroceder, fuerza direccion adelante
    if (direccion == DIR_ATRAS && pasos >= posJugador)
        direccion = DIR_ADELANTE;

    tMovimiento mov = {pasos, direccion, ID_JUGADOR};
    ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
}

void encolarMovimientosBandidos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, unsigned posJugador, unsigned cantPosiciones)
{
    unsigned i;
    unsigned pasos;
    char dir;

    for (i = 0; i < cantBandidos; i++)
    {
        if ((bandidos + i)->vivo == VIVO)
        {
            pasos = generarRandomUniforme(MAX_DADO);
            dir = calcularDireccionBandido((bandidos + i)->posEnRuta, posJugador, cantPosiciones);

            tMovimiento mov = {pasos, dir, (bandidos + i)->id};
            ponerEnColaDin(cola, &mov, sizeof(tMovimiento));
        }
    }
}

//void desencolarMovimientos(tCola *cola, tBandido *bandidos, unsigned cantBandidos, tEstadoJugador *jugador, tLista *lista, unsigned cantPosiciones, const tPartida *partida);
void desencolarMovimientos(tPartida *partida, unsigned cantBandidos)
{
    tMovimiento mov;
    unsigned i;

    while (!colaVaciaDin(&partida->movimientos))
    {
        sacarDeColaDin(&partida->movimientos, &mov, sizeof(tMovimiento));

        if (mov.id == ID_JUGADOR)
        {
            if (partida->jugador.estadoEnPartida.afectadoPorTormenta == FALSO)
            {
                if(mov.direccion == DIR_ADELANTE)
                    partida->cantMovsAdelante += mov.cantPasos;
                else
                    partida->cantMovsAtras += mov.cantPasos;

                animarMovimientoJugador(partida, mov.cantPasos, mov.direccion);
            }
        }
        else
        {
            for (i = 0; i < cantBandidos; i++)
            {
                if ((partida->bandidos + i)->id == mov.id && (partida->bandidos + i)->vivo == VIVO)
                    animarMovimientoBandido(partida, partida->bandidos + i, mov.cantPasos, mov.direccion);
            }
        }
    }
}

void moverJugador(tEstadoJugador *jugador, unsigned pasos, char direccion, unsigned cantPosiciones)
{
    if (direccion == DIR_ADELANTE)
    {
        unsigned nuevaPos = jugador->posEnRuta + pasos;
        if (nuevaPos > cantPosiciones)
            nuevaPos = 2 * cantPosiciones - nuevaPos; // rebote al final

        jugador->posEnRuta = nuevaPos;
    }
    else
    {
        jugador->posEnRuta -= pasos;
    }
}

void moverJugadorEnRuta(tEstadoJugador *jugador, tLista *ruta)
{
    tCasillero *casillero, casilleroNum;

    casilleroNum.numeroCasillero = jugador->posEnRuta;
    casillero = (tCasillero *)buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);

    if (casillero)
        casillero->jugadorAca = !casillero->jugadorAca;
}

void moverBandido(tBandido *bandido, unsigned pasos, char direccion, unsigned cantPosiciones)
{
    if (direccion == DIR_ADELANTE)
    {
        bandido->posEnRuta += pasos;
        if (bandido->posEnRuta > cantPosiciones)
            bandido->posEnRuta -= cantPosiciones; // atraviesa la salida
    }
    else
    {
        if (pasos >= bandido->posEnRuta)
            bandido->posEnRuta = cantPosiciones - (pasos - bandido->posEnRuta); // atraviesa entrada
        else
            bandido->posEnRuta -= pasos;
    }
}

void moverBandidoEnRuta(tBandido *bandido, const tMovimiento *mov, tLista *ruta, unsigned cantPosiciones)
{
    tCasillero *casillero, casilleroNum;

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = (tCasillero *)buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos--;

    moverBandido(bandido, mov->cantPasos, mov->direccion, cantPosiciones);

    casilleroNum.numeroCasillero = bandido->posEnRuta;
    casillero = (tCasillero *)buscarElemPorClaveLista(ruta, &casilleroNum, cmpCasillero);
    casillero->cantBandidos++;
}

void limpiarJuego (tJuego* juego)
{
    vaciarColaDin(&juego->partida.movimientos);
    vaciarColaDin(&juego->partida.registroMovimientos);
    liberarLista(&juego->listaRankingJugadores);
    liberarLista(&juego->partida.ruta); 
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
    case ERROR_ARCHIVO_CONFIG: puts("HUBO UN ERROR AL ABRIR EL ARCHIVO DE CONFIGURACION.");
        break;
    case ERROR_ARCHIVO_INDICE: puts("HUBO UN ERROR AL ABRIR EL ARCHIVO DE INDICE DE USUARIOS.");
        break;
    case ERROR_ARCHIVO_USUARIOS: puts("HUBO UN ERROR AL ABRIR EL ARCHIVO DE USUARIOS.");
        break;
    default: puts("ERROR DESCONOCIDO.");
        break;
    }
}
