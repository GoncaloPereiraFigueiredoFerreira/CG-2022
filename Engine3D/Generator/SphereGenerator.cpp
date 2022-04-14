#include "SphereGenerator.h"

using namespace std;

void sphere(float radius, int slices, int stacks, vector<float>& vertexB, vector<unsigned int>& indexB) {
    //Inicializacao do vector onde serao guardados os pontos
    unsigned int verticeCount = (stacks - 1) * slices + 2;
    vertexB.resize(verticeCount * 3);

    //Preenchimento do vector
    int indexLowestP = ((int) verticeCount - 2) * 3;
    vertexB[indexLowestP] = 0;
    vertexB[indexLowestP + 1] = -radius;
    vertexB[indexLowestP + 2] = 0;

    int indexHighestP = ((int) verticeCount - 1) * 3;
    vertexB[indexHighestP] = 0;
    vertexB[indexHighestP + 1] = radius;
    vertexB[indexHighestP + 2] = 0;

    float stacksAngInc = M_PI / stacks;
    float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
    float ang, height, r;

    for (int i = 0; i < stacks - 1; i++) {
        height = radius * sin(-M_PI_2 + stacksAngInc * (float) (i + 1));
        r = radius * cos(-M_PI_2 + stacksAngInc * (float) (i + 1)); //raio da "circunferencia" atual

        //Preenche valores da stack com indice i
        for (int j = 0; j < slices; j++) {
            ang = slicesAngInc * j;
            int index = (i * slices + j) * 3;
            vertexB[index] = r * sin(ang);
            vertexB[index + 1] = height;
            vertexB[index + 2] = r * cos(ang);
        }
    }

    //Draws the upper and lower stacks
    for (int j = 0; j < slices - 1; j++) {
        indexB.push_back(j);
        indexB.push_back(verticeCount - 2);
        indexB.push_back(j + 1);

        indexB.push_back(verticeCount - 1);
        indexB.push_back((stacks - 2) * slices + j);
        indexB.push_back((stacks - 2) * slices + j + 1);
    }

    //Desenha triangulos correspondentes � slice que completa uma volta
    indexB.push_back(slices - 1);
    indexB.push_back(verticeCount - 2);
    indexB.push_back(0);

    indexB.push_back(verticeCount - 1);
    indexB.push_back((stacks - 2) * slices + (slices - 1));
    indexB.push_back((stacks - 2) * slices);


    //Desenho das restantes stacks

    for (int i = 0; i < stacks - 2; i++) {

        for (int j = 0; j < slices - 1; j++) {

            /*    Desenha os triangulos com forma |\    */
            indexB.push_back(i * slices + j);
            indexB.push_back(i * slices + j + 1);
            indexB.push_back((i + 1) * slices + j);

            /*     Desenha os triangulos com forma \|    */
            indexB.push_back(i * slices + j + 1);
            indexB.push_back((i + 1) * slices + j + 1);
            indexB.push_back((i + 1) * slices + j);

        }


        //Desenha triangulos correspondentes � slice que completa uma volta

        /*    Desenha os triangulos com forma |\    */
        indexB.push_back(i * slices + slices - 1);
        indexB.push_back(i * slices);
        indexB.push_back((i + 1) * slices + slices - 1);

        /*     Desenha os triangulos com forma \|    */
        indexB.push_back(i * slices);
        indexB.push_back((i + 1) * slices);
        indexB.push_back((i + 1) * slices + slices - 1);
    }
}