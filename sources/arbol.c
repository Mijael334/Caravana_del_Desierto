#include "../include/arbol.h"
void crearArbolBinBusq(tArbolBinBusq* pt)
{
    *pt = NULL;
}

int insertarArbolBinBusq(tArbolBinBusq* pt, const void* d, unsigned cantbyte, int(* cmp)(const void* a1, const void* a2))
{
    int res;
    if(*pt)
    {
        res = cmp(d,(*pt)->info);
        if(res > 0)
        {
            return insertarArbolBinBusq(&(*pt)->der, d, cantbyte, cmp);
        }
        else
        {
            if(res < 0)
            {
                return insertarArbolBinBusq(&(*pt)->izq, d, cantbyte, cmp);
            }
            else
            {
                return DATO_DUPLICADO;
            }
        }
    }

    tNodoArbol *nue = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if(!nue)
    {
        fprintf(stderr, "no se puedo reservar memoria para el nuevo nodo\n");
        return SIN_MEMORIA;
    }
    nue->info = malloc(cantbyte);
    if(!nue->info)
    {
        fprintf(stderr, "no se pudo reservar memoria para el dato\n");
        free(nue);
        return SIN_MEMORIA;
    }
    memcpy(nue->info, d, cantbyte);
    nue->tamInfo = cantbyte;
    nue->der = NULL;
    nue->izq = NULL;
    *pt = nue;
    return TODO_OK;
}

int buscarEnArbolBinBusq(const tArbolBinBusq* pt, void* d, unsigned cantbyte, int(* cmp)(const void* a1, const void* a2))
{
    if(!*pt)
        return DATO_NO_ENCONTRADO;/// 0
    int res = cmp(d, (*pt)->info);
    if(res == 0)
    {
        ///memcpy(d, (*pt)->info, cantbyte);
        return DATO_ENCONTRADO; /// 1
    }else
    {
        if(res > 0)
        {
            return buscarEnArbolBinBusq(&(*pt)->der, d, cantbyte, cmp);
        }else
        {
            return buscarEnArbolBinBusq(&(*pt)->izq, d, cantbyte, cmp);
        }
    }

}


void eliminarArbol(tArbolBinBusq *pt)
{
    if(*pt)
    {
        eliminarArbol(&(*pt)->izq);
        eliminarArbol(&(*pt)->der);
        free((*pt)->info);
        free(*pt);
        *pt = NULL;
    }
}

void recorrerArbolInnorden(const tArbolBinBusq *pt, unsigned n, void *params, void (*accion)(void *info , unsigned tam, unsigned m, void *param))
{
   if(!pt)
        return;
   recorrerArbolInnorden(&(*pt)->izq, n, params, accion);
   accion((*pt)->info, (*pt)->tamInfo, n, params);
   recorrerArbolInnorden(&(*pt)->der, n, params, accion);
}
