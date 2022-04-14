#include <iostream>
#include "ConeGenerator.h"
#include "SphereGenerator.h"
#include "PlaneGenerator.h"
#include "BoxGenerator.h"
#include "TorusGenerator.h"
#include "string.h"
using namespace std;

int generateModelFile(string filename, vector<float>& vertexB, vector<unsigned int>& indexB) {
	ofstream fich;
	fich.open(filename, ios::out);

    if(fich.is_open()) {
        //Writes contents of vertexB
        for (int i = 0; i < vertexB.size() - 1; i++) {
            fich << vertexB[i] << ";";
        }
        fich << vertexB[vertexB.size() - 1] << "\n";

        //Writes contents of indexB
        for (int i = 0; i < indexB.size() - 1; i++) {
            fich << indexB[i] << ";";
        }

        fich << indexB[indexB.size() - 1] << "\n";

        fich.close();
        return 0;
    }

    return -1;
}

int readModelFromFile(string filename, vector<float>& vertexB, vector<unsigned int>& indexB) {
    std::ifstream fd;
    fd.open(filename, ios::in);

    if (!fd.is_open()) return -1;
	else {
		string line;
		string delimiter = ";";

		//Read vertices
		getline(fd, line);
		vector<string> vB = parseLine(line, delimiter);
        vertexB.reserve(vB.size());
        for(int i = 0; i < vB.size() ; i++)
            vertexB.push_back(stof(vB[i]));

        //Read indexes
        getline(fd, line);
		vector<string> iB = parseLine(line, delimiter);
        indexB.reserve(iB.size());
        for(int i = 0; i < iB.size() ; i++)
            indexB.push_back(stoul(iB[i]));

        return 0;
	}
}

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