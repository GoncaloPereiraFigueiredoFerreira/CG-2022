#include "lib/rapidxml-1.13/rapidxml.hpp"
#include <stdio.h>
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

class Lights{};


class LPoint:Lights{
	public:
	int posX;
	int posY;
	int posZ;
};

class LDirec:Lights{
	public:
	int dirX;
	int dirY;
	int dirZ;
};

class LSpotl:Lights{
	public:
	int posX;
	int posY;
	int posZ;
	int dirX;
	int dirY;
	int dirZ;
	int cutkff;
}




class Transform{};

class Translate : Transform{
	public:
	int x;
	int y;
	int z;
};

class Rotate : Transform{
	public:
	int angle;
	int x;
	int y;
	int z;
};

class Scale : Transform{
	public:
	int x;
	int y;
	int z;
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


class xmlInfo{
	public:
	Camera cameraInfo;
	Lights lightsList[8];
	Transform* transforms; // only one entry for each type of transform
						   // Order is relevant

	Model* modelList;
};





