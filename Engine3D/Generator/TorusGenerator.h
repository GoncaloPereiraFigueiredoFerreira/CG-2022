#ifndef GENERATOR_TORUS_H
#define GENERATOR_TORUS_H

#include <vector>
#include "../Auxiliar/AuxiliarMethods.h"
using namespace std;

void torus(float inner_radius, float outer_radius, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB);

#endif