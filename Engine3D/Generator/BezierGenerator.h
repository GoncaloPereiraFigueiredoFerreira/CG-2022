#ifndef GENERATOR_BEZIER_H
#define GENERATOR_BEZIER_H

#include "../Auxiliar/AuxiliarMethods.h"
#include <vector>
#include "../Auxiliar/Point.h"

void bezier(char *path,int tessellation,std::vector<float> &vertex_VBO,std::vector<unsigned int> &index_VBO);

#endif