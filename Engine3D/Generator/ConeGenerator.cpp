#include "ConeGenerator.h"
using namespace std;

void cone(float base, float height, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB) {
    unsigned int verticesCount;
    float angInc = 2 * M_PI / slices, //angle increment value
    heightInc = height / stacks,
    radiusDec = base / stacks;
    float ang, h, r, normal_y,length;
    float cos_y;

    h = 0;
    r = base;
    ang = 0;

    for (int j = 0; j < slices; j++) {
        ang = angInc * j;
        vertexB.push_back(r * sin(ang));
        vertexB.push_back(h);
        vertexB.push_back(r * cos(ang));

        normalB.push_back(0.0f);
        normalB.push_back(-1.0f);
        normalB.push_back(0.0f);

        textB.push_back(float(j)/float(slices));
        textB.push_back(0.0f);
    }

    cos_y = M_PI_2 - atan(height/base);
    normal_y = sin(cos_y);
    cos_y = cos(cos_y);
    normal_y = normal_y;

    //Fills vertexB buffer with the vertices coordinates
    for (int i = 0; i < stacks; i++) {
        h = heightInc * i;
        r = base - radiusDec * i;
        ang = 0;

        for (int j = 0; j < slices; j++) {
            ang = angInc * j;
            vertexB.push_back(r * sin(ang));
            vertexB.push_back(h);
            vertexB.push_back(r * cos(ang));

            normalB.push_back(sin(ang) * cos_y);
            normalB.push_back(normal_y);
            normalB.push_back(cos(ang) * cos_y);

            textB.push_back(float((j + 1))/float(stacks));
            textB.push_back(float(i)/float(slices));
        }
    }

    //Coordinates of base center
    vertexB.push_back(0);
    vertexB.push_back(0);
    vertexB.push_back(0);

    normalB.push_back(0.0f);
    normalB.push_back(-1.0f);
    normalB.push_back(0.0f);

    textB.push_back(0.5f);
    textB.push_back(0.0f);

    ang = 0;
    for(int i = 0; i < slices;i++){
        ang = angInc * i;

        //Coordinates of highest point
        vertexB.push_back(0);
        vertexB.push_back(height);
        vertexB.push_back(0);

        normalB.push_back(sin(ang) * cos_y);
        normalB.push_back(normal_y);
        normalB.push_back(cos(ang) * cos_y);

        textB.push_back(float(i)/float(slices));
        textB.push_back(1.0f);
    }

    verticesCount = vertexB.size() / 3;

    for (int i = 1; i <= stacks; i++) {

        for (int j = 1; j < slices; j++) {

            if (i) {
                indexB.push_back(i * slices + (j - 1));
                indexB.push_back((i - 1) * slices + j);
                indexB.push_back(i * slices + j);

                indexB.push_back((i - 1) * slices + (j - 1));
                indexB.push_back((i - 1) * slices + j);
                indexB.push_back(i * slices + (j - 1));
            }
        }

        //dar a volta aos indices
        if (i) {
            indexB.push_back((i - 1) * slices);
            indexB.push_back(i * slices);
            indexB.push_back(i * slices + (slices - 1));

            indexB.push_back((i - 1) * slices + (slices - 1));
            indexB.push_back((i - 1) * slices);
            indexB.push_back(i * slices + (slices - 1));
        }
    }

    //faz pontas
    for (int j = 1; j < slices; j++) {
        indexB.push_back(verticesCount - 1 - slices);
        indexB.push_back(j);
        indexB.push_back(j - 1);

        indexB.push_back((stacks - 1) * slices + (j - 1));
        indexB.push_back((stacks - 1) * slices + j);
        indexB.push_back(verticesCount - slices + j);
    }

    //da a volta aos indices
    indexB.push_back(verticesCount - 1 - slices);
    indexB.push_back(0);
    indexB.push_back(slices - 1);

    indexB.push_back(verticesCount - slices);
    indexB.push_back((stacks - 1) * slices + (slices - 1));
    indexB.push_back((stacks - 1) * slices);
}