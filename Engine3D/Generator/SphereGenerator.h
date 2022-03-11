#ifndef GENERATOR_SPHERE_H
#define GENERATOR_SPHERE_H

#include "../Auxiliar/AuxiliarMethods.h"
#include "../Models/Sphere.h"
#include <string>
#include <vector>
#include <fstream>

void generateSphereFile(std::string filename, Sphere sphere);
Sphere* readSphereFromFile(std::ifstream& fd);

#endif