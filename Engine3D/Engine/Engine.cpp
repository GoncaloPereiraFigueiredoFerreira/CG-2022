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

int cameraMode = 0;

int startX, startY, tracking = 0;
float alpha, beta1, r,sensibility = 0.01;

xmlInfo info;
vector<Group> groups;
unordered_map<char*, Model*> modelDic;
vector<Model*> solids;
Model* m;


void calculatePolarCoordinates(){
    float xAux, yAux, zAux;
    xAux = info.cameraInfo.xPos - info.cameraInfo.xLook;
    yAux = info.cameraInfo.yPos - info.cameraInfo.yLook;
    zAux = info.cameraInfo.zPos - info.cameraInfo.zLook;
    r = sqrt(pow(xAux, 2) + pow(yAux, 2) + pow(zAux, 2));
    alpha = atan(xAux / zAux);
    beta1 = asin(yAux / r);
}

/* ------- Generate Dictionary Of Models ------- */

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
			alpha += (startX - xx) * sensibility;
			beta1 += (startY - yy) * sensibility;

			if (beta1 > M_PI_2-0.1)
				beta1 = M_PI_2-0.1;
			else if (beta1 < -M_PI_2+0.1)
				beta1 = -M_PI_2+0.1;
		}
		else if (tracking == 2) {

			r -= startY - yy;
			if (r < 1)
				r = 1.0;
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

	deltaX = startX - xx;
	deltaY = startY - yy;

	if (tracking == 1) {

		alphaAux = alpha + deltaX * sensibility;
		betaAux = beta1 + deltaY * sensibility;

		if (betaAux > M_PI_2-0.1)
			betaAux = M_PI_2-0.1;
		else if (betaAux < -M_PI_2+0.1)
			betaAux = -M_PI_2+0.1;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta1;
		rAux = r - deltaY;
		if (rAux < 1)
			rAux = 1;
	}

    if(cameraMode == 0) {
        info.cameraInfo.xPos = info.cameraInfo.xLook + rAux * sin(alphaAux) * cos(betaAux);
        info.cameraInfo.zPos = info.cameraInfo.zLook + rAux * cos(alphaAux) * cos(betaAux);
        info.cameraInfo.yPos = info.cameraInfo.yLook + rAux * sin(betaAux);
    }
    else if (tracking == 1){ //cameraMode == 1
        info.cameraInfo.xLook = info.cameraInfo.xPos + rAux * sin(alphaAux + M_PI) * cos(-betaAux);
        info.cameraInfo.zLook = info.cameraInfo.zPos + rAux * cos(alphaAux + M_PI) * cos(-betaAux);
        info.cameraInfo.yLook = info.cameraInfo.yPos + rAux * sin(-betaAux);
    }

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
	}
	else if (key == 's' || key == 'S') {
		info.cameraInfo.xLook -= Dx;
        info.cameraInfo.zLook -= Dz;
        info.cameraInfo.xPos -= Dx;
        info.cameraInfo.zPos -= Dz;
	}
	else if (key == 'd' || key == 'D') {
		info.cameraInfo.xLook -= Dz;
        info.cameraInfo.zLook += Dx;
        info.cameraInfo.xPos -= Dz;
        info.cameraInfo.zPos += Dx;
	}
	else if (key == 'a' || key == 'A') {
		info.cameraInfo.xLook += Dz;
        info.cameraInfo.zLook -= Dx;
        info.cameraInfo.xPos += Dz;
        info.cameraInfo.zPos -= Dx;
	}
    else if (key == 'm' || key == 'M') {
		if(cameraMode == 0){
            cameraMode = 1;
            glutSetWindowTitle("CG@13 - FPS Mode");
        }
        else{
            cameraMode = 0;
            glutSetWindowTitle("CG@13");
        }
	}
	glutPostRedisplay();
}

void specialKeyFunc(int key_code, int x, int y) {
    if (key_code == GLUT_KEY_UP) {
		info.cameraInfo.yLook += 1;
       	info.cameraInfo.yPos += 1;
	}
	else if (key_code == GLUT_KEY_DOWN) {
		info.cameraInfo.yLook -= 1;
        info.cameraInfo.yPos -= 1;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
	if (argc == 2) {
		//Reads XML file
		info = readXML(argv[1]);
        calculatePolarCoordinates();
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