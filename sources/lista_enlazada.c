#include "../include/lista_enlazada.h"

void crearListaSimple(tListaSE *pl)
{
    *pl = NULL;
}

int listaVacia(const tListaSE *pl)
{
    return *pl == NULL;
}

int insertarEnOrdenLista(tListaSE *pl, const void *d, unsigned tam, int (*comparar)(const void *, const void *))
{
    tNodoLista *nue;
    while(*pl && comparar((*pl)->info, d) > 0)
    {
        pl = &(*pl)->sig;
    }
    nue = (tNodoLista *)malloc(sizeof(tNodoLista));
    if(nue == NULL)
    {
        return ERROR_MEM;
    }
    nue->info = malloc(tam);
    if(nue->info == NULL)
    {
        free(nue);
        return ERROR_MEM;
    }
    memcpy(nue->info, d, tam);
    nue->tamInfo = tam;
    nue->sig = *pl;
    *pl = nue;
    return OK;
}

void vaciarLista(tListaSE *pl)
{
    while(*pl)
    {
        tNodoLista *aux = *pl;
        *pl = aux->sig;
        free(aux->info);
        free(aux);
    }
}

void mostrarLista(const tListaSE *pl, void (*mostrar)(const void *))
{
    while(*pl)
    {
        mostrar((*pl)->info);
        pl = &(*pl)->sig;
    }
}
