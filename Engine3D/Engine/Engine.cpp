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

float angle_y = 0.0f;
float angle_x = 0.0f;
float angle_z = 0.0f;
float scale = 1.0f;
float xx = 0.0f, zz = 0.0f;
xmlInfo info;
vector<Group> groups;
unordered_map<char*, Model*> modelDic;
vector<Model*> solids;
Model *m;

int generateDicAux(Group tmpGroup, unordered_map<char*, Model*>* mapa) {
	for (int i = 0; i < tmpGroup.modelList.size(); i++) { //TODO  iterar os groups
		if ((*mapa).find(tmpGroup.modelList[i].sourceF) == (*mapa).end()) {
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
	auto *mapa = new unordered_map<char*, Model*>;
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
	//push matrix
	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		modelDic[info.groups.modelList[i].sourceF]->draw();
	}
	for (int i = 0; i < tmpGroup.groupChildren.size(); i++) {
		recursiveDraw(tmpGroup.groupChildren[i]);
	}
	//pop matrix
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(info.cameraInfo.xPos, info.cameraInfo.yPos, info.cameraInfo.zPos,
		info.cameraInfo.xLook, info.cameraInfo.yLook, info.cameraInfo.zLook,
		info.cameraInfo.xUp, info.cameraInfo.yUp, info.cameraInfo.xUp);

	// Geometric transformations
	glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
	glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
	glRotatef(angle_z, 0.0f, 0.0f, 1.0f);
	glTranslatef(xx, 0.0f, zz);
	glScalef(scale, scale, scale);

	recursiveDraw(info.groups);

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
    
    if(argc == 2){
        //Reads XML file
        info = readXML(argv[1]);
    }
    else{
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