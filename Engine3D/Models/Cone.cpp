#include "Cone.h"

void Cone::draw() {
    glBegin(GL_TRIANGLES);

	for (int i = 0; i < stacks; i++) {

		for (int j = 1; j < slices; j++) {

			if (i) {
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(mat[i][j].cx, mat[i][j].cy, mat[i][j].cz);

				glVertex3f(mat[i - 1][j - 1].cx, mat[i - 1][j - 1].cy, mat[i - 1][j - 1].cz);
				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
			}
		}

		//dar a volta aos indices
		if (i) {
			glVertex3f(mat[i - 1][0].cx, mat[i - 1][0].cy, mat[i - 1][0].cz);
			glVertex3f(mat[i][0].cx, mat[i][0].cy, mat[i][0].cz);
			glVertex3f(mat[i][slices - 1].cx, mat[i][slices - 1].cy, mat[i][slices - 1].cz);

			glVertex3f(mat[i - 1][slices - 1].cx, mat[i - 1][slices - 1].cy, mat[i - 1][slices - 1].cz);
			glVertex3f(mat[i - 1][0].cx, mat[i - 1][0].cy, mat[i - 1][0].cz);
			glVertex3f(mat[i][slices - 1].cx, mat[i][slices - 1].cy, mat[i][slices - 1].cz);
		}

	}

	//faz pontas
	for (int i = 1; i < slices;i++) {
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(mat[0][i].cx, mat[0][i].cy, mat[0][i].cz);
		glVertex3f(mat[0][i - 1].cx, mat[0][i - 1].cy, mat[0][i - 1].cz);

		glVertex3f(mat[stacks - 1][i - 1].cx, mat[stacks - 1][i - 1].cy, mat[stacks - 1][i - 1].cz);
		glVertex3f(mat[stacks - 1][i].cx, mat[stacks - 1][i].cy, mat[stacks - 1][i].cz);
		glVertex3f(0.0f, this->height, 0.0f);
	}

	//da a volta aos indices
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(mat[0][0].cx, mat[0][0].cy, mat[0][0].cz);
	glVertex3f(mat[0][slices - 1].cx, mat[0][slices - 1].cy, mat[0][slices - 1].cz);

	glVertex3f(0.0f, this->height, 0.0f);
	glVertex3f(mat[stacks - 1][slices - 1].cx, mat[stacks - 1][slices - 1].cy, mat[stacks - 1][slices - 1].cz);
	glVertex3f(mat[stacks - 1][0].cx, mat[stacks - 1][0].cy, mat[stacks - 1][0].cz);

	glEnd();
}