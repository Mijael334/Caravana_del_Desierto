#include "../include/cola_dinamica.h"

void crearColaDin(tCola *p)
{
    p->pri=NULL;
    p->ult=NULL;
}

int colaVaciaDin(const tCola *p)
{
    return p->pri==NULL;
}

int ponerEnColaDin(tCola *p, const void *info, unsigned tam)
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
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
    nue->sig = NULL;


    if(p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;

    p->ult = nue;

    return OK;
}

int sacarDeColaDin(tCola *p, void *info, unsigned tam)
{
    tNodo *elim=p->pri;

    if(p->pri==NULL)
        return ERROR_VACIO;

    memcpy(info,elim->info,MIN(tam,elim->tam));

    p->pri=elim->sig;

    free(elim->info);
    free(elim);

    if(p->pri==NULL)
        p->ult=NULL;

    return OK;
}

int verPrimeroColaDin(const tCola *p, void *info, unsigned tam)
{
    tNodo *aux=p->pri;

    if(p->pri==NULL)
        return ERROR_VACIO;

    memcpy(info, aux->info, MIN(tam, aux->tam));

    return OK;
}

void vaciarColaDin(tCola *p)
{
    tNodo *elim;

    while(p->pri)
    {
        elim=p->pri;
        p->pri=elim->sig;
        free(elim->info);
        free(elim);

    }

    p->ult=NULL;
}



