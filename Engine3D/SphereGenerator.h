#pragma once
#include "Sphere.h"
#include <string>
#include <vector>
#include <fstream>

void generateSphereFile(std::string filename, Sphere sphere);
Sphere* readSphereFromFile(std::ifstream& fd);