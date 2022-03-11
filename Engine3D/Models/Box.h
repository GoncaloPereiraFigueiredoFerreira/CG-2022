#ifndef MODELS_BOX_H
#define MODELS_BOX_H

#include <vector>
#include "../Auxiliar/AuxiliarMethods.h"
#include "Point.h"
#include "Model.h"
#include <GL/glut.h>

	class Box : public Model
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

        void draw() override;
    };

#endif