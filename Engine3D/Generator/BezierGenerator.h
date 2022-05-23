#ifndef GENERATOR_BEZIER_H
#define GENERATOR_BEZIER_H

#include "../Auxiliar/AuxiliarMethods.h"
#include "../Auxiliar/catmull-rom.h"
#include <vector>
#include "../Auxiliar/Point.h"

void bezier(char *path,int tessellation,std::vector<float> &vertex_VBO,std::vector<unsigned int> &index_VBO, std::vector<float>& normalB, std::vector<float>& textB);

#endif