#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Generator/Model.h"
#include "../XMLReader/xmlReader.hpp"
using namespace std;

class ModelVBO {
public:
    GLuint vertices, verticesCount, indices, normais;
    unsigned int indicesCount;

    ModelVBO(GLuint vertices, GLuint verticesCount, GLuint indices, unsigned int indicesCount, GLuint normais) : vertices(
        vertices), verticesCount(verticesCount), indices(indices), indicesCount(indicesCount),normais(normais) {}
};


int cameraMode = 0;
int wW=800,wH=800;
int timebase = 0, frame = 0;
int startX, startY, tracking = 0;
float alpha, beta1, r,sensibility = 0.01;

float diffuse[4] = {200.0f/255.0f,200.0f/255.0f,200.0f/255.0f,1.0f};
float ambient[4] = {50.0f/255.0f,50.0f/255.0f,50.0f/255.0f,1.0f};
float specular[4] = {0.0f,0.0f,0.0f,1.0f};
float emissive[4] = {0.0f,0.0f,0.0f,1.0f};
int shininess = 0;

xmlInfo info;
vector<Group> groups;
unordered_map<char*, ModelVBO*> modelDict;

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

int generateDic(Group tmpGroup, unordered_map<char*, ModelVBO*>& modelDict) {
	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		if (modelDict.find(tmpGroup.modelList[i].sourceF) == modelDict.end()) {  //Verificar se o elemento ja esta no mapa
            vector<float> vertexB; vector<unsigned int> indexB;vector<float> normalB;vector<float> textB;

			if (readModelFromFile(tmpGroup.modelList[i].sourceF, vertexB, indexB, normalB, textB) == -1) {
				cout << "Error: File\"" << tmpGroup.modelList[i].sourceF << "\" not found\n";
				return -1;
			}
			else {
                GLuint vertices, normais, verticesCount = vertexB.size() / 3, indices;
                unsigned int indicesCount = indexB.size();

                //Create VBO
                glGenBuffers(1, &vertices);

                //Copy vector to graphics card
                glBindBuffer(GL_ARRAY_BUFFER, vertices);
                glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(float), vertexB.data(), GL_STATIC_DRAW);

                //Create VBO
                glGenBuffers(1, &normais);

                //Copy vector to graphics card
                glBindBuffer(GL_ARRAY_BUFFER, normais);
                glBufferData(GL_ARRAY_BUFFER, normalB.size() * sizeof(float), normalB.data(), GL_STATIC_DRAW);

                glGenBuffers(1, &indices);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexB.size(), indexB.data(), GL_STATIC_DRAW);

                //inserts pair of file's name and respective VBO information
                auto m = new ModelVBO(vertices, verticesCount, indices, indicesCount,normais);
                modelDict.insert(std::make_pair(tmpGroup.modelList[i].sourceF, m));
            }
		}
	}

	for (int i = 0; i < tmpGroup.groupChildren.size(); i++)
		if(generateDic(tmpGroup.groupChildren[i], modelDict) == -1)
            return -1;

	return 1;
}

/* ------------------------------------------------- */


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	wW = w;
	wH = h;

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
	for (int i = 0; i < tmpGroup.transforms.size(); i++)
		tmpGroup.transforms[i]->apply();

	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		ModelVBO *m = modelDict[tmpGroup.modelList[i].sourceF];

		tmpGroup.modelList[i].color->apply();

        glBindBuffer(GL_ARRAY_BUFFER, m->vertices);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->indices);
        glBindBuffer(GL_ARRAY_BUFFER,m->normais);
        glNormalPointer(GL_FLOAT,0,0);
        glDrawElements(GL_TRIANGLES, m->indicesCount, GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < tmpGroup.groupChildren.size(); i++)
		recursiveDraw(tmpGroup.groupChildren[i]);

	glPopMatrix();
}

void renderText(const std::string text) {
	// Guardar a projeção anterior
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// Projecção ortogonal para que as coordenadas de desenho coincidam com o tamanho da
	//janela em pixeis
	gluOrtho2D(0, wW,wH, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	void* font = GLUT_BITMAP_HELVETICA_18;
	// Centrar o texto, calculando a dimensão da mensagem em pixeis
	float textw = glutBitmapLength(font, (unsigned char*) text.c_str());
	glRasterPos2d(wW/40, wH/35); // text position in pixels
	// Ignorar profundidade
	glDisable(GL_DEPTH_TEST);
	// Desenhar a mensagem, caracter a caracter
	for (char c : text)
	{
	glutBitmapCharacter(font, c);
	}
	// Restaurar as matrizes anteriores
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}


void renderScene(void) {
	// clear buffers
	int time;
	float fps;
	static char s[64];
	//sprintf(s, "FPS:");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set the camera
	glLoadIdentity();
	gluLookAt(info.cameraInfo.xPos, info.cameraInfo.yPos, info.cameraInfo.zPos,
		info.cameraInfo.xLook, info.cameraInfo.yLook, info.cameraInfo.zLook,
		info.cameraInfo.xUp, info.cameraInfo.yUp, info.cameraInfo.xUp);


	info.lightsList.apply(GL_LIGHT0);

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		
		sprintf(s, "FPS: %f", fps);
	}
	renderText(s);

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

	if (cameraMode ==1)
		yy = -yy;

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


	if (cameraMode ==1)
		yy = -yy;


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

    //glutPostRedisplay();
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
	//glutPostRedisplay();
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
    //glutPostRedisplay();
}

int main(int argc, char** argv) {


	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(wW, wH);
	glutCreateWindow("CG@G13");
    
    //init GLEW
    glewInit();

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Registration of the keyboard and mouse callbacks
	glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glutKeyboardFunc(defaultKeyFunc);
	glutSpecialFunc(specialKeyFunc);

    //Enabling Buffer of Vertex Functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

	//  OpenGL settings
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	if (argc == 2) {
		//Reads XML file
		try{
			info = readXML(argv[1]);
		}catch (const char* msg){
			cout << msg << endl;
			return -1;
		}
        calculatePolarCoordinates();
	}
	else {
		cout << "Invalid arguments" << endl;
		return -1;
	}
	
	info.lightsList.init(GL_LIGHT0);	
	
	if (!generateDic(info.groups, modelDict))
		return -1;

	// enter GLUT's main cycle
	glutMainLoop();


	return 1;
}