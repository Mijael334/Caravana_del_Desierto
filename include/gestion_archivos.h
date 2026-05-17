#ifndef GESTION_ARCHIVOS_H_INCLUDED
#define GESTION_ARCHIVOS_H_INCLUDED
///defino algunas macro que podemos que deSpues se puede cambiar para su mejor manejo
#define ERROR_ARCHIVO_CONFIG -2
#define TODO_OK 1
#define TAM_MAX 200

#include <stdio.h>
#include <string.h>

#define NOM_ARCH_CONFIG "config.txt"
#define NOM_ARCH_RANKING "ranking.dat"
#define NOM_ARCH_CARAVANA "caravana.txt"
/// talvez podria usar signed(ya que solo podriamos usar num > 0)
typedef struct
{
    unsigned cant_posiciones;
    unsigned vidas_incio;
    unsigned bandidos_max;
    unsigned premios_max;
    unsigned max_vidas_extras;
    unsigned oasis_max;
    unsigned tormenta_max;

} tConfig;

int abrir_archivo(FILE **pf, const char *nombre_archivo, const char *modo);
int lectura_de_configuracion(const char *archivo_config, tConfig *parametros);
#endif // GESTION_ARCHIVOS_H_INCLUDED
