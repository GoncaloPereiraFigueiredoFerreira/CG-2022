#ifndef MODELS_TORUS_H
#define MODELS_TORUS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Model.h"
#include "Point.h"
#include <vector>
#include <GL/glut.h>

class Torus : public Model
{
public:
	float inner_radius;
	float outer_radius;
	int slices;
	int stacks;
	std::vector<std::vector<Point>> mat;

	Torus(float inner_radius, float outer_radius,int slices,int stacks, std::vector<std::vector<Point>> mat) {
		this->inner_radius = inner_radius;
		this->outer_radius = outer_radius;
		this->mat = mat;
		this->slices = slices;
		this->stacks = stacks;
	}

	Torus(float inner_radius,float outer_radius,int slices,int stacks) {
		this->inner_radius = inner_radius;
		this->outer_radius = outer_radius;
		this->slices = slices;
		this->stacks = stacks;

		float radius = inner_radius + outer_radius;
		float slicesAngInc = 2 * M_PI / slices;
		float stacksAngInc = 2 * M_PI / stacks;
		float radius_x,radius_z,angle,aux_radius;
		mat.reserve(slices);

		for(int i = 0; i < stacks;i++){
			std::vector<Point> l;
			l.reserve(stacks);
			mat.push_back(l);
		}

		for(int i = 0;i < slices;i++){

			angle = slicesAngInc*i;

			radius_x = cos(angle) * radius;
			radius_z = sin(angle) * radius;

			for(int j = 0;j < stacks;j++){
				aux_radius = cos(stacksAngInc * j) * inner_radius; //anglo interno rebatido para o o plano xoz
				mat[j].push_back(Point(radius_x + aux_radius * cos(angle),sin(stacksAngInc*j)*inner_radius,radius_z + aux_radius*sin(angle)));
			}
		}
	}

    void draw() override;
};

#endif