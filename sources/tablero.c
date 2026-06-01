#include "../include/tablero.h"

int criterio_tormenta_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && ant->evento != EVENTO_TORMENTA && sig->evento != EVENTO_TORMENTA;
}

int criterio_bandido_ideal(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && act->cantBandidos == 0 && ant->cantBandidos == 0 && sig->cantBandidos == 0;
}

int criterio_tormenta_emergencia(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && ant->evento != EVENTO_TORMENTA;
}

int criterio_bandido_emergencia(const tCasillero *act, const tCasillero *ant, const tCasillero *sig)
{
    return act->evento == EVENTO_VACIO && act->cantBandidos == 0;
}

tNodoL *buscar_nodo_en_posicion(tLista *lista, int posicion)
{
    tNodoL *aux = *lista;
    for(int i = 0; i < posicion; i++)
    {
        aux = aux->sig;
    }
    return aux;
}

tNodoL *buscar_nodo_seguro(tLista *lista, int cant_posiciones, int (*es_ideal)(const tCasillero *, const tCasillero *, const tCasillero *), int (*es_emergencia)(const tCasillero *, const tCasillero *, const tCasillero *))
{
    int pos_rand = (rand() % (cant_posiciones - 2)) + 1;
    tNodoL *inicio = buscar_nodo_en_posicion(lista, pos_rand);
    tNodoL *aux = inicio;
    tNodoL *emergencia = NULL;
    do{
        tCasillero *act = (tCasillero *)aux->info;
        tCasillero *ant = (tCasillero *)aux->ant->info;
        tCasillero *sig = (tCasillero *)aux->sig->info;
        if(es_ideal(act, ant, sig))
        {
            return aux;
        }
        if(emergencia == NULL && es_emergencia(act, ant, sig))
        {
            emergencia = aux;
        }
        aux = aux->sig;
    }while(aux != inicio);
    return emergencia ? emergencia : inicio;
}

int configuracion_valida(const tConfig *configuracion)
{
    int disponibles, total_elementos;
    if(configuracion == NULL)
    {
        return 0;
    }
    if(configuracion->cant_posiciones <= 2)
    {
        return 0;
    }
    if(configuracion->premios_max < 0 || configuracion->max_vidas_extras < 0 || configuracion->oasis_max < 0 || configuracion->tormenta_max < 0 || configuracion->bandidos_max < 0)
    {
        return 0;
    }
    disponibles = configuracion->cant_posiciones - 2;
    total_elementos = configuracion->premios_max + configuracion->max_vidas_extras + configuracion->oasis_max + configuracion->tormenta_max + configuracion->bandidos_max;
    return total_elementos <= disponibles;
}

void distribuir_evento_aleatorio(tLista *lista, int cant_posiciones, int cantidad_maxima, tTipoEvento tipo_evento)
{
    tNodoL *aux = *lista;
    for(int i = 0; i < cantidad_maxima; i++)
    {
        int pasos = (rand() % (cant_posiciones - 2)) + 1;
        int colocado;
        tNodoL *inicio_busqueda;
        for(int j = 0; j < pasos; j++)
        {
            aux = aux->sig;
        }
        inicio_busqueda = aux;
        colocado = 0;
        do{
            tCasillero *casillero = (tCasillero *)aux->info;
            if(casillero->evento == EVENTO_VACIO)
            {
                casillero->evento = tipo_evento;
                colocado = 1;
                break;
            }
            aux = aux->sig;
        }while(aux != inicio_busqueda);
        if(!colocado)
        {
            return;
        }
    }
}

void ubicar_tormenta_segura(tLista *lista, int cant_posiciones)
{
    tNodoL *nodo = buscar_nodo_seguro(lista, cant_posiciones, criterio_tormenta_ideal, criterio_tormenta_emergencia);
    tCasillero *cas = (tCasillero *)nodo->info;
    cas->evento = EVENTO_TORMENTA;
}

void ubicar_bandido_seguro(tLista *lista, int cant_posiciones, tBandido *bandido, int id_bandido)
{
    tNodoL *nodo = buscar_nodo_seguro(lista, cant_posiciones, criterio_bandido_ideal, criterio_bandido_emergencia);
    tCasillero *cas = (tCasillero *)nodo->info;
    cas->cantBandidos = 1;
    bandido->id = id_bandido;
    bandido->posEnRuta = cas->numeroCasillero;
    bandido->vivo = VIVO;
}

int crear_tablero_circular(tLista *lista, const tConfig *configuracion, tBandido *bandidos)
{
    if(!configuracion_valida(configuracion))
    {
        return ERROR_ARCHIVO_CONFIG;
    }
    crearLista(lista);
    tCasillero casillero_aux;
    for(int i = 0; i < configuracion->cant_posiciones; i++)
    {
        casillero_aux.numeroCasillero = i;
        casillero_aux.evento = EVENTO_VACIO;
        casillero_aux.jugadorAca = 0;
        casillero_aux.cantBandidos = 0;
        if(insFinLista(lista, &casillero_aux, sizeof(tCasillero)) != OK)
        {
            liberarLista(lista);
            return ERROR_MEM;
        }
    }
    tCasillero *inicial = (tCasillero *)(*lista)->info;
    inicial->evento = EVENTO_INICIO;
    inicial->jugadorAca = 1;
    tCasillero *final = (tCasillero *)(*lista)->ant->info;
    final->evento = EVENTO_SALIDA;
    distribuir_evento_aleatorio(lista, configuracion->cant_posiciones, configuracion->premios_max, EVENTO_PREMIO);
    distribuir_evento_aleatorio(lista, configuracion->cant_posiciones, configuracion->max_vidas_extras, EVENTO_VIDA_EXTRA);
    distribuir_evento_aleatorio(lista, configuracion->cant_posiciones, configuracion->oasis_max, EVENTO_OASIS);
    for(int i = 0; i < configuracion->tormenta_max; i++)
    {
        ubicar_tormenta_segura(lista, configuracion->cant_posiciones);
    }
    for(int i = 0; i < configuracion->bandidos_max; i++)
    {
        ubicar_bandido_seguro(lista, configuracion->cant_posiciones, bandidos + i, INICIO_ID_BANDIDOS + i);
    }
    return TODO_OK;
}

const char simbolos_eventos[] = { '.',
                                  'O',
                                  'T',
                                  'V',
                                  'I',
                                  'S',
                                  'P'
                                };

void renderizar_tablero(tLista *lista, int cant_posiciones, FILE *destino)
{
    if(*lista == NULL || destino == NULL)
    {
        return;
    }
    tNodoL *aux = *lista;
    for(int i = 0; i < cant_posiciones; i++)
    {
        tCasillero *act = (tCasillero *)aux->info;
        char caracter_evento = simbolos_eventos[act->evento];
        fprintf(destino, "%02d:", act->numeroCasillero);
        if(act->jugadorAca)
        {
            if(act->cantBandidos > 0)
            {
                fprintf(destino, "[B J]\n");
            }
            else if(act->evento == EVENTO_VACIO)
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
            if(act->cantBandidos > 0)
            {
                fprintf(destino, "B\n");
            }
            else
            {
                fprintf(destino, "%c\n", caracter_evento);
            }
        }
        aux = aux->sig;
    }
}

void guardar_tablero_en_archivo(tLista *lista, int cant_posiciones)
{
    FILE *arch;
    if(abrir_archivo(&arch, NOM_ARCH_CARAVANA, "wt") != TODO_OK)
    {
        return;
    }
    renderizar_tablero(lista, cant_posiciones, arch);
    fclose(arch);
}
