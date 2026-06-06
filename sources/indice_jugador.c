#include "../include/indice_jugador.h"

int compararClaveIndice(const void *a, const void *b)
{
    const tIndice *idxA = (const tIndice *)a;
    const tIndice *idxB = (const tIndice *)b;

    return strcmp(idxA->clave.nombre, idxB->clave.nombre);
}

int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *arch, tArbolBinBusq *pa, unsigned tam, int limiteInferior, int limiteSuperior)
{
    if(limiteInferior > limiteSuperior)
    {
        *pa = NULL;
        return TODO_OK;
    }
    int medio = (limiteInferior + limiteSuperior) / 2;
    tNodoArbol *nue;
    nue = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if(!nue)
    {
        return SIN_MEMORIA;
    }
    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEMORIA;
    }
    fseek(arch, medio * tam, SEEK_SET);
    if(!fread(nue->info, tam, 1, arch))
    {
        free(nue->info);
        free(nue);
        return ERROR_LECTURA;
    }
    nue->izq = NULL;
    nue->der = NULL;
    *pa = nue;
    if(!(cargarDesdeArchivoBinarioOrdenadoArbol(arch, &nue->izq, tam, limiteInferior, medio - 1)))
    {
        vaciarArbol(pa);
        return ERROR;
    }
    if(!(cargarDesdeArchivoBinarioOrdenadoArbol(arch, &nue->der, tam, medio + 1, limiteSuperior)))
    {
        vaciarArbol(pa);
        return ERROR;
    }
    return TODO_OK;
}

int prepararLimitesCargaArbol(FILE *archivoIndice, tArbolBinBusq *pa, unsigned tam)
{
    int totalRegistros;
    fseek(archivoIndice, 0L, SEEK_END);
    totalRegistros = ftell(archivoIndice) / tam;
    return cargarDesdeArchivoBinarioOrdenadoArbol(archivoIndice, pa, tam, 0, totalRegistros - 1);
}

int cargarIndiceDesdeArchivo(tArbolBinBusq *pa, const char *nombreArchivoIndice, unsigned tam)
{
    FILE *arch;
    if(*pa)
    {
        return ERROR;
    }
    arch = fopen(nombreArchivoIndice, "rb");
    if(!arch)
    {
        return ERROR_APERTURA_ARCHIVO;
    }
    if(!prepararLimitesCargaArbol(arch, pa, tam))
    {
        fclose(arch);
        return ERROR;
    }
    fclose(arch);
    return TODO_OK;
}
