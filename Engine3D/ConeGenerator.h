#pragma once
#include "Cone.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

void generateConeFile(string filename, Cone cone);
Cone* readConeFromFile(std::ifstream& fd);