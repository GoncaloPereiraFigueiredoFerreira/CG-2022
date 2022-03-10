#pragma once
#include "Point.h"

	class Cone
	{
	public:
		float base;
		float height;
		int slices;
		int stacks;
		Point** mat;
		Cone(float base, float height, int slices, int stacks, Point** mat) {
			this->base = base;
			this->height = height;
			this->slices = slices;
			this->stacks = stacks;
			this->mat = mat;
		}
		Cone() {}
	};
