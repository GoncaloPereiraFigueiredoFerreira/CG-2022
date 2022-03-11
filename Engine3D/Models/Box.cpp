#include "Box.h"

void Box::draw() {
	glBegin(GL_TRIANGLES);

	for (int i = 0; i <= this->divisions;i++) {

		for (int j = 0;j <= this->divisions;j++) {

			if (i && j) {
				///bottom
				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j].cx, mat_b[i][j].cy, mat_b[i][j].cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);
				glVertex3f(mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cy, mat_b[i - 1][j - 1].cz);
				///

				///Right
				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cz, mat_b[i - 1][j].cy);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cz, mat_b[i][j - 1].cy);
				glVertex3f(mat_b[i][j].cx, mat_b[i][j].cz, mat_b[i][j].cy);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cz, mat_b[i - 1][j].cy);
				glVertex3f(mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cz, mat_b[i - 1][j - 1].cy);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cz, mat_b[i][j - 1].cy);
				///

				///front
				glVertex3f(mat_b[i - 1][j].cy, mat_b[i - 1][j].cx, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j - 1].cy, mat_b[i][j - 1].cx, mat_b[i][j - 1].cz);
				glVertex3f(mat_b[i][j].cy, mat_b[i][j].cx, mat_b[i][j].cz);

				glVertex3f(mat_b[i - 1][j].cy, mat_b[i - 1][j].cx, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i - 1][j - 1].cy, mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cz);
				glVertex3f(mat_b[i][j - 1].cy, mat_b[i][j - 1].cx, mat_b[i][j - 1].cz);
				///


				///Top
				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cy, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cy, mat_t[i][j - 1].cz);
				glVertex3f(mat_t[i][j].cx, mat_t[i][j].cy, mat_t[i][j].cz);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cy, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cy, mat_t[i - 1][j - 1].cz);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cy, mat_t[i][j - 1].cz);
				///

				///Left
				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cz, mat_t[i - 1][j].cy);
				glVertex3f(mat_t[i][j].cx, mat_t[i][j].cz, mat_t[i][j].cy);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cz, mat_t[i][j - 1].cy);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cz, mat_t[i - 1][j].cy);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cz, mat_t[i][j - 1].cy);
				glVertex3f(mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cz, mat_t[i - 1][j - 1].cy);
				///

				///Back
				glVertex3f(mat_t[i - 1][j].cy, mat_t[i - 1][j].cx, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i][j].cy, mat_t[i][j].cx, mat_t[i][j].cz);
				glVertex3f(mat_t[i][j - 1].cy, mat_t[i][j - 1].cx, mat_t[i][j - 1].cz);

				glVertex3f(mat_t[i - 1][j].cy, mat_t[i - 1][j].cx, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i][j - 1].cy, mat_t[i][j - 1].cx, mat_t[i][j - 1].cz);
				glVertex3f(mat_t[i - 1][j - 1].cy, mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cz);
				///
			}
		}
	}

	glEnd();
}