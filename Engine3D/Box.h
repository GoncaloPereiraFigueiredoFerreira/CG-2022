#pragma once
#include "Point.h"


	class Box
	{
	public:
		float length;
		int divisions;
		Point** mat_b;
		Point** mat_t;
		Box(float length, int divisions, Point** mat_b, Point** mat_t) {
			this->length = length;
			this->divisions = divisions;
			this->mat_b = mat_b;
			this->mat_t = mat_t;
		}
		Box() {}
	};
