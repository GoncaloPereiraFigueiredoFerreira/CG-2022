#include "lib/rapidxml-1.13/rapidxml.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

class Camera{
	public:
	int xPos;
	int yPos;
	int zPos;
	
	int xLook;
	int yLook;
	int zLook;

	int xUp;
	int yUp;
	int zUp;

	int fov;
	int near;
	int far;
};


class LPoint{
	public:
	int posX;
	int posY;
	int posZ;
};

class LDirec{
	public:
	int dirX;
	int dirY;
	int dirZ;
};

class LSpotl{
	public:
	int posX;
	int posY;
	int posZ;
	int dirX;
	int dirY;
	int dirZ;
	int cutoff;
};


class Lights{
	public:
	std::vector<LPoint> points;
	std::vector<LDirec> direct;
	std::vector<LSpotl> spotL;
};



class Translate {
	public:
	int x;
	int y;
	int z;
	int order; //defines the order of the transformations // if its -1, it means its not referenced in the xml file
};

class Rotate {
	public:
	int angle;
	int x;
	int y;
	int z;
	int order; //defines the order of the transformations // if its -1, it means its not referenced in the xml file
};

class Scale {
	public:
	int x;
	int y;
	int z;
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
	int diffuseR;
	int diffuseG;
	int diffuseB;

	int ambientR;
	int ambientG;
	int ambientB;

	int specularR;
	int specularG;
	int specularB;

	int emissiveR;
	int emissiveG;
	int emissiveB;

	int shine;

};

class Model{
	public:
	char* sourceF;
	char* textureF;  //Might be null
	Color color; //Might be null
};


class Group{
	public:
	Transform transforms;
	std::vector<Model> modelList;
	std::vector<Group> groupChildren;
};


class xmlInfo{
	public:
	Camera cameraInfo;
	Lights lightsList;
	Group groups;

};

xmlInfo readXML(std::string filename);




