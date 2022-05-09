#include "PlaneGenerator.h"
using namespace std;

void plane(float length, int divisions, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB) {
    float cxi = -(length / 2), cx = cxi, cy = 0, cz = -(length / 2);
    float incr = length / divisions; //side increment value

    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            vertexB.push_back(cx);
            vertexB.push_back(cy);
            vertexB.push_back(cz);
            cx += incr;

            normalB.push_back(0.0f);
            normalB.push_back(1.0f);
            normalB.push_back(0.0f);
        }
        cz += incr;
        cx = cxi;
    }

    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            if (i && j) {
                indexB.push_back((i - 1) * (divisions + 1) + (j - 1));
                indexB.push_back(i * (divisions + 1) + (j - 1));
                indexB.push_back(i * (divisions + 1) + j);

                indexB.push_back((i - 1) * (divisions + 1) + (j - 1));
                indexB.push_back(i * (divisions + 1) + j);
                indexB.push_back((i - 1) * (divisions + 1) + j);
            }
        }
    }
}