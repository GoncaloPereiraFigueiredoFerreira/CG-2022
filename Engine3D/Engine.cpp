#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "AuxiliarMethods.h"
#include "Generator.h"
#include "xmlReader.hpp"

using namespace std;

float angle_y = 0.0f;
float angle_x = 0.0f;
float angle_z = 0.0f;
float scale = 1.0f;
float xx = 0.0f, zz = 0.0f;
xmlInfo info;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawSphere(Sphere sphere) {
	int slices = sphere.slices, stacks = sphere.stacks;

	//Inicializacao da matriz onde serao guardados os pontos
	Point** mat = sphere.mat;

	//Definicao do modo que mostra apenas a linha que define o limite dos triangulos
	glBegin(GL_TRIANGLES);

	//Desenho da stack de baixo e de cima

	//Inicializacao do ponto cuja coordenada y é a mais baixa
	Point lowestP = sphere.lowestP;

	//Inicializacao do ponto cuja coordenada y é a mais alta
	Point highestP = sphere.highestP;

	for (int j = 0; j < slices - 1; j++) {
		glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
		glVertex3f(mat[0][j].cx, mat[0][j].cy, mat[0][j].cz);
		glVertex3f(mat[0][j + 1].cx, mat[0][j + 1].cy, mat[0][j + 1].cz);

		glVertex3f(mat[stacks - 2][j].cx, mat[stacks - 2][j].cy, mat[stacks - 2][j].cz);
		glVertex3f(highestP.cx, highestP.cy, highestP.cz);
		glVertex3f(mat[stacks - 2][j + 1].cx, mat[stacks - 2][j + 1].cy, mat[stacks - 2][j + 1].cz);
	}

	//Desenha triangulos correspondentes à slice que completa uma volta
	glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
	glVertex3f(mat[0][slices - 1].cx, mat[0][slices - 1].cy, mat[0][slices - 1].cz);
	glVertex3f(mat[0][0].cx, mat[0][0].cy, mat[0][0].cz);

	glVertex3f(mat[stacks - 2][slices - 1].cx, mat[stacks - 2][slices - 1].cy, mat[stacks - 2][slices - 1].cz);
	glVertex3f(highestP.cx, highestP.cy, highestP.cz);
	glVertex3f(mat[stacks - 2][0].cx, mat[stacks - 2][0].cy, mat[stacks - 2][0].cz);


	//Desenho das restantes stacks
	Point* p1, * p2, * p3;

	for (int i = stacks - 2; i > 0; i--) {

		for (int j = 0; j < slices - 1; j++) {

			/*    Desenha os triangulos com forma |\    */

			p1 = &mat[i - 1][j + 1]; p2 = &mat[i][j + 1]; p3 = &mat[i - 1][j];

			glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
			glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
			glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);

			/*     Desenha os triangulos com forma \|    */

			p1 = &mat[i][j];

			glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
			glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
			glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
		}


		//Desenha triangulos correspondentes à slice que completa uma volta

			/*    Desenha os triangulos com forma |\    */

		p1 = &mat[i - 1][0]; p2 = &mat[i][0]; p3 = &mat[i - 1][slices - 1];

		glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
		glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
		glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);

		/*     Desenha os triangulos com forma \|    */

		p1 = &mat[i][slices - 1];

		glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
		glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
		glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
	}

	glEnd();
}


void drawCone(Cone cone) {
	int slices      = cone.slices, 
		stacks	    = cone.stacks;
	Point** mat     = mallocMatrix(stacks, slices);
	float angInc    = 2 * M_PI / slices, //angle increment value
		  heightInc = cone.height / stacks,
		  radiusDec = cone.base / stacks;
	float ang, h, r;

	glColor3f(0.5f, 0.2f, 0.2f);

	for (int i = 0; i < stacks; i++) {
		h = heightInc * i;
		r = cone.base - radiusDec * i;

		glBegin(GL_TRIANGLES);
		ang = 0;
		Point p = Point(r * cos(ang), r * sin(ang), h);
		mat[i][0] = p;

		for (int j = 1; j < slices; j++) {
			ang = angInc * j;
			Point p = Point(r * cos(ang), r * sin(ang), h);
			mat[i][j] = p;

			if (i) {
				glColor3f(0.5f, 0.2f, 0.2f);
				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(p.cx, p.cy, p.cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);

				glColor3f(0.2f, 0.2f, 0.5f);

				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
				glVertex3f(mat[i - 1][j - 1].cx, mat[i - 1][j - 1].cy, mat[i - 1][j - 1].cz);
			}
		}

		//dar a volta aos indices
		if (i) {
			glColor3f(0.5f, 0.2f, 0.2f);
			glVertex3f(mat[i - 1][0].cx, mat[i - 1][0].cy, mat[i - 1][0].cz);
			glVertex3f(mat[i][0].cx, mat[i][0].cy, mat[i][0].cz);
			glVertex3f(mat[i][slices - 1].cx, mat[i][slices - 1].cy, mat[i][slices - 1].cz);

			glColor3f(0.2f, 0.2f, 0.5f);

			glVertex3f(mat[i - 1][0].cx, mat[i - 1][0].cy, mat[i - 1][0].cz);
			glVertex3f(mat[i][slices - 1].cx, mat[i][slices - 1].cy, mat[i][slices - 1].cz);
			glVertex3f(mat[i - 1][slices - 1].cx, mat[i - 1][slices - 1].cy, mat[i - 1][slices - 1].cz);
		}

		glEnd();
	}

	//faz pontas
	for (int i = 1; i < slices;i++) {
		glBegin(GL_TRIANGLES);

		glColor3f(0.2f, 0.5f, 0.2f);

		glVertex3f(mat[0][i].cx, mat[0][i].cy, mat[0][i].cz);
		glVertex3f(mat[0][i - 1].cx, mat[0][i - 1].cy, mat[0][i - 1].cz);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glVertex3f(mat[stacks - 1][i].cx, mat[stacks - 1][i].cy, mat[stacks - 1][i].cz);
		glVertex3f(0.0f, 0.0f, cone.height);
		glVertex3f(mat[stacks - 1][i - 1].cx, mat[stacks - 1][i - 1].cy, mat[stacks - 1][i - 1].cz);

	}

	//da a volta aos indices
	glVertex3f(mat[0][0].cx, mat[0][0].cy, mat[0][0].cz);
	glVertex3f(mat[0][slices - 1].cx, mat[0][slices - 1].cy, mat[0][slices - 1].cz);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(mat[stacks - 1][0].cx, mat[stacks - 1][0].cy, mat[stacks - 1][0].cz);
	glVertex3f(0.0f, 0.0f, cone.height);
	glVertex3f(mat[stacks - 1][slices - 1].cx, mat[stacks - 1][slices - 1].cy, mat[stacks - 1][slices - 1].cz);

	glEnd();
}

void drawPlane(Plane plane) {
	float cx = 0, cy = 0, cz = 0;
	float incr = plane.length / plane.divisions; //side increment value
	Point** mat = mallocMatrix(plane.divisions + 1, plane.divisions + 1);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i <= plane.divisions; i++) {
		for (int j = 0;j <= plane.divisions; j++) {
			Point p = Point(cx, cy, cz);
			mat[i][j] = p;
			cx += incr;
			if (i && j) {
				glColor3f(0.5f, 0.2f, 0.2f);

				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
				glVertex3f(p.cx, p.cy, p.cz);

				glColor3f(0.2f, 0.2f, 0.5f);

				glVertex3f(mat[i - 1][j].cx, mat[i - 1][j].cy, mat[i - 1][j].cz);
				glVertex3f(mat[i - 1][j - 1].cx, mat[i - 1][j - 1].cy, mat[i - 1][j - 1].cz);
				glVertex3f(mat[i][j - 1].cx, mat[i][j - 1].cy, mat[i][j - 1].cz);
			}
		}
		cz += incr;
		cx = 0;
	}

	glEnd();
}

void drawBox(Box box) {
	float cx = 0, cz = 0;
	float incr = box.length / box.divisions; //side increment value
	float cy_b = 0; //bottom y
	float cy_t = incr * box.divisions; //top y
	Point** mat_b = mallocMatrix(box.divisions + 1, box.divisions + 1);
	Point** mat_t = mallocMatrix(box.divisions + 1, box.divisions + 1);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i <= box.divisions;i++) {
		for (int j = 0;j <= box.divisions;j++) {
			Point p_b = Point(cx, cy_b, cz);
			Point p_t = Point(cx, cy_t, cz);
			mat_b[i][j] = p_b;
			mat_t[i][j] = p_t;
			cx += incr;
			if (i && j) {
				///bottom
				glColor3f(0.5f, 0.2f, 0.2f);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(p_b.cx, p_b.cy, p_b.cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);

				glColor3f(0.2f, 0.2f, 0.5f);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cy, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cy, mat_b[i][j - 1].cz);
				glVertex3f(mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cy, mat_b[i - 1][j - 1].cz);
				///

				///Right
				glColor3f(1.0f, 0.0f, 0.0f);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cz, mat_b[i - 1][j].cy);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cz, mat_b[i][j - 1].cy);
				glVertex3f(p_b.cx, p_b.cz, p_b.cy);

				glColor3f(0.0f, 1.0f, 0.0f);

				glVertex3f(mat_b[i - 1][j].cx, mat_b[i - 1][j].cz, mat_b[i - 1][j].cy);
				glVertex3f(mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cz, mat_b[i - 1][j - 1].cy);
				glVertex3f(mat_b[i][j - 1].cx, mat_b[i][j - 1].cz, mat_b[i][j - 1].cy);
				///

				///front
				glColor3f(0.2f, 0.2f, 1.0f);

				glVertex3f(mat_b[i - 1][j].cy, mat_b[i - 1][j].cx, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i][j - 1].cy, mat_b[i][j - 1].cx, mat_b[i][j - 1].cz);
				glVertex3f(p_b.cy, p_b.cx, p_b.cz);

				glColor3f(0.2f, 0.5f, 0.2f);

				glVertex3f(mat_b[i - 1][j].cy, mat_b[i - 1][j].cx, mat_b[i - 1][j].cz);
				glVertex3f(mat_b[i - 1][j - 1].cy, mat_b[i - 1][j - 1].cx, mat_b[i - 1][j - 1].cz);
				glVertex3f(mat_b[i][j - 1].cy, mat_b[i][j - 1].cx, mat_b[i][j - 1].cz);
				///


				///Top
				glColor3f(0.5f, 0.2f, 0.2f);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cy, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cy, mat_t[i][j - 1].cz);
				glVertex3f(p_t.cx, p_t.cy, p_t.cz);

				glColor3f(0.2f, 0.2f, 0.5f);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cy, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cy, mat_t[i - 1][j - 1].cz);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cy, mat_t[i][j - 1].cz);
				///

				///Left
				glColor3f(1.0f, 0.0f, 0.0f);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cz, mat_t[i - 1][j].cy);
				glVertex3f(p_t.cx, p_t.cz, p_t.cy);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cz, mat_t[i][j - 1].cy);

				glColor3f(0.0f, 1.0f, 0.0f);

				glVertex3f(mat_t[i - 1][j].cx, mat_t[i - 1][j].cz, mat_t[i - 1][j].cy);
				glVertex3f(mat_t[i][j - 1].cx, mat_t[i][j - 1].cz, mat_t[i][j - 1].cy);
				glVertex3f(mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cz, mat_t[i - 1][j - 1].cy);
				///

				///Back
				glColor3f(0.2f, 0.2f, 1.0f);

				glVertex3f(mat_t[i - 1][j].cy, mat_t[i - 1][j].cx, mat_t[i - 1][j].cz);
				glVertex3f(p_t.cy, p_t.cx, p_t.cz);
				glVertex3f(mat_t[i][j - 1].cy, mat_t[i][j - 1].cx, mat_t[i][j - 1].cz);

				glColor3f(0.2f, 0.5f, 0.2f);

				glVertex3f(mat_t[i - 1][j].cy, mat_t[i - 1][j].cx, mat_t[i - 1][j].cz);
				glVertex3f(mat_t[i][j - 1].cy, mat_t[i][j - 1].cx, mat_t[i][j - 1].cz);
				glVertex3f(mat_t[i - 1][j - 1].cy, mat_t[i - 1][j - 1].cx, mat_t[i - 1][j - 1].cz);
				///
			}
		}
		cz += incr;
		cx = 0;
	}

	glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	gluLookAt(5.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	/* EM COMENTÁRIO ENQUANTO NAO TIVER 100% FUNCIONAL
	gluLookAt(info.cameraInfo.xPos, info.cameraInfo.yPos, info.cameraInfo.zPos,
			  info.cameraInfo.xLook, info.cameraInfo.yLook, info.cameraInfo.zLook,
			  info.cameraInfo.xUp, info.cameraInfo.yUp, info.cameraInfo.xUp);

	gluPerspective(info.cameraInfo.fov, 
				   1.0, //aspect 
				   info.cameraInfo.near,
				   info.cameraInfo.far);
	*/

	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();


	// put the geometric transformations here

	glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
	glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
	glRotatef(angle_z, 0.0f, 0.0f, 1.0f);
	glTranslatef(xx, 0.0f, zz);
	glScalef(scale, scale, scale);


	// put drawing instructions here
	
	glColor3f(1, 1, 1);
		//drawPlane(3,3);
		//drawSphere(2.0f, 20, 20);
		//drawCone(1.5f, 3.0f, 20, 20);

	Box s;// = generateSphere(2.0f, 10, 10);
	//generateSphereFile("sphere.3d", s);

	std::ifstream fd;
	string line;
	fd.open("box.3d", ios::in);
	getline(fd, line);
	if (!line.compare("box"))
		s = *readBoxFromFile(fd);
	drawBox(s);
	fd.close();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

void defaultKeyFunc(unsigned char key, int x, int y) {
	if (key == 'a' || key == 'A') {
		angle_y = angle_y + 5;
		glutPostRedisplay();
	}
	else if (key == 'd' || key == 'D') {
		angle_y = angle_y - 5;
		glutPostRedisplay();
	}
	else if (key == 'w' || key == 'W') {
		angle_x = angle_x + 5;
		glutPostRedisplay();
	}
	else if (key == 's' || key == 'S') {
		angle_x = angle_x - 5;
		glutPostRedisplay();
	}
	else if (key == 'i' || key == 'I') {
		scale += 0.05f;
		glutPostRedisplay();
	}
	else if (key == 'o' || key == 'O') {
		if (scale > 0.05f) {
			scale -= 0.05f;
			glutPostRedisplay();
		}
	}
}

void specialKeyFunc(int key_code, int x, int y) {
	if (key_code == GLUT_KEY_UP) {
		zz -= 0.1f;
		glutPostRedisplay();
	}
	else if (key_code == GLUT_KEY_DOWN) {
		zz += 0.1f;
		glutPostRedisplay();
	}
	else if (key_code == GLUT_KEY_RIGHT) {
		xx += 0.1f;
		glutPostRedisplay();
	}
	else if (key_code == GLUT_KEY_LEFT) {
		xx -= 0.1f;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	info = readXML(argv[1]);

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(defaultKeyFunc);
	glutSpecialFunc(specialKeyFunc);


	//  OpenGL settings
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}