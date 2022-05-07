#ifndef XMLREADER_H
#define XMLREADER_H

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "lib/rapidxml-1.13/rapidxml.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "../Auxiliar/Point.h"
#include "../Auxiliar/catmull-rom.h"

//Camera

class Camera{
	public:
	double xPos;
	double yPos;
	double zPos;
	
	double xLook;
	double yLook;
	double zLook;

	double xUp;
	double yUp;
	double zUp;

	double fov;
	double near;
	double far;
};


//Lights

class LPoint{
public:
    LPoint(double posX, double posY, double posZ) : posX(posX), posY(posY), posZ(posZ) {}

public:
	double posX;
	double posY;
	double posZ;

	void apply(GLenum light){
		float aux[4] = {(float)posX,(float)posY,(float)posZ,1.0f};
		glLightfv(light,GL_POSITION,aux);
	}
};

class LDirec{
public:
    LDirec(double dirX, double dirY, double dirZ) : dirX(dirX), dirY(dirY), dirZ(dirZ) {}

public:
	double dirX;
	double dirY;
	double dirZ;

	void apply(GLenum light){
		float aux[4] = {(float)dirX,(float)dirY,(float)dirZ,0.0f};
		glLightfv(light,GL_POSITION,aux);
	}
};

class LSpotl{
public:
    LSpotl(double posX, double posY, double posZ, double dirX, double dirY, double dirZ, double cutoff) : posX(posX),
                                                                                                          posY(posY),
                                                                                                          posZ(posZ),
                                                                                                          dirX(dirX),
                                                                                                          dirY(dirY),
                                                                                                          dirZ(dirZ),
                                                                                                          cutoff(cutoff) {}

public:
	double posX;
	double posY;
	double posZ;
	double dirX;
	double dirY;
	double dirZ;
	double cutoff;

	void apply(GLenum light){
		float aux_p[4] = {(float)posX,(float)posY,(float)posZ,1.0f};
		float aux_d[3] = {(float)dirX,(float)dirY,(float)dirZ};
		glLightfv(light,GL_POSITION, aux_p);
		glLightfv(light,GL_SPOT_DIRECTION, aux_d);
		glLightf(light,GL_SPOT_CUTOFF,(float)cutoff);
	}
};


class Lights{
	public:
	std::vector<LPoint> points;
	std::vector<LDirec> direct;
	std::vector<LSpotl> spotL;

	void init(GLenum light0){
		int aux = 0;
		for(int i = 0; i < points.size();i++){
			glEnable(light0 + aux);
			aux++;
		}
		for(int i = 0; i < direct.size();i++){
			glEnable(light0 + aux);
			aux++;
		}
		for(int i = 0; i < spotL.size();i++){
			glEnable(light0 + aux);
			aux++;
		}
	}

	void apply(GLenum light0){
		int aux = 0;
		for(int i = 0; i < points.size();i++){
			points[i].apply(light0+aux);
			aux++;
		}
		for(int i = 0; i < direct.size();i++){
			direct[i].apply(light0+aux);
			aux++;
		}
		for(int i = 0; i < spotL.size();i++){
			spotL[i].apply(light0+aux);
			aux++;
		}
	}

};


//Transformations

class Transform{
	public:
    double x{};
    double y{};
    double z{};
    virtual void apply() {};
};

class TranslateS : public Transform {
	public:
    TranslateS(double x, double y, double z) : Transform(){
        this->x = x;
        this->y = y;
        this->z = z;
    }
	void apply() {
		glTranslatef(this->x, this->y, this->z);
	}
};

class TranslateD : public Transform {
	public:
	int time;
    bool align;
	std::vector<Point> points;
    TranslateD(int time, bool align,std::vector<Point> points) : Transform(){
        this->time = time;
		this->align = align;
		this->points = points;
    }
	void apply() {
		static float y[4] = {0.0f,1.0f,0.0f};

		double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 

		t /= this->time;
		t -= floor(t);

		float p[3];
		float div[3];

		//para modar so debug linhas do trajeto
		int tessellation = 100;
		float inc = 1.0f/tessellation;
		float t_aux = 0;
		glBegin(GL_LINE_LOOP); 
		for(int i = 0;i <= tessellation;i++){
			CatmullRomPoint(t_aux,points,p,div);
			glVertex3f(p[0],p[1],p[2]);
			t_aux += inc;
		}
		glEnd();
		//acaba

		CatmullRomPoint(t,points,p,div);
		glTranslatef(p[0], p[1], p[2]);
		if(this->align){		
			float m[16];
			getMatrizRotateCatmull(div,y,m);
			glMultMatrixf(m);
		}
	}
};





class RotateS : public Transform {
	public:
	double angle;
    RotateS(double x, double y, double z, double angle) : Transform(){
        this->x = x;
        this->y = y;
        this->z = z;
        this->angle = angle;
    }
	void apply() {
		glRotatef(this->angle, this->x, this->y, this->z);
	}
};
class RotateD : public Transform {
	public:
	double time;
    RotateD(double x, double y, double z, double time) : Transform(){
        this->x = x;
        this->y = y;
        this->z = z;
        this->time = time;
    }
	void apply() {
		double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; 

		t /= this->time;
		t -= floor(t);

		glRotatef(t * 360.0f, this->x , this->y, this->z);
	}
};

class Scale : public Transform {
	public:
    Scale(double x, double y, double z) : Transform(){
        this->x = x;
        this->y = y;
        this->z = z;
    }
	void apply() {
		glScalef(this->x, this->y, this->z);
	}
};


// Color

class Color{
public:
    Color(double diffuseR, double diffuseG, double diffuseB, double ambientR, double ambientG, double ambientB,
          double specularR, double specularG, double specularB, double emissiveR, double emissiveG, double emissiveB,
          double shine) {
        this->diffuseR = diffuseR;
        this->diffuseG = diffuseG;
        this->diffuseB = diffuseB;
        this->ambientR = ambientR;
        this->ambientG = ambientG;
        this->ambientB = ambientB;
        this->specularR = specularR;
        this->specularG = specularG;
        this->specularB = specularB;
        this->emissiveR = emissiveR;
        this->emissiveG = emissiveG;
        this->emissiveB = emissiveB;
        this->shine = shine;
    }

public:
	double diffuseR{};
	double diffuseG{};
	double diffuseB{};

	double ambientR{};
	double ambientG{};
	double ambientB{};

	double specularR{};
	double specularG{};
	double specularB{};

	double emissiveR{};
	double emissiveG{};
	double emissiveB{};

	double shine{};

	void apply(){
		float aux_d[4] = {(float)diffuseR/255,(float)diffuseG/255,(float)diffuseB/255,1.0f};
		float aux_a[4] = {(float)ambientR/255,(float)ambientG/255,(float)ambientB/255,1.0f};
		float aux_s[4] = {(float)specularR/255,(float)specularG/255,(float)specularB/255,1.0f};
		float aux_e[4] = {(float)emissiveR/255,(float)emissiveG/255,(float)emissiveB/255,1.0f};

		glMaterialfv(GL_FRONT, GL_DIFFUSE, aux_d);
		glMaterialfv(GL_FRONT, GL_AMBIENT, aux_a);
		glMaterialfv(GL_FRONT, GL_SPECULAR, aux_s);
		glMaterialfv(GL_FRONT, GL_EMISSION, aux_e);
		glMaterialf(GL_FRONT, GL_SHININESS, (float)shine);

	};
};



class ModelInfo{
	public:
	char* sourceF = nullptr;
	char* textureF = nullptr;  //Might be null
	Color* color = nullptr;
};


class Group{
	public:
	std::vector<Transform*> transforms{};
	std::vector<ModelInfo> modelList{};
	std::vector<Group> groupChildren{};
};


class xmlInfo{
	public:
	Camera cameraInfo{};
	Lights lightsList;
	Group groups;
};

xmlInfo readXML(std::string filename);

#endif