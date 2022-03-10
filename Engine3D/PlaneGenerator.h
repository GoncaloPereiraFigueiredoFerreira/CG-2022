#pragma once
#include "Plane.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

void generatePlaneFile(string filename, Plane plane);
Plane* readPlaneFromFile(std::ifstream& fd);