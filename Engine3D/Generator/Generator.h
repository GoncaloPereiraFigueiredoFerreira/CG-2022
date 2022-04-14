#ifndef ENGINEEXE_GENERATOR_H
#define ENGINEEXE_GENERATOR_H

#include <vector>
#include <stdio.h>
#include <string>

int readModelFromFile(std::string filename, std::vector<float>& vertexB, std::vector<unsigned int>& indexB);

#endif //ENGINEEXE_GENERATOR_H