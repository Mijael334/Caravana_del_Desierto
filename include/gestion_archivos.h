#ifndef GESTION_ARCHIVOS_H_INCLUDED
#define GESTION_ARCHIVOS_H_INCLUDED
///defino algunas macro que podemos que deSpues se puede cambiar para su mejor manejo
#define ERROR_ARCHIVO_CONFIG -2
#define ERROR_ARCHIVO_USUARIOS -3
#define ERROR_ARCHIVO_INDICE -4

#define TODO_OK 1
#define TAM_MAX 200

#include <stdio.h>
#include <string.h>

#include "../include/arbol.h"
#include "../include/cola_dinamica.h"

#define NOM_ARCH_CONFIG "docs/config.txt"
#define NOM_ARCH_USUARIOS "docs/usuarios.dat"
#define NOM_ARCH_CARAVANA "docs/caravana.txt"
#define ARCHIVO_INDICE "indice.bat"
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

int guardarIndiceEnArchivo(const tArbolBinBusq *pt, const char *nombreArchivoIndice);
void guardarNodoEnArchivo(void *info, unsigned tam, unsigned n, void *params);
#endif // GESTION_ARCHIVOS_H_INCLUDED
