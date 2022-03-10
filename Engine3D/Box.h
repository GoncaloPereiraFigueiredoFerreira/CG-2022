#pragma once
#include <vector>
#include "AuxiliarMethods.h"

	class Box : public Solid
	{
	public:
		float length;
		int divisions;
		std::vector<std::vector<Point>> mat_b;
		std::vector<std::vector<Point>> mat_t;

		Box(float length, int divisions, std::vector<std::vector<Point>> mat_b, std::vector<std::vector<Point>> mat_t) {
			this->length = length;
			this->divisions = divisions;
			this->mat_b = mat_b;
			this->mat_t = mat_t;
		}

		Box(float len, int divs) {
			length = len;
			divisions = divs;

			float cx = 0, cz = 0;
			float incr = length / divisions; //side increment value
			float cy_b = 0; //bottom y
			float cy_t = incr * divisions; //top y

			for (int i = 0; i <= divisions; i++) {
				std::vector<Point> l_b, l_t;
				for (int j = 0;j <= divisions; j++) {
					Point p_b = Point(cx, cy_b, cz);
					Point p_t = Point(cx, cy_t, cz);
					l_b.push_back(p_b);
					l_t.push_back(p_t);
					cx += incr;
				}
				cz += incr;
				cx = 0;
				mat_b.push_back(l_b);
				mat_t.push_back(l_t);
			}
		}
	};

	/*
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

		Box(float len, int divs) {
			length = len;
			divisions = divs;

			float cx = 0, cz = 0;
			float incr = length / divisions; //side increment value
			float cy_b = 0; //bottom y
			float cy_t = incr * divisions; //top y
			mat_b = mallocMatrix(divisions + 1, divisions + 1);
			mat_t = mallocMatrix(divisions + 1, divisions + 1);

			for (int i = 0; i <= divisions; i++) {
				for (int j = 0;j <= divisions; j++) {
					Point p_b = Point(cx, cy_b, cz);
					Point p_t = Point(cx, cy_t, cz);
					mat_b[i][j] = p_b;
					mat_t[i][j] = p_t;
					cx += incr;
				}
				cz += incr;
				cx = 0;
			}
		}
	};*/