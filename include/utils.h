#ifndef _GLOBAL_H
#define _GLOBAL_H
#define SUCESO_LLEGADA_LLAMADA 0
#define SUCESO_FIN_LLAMADA 1
#define SUCESO_FIN_SIMULACION 2
#define DEBUG 1
#define NOT_ONLY_MEAN 1
#define AB 12
#define BA 21
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
using namespace std;

// ########################### Definimos variables globales ###################
typedef struct{
    int suceso;
    float tiempo;
    float tllego;
    int tipo;
} suc;
float reloj;
float oldreloj;
float llamadas_total;
float llamadas_perdidas;
float lineas_ocupadas;
unsigned int simulaciones = 10;
unsigned int MEAN = 10;

// Lista de sucesos
list<suc> lsuc;
bool parar;
suc nodo;

float** informe;
unsigned int cont_simu;
unsigned int cont_mean;

ofstream myfile;

// Parámetros de entrada:
float media_llamada_AB = 12.0;
float media_llamada_BA = 10.0;
float media_duracion = 4*60;
float num_lineas_total;
float tparada = 12*60*60;

// Contadores estadísticos
float accum_ocupadas;

bool streambuffer;

// ##########################################################################
// ############################# Definimos Funciones ########################
// ######## Principales:

void inicializacion();
void temporizacion();
void suceso();
void llegada_llamada();
void fin_llamada();
void fin_simulacion();

// ##################################
// ####### Secundarias

float generador_exponencial(float media);
float generar_llamada(float media);
float generar_duracion(float media);

// ##################################
// ####### Auxiliares:

void insertar_lsuc(suc n);
bool compare(const suc &s1, const suc &s2);
void generador_informes(unsigned int simulaciones);

void openfile();
//https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void progress_bar(float progress);
//https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
string get_selfpath();
// Genera a partir de valores enteros un rango en flotante de 0-1
template<typename T, typename G>
float raNge1(T a, G b);

// ##########################################################################
#endif
