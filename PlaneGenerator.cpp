#include "AuxiliarMethods.h"

void generatePlane(float lenght, int div) {
    float cx = 0, cy = 0, cz = 0;
    float incr = lenght / div; //side increment value
    Point** mat = mallocMatrix(div + 1, div + 1);

    for (int i = 0; i <= div;i++) {
        for (int j = 0;j <= div;j++) {
            Point p = Point(cx, cy, cz);
            mat[i][j] = p;
            cx += incr;
        }
        cz += incr;
        cx = 0;
    }

    freeMatrix(mat, div + 1);
}