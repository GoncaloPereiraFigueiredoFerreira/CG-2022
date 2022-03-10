#pragma once
#include "Sphere.h"
#include <string>
#include <vector>
#include <fstream>

Sphere generateSphere(float radius, int slices, int stacks);
void generateSphereFile(std::string filename, Sphere sphere);
std::vector<std::string> parseLine(std::string line, std::string delimiter);
Sphere* readSphereFromFile(std::ifstream& fd);