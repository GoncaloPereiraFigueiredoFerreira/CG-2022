#include "Sphere.h"

void Sphere::draw() {
	glBegin(GL_TRIANGLES);

	//Draws the upper and lower stacks

	for (int j = 0; j < slices - 1; j++) {
		glVertex3f(mat[0][j].cx, mat[0][j].cy, mat[0][j].cz);
		glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
		glVertex3f(mat[0][j + 1].cx, mat[0][j + 1].cy, mat[0][j + 1].cz);

		glVertex3f(highestP.cx, highestP.cy, highestP.cz);
		glVertex3f(mat[stacks - 2][j].cx, mat[stacks - 2][j].cy, mat[stacks - 2][j].cz);
		glVertex3f(mat[stacks - 2][j + 1].cx, mat[stacks - 2][j + 1].cy, mat[stacks - 2][j + 1].cz);
	}

	//Desenha triangulos correspondentes � slice que completa uma volta
	glVertex3f(mat[0][slices - 1].cx, mat[0][slices - 1].cy, mat[0][slices - 1].cz);
	glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
	glVertex3f(mat[0][0].cx, mat[0][0].cy, mat[0][0].cz);

	glVertex3f(highestP.cx, highestP.cy, highestP.cz);
	glVertex3f(mat[stacks - 2][slices - 1].cx, mat[stacks - 2][slices - 1].cy, mat[stacks - 2][slices - 1].cz);
	glVertex3f(mat[stacks - 2][0].cx, mat[stacks - 2][0].cy, mat[stacks - 2][0].cz);


	//Desenho das restantes stacks
	Point* p1, * p2, * p3;


	for (int i = 0; i < stacks - 2; i++) {

		for (int j = 0; j < slices - 1; j++) {

			/*    Desenha os triangulos com forma |\    */

			p1 = &mat[i][j]; p2 = &mat[i][j + 1]; p3 = &mat[i + 1][j];

			glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
			glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
			glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);

			/*     Desenha os triangulos com forma \|    */

			p1 = &mat[i + 1][j + 1];

			glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
			glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
			glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
		}


		//Desenha triangulos correspondentes � slice que completa uma volta

		/*    Desenha os triangulos com forma |\    */

		p1 = &mat[i][slices - 1]; p2 = &mat[i][0]; p3 = &mat[i + 1][slices - 1];

		glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
		glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
		glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);

		/*     Desenha os triangulos com forma \|    */

		p1 = &mat[i + 1][0];

		glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
		glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
		glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
	}

	glEnd();
}