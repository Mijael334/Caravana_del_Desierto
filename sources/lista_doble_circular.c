#include "../include/lista_doble_circular.h"

void crearLista(tLista *p)
{
    *p=NULL;
}

int insFinLista(tLista *p, void *info, unsigned tam)
{
    tNodoL *nue;
    tNodoL *act = *p, *auxant, *auxsig;

    nue = (tNodoL*)malloc(sizeof(tNodoL));
    if(!nue)
        return ERROR_MEM;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEM;
    }

    memcpy(nue->info, info, tam);
    nue->tam = tam;

    if(!*p)
    {
        nue->sig = nue;
        nue->ant = nue;
        *p=nue;
    }
    else
    {
        auxsig=act;
        auxant=act->ant;

        nue->sig=act;
        nue->ant=auxant;

        auxsig->ant=nue;
        auxant->sig=nue;
    }

    return OK;
}

int insPriLista(tLista *p, void *info, unsigned tam)
{
    tNodoL *nue;
    tNodoL *act = *p, *auxant, *auxsig;

    nue = (tNodoL*)malloc(sizeof(tNodoL));
    if(!nue)
        return ERROR_MEM;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEM;
    }

    memcpy(nue->info, info, tam);
    nue->tam = tam;

    if(!*p)
    {
        nue->sig = nue;
        nue->ant = nue;
    }
    else
    {
        auxsig = act;
        auxant = act->ant;

        nue->sig = auxsig;
        nue->ant = auxant;

        auxsig->ant = nue;
        auxant->sig = nue;
    }

    *p=nue;

    return OK;
}


void* buscarElemPorClaveLista(const tLista *p, const void *clave, int (*cmp)(const void*, const void*))
{
    tNodoL *act = *p;

    if (!*p)
        return NULL;

    if (cmp(act->info, clave) == 0)
        return act->info;

    act = act->sig;

    while (act != *p)
    {
        if (cmp(act->info, clave) == 0)
            return act->info;

        act = act->sig;
    }

    return NULL;
}

void liberarLista(tLista *p)
{
    if(!*p)
        return;

    tNodoL *act=*p;
    tNodoL *auxsig,*auxant=act->ant;

    auxant->sig=NULL;


    while(act)
    {
        auxsig=act->sig;
        free(act->info);
        free(act);
        act=auxsig;
    }

    *p=NULL;

}

void recorrerLista(const tLista *p, void (*procesar)(void*))
{
    if(!*p)
        return;
    tNodoL *act = *p;
    procesar(act->info);
    act = act->sig;
    while(act != *p)
    {
        procesar(act->info);
        act = act->sig;
    }
}
