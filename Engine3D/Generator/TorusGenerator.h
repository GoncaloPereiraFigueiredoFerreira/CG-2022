#ifndef GENERATOR_TORUS_H
#define GENERATOR_TORUS_H

#include "../Models/Torus.h"
#include <string>
#include <vector>
#include <fstream>
#include "../Auxiliar/AuxiliarMethods.h"
using namespace std;

void generateTorusFile(string filename, Torus torus);
Torus* readTorusFromFile(std::ifstream& fd);

#endif