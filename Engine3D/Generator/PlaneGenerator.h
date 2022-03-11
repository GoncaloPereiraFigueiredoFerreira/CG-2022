#ifndef GENERATOR_PLANE_H
#define GENERATOR_PLANE_H

#include "../Models/Plane.h"
#include <string>
#include <vector>
#include <fstream>
#include "../Auxiliar/AuxiliarMethods.h"
using namespace std;

void generatePlaneFile(string filename, Plane plane);
Plane* readPlaneFromFile(std::ifstream& fd);

#endif