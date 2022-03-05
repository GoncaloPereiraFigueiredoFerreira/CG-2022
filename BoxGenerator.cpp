#include "AuxiliarMethods.h"

void generateBox(float lenght, int div) {
    float cx = 0, cz = 0;
    float incr = lenght / div; //side increment value
    float cy_b = 0; //bottom y
    float cy_t = incr * div; //top y
    Point** mat_b = mallocMatrix(div + 1, div + 1);
    Point** mat_t = mallocMatrix(div + 1, div + 1);

    for (int i = 0; i <= div;i++) {
        for (int j = 0;j <= div;j++) {
            Point p_b = Point(cx, cy_b, cz);
            Point p_t = Point(cx, cy_t, cz);
            mat_b[i][j] = p_b;
            mat_t[i][j] = p_t;
            cx += incr;
        }
        cz += incr;
        cx = 0;
    }

    freeMatrix(mat_b, div + 1);
    freeMatrix(mat_t, div + 1);
}