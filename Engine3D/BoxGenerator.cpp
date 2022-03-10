#include "AuxiliarMethods.h"
#include "BoxGenerator.h"
using namespace std;

Box generateBox(float lenght, int div) {
	float cx = 0, cz = 0;
	float incr = lenght / div; //side increment value
	float cy_b = 0; //bottom y
	float cy_t = incr * div; //top y
	Point** mat_b = mallocMatrix(div + 1, div + 1);
	Point** mat_t = mallocMatrix(div + 1, div + 1);

	for (int i = 0; i <= div;i++) {
		for (int j = 0;j <= div;j++) {
			Point p_b = Point(cx, cy_b, cz);
			Point p_t = Point(cx, cy_t, cz);
			mat_b[i][j] = p_b;
			mat_t[i][j] = p_t;
			cx += incr;
		}
		cz += incr;
		cx = 0;
	}
	return Box(lenght, div, mat_b, mat_t);
}

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

	Point** mat_b = box.mat_b; Point** mat_t = box.mat_t; Point p;

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
	Point** mat_b;
	Point** mat_t;


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

		mat_b = mallocMatrix(divisions + 1, divisions + 1);
		mat_t = mallocMatrix(divisions + 1, divisions + 1);

		for (int i = 0; getline(fd, line); i++) {
			tokens = parseLine(line, delimiter);
			for (int j = 0, tokenctr = 0; j < divisions; j++) {
				coords = parseLine(tokens[tokenctr++], ",");
				mat_b[i][j] = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));
				coords = parseLine(tokens[tokenctr++], ",");
				mat_t[i][j] = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));
			}
		}

		Box* b = (Box*)malloc(sizeof(Box));
		if (b != NULL)
			(*b) = Box(length, divisions, mat_b, mat_t);

		return b;
	}

	return NULL;
}