#ifndef MODELS_POINT_H
#define MODELS_POINT_H

class Point
{
public:
    float cx;
    float cy;
    float cz;
    Point() {
        cx = 0;
        cy = 0;
        cz = 0;
    }
    Point(float x, float y, float z) {
        cx = x;
        cy = y;
        cz = z;
    }
};

#endif
