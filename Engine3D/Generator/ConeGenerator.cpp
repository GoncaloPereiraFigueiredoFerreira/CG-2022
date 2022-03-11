#include "ConeGenerator.h"
using namespace std;

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

	//3rd line is the point with lowest y
	fich << cone.lowestP.cx << "," << cone.lowestP.cy << "," << cone.lowestP.cz << "\n";

	//4th line is the point with highest z
	fich << cone.highestP.cx << "," << cone.highestP.cy << "," << cone.highestP.cz << "\n";

	auto mat = cone.mat; Point p;

	for (int i = 0; i < cone.stacks; i++) {

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
	std::vector<std::vector<Point>> mat;

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

		//Read and parse lowest point
		getline(fd, line);
		tokens = parseLine(line, delimiter);
		coords = parseLine(tokens[0], ",");
		Point lowestP = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));


		//Read and parse highest point
		getline(fd, line);
		tokens = parseLine(line, delimiter);
		coords = parseLine(tokens[0], ",");
		Point highestP = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));


		//Read and parse each row of points that divide 2 different stacks
		mat.reserve(stacks);

		for (int i = 0; getline(fd, line); i++) {
			std::vector<Point> l;
			l.reserve(slices);
			tokens = parseLine(line, delimiter);

			for (int j = 0; j < tokens.size(); j++) {
				coords = parseLine(tokens[j], ",");
				l.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
			}

			mat.push_back(l);
		}

		return new Cone(base, height, slices, stacks, lowestP, highestP, mat);
	}

	return NULL;
}