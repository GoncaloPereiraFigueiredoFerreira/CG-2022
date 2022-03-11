#include "SphereGenerator.h"

using namespace std;

void generateSphereFile(string filename, Sphere sphere) {
	ofstream fich;
	fich.open(filename, ios::out);

	fich << "sphere" << "\n";

	fich << sphere.radius << ";" << sphere.slices << ";" << sphere.stacks << "\n";

	//3rd line is the point with lowest y
	fich << sphere.lowestP.cx << "," << sphere.lowestP.cy << "," << sphere.lowestP.cz << "\n";

	//4th line is the point with highest z
	fich << sphere.highestP.cx << "," << sphere.highestP.cy << "," << sphere.highestP.cz << "\n";

	//Each row has the all the points that divide 2 different stacks
	std::vector<std::vector<Point>> mat = sphere.mat; Point p;

	for (int i = 0; i < sphere.stacks - 1; i++) {

		for (int j = 0; j < sphere.slices - 1; j++) {
			p = mat[i][j];
			fich << p.cx << "," << p.cy << "," << p.cz << ";";
		}

		p = mat[i][sphere.slices - 1];
		fich << p.cx << "," << p.cy << "," << p.cz << "\n";
	}
	fich.close();
}

// File descriptor should point to the start of the second line
// (first line of the file must indicate the type of object)
Sphere* readSphereFromFile(std::ifstream& fd) {
	float radius;
	int slices, stacks;
	Point lowestP, highestP;
	std::vector<std::vector<Point>> mat;
	int k = 1;

	if (!fd.is_open()) cout << "Open: No such file!";
	else {
		string line;
		string delimiter = ";";

		//Read and parse first line
		getline(fd, line);
		vector<string> tokens = parseLine(line, delimiter);
		radius = stof(tokens[0]);
		slices = stoi(tokens[1]);
		stacks = stoi(tokens[2]);


		//Reserves space for the vectors where the points will be stored
		mat.reserve(stacks - 1);


		vector<string> coords;

		//Read and parse lowest point
		getline(fd, line);
		tokens = parseLine(line, delimiter);
		coords = parseLine(tokens[0], ",");
		lowestP = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));


		//Read and parse highest point
		getline(fd, line);
		tokens = parseLine(line, delimiter);
		coords = parseLine(tokens[0], ",");
		highestP = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));


		//Read and parse each row of points that divide 2 different stacks

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

		return new Sphere(radius, slices, stacks, lowestP, highestP, mat);
	}

	return NULL;
}