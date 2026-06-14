#include "../include/tablero.h"

int configuracion_valida(const tConfig *configuracion)
{
    int disponibles, total_elementos;
    if(configuracion == NULL)
    {
        return 0; // no hay arch config
    }
    if(configuracion->cant_posiciones <= 2)
    {
        return 0; // no se puede usar el tablero
    }
    if(configuracion->premios_max + configuracion->max_vidas_extras + configuracion->oasis_max + configuracion->tormenta_max + configuracion->bandidos_max >= configuracion->cant_posiciones)
    {
        return 0; //error de parametros en archivos
    }
    disponibles = configuracion->cant_posiciones - 2;
    total_elementos = configuracion->premios_max + configuracion->max_vidas_extras + configuracion->oasis_max + configuracion->tormenta_max + configuracion->bandidos_max;
    return total_elementos <= disponibles;
}

int criterio_tormenta_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && ant->evento != EVENTO_TORMENTA && sig->evento != EVENTO_TORMENTA;
}

int criterio_bandido_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && act->cantBandidos == 0 && ant->cantBandidos == 0 && sig->cantBandidos == 0;
}

int criterio_casillero_libre(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && act->cantBandidos == 0;
}

void distribuir_elemento(tLista *lista, int cant_posiciones, int cant_maxima, int (*criterio)(const tCasillero *, const tCasillero *, const tCasillero *), tTipoEvento tipo_evento, int es_bandido, tBandido *bandido)
{
    tCasillero clave_act, clave_ant, clave_sig;
    tCasillero *act, *ant, *sig;
    unsigned pos_inicial, pos_actual, pos_anterior, pos_siguiente;
    tCasillero *emergencia;
    int i, colocado;
    int casilleros_utilizables = cant_posiciones - 2;
    int vueltas_dadas = 0;
    for(i = 0; i < cant_maxima; i++)
    {
        emergencia = NULL;
        colocado = 0;
        pos_inicial = (generarRandomUniforme(cant_posiciones - 2)) + 2;
        pos_actual = pos_inicial;
        do
        {
            pos_anterior = ((pos_actual - 2 + cant_posiciones) % cant_posiciones) + 1;
            pos_siguiente = (pos_actual % cant_posiciones) + 1;
            clave_act.numeroCasillero = pos_actual;
            clave_ant.numeroCasillero = pos_anterior;
            clave_sig.numeroCasillero = pos_siguiente;
            act = (tCasillero *)buscarElemPorClaveLista(lista, &clave_act, cmpCasillero);
            ant = (tCasillero *)buscarElemPorClaveLista(lista, &clave_ant, cmpCasillero);
            sig = (tCasillero *)buscarElemPorClaveLista(lista, &clave_sig, cmpCasillero);
            if(act && act->numeroCasillero != cant_posiciones && act->numeroCasillero != 1)
            {
                if(ant && sig)
                {
                    if(criterio(act, ant, sig))
                    {
                        if(es_bandido)
                        {
                            act->cantBandidos++;
                            (bandido + i)->posEnRuta = act->numeroCasillero;
                        }
                        else
                        {
                            act->evento = tipo_evento;
                        }
                        colocado = 1;
                    }
                    else
                    {
                        if(!emergencia)
                        {
                            if(es_bandido)
                            {
                                emergencia = act;
                            }
                            else
                            {
                                if(criterio_casillero_libre(act, ant, sig))
                                {
                                    emergencia = act;
                                }
                            }
                        }
                    }
                }
            }
            pos_actual++;
            if(pos_actual >= (unsigned)cant_posiciones)
            {
                pos_actual = 2;
            }
            vueltas_dadas++;
        }while(vueltas_dadas < casilleros_utilizables && !colocado);
        if(!colocado && emergencia)
        {
            if(es_bandido)
            {
                emergencia->cantBandidos++;
                (bandido + i)->posEnRuta = emergencia->numeroCasillero;
            }
            else
            {
                emergencia->evento = tipo_evento;
            }
        }
        if(es_bandido && (colocado || emergencia))
        {
            (bandido + i)->id = INICIO_ID_BANDIDOS + i;
            (bandido + i)->vivo = VIVO;
        }
    }
}

//int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos)
//{
//    tCasillero casillero_aux;
//    tCasillero clave;
//    tCasillero *inicial, *final;
//    unsigned i;
//
//    if(!configuracion_valida(configuracion))
//        return ERROR_ARCHIVO_CONFIG;
//
//
//    liberarLista(lista);
//    crearLista(lista);
//
//    // creo n casilleros vacios numerados de 1 a n
//    for(i = 0; i < configuracion->cant_posiciones; i++)
//    {
//        casillero_aux.numeroCasillero = i + 1;
//        casillero_aux.evento = EVENTO_VACIO;
//        casillero_aux.jugadorAca = 0;
//        casillero_aux.cantBandidos = 0;
//        if(insFinLista(lista, &casillero_aux, sizeof(tCasillero)) != OK)
//        {
//            liberarLista(lista);
//            return ERROR_MEM;
//        }
//    }
//
//    // marco Inicio y Salida
//    clave.numeroCasillero = 1;
//    inicial = (tCasillero*) buscarElemPorClaveLista(lista, &clave, cmpCasillero);
//    inicial->evento = EVENTO_INICIO;
//    inicial->jugadorAca = 1;
//    clave.numeroCasillero = configuracion->cant_posiciones;
//    final = (tCasillero*) buscarElemPorClaveLista(lista, &clave, cmpCasillero);
//    final->evento = EVENTO_SALIDA;
//    // distribuir eventos
//    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->tormenta_max, criterio_tormenta_ideal, EVENTO_TORMENTA, 0, NULL);
//    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->premios_max, criterio_casillero_libre, EVENTO_PREMIO, 0, NULL);
//    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->max_vidas_extras, criterio_casillero_libre, EVENTO_VIDA_EXTRA, 0, NULL);
//    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->oasis_max, criterio_casillero_libre, EVENTO_OASIS, 0, NULL);
//    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->bandidos_max, criterio_bandido_ideal, EVENTO_VACIO, 1, bandidos);
//    return TODO_OK;
//}

//void renderizar_tablero(tLista *lista, int cant_posiciones, FILE *destino)
//{
//    const char simbolos_eventos[] = { '.',
//                                    'O',
//                                    'T',
//                                    'V',
//                                    'I',
//                                    'S',
//                                    'P'
//                                    };
//    tCasillero clave;
//    tCasillero *actual;
//    char caracter_evento;
//    int i, j;
//    if(*lista == NULL || destino == NULL)
//    {
//        return;
//    }
//    for(i = 0; i < cant_posiciones; i++)
//    {
//        clave.numeroCasillero = i + 1;
//        actual = (tCasillero *)buscarElemPorClaveLista(lista, &clave, cmpCasillero);
//        if(!actual)
//        {
//            continue;
//        }
//        caracter_evento = simbolos_eventos[actual->evento];
//        fprintf(destino, "%02d:", actual->numeroCasillero);
//        if(actual->jugadorAca)
//        {
//            if(actual->cantBandidos > 0)
//            {
//                fprintf(destino, "[");
//                for(j = 0; j < actual->cantBandidos; j++)
//                {
//                    fprintf(destino, "B ");
//                }
//                fprintf(destino, "J]\n");
//            }
//            else if(actual->evento == EVENTO_VACIO)
//            {
//                fprintf(destino, "[J]\n");
//            }
//            else
//            {
//                fprintf(destino, "[%c J]\n", caracter_evento);
//            }
//        }
//        else
//        {
//            if(actual->cantBandidos > 0)
//            {
//                if(actual->evento != EVENTO_VACIO)
//                {
//                    fprintf(destino, "%c (", caracter_evento);
//                    for(j = 0; j < actual->cantBandidos; j++)
//                    {
//                        fprintf(destino, (j == actual->cantBandidos - 1) ? "B" : "B ");
//                    }
//                    fprintf(destino, ")\n");
//                }
//                else
//                {
//                    for(j = 0; j < actual->cantBandidos; j++)
//                    {
//                        fprintf(destino, "B ");
//                    }
//                    fprintf(destino, "\n");
//                }
//            }
//            else
//            {
//                fprintf(destino, "%c\n", caracter_evento);
//            }
//        }
//    }
//}

//void guardar_tablero_en_archivo(tLista *lista, int cant_posiciones)
//{
//    FILE *arch;
//    if(abrir_archivo(&arch, NOM_ARCH_CARAVANA, "wt") != TODO_OK)
//    {
//        return;
//    }
//    renderizar_tablero(lista, cant_posiciones, arch);
//    fclose(arch);
//}


int cmpCasillero (const void* a, const void* b)
{
    const tCasillero* c1 = (const tCasillero*) a;
    const tCasillero* c2 = (const tCasillero*) b;

    return c1->numeroCasillero - c2->numeroCasillero;
}

int cmpCasilleroEvento (const void* a, const void* b)
{
    const tCasillero* c1 = (const tCasillero*) a;
    const tCasillero* c2 = (const tCasillero*) b;

    return c1->evento == c2->evento;
}

void imprimirCasillero(void *info, FILE *destino)
{
    const char simbolos_eventos[] = { '.',
                                     'O',
                                     'T',
                                     'V',
                                     'I',
                                     'S',
                                     'P'
                                    };
    tCasillero *actual = (tCasillero *)info;
    char caracter_evento = simbolos_eventos[actual->evento];
    int j;
    fprintf(destino, "%02d:", actual->numeroCasillero);
    if(actual->jugadorAca)
    {
        if(actual->cantBandidos > 0)
        {
            fprintf(destino, "[");
            for(j = 0; j < actual->cantBandidos; j++)
            {
                fprintf(destino, "B ");
            }
            fprintf(destino, "J]\n");
        }
        else if(actual->evento == EVENTO_VACIO)
        {
            fprintf(destino, "[J]\n");
        }
        else
        {
            fprintf(destino, "[%c J]\n", caracter_evento);
        }
    }
    else
    {
        if(actual->cantBandidos > 0)
        {
            if(actual->evento != EVENTO_VACIO)
            {
                fprintf(destino, "%c [", caracter_evento);
                for(j = 0; j < actual->cantBandidos; j++)
                {
                    fprintf(destino, (j == actual->cantBandidos - 1) ? "B" : "B ");
                }
                fprintf(destino, "]\n");
            }
            else
            {
                for(j = 0; j < actual->cantBandidos; j++)
                {
                    fprintf(destino, "B ");
                }
                fprintf(destino, "\n");
            }
        }
        else
        {
            fprintf(destino, "%c\n", caracter_evento);
        }
    }
}

void renderizar_tablero(const tLista *lista, FILE *destino)
{
    if(*lista == NULL || destino == NULL)
    {
        return;
    }
    recorrerLista(lista, imprimirCasillero, destino);
}

void guardar_tablero_en_archivo(tLista *lista)
{
    FILE *arch;
    if(abrir_archivo(&arch, NOM_ARCH_CARAVANA, "wt") != TODO_OK)
    {
        return;
    }
    renderizar_tablero(lista, arch);
    fclose(arch);
}

void vaciar_datos_casillero(void *dato, FILE *pf)
{
    tCasillero *casillero = (tCasillero *)dato;
    casillero->evento = EVENTO_VACIO;
    casillero->jugadorAca = FALSO;
    casillero->cantBandidos = 0;
}

int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos)
{
    tCasillero casillero_aux;
    tCasillero clave;
    tCasillero *inicial, *final;
    unsigned i;

    if(!configuracion_valida(configuracion))
        return ERROR_ARCHIVO_CONFIG;

    if(*lista == NULL)
    {
        for(i = 0; i < configuracion->cant_posiciones; i++)
        {
            casillero_aux.numeroCasillero = i + 1;
            casillero_aux.evento = EVENTO_VACIO;
            casillero_aux.jugadorAca = FALSO;
            casillero_aux.cantBandidos = 0;
            if(insFinLista(lista, &casillero_aux, sizeof(tCasillero)) != OK)
            {
                liberarLista(lista);
                return ERROR_MEM;
            }
        }
    }
    
    clave.numeroCasillero = 1;
    inicial = (tCasillero*) buscarElemPorClaveLista(lista, &clave, cmpCasillero);
    inicial->evento = EVENTO_INICIO;
    inicial->jugadorAca = VERDADERO;
    clave.numeroCasillero = configuracion->cant_posiciones;
    final = (tCasillero*) buscarElemPorClaveLista(lista, &clave, cmpCasillero);
    final->evento = EVENTO_SALIDA;
    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->tormenta_max, criterio_tormenta_ideal, EVENTO_TORMENTA, 0, NULL);
    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->premios_max, criterio_casillero_libre, EVENTO_PREMIO, 0, NULL);
    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->max_vidas_extras, criterio_casillero_libre, EVENTO_VIDA_EXTRA, 0, NULL);
    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->oasis_max, criterio_casillero_libre, EVENTO_OASIS, 0, NULL);
    distribuir_elemento(lista, configuracion->cant_posiciones, configuracion->bandidos_max, criterio_bandido_ideal, EVENTO_VACIO, 1, bandidos);
    return TODO_OK;
}
