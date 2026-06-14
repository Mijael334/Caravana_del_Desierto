#include "../include/indice_jugador.h"

int cmpClaveIndice(const void *a, const void *b)
{
    const tIndice *idxA = (const tIndice *)a;
    const tIndice *idxB = (const tIndice *)b;

    return strcmp(idxA->clave.nombre, idxB->clave.nombre);
}

int indexarArchivoUsuariosOrdenado(tArbolBinBusq *pa, const char *nombreArchivoUsuarios)
{
    int ret;
    FILE *find;

    ret = abrir_archivo(&find, nombreArchivoUsuarios, "rb");

    if(ret != TODO_OK)
        return ret;

    fseek(find, 0L, SEEK_END);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa, find, 0, ftell(find)/sizeof(tIndice) - 1);
    fclose(find);

    return TODO_OK;
}

void cargarArchivoOrdenadoEnIndiceBalanceado(tArbolBinBusq *pa, FILE *arch, int inicio, int fin)
{
    int medio;
    if(inicio > fin)
        return;

    medio = (fin + inicio) / 2;
    tIndice ind;
    fseek(arch, medio * sizeof(tIndice), SEEK_SET);
    fread(&ind, 1, sizeof(tIndice), arch);
    insertarArbolBinBusq(pa, &ind, sizeof(tIndice), cmpClaveIndice);

    cargarArchivoOrdenadoEnIndiceBalanceado(pa ,arch, inicio, medio - 1);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa , arch, medio + 1, fin);
}

int registrarNuevoUsuarioEnIndice(tArbolBinBusq *arbol, const char *nombre, unsigned posNueva, const char *nombreArchivoIndice)
{
    tIndice nuevo;
    int ret;

    strncpy(nuevo.clave.nombre, nombre, TAM_MAX_NOM - 1);
    nuevo.clave.nombre[TAM_MAX_NOM - 1] = '\0';
    nuevo.indiceRegistro = posNueva;

    ret = insertarArbolBinBusq(arbol, &nuevo, sizeof(tIndice), cmpClaveIndice);
    if(ret != TODO_OK)
        return ret;

    return guardarIndiceEnArchivo(arbol, nombreArchivoIndice);
}

int crearArchivoIndiceOrdenado(tArbolBinBusq *pa, const char *nombreArchivo)
{
    FILE *fInd;
    tIndice ind;
    int pos = 0;
    ///hay que definir una estructura usuario
    ///tenga nombre del jugador y el Id de partida
    //por ahora usare esta
    tJugador jugador;
    if(abrir_archivo(fInd, nombreArchivo, "rb"))
    {
        return ERROR_ARCHIVO_INDICE;
    }
    while(fread(&jugador, sizeof(tJugador), 1, fInd))
    {
        memcpy(ind.clave.nombre, jugador.nombre);
        ind.indiceRegistro = pos;
        insertarArbolBinBusq(pa, &ind, sizeof(tIndice),cmpClaveIndice);
        pos++;
    }
    fclose(fInd);
    return TODO_OK;
}
