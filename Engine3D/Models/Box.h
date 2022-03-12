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
		std::vector<std::vector<Point>> mat_u;
		std::vector<std::vector<Point>> mat_t;
		std::vector<std::vector<Point>> mat_f;
		std::vector<std::vector<Point>> mat_b;
		std::vector<std::vector<Point>> mat_r;
		std::vector<std::vector<Point>> mat_l;

		Box(float length, int divisions, std::vector<std::vector<Point>> mat_u, std::vector<std::vector<Point>> mat_t, std::vector<std::vector<Point>> mat_f, std::vector<std::vector<Point>> mat_b, std::vector<std::vector<Point>> mat_r, std::vector<std::vector<Point>> mat_l) {
			this->length = length;
			this->divisions = divisions;
			this->mat_u = mat_u;
			this->mat_t = mat_t;
			this->mat_f = mat_f;
			this->mat_b = mat_b;
			this->mat_r = mat_r;
			this->mat_l = mat_l;
		}

		Box(float len, int divs) {
			length = len;
			divisions = divs;

			float half_lengh = length / 2;
			float cx, cxi = cx = -half_lengh, cz = -half_lengh;
			float incr = length / divisions; //side increment value
			float cy_b = -half_lengh; //bottom y
			float cy_t = incr * divisions - half_lengh; //top y

			for (int i = 0; i <= divisions; i++) {
				std::vector<Point> l_u, l_t, l_f, l_b, l_r, l_l;
				for (int j = 0;j <= divisions; j++) {
					Point p_u = Point(cx, cy_b, cz);
					Point p_t = Point(cx, cy_t, cz);
					Point p_f = Point(cy_b, cx, cz);
					Point p_b = Point(cy_t, cx , cz);
					Point p_r = Point(cx, cz, cy_b);
					Point p_l = Point(cx, cz, cy_t);
					l_u.push_back(p_u);
					l_t.push_back(p_t);
					l_f.push_back(p_f);
					l_b.push_back(p_b);
					l_r.push_back(p_r);
					l_l.push_back(p_l);
					cx += incr;
				}
				cz += incr;
				cx = cxi;
				mat_u.push_back(l_u);
				mat_t.push_back(l_t);
				mat_f.push_back(l_f);
				mat_b.push_back(l_b);
				mat_r.push_back(l_r);
				mat_l.push_back(l_l);
			}
		}

        void draw() override;
    };

#endif