#include "../include/testing.h"
#include "../include/logica_juego.h"

int testingCrearArchivos()
{
    int ret;

    ret = crearArchUsuariosTesting();

    if(ret != TODO_OK)
        return ret;

    ret = crearArchPartidasTesting();
    
    if(ret != TODO_OK)
        return ret;

    ret = crearArchIndiceTesting();

    return ret;
}

int testingMostrarArchivos()
{
    int ret;

    system("CLS");

    puts("============MOSTRANDO USUARIO.dat===============");
    printf("%-20s %-20s\n", "USUARIO", "NICKNAME");
    ret = mostrarArchBin(NOM_ARCH_USUARIOS, sizeof(tUsuario), stdout, mostrarUsuarios);

    if(ret != TODO_OK)
        return ret;

    puts("============MOSTRANDO PARTIDAS.dat===============");
    printf("%-10s %-20s %-20s %-10s %-10s %-10s %-10s %-10s\n","ID", "USERNAME", "NICKNAME", "PUNTOS", "VIDAS", "FORWARD","BACKWARD", "RESULTADO");
    ret = mostrarArchBin(NOM_ARCH_PARTIDAS, sizeof(tReportePartida), stdout, mostrarPartidas);

    if(ret != TODO_OK)
        return ret;

    puts("============MOSTRANDO INDICE.idx===============");
    printf("%-20s %-10s\n", "CLAVE", "POS");
    ret = mostrarArchBin(NOM_ARCH_INDICE_USUARIOS, sizeof(tIndice), stdout, mostrarIndice);

    system("pause");

    return ret;
}

int crearArchUsuariosTesting()
{
    FILE* fp; 
    tUsuario users[TESTING_CANT_USUARIOS] ={{"Juan", "juansito"},
                                            {"Micaela", "mica346"},
                                            {"juan", "juampi123"},
                                            {"Javier542", "javi"},
                                            {"fantasma", "fantasmita(hola)"},
                                            {"aaaaaaaaaaaaaaaaa","Miguel"},
                                            {"1franco53", "free"},
                                            {"malloc", "mica346"},
                                            {"AMILCAR", "rambling123"},
                                            {"juego321", "rambling123"},
                                            {"tester:)", "testing"},
                                            {"Hola mundo", "odaaa"}};

    if(abrir_archivo(&fp, NOM_ARCH_USUARIOS, "wb") != TODO_OK)
        return ERROR_TESTING;

    fwrite(users, sizeof(tUsuario), TESTING_CANT_USUARIOS, fp);

    fclose(fp);

    return TODO_OK;
}

int crearArchPartidasTesting()
{
    FILE* fp;
    tReportePartida reportes[TESTING_CANT_PARTIDAS] = {     {1,  {"juan", "juampi123"},              4, 2, 60, 24, "VICTORIA"},
                                                            {2,  {"Juan", "juansito"},              10, 3, 85, 10, "VICTORIA"},
                                                            {3,  {"Micaela", "mica346"},            2, 0, 30, 18, "DERROTA"},
                                                            {4,  {"Javier542", "javi"},             6, 1, 55, 20, "VICTORIA"},
                                                            {5,  {"fantasma", "fantasmita(hola)"},  1, 0, 15, 25, "DERROTA"},
                                                            {6,  {"aaaaaaaaaaaaaaaaa", "Miguel"},   8, 2, 72, 12, "VICTORIA"},
                                                            {7,  {"1franco53", "free"},             3, 0, 28, 22, "DERROTA"},
                                                            {8,  {"malloc", "mica346"},             5, 1, 48, 17, "VICTORIA"},
                                                            {9,  {"AMILCAR", "rambling123"},        0, 0, 10, 30, "DERROTA"},
                                                            {10, {"juego321", "rambling123"},       9, 3, 90,  5, "VICTORIA"},
                                                            {11, {"tester:)", "testing"},           4, 1, 45, 16, "DERROTA"},
                                                            {12, {"Hola mundo", "odaaa"},           7, 2, 67, 14, "VICTORIA"},
                                                            {13, {"juan", "juampi123"},             6, 1, 58, 21, "VICTORIA"},
                                                            {14, {"Juan", "juansito"},              2, 0, 25, 26, "DERROTA"},
                                                            {15, {"Micaela", "mica346"},            9, 2, 80,  8, "VICTORIA"},
                                                            {16, {"malloc", "mica346"},             1, 0, 18, 29, "DERROTA"},
                                                            {17, {"juego321", "rambling123"},       7, 1, 64, 13, "VICTORIA"},
                                                            {18, {"AMILCAR", "rambling123"},        3, 0, 35, 20, "DERROTA"},
                                                            {19, {"tester:)", "testing"},           8, 2, 74, 11, "VICTORIA"},
                                                            {20, {"Juan", "juansito"},              5, 1, 50, 19, "VICTORIA"},
                                                            {21, {"juan", "juampi123"},             0, 0, 12, 35, "DERROTA"},
                                                            {22, {"fantasma", "fantasmita(hola)"},  4, 1, 42, 18, "VICTORIA"},
                                                            {23, {"Hola mundo", "odaaa"},           2, 0, 27, 24, "DERROTA"},
                                                            {24, {"1franco53", "free"},             6, 2, 61, 15, "VICTORIA"},
                                                            {25, {"juego321", "rambling123"},      10, 3, 95,  3, "VICTORIA"}};

    if(abrir_archivo(&fp, NOM_ARCH_PARTIDAS, "wb") != TODO_OK)
        return ERROR_TESTING;

    fwrite(reportes, sizeof(tReportePartida), TESTING_CANT_PARTIDAS, fp);

    fclose(fp);

    return TODO_OK;
}

int crearArchIndiceTesting()
{
    FILE* fp;
    tIndice indices[TESTING_CANT_USUARIOS]={{{"1franco53"}, 6},
                                            {{"AMILCAR"}, 8},
                                            {{"Hola mundo"}, 11},
                                            {{"Javier542"}, 3},
                                            {{"Juan"}, 0},
                                            {{"Micaela"}, 1},
                                            {{"aaaaaaaaaaaaaaaaa"}, 5},
                                            {{"fantasma"}, 4},
                                            {{"juego321"}, 9},
                                            {{"juan"}, 2},
                                            {{"malloc"}, 7},
                                            {{"tester:)"}, 10}};

    if(abrir_archivo(&fp, NOM_ARCH_INDICE_USUARIOS, "wb") != TODO_OK)
        return ERROR_TESTING;

    fwrite(indices, sizeof(tIndice), TESTING_CANT_USUARIOS, fp);

    fclose(fp);

        return TODO_OK;
}

int mostrarArchBin (const char* nomArch, unsigned tamReg, FILE* salida, void (*mostrar) (const void*, void*))
{
    FILE* fp = fopen(nomArch, "rb");
    void* reg;

    if(!fp)
        return ERROR_TESTING_MOSTRAR;

    reg = malloc(tamReg);

    if(!reg)
    {
        fclose(fp);
        return ERROR_TESTING_MOSTRAR;
    }

    while (fread(reg, tamReg, 1, fp))
        mostrar(reg, salida);
    
    fclose(fp);
    free(reg);

    return TODO_OK;
}

void mostrarUsuarios(const void* reg, void* dato)
{
    const tUsuario* user = (const tUsuario*) reg;
    FILE* salida = (FILE*) dato;

    fprintf(salida, "%-20s %-20s\n", user->username, user->nickname);
}

void mostrarPartidas(const void* reg, void* dato)
{
    const tReportePartida* reporte = (const tReportePartida*) reg;
    FILE* salida = (FILE*) dato;

    fprintf(salida, "%-10u %-20s %-20s %-10u %-10u %-10u %-10u %-10s\n", reporte->idPartida, reporte->usuario.username, reporte->usuario.nickname, reporte->puntosObtenidos,
                                                          reporte->vidasRestantes, reporte->forwardCasillas, reporte->backwardCasillas, reporte->resultado);
}

void mostrarIndice(const void* reg, void* dato)
{
    const tIndice* indice = (const tIndice*) reg;
    FILE* salida = (FILE*) dato;

    fprintf(salida, "%-20s %-10u\n", indice->clave.nombre, indice->indiceRegistro);
}