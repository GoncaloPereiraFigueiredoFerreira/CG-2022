#ifndef GENERATOR_BOX_H
#define GENERATOR_BOX_H

#include "../Auxiliar/AuxiliarMethods.h"
#include "../Models/Box.h"
#include "../Models/Point.h"
#include <string>
#include <vector>
#include <fstream>

void generateBoxFile(std::string filename, Box box);
Box* readBoxFromFile(std::ifstream& fd);

#endif