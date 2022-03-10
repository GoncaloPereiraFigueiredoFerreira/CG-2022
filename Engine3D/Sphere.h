#pragma once
#include "Point.h"

class Sphere
{
public:
	float radius;
	int stacks;
	int slices;
	Point lowestP;
	Point highestP;
	Point** mat; //matriz com os pontos que dividem as diversas stacks
	Sphere(float radius, int slices, int stacks, Point lowestP, Point highestP, Point** mat) {
		this->radius = radius;
		this->stacks = stacks;
		this->slices = slices;
		this->lowestP = lowestP;
		this->highestP = highestP;
		this->mat = mat;
	}
	Sphere() {}
};