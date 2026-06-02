#include "../include/gestion_archivos.h"

int abrir_archivo(FILE **fp, const char *nombre_archivo, const char *modo)
{
    *fp = fopen(nombre_archivo, modo);
    if(!*fp)
    {
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
        if(strcmp(clave, "cantidad_posiciones") == 0)
            parametros->cant_posiciones = valor;
        if(strcmp(clave, "vidas_inicio") == 0)
            parametros->vidas_incio = valor;
        if(strcmp(clave, "maximo_bandidos") == 0)
            parametros->bandidos_max = valor;
        if(strcmp(clave, "maximo_premios") == 0)
            parametros->premios_max = valor;
        if(strcmp(clave, "maximo_vidas_extra") == 0)
            parametros->max_vidas_extras = valor;
        if(strcmp(clave, "maximo_oasis") == 0)
            parametros->oasis_max = valor;
        if(strcmp(clave, "maximo_tormentas") == 0)
            parametros->tormenta_max = valor;
    }
    fclose(fconfig);
    return TODO_OK;
}
