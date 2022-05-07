#ifndef GENERATOR_CONE_H
#define GENERATOR_CONE_H

#include "../Auxiliar/AuxiliarMethods.h"
#include <vector>
using namespace std;

void cone(float base, float height, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB, std::vector<float>& normalB);

#endif