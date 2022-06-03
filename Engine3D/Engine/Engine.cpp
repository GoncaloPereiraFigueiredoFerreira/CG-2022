#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <IL/il.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include "../Generator/Model.h"
#include "../XMLReader/xmlReader.hpp"
using namespace std;

class ModelVBO {
public:
    GLuint vertices, verticesCount, indices, normais,texCoords;
    unsigned int indicesCount;
    float cube[6];

    ModelVBO(GLuint vertices, GLuint verticesCount, GLuint indices, unsigned int indicesCount, GLuint normais,GLuint texCoords,float *cube) : vertices(
        vertices), verticesCount(verticesCount), indices(indices), indicesCount(indicesCount),normais(normais),texCoords(texCoords) {

    	for(int i = 0;i < 6;i++){
    		this->cube[i] = cube[i];
    	}

    }
};

int wW=800,wH=800;
int timebase = 0, frame = 0;
int nmodels = 0;

float emissive_def[4] = {0.0f,0.0f,0.0f,1.0f};
float emissive_full[4] = {1.0f,1.0f,1.0f,1.0f};
int shininess = 0;

xmlInfo info;
vector<Group> groups;
unordered_map<char*, ModelVBO*> modelDict;
unordered_map<char*, GLuint> textureDict;

// ******* Auxiliar ******* //

float calcCamRadius(){
    float dirX = (float) (info.cameraInfo.xLook - info.cameraInfo.xPos),
                  dirY = (float) (info.cameraInfo.yLook - info.cameraInfo.yPos),
                  dirZ = (float) (info.cameraInfo.zLook - info.cameraInfo.zPos);
    return sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
}

// ******* Camera Orientation ******* //

int startX, startY, cameraMode = 1; // Camera Mode: 0 - Fixed Look Point | 1 - FPS Mode
float lastCenterModeRadius, myalpha = 0, mybeta, mybetadif = 0, cameraSensivity = 0.01;

void initCameraVars(){
    float cameraD[3] = {(float) (info.cameraInfo.xLook - info.cameraInfo.xPos),
                        (float) (info.cameraInfo.yLook - info.cameraInfo.yPos),
                        (float) (info.cameraInfo.zLook - info.cameraInfo.zPos)};
    float up[3] = {(float) info.cameraInfo.xUp, (float) info.cameraInfo.yUp, (float) info.cameraInfo.zUp};
    lastCenterModeRadius = sqrt(pow(cameraD[0], 2) + pow(cameraD[1], 2) + pow(cameraD[2], 2));
    normalize(cameraD);
    normalize(up);
    mybeta = acos(cameraD[0]*up[0] + cameraD[1]*up[1] + cameraD[2]*up[2]);
}

void calculateNewCameraParam() {
    float cameraD[3] = {};
    if (cameraMode) { //vector pointing from camera pos to camera look point (FPS MODE)
        cameraD[0] = (float) (info.cameraInfo.xLook - info.cameraInfo.xPos);
        cameraD[1] = (float) (info.cameraInfo.yLook - info.cameraInfo.yPos);
        cameraD[2] = (float) (info.cameraInfo.zLook - info.cameraInfo.zPos);
    } else{ //vector pointing from camera look point to camera pos (FIXED CENTER MODE)
        cameraD[0] = (float) (info.cameraInfo.xPos - info.cameraInfo.xLook);
        cameraD[1] = (float) (info.cameraInfo.yPos - info.cameraInfo.yLook);
        cameraD[2] = (float) (info.cameraInfo.zPos - info.cameraInfo.zLook);
    }

    float right[3] = {0};
    float up[3] = {(float) info.cameraInfo.xUp, (float) info.cameraInfo.yUp, (float) info.cameraInfo.zUp};
    getNormal(cameraD, up, right); //Calculates camera right vector
    getNormal(right, cameraD, up); //Calculates camera up vector

    float res[4] = {0}, point[4] = {cameraD[0], cameraD[1], cameraD[2], 1};
    bool changed = false;

    if (myalpha != 0) {
        float vx = up[0],
                vy = up[1],
                vz = up[2],
                vxSquared = pow(vx, 2),
                vySquared = pow(vy, 2),
                vzSquared = pow(vz, 2);
        float x[4] = {vxSquared + (1 - vxSquared) * cos(myalpha), vx * vy * (1 - cos(myalpha)) - vz * sin(myalpha),
                      vx * vz * (1 - cos(myalpha)) + vy * sin(myalpha), 0},
                y[4] = {vx * vy * (1 - cos(myalpha)) + vz * sin(myalpha), vySquared + (1 - vySquared) * cos(myalpha),
                        vy * vz * (1 - cos(myalpha)) - vx * sin(myalpha), 0},
                z[4] = {vx * vz * (1 - cos(myalpha)) - vy * sin(myalpha),
                        vy * vz * (1 - cos(myalpha)) + vx * sin(myalpha),
                        vzSquared + (1 - vzSquared) * cos(myalpha), 0};
        float rotMatrix[16] = {};
        buildRotMatrix(x, y, z, rotMatrix);
        multMatrixVector(rotMatrix, point, res);
        point[0] = res[0];
        point[1] = res[1];
        point[2] = res[2];
        point[3] = res[3]; //updates point so it can be used to apply the alteration with the beta value
        changed = true;
    }

    if (mybetadif != 0) {
        float vx = right[0],
                vy = right[1],
                vz = right[2],
                vxSquared = (float) pow(vx, 2),
                vySquared = (float) pow(vy, 2),
                vzSquared = (float) pow(vz, 2);
        float x[4] = {vxSquared + (1 - vxSquared) * cos(mybetadif),
                      vx * vy * (1 - cos(mybetadif)) - vz * sin(mybetadif),
                      vx * vz * (1 - cos(mybetadif)) + vy * sin(mybetadif), 0},
                y[4] = {vx * vy * (1 - cos(mybetadif)) + vz * sin(mybetadif),
                        vySquared + (1 - vySquared) * cos(mybetadif),
                        vy * vz * (1 - cos(mybetadif)) - vx * sin(mybetadif), 0},
                z[4] = {vx * vz * (1 - cos(mybetadif)) - vy * sin(mybetadif),
                        vy * vz * (1 - cos(mybetadif)) + vx * sin(mybetadif),
                        vzSquared + (1 - vzSquared) * cos(mybetadif), 0};
        float rotMatrix[16] = {};
        buildRotMatrix(x, y, z, rotMatrix);
        multMatrixVector(rotMatrix, point, res);
        changed = true;
    }

    if (changed) {
        if (cameraMode) { //updates camera look point (FPS MODE)
            info.cameraInfo.xLook = info.cameraInfo.xPos + res[0];
            info.cameraInfo.yLook = info.cameraInfo.yPos + res[1];
            info.cameraInfo.zLook = info.cameraInfo.zPos + res[2];
        } else{ //updates camera position (FIXED CENTER MODE)
            info.cameraInfo.xPos = info.cameraInfo.xLook + res[0] * lastCenterModeRadius;
            info.cameraInfo.yPos = info.cameraInfo.yLook + res[1] * lastCenterModeRadius;
            info.cameraInfo.zPos = info.cameraInfo.zLook + res[2] * lastCenterModeRadius;
        }
    }
}


// ******* Camera Movimentation ******* //
set<unsigned char> keysBeingPressed;
float moveSensivity = 1.0f;

//add vector b to vector a
void inline addVector(float* a, float* b){
    for(int i = 0; i < 3; i++)
        a[i] += b[i];
}

//sub vector b to vector a
void inline subVector(float* a, float* b){
    for(int i = 0; i < 3; i++)
        a[i] -= b[i];
}

void performKeyFunctions(){
    float cameraD[3] = {(float) (info.cameraInfo.xLook - info.cameraInfo.xPos),
                        (float) (info.cameraInfo.yLook - info.cameraInfo.yPos),
                        (float) (info.cameraInfo.zLook - info.cameraInfo.zPos)};
    float right[3] = {};
    float up[3] = {(float) info.cameraInfo.xUp, (float) info.cameraInfo.yUp, (float) info.cameraInfo.zUp};
    getNormal(cameraD, up, right); //Calculates camera right vector
    getNormal(right, cameraD, up); //Calculates camera up vector

    //Movements of
    float Look[3] = {0}, Pos[3] = {0};
    for(auto key = keysBeingPressed.begin(); key != keysBeingPressed.end(); key++) {
        if (*key == 'w' || *key == 'W') {
            addVector(Look, cameraD);
            addVector(Pos, cameraD);
        } else if (*key == 's' || *key == 'S') {
            subVector(Look, cameraD);
            subVector(Pos, cameraD);
        } else if (*key == 'd' || *key == 'D') {
            addVector(Look, right);
            addVector(Pos, right);
        } else if (*key == 'a' || *key == 'A') {
            subVector(Look, right);
            subVector(Pos, right);
        } else if (*key == ' '){
            addVector(Look, up);
            addVector(Pos, up);
        } else if (*key == 'c' || *key == 'C'){
            subVector(Look, up);
            subVector(Pos, up);
        }
    }

    info.cameraInfo.xLook += Look[0] * moveSensivity;
    info.cameraInfo.yLook += Look[1] * moveSensivity;
    info.cameraInfo.zLook += Look[2] * moveSensivity;
    info.cameraInfo.xPos += Pos[0] * moveSensivity;
    info.cameraInfo.yPos += Pos[1] * moveSensivity;
    info.cameraInfo.zPos += Pos[2] * moveSensivity;
}

int loadTexture(std::string s) {

	unsigned int t,tw,th;
	unsigned char *texData;
	unsigned int texID = 0;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1,&t);
	ilBindImage(t);
	if (ilLoadImage((ILstring)s.c_str()) == IL_TRUE){
		tw = ilGetInteger(IL_IMAGE_WIDTH);
		th = ilGetInteger(IL_IMAGE_HEIGHT);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		texData = ilGetData();

		glGenTextures(1,&texID);
		
		glBindTexture(GL_TEXTURE_2D,texID);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return texID;
}

/* ------- Generate Dictionary Of Models ------- */

int generateDic(Group tmpGroup, unordered_map<char*, ModelVBO*>& modelDict,unordered_map<char*, GLuint>& textureDict) {
	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		// cria modelDict
		if (modelDict.find(tmpGroup.modelList[i].sourceF) == modelDict.end()) {  //Verificar se o elemento ja esta no mapa
            vector<float> vertexB; vector<unsigned int> indexB;vector<float> normalB;vector<float> textB;
            float cube[6] = {0};

			if (readModelFromFile(tmpGroup.modelList[i].sourceF, vertexB, indexB, normalB, textB,cube) == -1) {
				cout << "Error: File\"" << tmpGroup.modelList[i].sourceF << "\" not found\n";
				return -1;
			}
			else {
                GLuint vertices, normais, verticesCount = vertexB.size() / 3, indices,texCoords;
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

                //Create VBO indices 
                glGenBuffers(1, &indices);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexB.size(), indexB.data(), GL_STATIC_DRAW);
 
                //Create VBO textura
                glGenBuffers(1, &texCoords);
				glBindBuffer(GL_ARRAY_BUFFER, texCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textB.size(), textB.data(), GL_STATIC_DRAW);

                //inserts pair of file's name and respective VBO information
                auto m = new ModelVBO(vertices, verticesCount, indices, indicesCount,normais,texCoords,cube);
                modelDict.insert(std::make_pair(tmpGroup.modelList[i].sourceF, m));
            }
		}

		//cria textureDict
		if (tmpGroup.modelList[i].textureF && textureDict.find(tmpGroup.modelList[i].textureF) == textureDict.end()) {  //Verificar se o elemento ja esta no mapa

			GLuint textID = loadTexture(tmpGroup.modelList[i].textureF);

            //inserts pair of file's name and respective VBO information
            textureDict.insert(std::make_pair(tmpGroup.modelList[i].textureF, textID));
		}
	}

	for (int i = 0; i < tmpGroup.groupChildren.size(); i++)
		if(generateDic(tmpGroup.groupChildren[i], modelDict,textureDict) == -1)
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

static inline void getMPVMatrix(float *a){
	float m[16],p[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,m);
	glGetFloatv(GL_PROJECTION_MATRIX,p);

	glPushMatrix();
	glLoadMatrixf(p);
	glMultMatrixf(m);
	glGetFloatv(GL_MODELVIEW_MATRIX,a);
	glPopMatrix();
}

static inline char checkDraw(float *m,float *points){
	char flag = true;
	float c1[4],c2[4],p1[3],p2[3];
	for(int i = 0; i < 3 && flag;i++){
		for(int j = 0;j < 4;j++){
			c1[j] = m[i+4*j] + m[4 * j + 3];
			c2[j] = m[i+4*j] - m[4 * j + 3];
		}
		for(int j = 0;j < 3;j++){
			p1[j] = (c1[j] > 0) ? points[j*2]:points[j*2+1];
			p2[j] = (c2[j] < 0) ? points[j*2]:points[j*2+1];
		}

		flag = (0 <= p1[0] * c1[0] + p1[1] * c1[1] + p1[2] * c1[2] + c1[3]) &&
		       (0 >= p2[0] * c2[0] + p2[1] * c2[1] + p2[2] * c2[2] + c2[3]);
	}

	return flag;
}

void recursiveDraw(Group tmpGroup) {

	float matrix[16];

	glPushMatrix();
	for (int i = 0; i < tmpGroup.transforms.size(); i++)
		tmpGroup.transforms[i]->apply();

	getMPVMatrix(matrix);

	for (int i = 0; i < tmpGroup.modelList.size(); i++) {
		ModelVBO *m = modelDict[tmpGroup.modelList[i].sourceF];
		if(checkDraw(matrix,m->cube)){
			nmodels++;
			GLuint texID = 0;

			tmpGroup.modelList[i].color->apply();

			/*glBegin(GL_QUADS);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);

				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);

				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);

				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);

				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[4] + 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[4] + 0.1);

				glVertex3f(m->cube[0] + 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[0] + 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[3] - 0.1,m->cube[5] - 0.1);
				glVertex3f(m->cube[1] - 0.1,m->cube[2] + 0.1,m->cube[5] - 0.1);
			glEnd();*/

	        glBindBuffer(GL_ARRAY_BUFFER, m->vertices);
	        glVertexPointer(3, GL_FLOAT, 0, 0);
	        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->indices);
	        glBindBuffer(GL_ARRAY_BUFFER,m->normais);
	        glNormalPointer(GL_FLOAT,0,0);

	        if (tmpGroup.modelList[i].textureF && textureDict.find(tmpGroup.modelList[i].textureF) != textureDict.end()){
	        	texID = textureDict[tmpGroup.modelList[i].textureF];

	        	glBindTexture(GL_TEXTURE_2D, texID);

	        	glBindBuffer(GL_ARRAY_BUFFER,m->texCoords);
				glTexCoordPointer(2,GL_FLOAT,0,0);
	        }

	        glDrawElements(GL_TRIANGLES, m->indicesCount, GL_UNSIGNED_INT, 0);

	        glBindTexture(GL_TEXTURE_2D, 0);
    	}
	}

	for (int i = 0; i < tmpGroup.groupChildren.size(); i++)
		recursiveDraw(tmpGroup.groupChildren[i]);

	glPopMatrix();
}

void renderText(const std::string text,double posx, double posy) {
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
	// Ignorar profundidade
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive_full);
	glRasterPos2d(posx, posy); // text position in pixels
	
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
	//glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive_def);
}

void renderScene(void) {
	// clear buffers
	int time;
	float fps;
	static char s[64];
	//sprintf(s, "FPS:");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set the camera
    performKeyFunctions(); //Perform camera move actions
	glLoadIdentity();
	gluLookAt(info.cameraInfo.xPos, info.cameraInfo.yPos, info.cameraInfo.zPos,
		info.cameraInfo.xLook, info.cameraInfo.yLook, info.cameraInfo.zLook,
		info.cameraInfo.xUp, info.cameraInfo.yUp, info.cameraInfo.zUp);


	info.lightsList.apply(GL_LIGHT0);


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

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		
		sprintf(s, "FPS: %.3f", fps);
	}
	renderText(s,wW/40, wH/35);
	static char s2[120];

	sprintf(s2, "Look x: %.3f Look y: %.3f Look z: %.3f", info.cameraInfo.xLook,info.cameraInfo.yLook,info.cameraInfo.zLook);
	renderText(s2,wW/40, wH/35 + 20);
	sprintf(s2, "Pos x: %.3f Pos y: %.3f Pos z: %.3f", info.cameraInfo.xPos,info.cameraInfo.yPos,info.cameraInfo.zPos);
	renderText(s2,wW/40, wH/35 + 40);
	sprintf(s2, "Numero de modelos: %d", nmodels);
	renderText(s2,wW/40, wH/35 + 60);
	nmodels = 0;

	// End of frame
	glutSwapBuffers();
}


// functions to process mouse events
void processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            startX = xx;
            startY = yy;
        } else if (cameraMode == 0) {
            if (button == 3 || button == 4) {
                //Calculates orientation vector and normalizes it
                float cameraD[3] = {(float) (info.cameraInfo.xLook - info.cameraInfo.xPos),
                                    (float) (info.cameraInfo.yLook - info.cameraInfo.yPos),
                                    (float) (info.cameraInfo.zLook - info.cameraInfo.zPos)};

                if (button == 3) { //Scroll Up
                    float dist = sqrt(pow(cameraD[0], 2) + pow(cameraD[1], 2) + pow(cameraD[2], 2));
                    normalize(cameraD);
                    if (dist > moveSensivity * 10) {
                        info.cameraInfo.xPos += cameraD[0] * moveSensivity * 10;
                        info.cameraInfo.yPos += cameraD[1] * moveSensivity * 10;
                        info.cameraInfo.zPos += cameraD[2] * moveSensivity * 10;
                    }
                } else { //Scroll Down
                    normalize(cameraD);
                    info.cameraInfo.xPos -= cameraD[0] * moveSensivity * 10;
                    info.cameraInfo.yPos -= cameraD[1] * moveSensivity * 10;
                    info.cameraInfo.zPos -= cameraD[2] * moveSensivity * 10;
                }

                lastCenterModeRadius = calcCamRadius();
            }
        }
    }
}


void processMouseMotion(int xx, int yy) {
    myalpha = ((float) (startX - xx)) * cameraSensivity;
    float angY = mybeta + ((float) (startY - yy)) * cameraSensivity;
    if (angY > M_PI) angY = M_PI - 0.01;
    else if (angY < 0) angY = 0.01;
    startX = xx;
    startY = yy;
    mybetadif = angY - mybeta;
    mybeta += mybetadif;
    calculateNewCameraParam();
}

// function to process keyboard events

void defaultKeyFunc(unsigned char key, int x, int y) {
    //Movements of
    if (key == 'w' || key == 'W' || key == 's' || key == 'S' || key == 'd' || key == 'D' || key == 'a' || key == 'A' || key == ' ' || key == 'c' || key == 'C') {
        keysBeingPressed.insert(key);
	}
    else if (key == 'u' || key == 'U'){
        moveSensivity += 0.05;
    }
    else if (key == 'i' || key == 'I'){
        if (moveSensivity > 0.1)
            moveSensivity -= 0.05;
    }
    else if (key == 'm' || key == 'M') {
		if(cameraMode == 0){
            lastCenterModeRadius = calcCamRadius();
            mybeta = M_PI - mybeta;
            cameraMode = 1;
            glutSetWindowTitle("CG@13 - FPS Mode");
        }
        else{
            cameraMode = 0;
            mybeta = M_PI - mybeta;

            //Adjust look point to avoid jumps
            float cameraD[3] = {(float) (info.cameraInfo.xLook - info.cameraInfo.xPos),
                                (float) (info.cameraInfo.yLook - info.cameraInfo.yPos),
                                (float) (info.cameraInfo.zLook - info.cameraInfo.zPos)};
            normalize(cameraD);
            info.cameraInfo.xLook = info.cameraInfo.xPos + lastCenterModeRadius * cameraD[0];
            info.cameraInfo.yLook = info.cameraInfo.yPos + lastCenterModeRadius * cameraD[1];
            info.cameraInfo.zLook = info.cameraInfo.zPos + lastCenterModeRadius * cameraD[2];

            glutSetWindowTitle("CG@13");
        }
	}
    else if (key == 'j' || key == 'J'){
        cameraSensivity += cameraSensivity * 0.01f;
    }
    else if (key == 'k' || key == 'K'){
        if (cameraSensivity > 0.001)
            cameraSensivity -= cameraSensivity * 0.01f;
    }
}

void defaultKeyUpFunc(unsigned char key, int x, int y){
    keysBeingPressed.erase(key);
}

int main(int argc, char** argv) {
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(wW, wH);
	glutCreateWindow("CG@G13 - FPS Mode");
    
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
    glutKeyboardUpFunc(defaultKeyUpFunc);

    //Enabling Buffer of Vertex Functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//  OpenGL settings
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

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
        initCameraVars(); //Calculates angle between world's up vector and camera's look vector & sets initial "fixed center mode" radius
	}
	else {
		cout << "Invalid arguments" << endl;
		return -1;
	}
	
	info.lightsList.init(GL_LIGHT0);	
	
	if (!generateDic(info.groups, modelDict,textureDict))
		return -1;

	// enter GLUT's main cycle
	glutMainLoop();


	return 1;
}