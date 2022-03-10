#pragma once
#include "Cone.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

Cone generateCone(float base, float height, int slices, int stacks);
void generateConeFile(string filename, Cone cone);
Cone* readConeFromFile(std::ifstream& fd);