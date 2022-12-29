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

void openfile(){
    // Try to store the results in a file
    string path;
    stringstream oss;
    path = get_selfpath();
    path = path.substr(0,path.find_last_of("/\\") + 1) + "../resultados/" ;
    oss << "sir-a:" << a << "-b:" << b << "-dt:" << dt <<
        "-I:" << estado[0] << "-S:" << estado[1] << "-R:" << estado[2] <<
        "-M:"<< runge << "-C:" << imunidad << ".txt";
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
