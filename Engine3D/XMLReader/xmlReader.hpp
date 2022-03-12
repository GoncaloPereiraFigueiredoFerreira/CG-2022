#ifndef XMLREADER_H
#define XMLREADER_H

#include "lib/rapidxml-1.13/rapidxml.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

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


class LPoint{
	public:
	double posX;
	double posY;
	double posZ;
};

class LDirec{
	public:
	double dirX;
	double dirY;
	double dirZ;
};

class LSpotl{
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



class Translate {
	public:
	double x;
	double y;
	double z;
	int order; //defines the order of the transformations // if its -1, it means its not referenced in the xml file
};

class Rotate {
	public:
	double angle;
	double x;
	double y;
	double z;
	int order; //defines the order of the transformations // if its -1, it means its not referenced in the xml file
};

class Scale {
	public:
	double x;
	double y;
	double z;
	int order; //defines the order of the transformations // if its -1, it means its not referenced in the xml file
};


class Transform{
	public:
	Translate transl;
	Rotate rotate;
	Scale scale;
};




class Color{
	public:
	double diffuseR;
	double diffuseG;
	double diffuseB;

	double ambientR;
	double ambientG;
	double ambientB;

	double specularR;
	double specularG;
	double specularB;

	double emissiveR;
	double emissiveG;
	double emissiveB;

	double shine;

};

class ModelXML{
	public:
	char* sourceF;
	char* textureF;  //Might be null
	Color color; //Might be null
};


class Group{
	public:
	Transform transforms;
	std::vector<ModelXML> modelList;
	std::vector<Group> groupChildren;
};


class xmlInfo{
	public:
	Camera cameraInfo;
	Lights lightsList;
	Group groups;

};

xmlInfo readXML(std::string filename);

#endif