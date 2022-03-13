#include "Plane.h"

void Plane::draw() {
	glBegin(GL_TRIANGLES);

	for (int i = 0; i <= this->divisions; i++) {
		for (int j = 0;j <= this->divisions; j++) {
			if (i && j) {
				glVertex3f(mat[i - 1][j - 1].cx, mat[i - 1][j - 1].cy, mat[i - 1][j - 1].cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
				glVertex3f(mat[i][j].cx, mat[i][j].cy, mat[i][j].cz);

				glVertex3f(mat[i - 1][j - 1].cx, mat[i - 1][j - 1].cy, mat[i - 1][j - 1].cz);
				glVertex3f(mat[i][j].cx, mat[i][j].cy, mat[i][j].cz);
				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
			}
		}
	}

	glEnd();
}