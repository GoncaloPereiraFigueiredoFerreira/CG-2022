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
#include <climits>
#include <unordered_map>
#include <stdexcept>
#include "../Auxiliar/AuxiliarMethods.h"
#include "../Generator/Generator.h"
#include "../XMLReader/xmlReader.hpp"
using namespace std;

int startX, startY, tracking = 0;
float alpha, beta, r;

xmlInfo info;
vector<Group> groups;
unordered_map<char*, Model*> modelDic;
vector<Model*> solids;
Model* m;

int generateDicAux(Group tmpGroup, unordered_map<char*, Model*>* mapa) {
	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		if ((*mapa).find(tmpGroup.modelList[i].sourceF) == (*mapa).end()) {  //Verificar se o elemento ja esta no mapa
			Model* m;

			FILE* file;

			/*first check if the file exists...*/
			file = fopen(tmpGroup.modelList[i].sourceF, "r");
			if (file == nullptr) {
				cout << "Error: File\"" << tmpGroup.modelList[i].sourceF << "\" not found\n";
				return 0;
			}
			else {
				std::ifstream fd;
				fd.open(tmpGroup.modelList[i].sourceF, ios::in);
				//if(fd.) cout << "Error: File\"" << tmpGroup.modelList[i].sourceF << "\" not found\n";
				string line;
				getline(fd, line);

				if (line == "sphere") { //read Sphere
					m = readSphereFromFile(fd);
				}
				else if (line == "plane") { //read plane
					m = readPlaneFromFile(fd);
				}
				else if (line == "box") { //read box
					m = readBoxFromFile(fd);
				}
				else if (line == "cone") { //read cone
					m = readConeFromFile(fd);
				}
				else if (line == "torus") { //read torus
					m = readTorusFromFile(fd);
				}
				fclose(file);
			}
			mapa->insert(pair<char*, Model*>(tmpGroup.modelList[i].sourceF, m));
		}
	}
	for (int i = 0; i < tmpGroup.groupChildren.size(); i++) {
		generateDicAux(tmpGroup.groupChildren[i], mapa);
	}
	return 1;
}

int generateDic(xmlInfo xmlinfo) {
	auto* mapa = new unordered_map<char*, Model*>;
	if (!generateDicAux(xmlinfo.groups, mapa)) {
		return 0;
	};
	modelDic = *mapa;
	return 1;
}

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
	gluPerspective(info.cameraInfo.fov,
		ratio, //aspect
		info.cameraInfo.near,
		info.cameraInfo.far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

//Faltam as transformacoes
void recursiveDraw(Group tmpGroup) {
	glPushMatrix();
	for (int i = 0; i < tmpGroup.transforms.size(); i++) {
		tmpGroup.transforms[i]->apply();
		//cout << "apply\n";
	}
	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		//cout << "gonna draw\n";
		modelDic[tmpGroup.modelList[i].sourceF]->draw();
		//cout << "drawn\n";
	}
	for (int i = 0; i < tmpGroup.groupChildren.size(); i++) {
		recursiveDraw(tmpGroup.groupChildren[i]);
	}
	glPopMatrix();
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(info.cameraInfo.xPos, info.cameraInfo.yPos, info.cameraInfo.zPos,
		info.cameraInfo.xLook, info.cameraInfo.yLook, info.cameraInfo.zLook,
		info.cameraInfo.xUp, info.cameraInfo.yUp, info.cameraInfo.xUp);


    //Desenho dos eixos
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

	glColor3f(1.0f, 1.0f, 1.0f);
	recursiveDraw(info.groups);

	// End of frame
	glutSwapBuffers();
}


// functions to process mouse events
void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	float deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}

	info.cameraInfo.xPos = info.cameraInfo.xLook + rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	info.cameraInfo.zPos = info.cameraInfo.zLook + rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	info.cameraInfo.yPos = info.cameraInfo.yLook + rAux * sin(betaAux * 3.14 / 180.0);

    glutPostRedisplay();
}

// function to process keyboard events

void defaultKeyFunc(unsigned char key, int x, int y) {
        float Dx = info.cameraInfo.xLook - info.cameraInfo.xPos,
          Dz = info.cameraInfo.zLook - info.cameraInfo.zPos;

    float normD = sqrt(pow(Dx,2) + pow(Dz, 2));

    //Transforming in unit vector
    Dx /= normD;
    Dz /= normD;

    //Movements of
	if (key == 'w' || key == 'W') {
		info.cameraInfo.xLook += Dx;
        info.cameraInfo.zLook += Dz;
        info.cameraInfo.xPos += Dx;
        info.cameraInfo.zPos += Dz;
		glutPostRedisplay();
	}
	else if (key == 's' || key == 'S') {
		info.cameraInfo.xLook -= Dx;
        info.cameraInfo.zLook -= Dz;
        info.cameraInfo.xPos -= Dx;
        info.cameraInfo.zPos -= Dz;
		glutPostRedisplay();
	}
	else if (key == 'd' || key == 'D') {
		info.cameraInfo.xLook -= Dz;
        info.cameraInfo.zLook += Dx;
        info.cameraInfo.xPos -= Dz;
        info.cameraInfo.zPos += Dx;
		glutPostRedisplay();
	}
	else if (key == 'a' || key == 'A') {
		info.cameraInfo.xLook += Dz;
        info.cameraInfo.zLook -= Dx;
        info.cameraInfo.xPos += Dz;
        info.cameraInfo.zPos -= Dx;
		glutPostRedisplay();
	}
}

void specialKeyFunc(int key_code, int x, int y) {
        if (key_code == GLUT_KEY_UP) {
		info.cameraInfo.yLook += 1;
        info.cameraInfo.yPos += 1;
		glutPostRedisplay();
	}
	else if (key_code == GLUT_KEY_DOWN) {
		info.cameraInfo.yLook -= 1;
        info.cameraInfo.yPos -= 1;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	if (argc == 2) {
		//Reads XML file
		info = readXML(argv[1]);
        r = sqrt(pow(info.cameraInfo.xPos, 2) + pow(info.cameraInfo.yPos, 2) + pow(info.cameraInfo.zPos, 2));
        alpha = atan(info.cameraInfo.xPos / info.cameraInfo.zPos) * 180 / M_PI;
        beta = asin(info.cameraInfo.yPos / r) * 180 / M_PI;
	}
	else {
		cout << "Invalid arguments" << endl;
		return -1;
	}

	if (!generateDic(info)) {
		return -1;
	};

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@G13");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Registration of the keyboard and mouse callbacks
	glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
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