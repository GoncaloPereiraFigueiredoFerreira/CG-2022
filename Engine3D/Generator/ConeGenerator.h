#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include "../Models/Cone.h"
#include "../Auxiliar/AuxiliarMethods.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

void generateConeFile(string filename, Cone cone);
Cone* readConeFromFile(std::ifstream& fd);

#endif