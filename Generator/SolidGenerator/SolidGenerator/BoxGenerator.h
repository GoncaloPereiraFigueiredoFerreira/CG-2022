#pragma once
#include "Box.h"
#include <string>
#include <vector>
#include <fstream>

Box generateBox(float lenght, int div);
void generateBoxFile(std::string filename, Box box);
Box* readBoxFromFile(std::ifstream& fd);