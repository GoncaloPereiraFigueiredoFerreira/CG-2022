#include "BoxGenerator.h"
using namespace std;

void generateBoxFile(string filename, Box box) {
	ofstream fich;
	fich.open(filename, ios::out);
	/*
		Default separator of fields: ';'

		1st line: type
		2nd line: [;argument]
			- [;argument] is optional, and can be repeated, in this case for plane, we should write the length and the number of divisions

		Points format: "<coord x>,<coord y>,<coord z>
	*/

	fich << "box" << "\n";

	fich << box.length << ";" << box.divisions << "\n";

	std::vector<std::vector<Point>> mat_b = box.mat_b; 
	std::vector<std::vector<Point>> mat_t = box.mat_t; 
	Point p;

	for (int i = 0; i < box.divisions; i++) { //Percorrer eixo dos Z's

		for (int j = 0; j < box.divisions - 1; j++) { //Percorrer eixo dos X's
			p = mat_b[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_t[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}

		p = mat_b[i][box.divisions - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << ";";
		p = mat_t[i][box.divisions - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << "\n";
	}
	fich.close();
}

Box* readBoxFromFile(std::ifstream& fd) {
	float length;
	int divisions;
	std::vector<std::vector<Point>> mat_b;
	std::vector<std::vector<Point>> mat_t;


	if (!fd.is_open()) cout << "Open: No such file!";
	else {
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);

		vector<string> tokens = parseLine(line, delimiter);

		length = stof(tokens[0]);
		divisions = stoi(tokens[1]);

		vector<string> coords;

		for (int i = 0; getline(fd, line); i++) {
			std::vector<Point> l_b, l_t;
			tokens = parseLine(line, delimiter);
			for (int j = 0, tokenctr = 0; j < divisions; j++) {
				coords = parseLine(tokens[tokenctr++], ",");
				l_b.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_t.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
			}
			mat_b.push_back(l_b);
			mat_t.push_back(l_t);
		}

		Box* b = (Box*) malloc(sizeof(Box));
		if (b != NULL)
			(*b) = Box(length, divisions, mat_b, mat_t);

		return b;
	}

	return NULL;
}