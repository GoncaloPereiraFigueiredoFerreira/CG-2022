#ifndef MODELS_PLANE_H
#define MODELS_PLANE_H
#include "Model.h"
#include "Point.h"
#include <vector>
#include <GL/glut.h>

class Plane : public Model
{
public:
	float length;
	int divisions;
	std::vector<std::vector<Point>> mat;

	Plane(float length, int divisions, std::vector<std::vector<Point>> mat) {
		this->length = length;
		this->divisions = divisions;
		this->mat = mat;
	}

	Plane(float length, int div) {
		this->length = length;
		this->divisions = div;

		float cx = 0, cy = 0, cz = 0;
		float incr = length / div; //side increment value
		mat.reserve(div + 1);

		for (int i = 0; i <= div;i++) {
			std::vector<Point> l;
			l.reserve(div + 1);

			for (int j = 0; j <= div; j++) {
				l.push_back(Point(cx, cy, cz));
				cx += incr;
			}
			cz += incr;
			cx = 0;

			mat.push_back(l);
		}
	}

    void draw() override;
};

#endif