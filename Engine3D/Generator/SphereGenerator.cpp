#include "SphereGenerator.h"

using namespace std;

void sphere(float radius, int slices, int stacks, vector<float> vvector, vector<unsigned int> ivector) {
    //Inicializacao do vector onde serao guardados os pontos
    unsigned int verticeCount = (stacks - 1) * slices + 2;
    vvector.resize(verticeCount * 3);

    //Preenchimento do vector
    int indexLowestP = ((int) verticeCount - 2) * 3;
    vvector[indexLowestP] = 0;
    vvector[indexLowestP + 1] = -radius;
    vvector[indexLowestP + 2] = 0;

    int indexHighestP = ((int) verticeCount - 1) * 3;
    vvector[indexHighestP] = 0;
    vvector[indexHighestP + 1] = radius;
    vvector[indexHighestP + 2] = 0;

    float stacksAngInc = M_PI / stacks;
    float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
    float ang, height, r;

    for (int i = 0; i < stacks - 1; i++) {
        height = radius * sin(-M_PI_2 + stacksAngInc * (float) (i + 1));
        r = radius * cos(-M_PI_2 + stacksAngInc * (float) (i + 1)); //raio da "circunferencia" atual

        //Preenche valores da stack com indice i
        for (int j = 0; j < slices; j++) {
            ang = slicesAngInc * j;
            int index = (i * slices + j) * 3;
            vvector[index] = r * sin(ang);
            vvector[index + 1] = height;
            vvector[index + 2] = r * cos(ang);
        }
    }

    //Draws the upper and lower stacks
    for (int j = 0; j < slices - 1; j++) {
        ivector.push_back(j);
        ivector.push_back(verticeCount - 2);
        ivector.push_back(j + 1);

        ivector.push_back(verticeCount - 1);
        ivector.push_back((stacks - 2) * slices + j);
        ivector.push_back((stacks - 2) * slices + j + 1);
    }

    //Desenha triangulos correspondentes � slice que completa uma volta
    ivector.push_back(slices - 1);
    ivector.push_back(verticeCount - 2);
    ivector.push_back(0);

    ivector.push_back(verticeCount - 1);
    ivector.push_back((stacks - 2) * slices + (slices - 1));
    ivector.push_back((stacks - 2) * slices);


    //Desenho das restantes stacks

    for (int i = 0; i < stacks - 2; i++) {

        for (int j = 0; j < slices - 1; j++) {

            /*    Desenha os triangulos com forma |\    */
            ivector.push_back(i * slices + j);
            ivector.push_back(i * slices + j + 1);
            ivector.push_back((i + 1) * slices + j);

            /*     Desenha os triangulos com forma \|    */
            ivector.push_back(i * slices + j + 1);
            ivector.push_back((i + 1) * slices + j + 1);
            ivector.push_back((i + 1) * slices + j);

        }


        //Desenha triangulos correspondentes � slice que completa uma volta

        /*    Desenha os triangulos com forma |\    */
        ivector.push_back(i * slices + slices - 1);
        ivector.push_back(i * slices);
        ivector.push_back((i + 1) * slices + slices - 1);

        /*     Desenha os triangulos com forma \|    */
        ivector.push_back(i * slices);
        ivector.push_back((i + 1) * slices);
        ivector.push_back((i + 1) * slices + slices - 1);
    }
}

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