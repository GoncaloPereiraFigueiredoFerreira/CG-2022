#include "Box.h"

void Box::draw() {
	glBegin(GL_TRIANGLES);

	for (int i = 0; i <= divisions;i++) {

		for (int j = 0;j <= divisions;j++) {

			if (i && j) {
				///under
				glVertex3f(mat_u[i - 1][j].cx, mat_u[i - 1][j].cy, mat_u[i - 1][j].cz);
				glVertex3f(mat_u[i][j].cx, mat_u[i][j].cy, mat_u[i][j].cz);
				glVertex3f(mat_u[i][j - 1].cx, mat_u[i][j - 1].cy, mat_u[i][j - 1].cz);

				glVertex3f(mat_u[i - 1][j].cx, mat_u[i - 1][j].cy, mat_u[i - 1][j].cz);
				glVertex3f(mat_u[i][j - 1].cx, mat_u[i][j - 1].cy, mat_u[i][j - 1].cz);
				glVertex3f(mat_u[i - 1][j - 1].cx, mat_u[i - 1][j - 1].cy, mat_u[i - 1][j - 1].cz);
				///

				///Right
				glVertex3f(mat_r[i - 1][j].cx, mat_r[i - 1][j].cy, mat_r[i - 1][j].cz);
				glVertex3f(mat_r[i][j - 1].cx, mat_r[i][j - 1].cy, mat_r[i][j - 1].cz);
				glVertex3f(mat_r[i][j].cx, mat_r[i][j].cy, mat_r[i][j].cz);

				glVertex3f(mat_r[i - 1][j].cx, mat_r[i - 1][j].cy, mat_r[i - 1][j].cz);
				glVertex3f(mat_r[i - 1][j - 1].cx, mat_r[i - 1][j - 1].cy, mat_r[i - 1][j - 1].cz);
				glVertex3f(mat_r[i][j - 1].cx, mat_r[i][j - 1].cy, mat_r[i][j - 1].cz);
				///

				///front
				glVertex3f(mat_f[i - 1][j].cx, mat_f[i - 1][j].cy, mat_f[i - 1][j].cz);
				glVertex3f(mat_f[i][j - 1].cx, mat_f[i][j - 1].cy, mat_f[i][j - 1].cz);
				glVertex3f(mat_f[i][j].cx, mat_f[i][j].cy, mat_f[i][j].cz);

				glVertex3f(mat_f[i - 1][j].cx, mat_f[i - 1][j].cy, mat_f[i - 1][j].cz);
				glVertex3f(mat_f[i - 1][j - 1].cx, mat_f[i - 1][j - 1].cy, mat_f[i - 1][j - 1].cz);
				glVertex3f(mat_f[i][j - 1].cx, mat_f[i][j - 1].cy, mat_f[i][j - 1].cz);
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
				glVertex3f(mat_l[i - 1][j].cx, mat_l[i - 1][j].cy, mat_l[i - 1][j].cz);
				glVertex3f(mat_l[i][j].cx, mat_l[i][j].cy, mat_l[i][j].cz);
				glVertex3f(mat_l[i][j - 1].cx, mat_l[i][j - 1].cy, mat_l[i][j - 1].cz);

				glVertex3f(mat_l[i - 1][j].cx, mat_l[i - 1][j].cy, mat_l[i - 1][j].cz);
				glVertex3f(mat_l[i][j - 1].cx, mat_l[i][j - 1].cy, mat_l[i][j - 1].cz);
				glVertex3f(mat_l[i - 1][j - 1].cx, mat_l[i - 1][j - 1].cy, mat_l[i - 1][j - 1].cz);
				///

				///Back
				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j].cx, mat_b[i][j].cy, mat_b[i][j].cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);
				glVertex3f(mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cy, mat_b[i - 1][j - 1].cz);
				///
			}
		}
	}

	glEnd();
}