#include "utils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <unistd.h>
#define PATH_MAX 1024
using namespace std;
using namespace std::chrono;

int main(int argc,char** argv){
    if(argc<=2){
        cerr << "[ERROR]: Wrong execution format" << endl;
        cerr << "./main streambuffer(0:tty,1:file) num_total_lineas(0,inf) "
             << "OPTIONAL: num_simulaciones(1,inf) mean(1,inf)" << endl;
        exit(-1);
    }

    streambuffer = atoi(argv[1]);
    num_lineas_total = atoi(argv[2]);
    if(argc>3){
        if(atoi(argv[3])>0)
            simulaciones = atoi(argv[3]);
    }
    if(argc>4){
        if(atoi(argv[4])>0){
            MEAN = atoi(argv[4]);
        }
    }

    informe = (float **) malloc (simulaciones*sizeof(float *));
    unsigned int i;
    for(i=0; i<simulaciones; i++)
        informe[i] = (float *) malloc (5*sizeof(float));

    //srandom(time(NULL));
    srandom(140421);
    if(streambuffer)
        openfile();

    for (cont_mean = 0; cont_mean < MEAN; ++cont_mean) {
        for (cont_simu = 0; cont_simu < simulaciones; ++cont_simu) {
            if(streambuffer==1){
                progress_bar(
                        raNge1(cont_simu+cont_mean*simulaciones,MEAN*simulaciones)
                        );
            }
            inicializacion();
            while(parar==false){
                temporizacion();
                suceso();
            }
        }
        generador_informes(simulaciones);
    }
    if(streambuffer==1){
        progress_bar(1);
    }

    for (i = 0; i < simulaciones; ++i) {
        free(informe[i]);
    }
    free(informe);

    cout << endl;
    return 0;
}

void inicializacion(){
    // inicializacion de variables de estado
    reloj = 0.0;
    lineas_ocupadas = 0;
    llamadas_total = 0;
    llamadas_perdidas = 0;

    // inicializacion de contadores estadisticos
    accum_ocupadas = 0;
    llamadas_total = 0;

    // inicializacion de la lista de sucesos
    while(!lsuc.empty()) {lsuc.pop_front();}
    nodo.tllego = 0;
    nodo.suceso = SUCESO_FIN_SIMULACION;
    nodo.tiempo = reloj+tparada;
    insertar_lsuc(nodo);
    nodo.suceso = SUCESO_LLEGADA_LLAMADA;
    nodo.tipo = AB;
    nodo.tiempo = reloj+generar_llamada(media_llamada_AB);
    insertar_lsuc(nodo);
    nodo.suceso = SUCESO_LLEGADA_LLAMADA;
    nodo.tipo = BA;
    nodo.tiempo = reloj+generar_llamada(media_llamada_BA);
    insertar_lsuc(nodo);

    parar=false;
}

bool compare(const suc &s1, const suc &s2)
{ return s1.tiempo < s2.tiempo; }

/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
    lsuc.push_back(n);
    // Mantener ordenada la lista por el tiempo de los sucesos
    lsuc.sort(compare);
}

/* Procedimiento temporizacion */
void temporizacion()
{
    nodo = lsuc.front();
    lsuc.pop_front();
    oldreloj = reloj;
    reloj = nodo.tiempo;
}

void suceso(){
    switch(nodo.suceso)
    {
        case SUCESO_LLEGADA_LLAMADA: llegada_llamada(); break;
        case SUCESO_FIN_LLAMADA: fin_llamada(); break;
        case SUCESO_FIN_SIMULACION: fin_simulacion(); break;
    }
}

void llegada_llamada(){
    // Acumular cambio ocupadas;
    accum_ocupadas += (reloj-oldreloj)*lineas_ocupadas;

    switch(nodo.tipo){
        case AB:
            nodo.suceso = SUCESO_LLEGADA_LLAMADA;
            nodo.tiempo = reloj+generar_llamada(media_llamada_AB);
            nodo.tipo = AB;
            nodo.tllego = 0;
            insertar_lsuc(nodo);
            break;
        case BA:
            nodo.suceso = SUCESO_LLEGADA_LLAMADA;
            nodo.tiempo = reloj+generar_llamada(media_llamada_BA);
            nodo.tipo = BA;
            nodo.tllego = 0;
            insertar_lsuc(nodo);
            break;
    }

    if(lineas_ocupadas < num_lineas_total){
        nodo.suceso = SUCESO_FIN_LLAMADA;
        nodo.tiempo = reloj+generar_duracion(media_duracion);
        nodo.tllego = reloj;
        insertar_lsuc(nodo);
        lineas_ocupadas++;
    }else{
        llamadas_perdidas++;
    }
    llamadas_total++;
}

void fin_llamada(){
    // Acumular cambio ocupadas;
    accum_ocupadas += (reloj-oldreloj)*lineas_ocupadas;
    lineas_ocupadas--;
}

void fin_simulacion(){
    parar = true;
    accum_ocupadas += (reloj-oldreloj)*lineas_ocupadas;

    informe[cont_simu][0] = accum_ocupadas/reloj;
    informe[cont_simu][1] = (accum_ocupadas/num_lineas_total)/reloj;
    informe[cont_simu][2] = llamadas_total;
    informe[cont_simu][3] = llamadas_perdidas;
    informe[cont_simu][4] = llamadas_perdidas/llamadas_total;
    if(DEBUG && NOT_ONLY_MEANS){
        stringstream oss;
        if(streambuffer==0){
            if(cont_simu==0){
                oss << "Ocupadas" << setw(20) << "Proporcion" << setw(20)
                    << "Total llamadas" << setw(20) << "Pérdidas"
                    << setw(20) << "Proporción" << endl;
            }
            oss << setprecision(2) << fixed << informe[cont_simu][0]
                << setw(20) << informe[cont_simu][1]
                << setw(20) << informe[cont_simu][2]
                << setw(20) << informe[cont_simu][3]
                << setw(20) << informe[cont_simu][4] << endl;
            cout << oss.str();
        }else {
            oss << setprecision(2) << fixed << informe[cont_simu][0]
                << setw(20) << informe[cont_simu][1]
                << setw(20) << informe[cont_simu][2]
                << setw(20) << informe[cont_simu][3]
                << setw(20) << informe[cont_simu][4] << endl;
            myfile << oss.str();
        }
    }
}

void generador_informes(unsigned int simulaciones){
    float media[5], dt[5];
    unsigned int i,j;

    for(j=0; j<5; j++)
    {
        media[j] = 0;
        for(i=0; i<simulaciones; i++)
            media[j] += informe[i][j];
        media[j] /= simulaciones;

        dt[j] = 0;
        for(i=0; i<simulaciones; i++)
            dt[j] += informe[i][j] * informe[i][j];
        dt[j] = sqrt((dt[j]-simulaciones*media[j]*media[j]) / (simulaciones-1.0));
    }

    stringstream oss;
    if(streambuffer==0){
        if(cont_mean==0){
            cout << " ############## Medias Obtenidas ############# " << endl;
            oss << "Ocupadas" << setw(5) << "dsv" << setw(15)
                << "Proporcion" << setw(5) << "dsv" << setw(15)
                << "Total llamadas" << setw(5) << "dsv" << setw(15)
                << "Pérdidas" << setw(5) << "dsv" << setw(15)
                << "Proporción" << setw(5) << "dsv" << endl;
        }
        oss << setprecision(2) << fixed << media[0] << setw(10) << dt[0]
            << setw(10) << media[1] << setw(10) << dt[1]
            << setw(10) << media[2] << setw(10) << dt[2]
            << setw(10) << media[3] << setw(10) << dt[3]
            << setw(10) << media[4] << setw(10) << dt[4] << endl;
        cout << oss.str();
    }else{
        if(NOT_ONLY_MEANS)
            myfile << "\n\n";
        oss << setprecision(2) << fixed << media[0] << setw(10) << dt[0]
            << setw(10) << media[1] << setw(10) << dt[1]
            << setw(10) << media[2] << setw(10) << dt[2]
            << setw(10) << media[3] << setw(10) << dt[3]
            << setw(10) << media[4] << setw(10) << dt[4] << endl;
        myfile << oss.str();
        if(NOT_ONLY_MEANS)
            myfile << "\n\n";
    }


}


float generador_exponencial(float media){
    float u;
    u = (float) random();
    u = (float) (u / (RAND_MAX+1.0));
    return (-media*log(1-u));
}

float generar_llamada(float media){
    return generador_exponencial(media);
}

float generar_duracion(float media){
    return generador_exponencial(media);
}

void openfile(){
    // Try to store the results in a file
    string path;
    stringstream oss;
    path = get_selfpath();
    path = path.substr(0,path.find_last_of("/\\") + 1) + "../resultados/" ;
    oss << "telefon-N:" << num_lineas_total << ".txt";
    cout << oss.str() << endl;
    myfile.open(path + oss.str(),ios::out|ios::trunc);
    if(!myfile.is_open()){
        cerr << "[ERROR]: Couldn't open file to save results" << endl;
        streambuffer = 0;
    }
}

//https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void progress_bar(float progress){
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

//https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
string get_selfpath() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
        buff[len] = '\0';
        return std::string(buff);
    }
    /* handle error condition */
    return "";
}

template<typename T, typename G>
float raNge1(T a, G b){
    return float(a) / float(b);
}
