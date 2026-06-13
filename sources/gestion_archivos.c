#include "../include/gestion_archivos.h"

int abrir_archivo(FILE **fp, const char *nombre_archivo, const char *modo)
{
    *fp = fopen(nombre_archivo, modo);
    if(!*fp)
    {
        if (errno == ENOENT)
            return ERROR_ARCHIVO_NO_EXISTE;
        
        fprintf(stderr,"Error en al abrir el archivo %s en modo %s \n", nombre_archivo, modo);
        return ERROR_ARCHIVO_CONFIG;
    }
    return TODO_OK;
}

int lectura_de_configuracion(const char *archivo_config, tConfig *parametros)
{
    FILE *fconfig;
    char clave[TAM_MAX];
    unsigned valor;
//    if(!abrir_archivo(&fconfig, archivo_config, "rt"))
    if(abrir_archivo(&fconfig, archivo_config, "rt"))
    {
        return ERROR_ARCHIVO_CONFIG;
    }
    ///tal vez pueda validar cada ingreso que sea valor > 0, para evitar errores
    while (fscanf(fconfig, "%[^:]:%u\n",clave, &valor) ==  2)
    {
        if(strcmp(clave, NOM_CONFIG_CANT_POSICIONES) == 0)
            parametros->cant_posiciones = valor;
        if(strcmp(clave, NOM_CONFIG_VIDAS_INI) == 0)
            parametros->vidas_incio = valor;
        if(strcmp(clave, NOM_CONFIG_MAX_BANDIDOS) == 0)
            parametros->bandidos_max = valor;
        if(strcmp(clave, NOM_CONFIG_MAX_PREMIOS) == 0)
            parametros->premios_max = valor;
        if(strcmp(clave, NOM_CONFIG_MAX_VIDAS_EXTRAS) == 0)
            parametros->max_vidas_extras = valor;
        if(strcmp(clave, NOM_CONFIG_MAX_OASIS) == 0)
            parametros->oasis_max = valor;
        if(strcmp(clave, NOM_CONFIG_MAX_TORMENTAS) == 0)
            parametros->tormenta_max = valor;
    }
    fclose(fconfig);
    return TODO_OK;
}

int guardarIndiceEnArchivo(const tArbolBinBusq *pt, const char *nombreArchivoIndice)
{
    FILE *fInd;
    if(abrir_archivo(&fInd, nombreArchivoIndice, "wb"))
    {
        return ERROR_ARCHIVO_INDICE;
    }
    recorrerArbolInnorden(pt, 0, fInd, guardarNodoEnArchivo);
    fclose(fInd);
    return TODO_OK;
}

void guardarNodoEnArchivo(void *info, unsigned tam, unsigned n, void *params)
{
    FILE *fp = (FILE*)params;
    fwrite(info, tam, 1, fp);
}
