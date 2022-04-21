#ifndef ENGINEEXE_MODEL_H
#define ENGINEEXE_MODEL_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "string.h"
#include "../Auxiliar/AuxiliarMethods.h"

int generateModelFile(std::string filename, std::vector<float>& vertexB, std::vector<unsigned int>& indexB);
int readModelFromFile(std::string filename, std::vector<float>& vertexB, std::vector<unsigned int>& indexB);

#endif //ENGINEEXE_MODEL_H