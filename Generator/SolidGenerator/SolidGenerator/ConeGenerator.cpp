#include "AuxiliarMethods.h"

void generateCone(float base, float height, int slices, int stacks) {
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    Point** mat = mallocMatrix(stacks, slices);
    float angInc = 2 * M_PI / slices, //angle increment value
        heightInc = height / stacks,
        radiusDec = base / stacks;
    float ang, h, r;

    for (int i = 0; i < stacks; i++) {
        h = cz + heightInc * i;
        r = base - radiusDec * i;

        ang = 0;
        Point p = Point(cx + r * cos(ang), cy + r * sin(ang), h);
        mat[i][0] = p;

        for (int j = 1; j < slices; j++) {
            ang = angInc * j;
            Point p = Point(cx + r * cos(ang), cy + r * sin(ang), h);
            mat[i][j] = p;
        }
    }

    freeMatrix(mat, stacks);
}