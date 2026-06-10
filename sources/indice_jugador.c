#include "../include/indice_jugador.h"

int compararClaveIndice(const void *a, const void *b)
{
    const tIndice *idxA = (const tIndice *)a;
    const tIndice *idxB = (const tIndice *)b;

    return strcmp(idxA->clave.nombre, idxB->clave.nombre);
}

int indexarArchivoUsuariosOrdenado(tArbolBinBusq *pa, const char *nombreArchivoUsuarios)
{
    FILE *find;
    if(abrir_archivo(&find, nombreArchivoUsuarios, "rb"))
    {
        return ERROR_ARCHIVO_USUARIOS;
    }
    fseek(find, 0L, SEEK_END);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa, find, 0, ftell(find)/sizeof(tIndice) - 1);
    fclose(find);
    return TODO_OK;
}

void cargarArchivoOrdenadoEnIndiceBalanceado(tArbolBinBusq *pa, FILE *arch, int inicio, int fin)
{
    int medio = (fin + inicio) / 2;
    if(inicio > fin)
        return;

    tIndice ind;
    fseek(arch, medio * sizeof(tIndice), SEEK_SET);
    fread(&ind, 1, sizeof(tIndice), arch);
    insertarArbolBinBusq(pa, &ind, sizeof(tIndice), cmpClaveIndice);

    cargarArchivoOrdenadoEnIndiceBalanceado(pa ,arch, inicio, medio - 1);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa , arch, medio + 1, fin);
}

int busquedaIndexada(const tArbolBinBusq *pa, void *dato, unsigned cantbyte, int(*cmp)(const void *a1, const void *a2))
{
    return buscarEnArbolBinBusq(pa, dato, cantbyte, cmp);
}
