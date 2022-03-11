#include "PlaneGenerator.h"
using namespace std;

void generatePlaneFile(string filename, Plane plane) {
	ofstream fich;
	fich.open(filename, ios::out);
	/*
		Default separator of fields: ';'

		1st line: type
		2nd line: [;argument]
			- [;argument] is optional, and can be repeated, in this case for plane, we should write the length and the number of divisions

		Points format: "<coord x>,<coord y>,<coord z>
	*/

	fich << "plane" << "\n";

	fich << plane.length << ";" << plane.divisions << "\n";

	auto mat = plane.mat; Point p;

	for (int i = 0; i < plane.divisions; i++) { //Percorrer eixo dos Z's

		for (int j = 0; j < plane.divisions - 1; j++) { //Percorrer eixo dos X's
			p = mat[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}

		p = mat[i][plane.divisions - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << "\n";
	}
	fich.close();
}

Plane* readPlaneFromFile(std::ifstream& fd) {
	float length;
	int divisions;
	std::vector<std::vector<Point>> mat;

	if (!fd.is_open()) cout << "Open: No such file!";
	else {
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);

		vector<string> tokens = parseLine(line, delimiter);

		length = stof(tokens[0]);
		divisions = stoi(tokens[1]);
		mat.reserve(divisions + 1);


		vector<string> coords;

		for (int i = 0; getline(fd, line); i++) {
			std::vector<Point> l;
			l.reserve(divisions + 1);
			tokens = parseLine(line, delimiter);

			for (int j = 0; j < tokens.size(); j++) {
				coords = parseLine(tokens[j], ",");
				l.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
			}

			mat.push_back(l);
		}

		return new Plane(length, divisions, mat);
	}

	return NULL;
}