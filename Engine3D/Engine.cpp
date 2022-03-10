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
#include "build/Drawings.h"

using namespace std;

float angle_y = 0.0f;
float angle_x = 0.0f;
float angle_z = 0.0f;
float scale = 1.0f;
float xx = 0.0f, zz = 0.0f;
xmlInfo info;
vector<Group> groups;

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

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	
	gluLookAt(5.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	
	/*
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
	/*
	xmlInfo info = readXML(argv[1]);
	std::ifstream fd;
	
	// For now (Phase 1) we will only look at the 3d files contained in the first group, avoiding nested group models
	vector<string> files3d;
	vector<Model> x = info.groups.modelList;
	for (int i=0; i < x.size(); i++){
		files3d.push_back(x[i].sourceF);
	}

	for (int i=0; i < files3d.size(); i++){
		fd.open(files3d[i], ios::in);
		string line; 
		getline(fd, line);
		
		if (line == "sphere"){
			//read Sphere
			Sphere au1 = *readSphereFromFile(fd);
			//adicionar a estrutura de dados
		}
		else if (line == "plane"){
			//read plane
			Plane au2 = *readPlaneFromFile(fd);
			//adicionar a estrutura de dados

		}
		else if (line == "box"){
			//read box
			Box au3 = *readBoxFromFile(fd);
			//adicionar a estrutura de dados
		}
		else if  (line == "cone"){
			//read cone
			Cone au4 = *readConeFromFile(fd);
			//adicionar a estrutura de dados
		}

		fd.close();
		
	}

	*/

	std::ifstream fd;
	string line;

	fd.open("cone.3d", ios::in);
	getline(fd, line);
	if (!line.compare("cone"))
		s = readConeFromFile(fd);
	fd.close();


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