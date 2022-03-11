#ifndef MODELS_SPHERE_H
#define MODELS_SPHERE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Model.h"
#include "Point.h"
#include <GL/glut.h>

class Sphere : public Model
{
public:
	float radius;
	int stacks;
	int slices;
	Point lowestP;
	Point highestP;
	std::vector<std::vector<Point>> mat; //matriz com os pontos que dividem as diversas stacks

	Sphere(float radius, int slices, int stacks, Point lowestP, Point highestP, std::vector<std::vector<Point>> mat) {
		this->radius = radius;
		this->stacks = stacks;
		this->slices = slices;
		this->lowestP = lowestP;
		this->highestP = highestP;
		this->mat = mat;
	}

	Sphere(float radius, int slices, int stacks) {
		this->radius = radius;
		this->slices = slices;
		this->stacks = stacks;

		//Inicializacao da matriz onde serao guardados os pontos
		float stacksAngInc = M_PI / stacks;
		float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
		float ang, height, r;
		mat.reserve(stacks - 1);

		for (int i = 0; i < stacks - 1; i++) {
			std::vector<Point> l; 
			l.reserve(slices);

			height = radius * sin(-M_PI_2 + stacksAngInc * (float)(i + 1));
			r = radius * cos(-M_PI_2 + stacksAngInc * (float)(i + 1)); //raio da "circunferencia" atual

			for (int j = 0; j < slices; j++) {
				ang = slicesAngInc * j;
				l.push_back(Point(r * cos(ang), height, r * sin(ang)));
			}

			mat.push_back(l);
		}

		//Ponto cuja coordenada y � a mais baixa
		lowestP = Point(0, -radius, 0);

		//Ponto cuja coordenada y � a mais alta
		highestP = Point(0, radius, 0);
	}

    void draw() override;
};

#endif