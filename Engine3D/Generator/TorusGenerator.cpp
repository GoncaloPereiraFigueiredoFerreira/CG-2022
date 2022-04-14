#include "TorusGenerator.h"
using namespace std;

void torus(float inner_radius, float outer_radius, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB) {
    float radius = inner_radius + outer_radius;
    float slicesAngInc = 2 * M_PI / slices;
    float stacksAngInc = 2 * M_PI / stacks;
    float radius_x, radius_z, angle, aux_radius;
    vertexB.reserve(slices * stacks * 3);

    for (int i = 0; i < slices; i++) {

        angle = slicesAngInc * i;

        radius_x = cos(angle) * radius;
        radius_z = sin(angle) * radius;

        for (int j = 0; j < stacks; j++) {
            aux_radius = cos(stacksAngInc * j) * inner_radius; //anglo interno rebatido para o o plano xoz
            vertexB.push_back(radius_x + aux_radius * cos(angle));
            vertexB.push_back(sin(stacksAngInc * j) * inner_radius);
            vertexB.push_back(radius_z + aux_radius * sin(angle));
        }
    }

    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
        	indexB.push_back(i*stacks+j);
        	indexB.push_back(i*stacks+(j + 1)%stacks);
        	indexB.push_back(((i+1)% slices)*stacks+j);

        	indexB.push_back(i*stacks+(j + 1)%stacks);
        	indexB.push_back(((i+1)% slices)*stacks+((j+1)%stacks));
        	indexB.push_back(((i+1)% slices)*stacks+j);
        }
    }
}