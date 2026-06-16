#ifndef TESTING_H_INCLUDED
#define TESTING_H_INCLUDED

#include <stdio.h>

#define TESTING_CANT_USUARIOS 12
#define TESTING_CANT_PARTIDAS 25

int testingCrearArchivos();
int testingMostrarArchivos();
int crearArchUsuariosTesting();
int crearArchPartidasTesting();
int crearArchIndiceTesting();
int mostrarArchBin (const char* nomArch, unsigned tamReg, FILE* salida, void (*mostrar) (const void*, void*));
void mostrarUsuarios(const void* reg, void* dato);
void mostrarPartidas(const void* reg, void* dato);
void mostrarIndice(const void* reg, void* dato);

#endif // TESTING_H_INCLUDED
