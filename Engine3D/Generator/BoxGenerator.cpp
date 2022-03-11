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

	std::vector<std::vector<Point>> mat_u = box.mat_u; 
	std::vector<std::vector<Point>> mat_t = box.mat_t;
	std::vector<std::vector<Point>> mat_f = box.mat_f;
	std::vector<std::vector<Point>> mat_b = box.mat_b;
	std::vector<std::vector<Point>> mat_r = box.mat_r;
	std::vector<std::vector<Point>> mat_l = box.mat_l; 
	Point p;

	for (int i = 0; i <= box.divisions; i++) { //Percorrer eixo dos Z's

		for (int j = 0; j <= box.divisions ; j++) { //Percorrer eixo dos X's
			p = mat_u[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_t[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_f[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_b[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_r[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
			p = mat_l[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}
		fich << std::endl;
	}
	fich.close();
}

Box* readBoxFromFile(std::ifstream& fd){
	float length;
	int divisions;
	std::vector<std::vector<Point>> mat_u;
	std::vector<std::vector<Point>> mat_t;
	std::vector<std::vector<Point>> mat_f;
	std::vector<std::vector<Point>> mat_b;
	std::vector<std::vector<Point>> mat_r;
	std::vector<std::vector<Point>> mat_l;


	if (!fd.is_open()) cout << "Open: No such file!";
	else{
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);

		vector<string> tokens = parseLine(line, delimiter);

		length = stof(tokens[0]);
		divisions = stoi(tokens[1]);

		mat_u.reserve(divisions + 1);
		mat_t.reserve(divisions + 1);
		mat_f.reserve(divisions + 1);
		mat_b.reserve(divisions + 1);
		mat_r.reserve(divisions + 1);
		mat_l.reserve(divisions + 1);

		vector<string> coords;

		for (int i = 0; getline(fd, line); i++){
			std::vector<Point> l_u,l_t,l_f,l_b,l_r,l_l;
			tokens = parseLine(line, delimiter);
			for (int j = 0, tokenctr = 0; j <= divisions; j++) {
				l_u.reserve(divisions);
				l_t.reserve(divisions);
				l_f.reserve(divisions);
				l_b.reserve(divisions);
				l_r.reserve(divisions);
				l_l.reserve(divisions);

				coords = parseLine(tokens[tokenctr++], ",");
				l_u.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_t.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_f.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_b.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_r.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
				coords = parseLine(tokens[tokenctr++], ",");
				l_l.push_back(Point(stof(coords[0]), stof(coords[1]), stof(coords[2])));
			}
			mat_u.push_back(l_u);
			mat_t.push_back(l_t);
			mat_f.push_back(l_f);
			mat_b.push_back(l_b);
			mat_r.push_back(l_r);
			mat_l.push_back(l_l);
		}

		return new  Box(length, divisions, mat_u, mat_t, mat_f, mat_b , mat_r, mat_l);
	}

	return NULL;
}