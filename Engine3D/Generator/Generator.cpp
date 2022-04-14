#include <iostream>
#include "ConeGenerator.h"
#include "SphereGenerator.h"
#include "PlaneGenerator.h"
#include "BoxGenerator.h"
#include "TorusGenerator.h"
#include "string.h"
#include "Model.h"
using namespace std;

int main(int argc, char** argv)
{
    vector<float> vertexB;
    vector<unsigned int> indexB;
    string filename;
    bool generated = true;

	if (argc == 7) { 
		if(strcmp(argv[1], "cone") == 0 && atoi(argv[5]) >= 1 && atoi(argv[4]) >= 1){//CONE GENERATOR
			cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), vertexB, indexB);
			filename = argv[6];
		}
		else if(strcmp(argv[1], "torus") == 0 && atoi(argv[5]) >= 1 && atoi(argv[4]) >= 1){//TORUS GENERATOR
			torus(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), vertexB, indexB);
			filename = argv[6];
		}else {
            printf("Invalid arguments!\n");
            generated = false;
        }
	}
	else if (argc == 6 && strcmp(argv[1], "sphere") == 0 && atoi(argv[3]) >= 1 && atoi(argv[4]) >= 1) { //SPHERE GENERATOR
		sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), vertexB, indexB);
        filename = argv[5];
	}
	else if (argc == 5) {
		if (strcmp(argv[1], "plane") == 0 && atoi(argv[3]) >= 1) { //PLANE GENERATOR
			plane(atof(argv[2]), atoi(argv[3]), vertexB, indexB);
			filename = argv[4];
		}
		else if (strcmp(argv[1], "box") == 0 && atoi(argv[3]) >= 1) { //BOX GENERATOR
			box(atof(argv[2]), atoi(argv[3]), vertexB, indexB);
			filename = argv[4];
		}else {
            printf("Invalid arguments!\n");
            generated = false;
        }
	}
	else {
		//Write help code here
		cout << "Invalid arguments!\n";
        generated = false;
	}

    if(generated) generateModelFile(filename, vertexB, indexB);
}