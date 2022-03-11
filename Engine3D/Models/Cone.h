#ifndef MODELS_CONE_H
#define MODELS_CONE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "../Auxiliar/AuxiliarMethods.h"
#include "Model.h"
#include "Point.h"
#include <GL/glut.h>

class Cone : public Model
{
public:
	float base;
	float height;
	int slices;
	int stacks;
	std::vector<std::vector<Point>> mat;

	Cone(float base, float height, int slices, int stacks, std::vector<std::vector<Point>> mat) {
		this->base = base;
		this->height = height;
		this->slices = slices;
		this->stacks = stacks;
		this->mat = mat;
	}

	Cone(float base, float height, int slices, int stacks) {
		this->base = base;
		this->height = height;
		this->slices = slices;
		this->stacks = stacks;
		this->mat.reserve(stacks);

		float angInc = 2 * M_PI / slices, //angle increment value
			heightInc = height / stacks,
			radiusDec = base / stacks;
		float ang, h, r;

		for (int i = 0; i < stacks; i++) {
			std::vector<Point> l;
			l.reserve(slices);
			h = heightInc * i;
			r = base - radiusDec * i;
			ang = 0;

			for (int j = 0; j < slices; j++) {
				ang = angInc * j;
				l.push_back(Point(r * cos(ang), r * sin(ang), h));
			}

			mat.push_back(l);
		}
	}

    void draw() override;
};

#endif