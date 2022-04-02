#include "Torus.h"

void Torus::draw() {
	glBegin(GL_TRIANGLES);

	for(int i = 0; i < slices;i++){
		for(int j = 0; j < stacks;j++){
			glVertex3f(mat[j][i].cx,mat[j][i].cy,mat[j][i].cz);
			glVertex3f(mat[(j + 1)%stacks][i].cx,mat[(j + 1)%stacks][i].cy,mat[(j + 1)%stacks][i].cz);
			glVertex3f(mat[j][(i + 1)%slices].cx,mat[j][(i + 1)%slices].cy,mat[j][(i + 1)%slices].cz);

			glVertex3f(mat[(j + 1)%stacks][i].cx,mat[(j + 1)%stacks][i].cy,mat[(j + 1)%stacks][i].cz);
			glVertex3f(mat[(j + 1)%stacks][(i + 1)%slices].cx,mat[(j + 1)%stacks][(i + 1)%slices].cy,mat[(j + 1)%stacks][(i + 1)%slices].cz);
			glVertex3f(mat[j][(i + 1)%slices].cx,mat[j][(i + 1)%slices].cy,mat[j][(i + 1)%slices].cz);
		}

	}

	glEnd();
}