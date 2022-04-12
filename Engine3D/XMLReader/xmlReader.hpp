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
#include "../Models/Point.h"

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
};

class LDirec{
public:
    LDirec(double dirX, double dirY, double dirZ) : dirX(dirX), dirY(dirY), dirZ(dirZ) {}

public:
	double dirX;
	double dirY;
	double dirZ;
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
};


class Lights{
	public:
	std::vector<LPoint> points;
	std::vector<LDirec> direct;
	std::vector<LSpotl> spotL;
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
	vector<Point> points;
    TranslateD(int time, bool align,vector<Point> points) : Transform(){
        this->time = time;
		this->align = align;
		this->points = points;
        
    }
	void apply() {
		//glTranslatef(this->x, this->y, this->z);
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
		//glRotatef(this->time, this->x, this->y, this->z);
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