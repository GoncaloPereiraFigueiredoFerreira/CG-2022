#include "TorusGenerator.h"
using namespace std;

void torus(float inner_radius, float outer_radius, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB) {
    float radius = inner_radius + outer_radius;
    float slicesAngInc = 2 * M_PI / slices;
    float stacksAngInc = 2 * M_PI / stacks;
    float radius_x, radius_z, angle, aux_radius, cos_B, sin_B;

    vertexB.reserve(slices * stacks * 3);
    normalB.reserve(slices * stacks * 3);

    for (int i = 0; i <= slices; i++) {

        angle = slicesAngInc * i;

        cos_B = cos(angle);
        sin_B = sin(angle);

        radius_x = cos_B * radius;
        radius_z = sin_B * radius;

        for (int j = 0; j <= stacks; j++) {
            aux_radius = cos(stacksAngInc * j) * inner_radius; //anglo interno rebatido para o o plano xoz
            vertexB.push_back(radius_x + aux_radius * cos_B);
            vertexB.push_back(sin(stacksAngInc * j) * inner_radius);
            vertexB.push_back(radius_z + aux_radius * sin_B);

            normalB.push_back(cos(stacksAngInc * j) * cos_B);
            normalB.push_back(sin(stacksAngInc * j));
            normalB.push_back(cos(stacksAngInc * j) * sin_B);

            textB.push_back(float(i)/float(slices));
            textB.push_back(float(j)/float(stacks));
        }
    }

    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
        	indexB.push_back(i*(stacks+1)+j);
        	indexB.push_back(i*(stacks+1)+(j + 1));
        	indexB.push_back((i+1)*(stacks+1)+j);

        	indexB.push_back(i*(stacks+1)+(j + 1));
        	indexB.push_back((i+1)*(stacks+1)+(j+1));
        	indexB.push_back((i+1)*(stacks+1)+j);
        }
    }
}