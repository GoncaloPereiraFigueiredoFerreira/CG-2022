#ifndef CATMULL_H
#define CATMULL_H

#include <math.h>
#include <vector>
#include "Point.h"

void CatmullRomPoint(float gt, std::vector<Point> p,float *pos, float *deriv);
void getMatrizRotateCatmull(float *div,float *y,float *m);
void getNormal(float *m,float *v,float *res);
void normalize(float *a);
void buildRotMatrix(float *x, float *y, float *z, float *m);
void multMatrixVector(float *m, float *v, float *res);

#endif