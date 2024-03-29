#include "SphereGenerator.h"

using namespace std;

void sphere(float radius, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB, vector<float>& normalB, vector<float>& textB) {
    //Inicializacao do vector onde serao guardados os pontos
    unsigned int verticeCount = (stacks - 1) * (slices + 1) + 2;
    vertexB.resize(verticeCount * 3);
    normalB.resize(verticeCount * 3);
    textB.resize(verticeCount * 2);

    //Preenchimento do vector
    int indexLowestP = ((int) verticeCount - 2) * 3;
    vertexB[indexLowestP] = 0;
    vertexB[indexLowestP + 1] = -radius;
    vertexB[indexLowestP + 2] = 0;

    normalB[indexLowestP] = 0;
    normalB[indexLowestP + 1] = -1;
    normalB[indexLowestP + 2] = 0;

    indexLowestP = ((int) verticeCount - 2) * 2;
    textB[indexLowestP] = 0.5f;
    textB[indexLowestP + 1] = 0.0f;


    int indexHighestP = ((int) verticeCount - 1) * 3;
    vertexB[indexHighestP] = 0;
    vertexB[indexHighestP + 1] = radius;
    vertexB[indexHighestP + 2] = 0;

    normalB[indexHighestP] = 0;
    normalB[indexHighestP + 1] = 1;
    normalB[indexHighestP + 2] = 0;

    indexHighestP = ((int) verticeCount - 1) * 2;
    textB[indexHighestP] = 0.5f;
    textB[indexHighestP + 1] = 1.0f;

    float stacksAngInc = M_PI / stacks;
    float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
    float ang, height, r, length , normal_y;

    for (int i = 0; i < stacks - 1; i++) {
        height = radius * sin(-M_PI_2 + stacksAngInc * (float) (i + 1));
        r = radius * cos(-M_PI_2 + stacksAngInc * (float) (i + 1)); //raio da "circunferencia" atual
        length = sqrt(pow(sin(-M_PI_2 + stacksAngInc * (float) (i + 1)),2) + 1.0f);
        normal_y = sin(-M_PI_2 + stacksAngInc * (float) (i + 1));

        //Preenche valores da stack com indice i
        for (int j = 0; j <= slices; j++) {
            ang = slicesAngInc * j;
            int index = (i * (slices + 1) + j) * 3;
            vertexB[index] = r * sin(ang);
            vertexB[index + 1] = height;
            vertexB[index + 2] = r * cos(ang);

            normalB[index] = cos(-M_PI_2 + stacksAngInc * (float) (i + 1)) * sin(ang);
            normalB[index + 1] = normal_y;
            normalB[index + 2] = cos(-M_PI_2 + stacksAngInc * (float) (i + 1)) * cos(ang);

            index = (i * (slices + 1) + j) * 2;

            textB[index] = (float(j)/float(slices));
            textB[index + 1] = (float((i+1))/float(stacks));
        }
    }

    //Draws the upper and lower stacks
    for (int j = 0; j < slices; j++) {
        indexB.push_back(j);
        indexB.push_back(verticeCount - 2);
        indexB.push_back(j + 1);

        indexB.push_back(verticeCount - 1);
        indexB.push_back((stacks - 2) * (slices + 1) + j);
        indexB.push_back((stacks - 2) * (slices + 1) + j + 1);
    }


    //Desenha triangulos correspondentes � slice que completa uma volta
    indexB.push_back(slices);
    indexB.push_back(verticeCount - 2);
    indexB.push_back(0);

    indexB.push_back(verticeCount - 1);
    indexB.push_back((stacks - 2) * (slices + 1) + slices);
    indexB.push_back((stacks - 2) * (slices + 1));


    //Desenho das restantes stacks

    for (int i = 0; i < stacks - 2; i++) {

        for (int j = 0; j < slices ; j++) {

            /*    Desenha os triangulos com forma |\    */
            indexB.push_back(i * (slices + 1) + j);
            indexB.push_back(i * (slices + 1) + j + 1);
            indexB.push_back((i + 1) * (slices + 1) + j);

            /*     Desenha os triangulos com forma \|    */
            indexB.push_back(i * (slices + 1) + j + 1);
            indexB.push_back((i + 1) * (slices + 1) + j + 1);
            indexB.push_back((i + 1) * (slices + 1) + j);

        }

    }
}