#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <iostream>

using namespace std;

void openfile();

//https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void progress_bar(float progress);
//https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
string get_selfpath();

// Genera a partir de valores enteros un rango en flotante de 0-1
template<typename T, typename G>
float raNge1(T a, G b);

#endif
