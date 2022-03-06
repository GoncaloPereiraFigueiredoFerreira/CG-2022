#pragma once
#include "Point.h"

class Plane
{
public:
	float length;
	int divisions;
	Point** mat;
	Plane(float length, int divisions,Point** mat) {
		this->length = length;
		this->divisions = divisions;
		this->mat = mat;
	}
	Plane() {}
};