#include "TorusGenerator.h"
using namespace std;

void generateTorusFile(string filename, Torus torus) {
	ofstream fich;
	fich.open(filename, ios::out);
	/*
		Default separator of fields: ';'

		1st line: type
		2nd line: [;argument]
			- [;argument] is optional, and can be repeated, in this case for plane, we should write the length and the number of divisions

		Points format: "<coord x>,<coord y>,<coord z>
	*/

	fich << "torus" << "\n";

	fich << torus.inner_radius << ";" << torus.outer_radius << ";" << torus.slices << ";" << torus.stacks << "\n";

	auto mat = torus.mat; Point p;

	for (int i = 0; i < torus.stacks ; i++) { //Percorrer eixo dos Z's

		for (int j = 0; j < torus.slices - 1 ; j++) { //Percorrer eixo dos X's
			p = mat[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}

		p = mat[i][torus.slices - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << "\n";
	}
	fich.close();
}

Torus* readTorusFromFile(std::ifstream& fd) {
	float inner_radius,outer_radius;
	int slices,stacks;
	std::vector<std::vector<Point>> mat;

	if (!fd.is_open()) cout << "Open: No such file!";
	else {
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);

		vector<string> tokens = parseLine(line, delimiter);

		inner_radius = stof(tokens[0]);
		outer_radius = stoi(tokens[1]);
		slices = stoi(tokens[2]);
		stacks = stoi(tokens[3]);
		mat.reserve(stacks);


		vector<string> coords;

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

		return new Torus(inner_radius, outer_radius, slices,stacks,mat);
	}

	return NULL;
}