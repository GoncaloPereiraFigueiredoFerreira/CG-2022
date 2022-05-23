#include "BoxGenerator.h"
using namespace std;

void insertCoordinates(vector<float>& vertexB, int index, float x, float y, float z){
    vertexB[index] = x;
    vertexB[index + 1] = y;
    vertexB[index + 2] = z;
}

void box(float length, int divisions, vector<float>& vertexB, vector<unsigned int>& indexB,vector<float>& normalB, vector<float>& textB) {
    int verticesPerFace = (divisions + 1) * (divisions + 1);
    vertexB.resize(verticesPerFace * 6 * 3);
    textB.resize(verticesPerFace * 6 * 2);
    normalB.resize(verticesPerFace * 6 * 3);

    float half_lengh = length / 2;
    float cx, cxi = cx = -half_lengh, cz = -half_lengh;
    float incr = length / divisions; //side increment value
    float cy_b = -half_lengh; //bottom y
    float cy_t = incr * divisions - half_lengh; //top y

    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            insertCoordinates(vertexB, (i * (divisions + 1) + j) * 3, cx, cy_b, cz); //under
            insertCoordinates(normalB, (i * (divisions + 1) + j) * 3, 0.0f, -1.0f, 0.0f); //under
            textB[(i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//under

            insertCoordinates(vertexB, (verticesPerFace + i * (divisions + 1) + j) * 3, cy_t, cx , cz); //front
            insertCoordinates(normalB, (verticesPerFace + i * (divisions + 1) + j) * 3, 1.0f, 0.0f , 0.0f); //front
            textB[(verticesPerFace + i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(verticesPerFace + i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//front

            insertCoordinates(vertexB, (2 * verticesPerFace + i * (divisions + 1) + j) * 3, cx, cz, cy_t); //left
            insertCoordinates(normalB, (2 * verticesPerFace + i * (divisions + 1) + j) * 3, 0.0f, 0.0f, 1.0f); //left
            textB[(2 * verticesPerFace + i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(2 * verticesPerFace + i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//left

            insertCoordinates(vertexB, (3 * verticesPerFace + i * (divisions + 1) + j) * 3, cx, cy_t, cz); //top
            insertCoordinates(normalB, (3 * verticesPerFace + i * (divisions + 1) + j) * 3, 0.0f, 1.0f, 0.0f); //top
            textB[(3 * verticesPerFace + i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(3 * verticesPerFace + i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//top

            insertCoordinates(vertexB, (4 * verticesPerFace + i * (divisions + 1) + j) * 3, cy_b, cx, cz); //back
            insertCoordinates(normalB, (4 * verticesPerFace + i * (divisions + 1) + j) * 3, -1.0f, 0.0f, 0.0f); //back
            textB[(4 * verticesPerFace + i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(4 * verticesPerFace + i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//back

            insertCoordinates(vertexB, (5 * verticesPerFace + i * (divisions + 1) + j) * 3, cx, cz, cy_b); //right
            insertCoordinates(normalB, (5 * verticesPerFace + i * (divisions + 1) + j) * 3, 0.0f, 0.0f, -1.0f); //right
            textB[(5 * verticesPerFace + i * (divisions + 1) + j) * 2] = float(i) * incr;textB[(5 * verticesPerFace + i * (divisions + 1) + j) * 2 + 1] = float(j) * incr;//right

            cx += incr;
        }
        cz += incr;
        cx = cxi;
    }

    for (int i = 0; i <= divisions;i++) {

		for (int j = 0;j <= divisions;j++) {

			if (i && j) {
				///under & back & left
				for(int offset = 0; offset <= 2 * verticesPerFace; offset += verticesPerFace) {
                    indexB.push_back(offset + (i - 1) * (divisions + 1) + (j - 1));
                    indexB.push_back(offset + i * (divisions + 1) + j);
                    indexB.push_back(offset + i * (divisions + 1) + (j - 1));

                    indexB.push_back(offset + (i - 1) * (divisions + 1) + (j - 1));
                    indexB.push_back(offset + (i - 1) * (divisions + 1) + j);
                    indexB.push_back(offset + i * (divisions + 1) + j);
                }

				///top & front & right
				for(int offset = 3 * verticesPerFace; offset <= 5 * verticesPerFace; offset += verticesPerFace) {
                    indexB.push_back(offset + (i - 1) * (divisions + 1) + (j - 1));
                    indexB.push_back(offset + i * (divisions + 1) + (j - 1));
                    indexB.push_back(offset + i * (divisions + 1) + j);

                    indexB.push_back(offset + (i - 1) * (divisions + 1) + (j - 1));
                    indexB.push_back(offset + i * (divisions + 1) + j);
                    indexB.push_back(offset + (i - 1) * (divisions + 1) + j);
                }
			}
		}
	}
}