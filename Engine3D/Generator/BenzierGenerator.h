#ifndef GENERATOR_BENZIER_H
#define GENERATOR_BENZIER_H

#include "../Auxiliar/AuxiliarMethods.h"
#include <vector>
#include "../Auxiliar/Point.h"

void benzier(char *path,int tessellation,std::vector<float> &vertex_VBO,std::vector<unsigned int> &index_VBO);

#endif