#pragma once
#include "Box.h"
#include <string>
#include <vector>
#include <fstream>

void generateBoxFile(std::string filename, Box box);
Box* readBoxFromFile(std::ifstream& fd);