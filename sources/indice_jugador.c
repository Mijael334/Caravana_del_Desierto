#include "../include/indice_jugador.h"

int compararClaveIndice(const void *a, const void *b)
{
    const tIndice *idxA = (const tIndice *)a;
    const tIndice *idxB = (const tIndice *)b;

    return strcmp(idxA->clave.nombre, idxB->clave.nombre);
}

//int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *arch, tArbolBinBusq *pa, unsigned tam, int limiteInferior, int limiteSuperior)
//{
//    if(limiteInferior > limiteSuperior)
//    {
//        *pa = NULL;
//        return TODO_OK;
//    }
//    int medio = (limiteInferior + limiteSuperior) / 2;
//    tNodoArbol *nue;
//    nue = (tNodoArbol *)malloc(sizeof(tNodoArbol));
//    if(!nue)
//    {
//        return SIN_MEMORIA;
//    }
//    nue->info = malloc(tam);
//    if(!nue->info)
//    {
//        free(nue);
//        return SIN_MEMORIA;
//    }
//    fseek(arch, medio * tam, SEEK_SET);
//    if(!fread(nue->info, tam, 1, arch))
//    {
//        free(nue->info);
//        free(nue);
//        return ERROR_LECTURA;
//    }
//    nue->izq = NULL;
//    nue->der = NULL;
//    *pa = nue;
//    tIndice ind;
//    fseek(arch, medio * tam, SEEK_SET);
//    fread(&ind, tam, 1, arch);
//    insertarArbolBinBusq(pa, &ind, sizeof(tIndice), cmpClaveIndice);
//    if(!(cargarDesdeArchivoBinarioOrdenadoArbol(arch, &(*pa)->izq, tam, limiteInferior, medio - 1)))
//    {
//        eliminarArbol(pa);
//
//        ///return ERROR;
//        return 1;
//    }
//    if(!(cargarDesdeArchivoBinarioOrdenadoArbol(arch, &(*pa)->der, tam, medio + 1, limiteSuperior)))
//    {
//        eliminarArbol(pa);
//        ///return ERROR;
//        return 1;
//    }
//    return TODO_OK;
//}

int indexarArchivoUsuariosOrdenado(tArbolBinBusq *pa, const char *nombreArchivoUsuarios)
{
    FILE *find;
    int cantidadReg;
    if(abrir_archivo(&find, nombreArchivoUsuarios, "rb"))
    {
        return ERROR_ARCHIVO_USUARIOS;
    }
    fseek(find, 0L, SEEK_END);
    cantidadReg = ftell(find)/sizeof(tIndice);
    cargarArchivoOrdenadoEnIndiceBalanceado(pa, find, 0,cantidadReg - 1);
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

//int prepararLimitesCargaArbol(FILE *archivoIndice, tArbolBinBusq *pa, unsigned tam)
//{
//    int totalRegistros;
//    fseek(archivoIndice, 0L, SEEK_END);
//    totalRegistros = ftell(archivoIndice) / tam;
//    return cargarDesdeArchivoBinarioOrdenadoArbol(archivoIndice, pa, tam, 0, totalRegistros - 1);
//}
//
//int cargarIndiceDesdeArchivo(tArbolBinBusq *pa, const char *nombreArchivoIndice, unsigned tam)
//{
//    FILE *fInd;
//    if(abrir_archivo(&fInd, nombreArchivoIndice, "rb"))
//    {
//        return ERROR_ARCHIVO_INDICE;
//    }
//    if(!prepararLimitesCargaArbol(fInd, pa, tam))
//    {
//        fclose(fInd);
//        return ERROR_MEM;
//    }
//    fclose(fInd);
//    return TODO_OK;
//}


int busquedaIndexada(const tArbolBinBusq *pa, void *dato, unsigned cantbyte, int(*cmp)(const void *a1, const void *a2))
{
    return buscarEnArbolBinBusq(pa, dato, cantbyte, cmp);
}
