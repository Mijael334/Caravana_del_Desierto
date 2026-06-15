#include "../include/lista_enlazada.h"

void crearListaSimple(tListaSE *pl)
{
    *pl = NULL;
}

int listaVacia(const tListaSE *pl)
{
    return *pl == NULL;
}

int insertarEnOrdenLista(tListaSE *pl, const void *d, unsigned tam, int (*comparar)(const void *, const void *), int (*acum)(void** , unsigned* , const void*, unsigned))
{
    tNodoLista *nue;

    while(*pl && comparar((*pl)->info, d) > 0)
    {
        pl = &(*pl)->sig;
    }
    if(*pl && comparar((*pl)->info, d) == 0)
    {
        if(acum)
            if(acum(&(*pl)->info, &(*pl)->tamInfo, d, tam) != OK)
                return ERROR_MEM;

        return CLA_DUP;
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

void mostrarLista(const tListaSE *pl, void (*mostrar)(const void *, void*), void* paramExtra)
{
    while(*pl)
    {
        mostrar((*pl)->info,paramExtra);
        pl = &(*pl)->sig;
    }
}

void ordenarLista (tListaSE* pl, int (*cmp)(const void*, const void*))
{
    tListaSE* menor;
    tNodoLista* aux;

    while (*pl)
    {
        menor = _buscarMenor(pl, cmp);

        if(menor != pl)
        {
            aux = *menor;

            *menor = aux->sig;
            
            aux->sig = *pl;

            *pl = aux;
        }

        pl = &(*pl)->sig;
    }
}

tListaSE* _buscarMenor (tListaSE* pl, int (*cmp)(const void*, const void*))
{
    tListaSE* menor = pl;

    while (*pl)
    {
        if(cmp((*pl)->info, (*menor)->info) < 0)
            menor = pl;
        
        pl = &(*pl)->sig;
    }

    return menor;
}
