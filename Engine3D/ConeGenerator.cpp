#include "AuxiliarMethods.h"
#include "ConeGenerator.h"
using namespace std;

Cone generateCone(float base, float height, int slices, int stacks) {
	float cx = 0.0f, cy = 0.0f, cz = 0.0f;
	Point** mat = mallocMatrix(stacks, slices);
	float angInc = 2 * M_PI / slices, //angle increment value
		heightInc = height / stacks,
		radiusDec = base / stacks;
	float ang, h, r;

	for (int i = 0; i < stacks; i++) {
		h = cz + heightInc * i;
		r = base - radiusDec * i;

		ang = 0;
		Point p = Point(cx + r * cos(ang), cy + r * sin(ang), h);
		mat[i][0] = p;

		for (int j = 1; j < slices; j++) {
			ang = angInc * j;
			Point p = Point(cx + r * cos(ang), cy + r * sin(ang), h);
			mat[i][j] = p;
		}
	}

	//freeMatrix(mat, stacks);
	return Cone(base, height, slices, stacks, mat);
}

void generateConeFile(string filename, Cone cone) {
	ofstream fich;
	fich.open(filename, ios::out);
	/*
		Default separator of fields: ';'

		1st line: type
		2nd line: [;argument]
			- [;argument] is optional, and can be repeated, in this case for sphere, we should write the radius, number of stacks and the number of slices

		Points format: "<coord x>,<coord y>,<coord z>
	*/

	fich << "cone" << "\n";
	fich << cone.base << ";" << cone.height << ";" << cone.slices << ";" << cone.stacks << "\n";

	Point** mat = cone.mat; Point p;

	for (int i = 0; i < cone.stacks - 1; i++) {

		for (int j = 0; j < cone.slices - 1; j++) {
			p = mat[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}

		p = mat[i][cone.slices - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << "\n";
	}
	fich.close();
}

Cone* readConeFromFile(std::ifstream& fd) {
	float base;
	float height;
	int slices;
	int stacks;
	Point** mat;

	if (!fd.is_open()) cout << "Open: No such file!";
	else {
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);

		vector<string> tokens = parseLine(line, delimiter);

		base = stof(tokens[0]);
		height = stof(tokens[1]);
		slices = stoi(tokens[2]);
		stacks = stoi(tokens[3]);


		vector<string> coords;

		//Read and parse each row of points that divide 2 different stacks
		mat = mallocMatrix(stacks - 1, slices);

		for (int i = 0; getline(fd, line); i++) {
			tokens = parseLine(line, delimiter);

			for (int j = 0; j < tokens.size(); j++) {
				coords = parseLine(tokens[j], ",");
				mat[i][j] = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));
			}
		}

		Cone* c = (Cone*)malloc(sizeof(Cone));
		if (c != NULL)
			(*c) = Cone(base, height, slices, stacks, mat);

		return c;
	}

	return NULL;
}