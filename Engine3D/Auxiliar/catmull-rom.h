#ifndef CATMULL_H
#define CATMULL_H

#include <math.h>
#include <vector>
#include "Point.h"

void CatmullRomPoint(float gt, std::vector<Point> p,float *pos, float *deriv);

#endif