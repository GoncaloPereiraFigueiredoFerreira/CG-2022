#pragma once
#include "Plane.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

Plane generatePlane(float length, int div);
void generatePlaneFile(string filename, Plane plane);
Plane* readPlaneFromFile(std::ifstream& fd);