#include "Generator.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc == 7 && strcmp(argv[1], "cone") == 0) { //CONE GENERATOR
		Cone c = generateCone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
		generateConeFile(argv[6], c);
	}
	else if (argc == 6 && strcmp(argv[1], "sphere") == 0) { //SPHERE GENERATOR
		Sphere s = generateSphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
		generateSphereFile(argv[5], s);
	}
	else if (argc == 5) {
		if (strcmp(argv[1], "plane") == 0) { //PLANE GENERATOR
			Plane p = generatePlane(atof(argv[2]), atoi(argv[3]));
			generatePlaneFile(argv[4], p);
		}
		else if (strcmp(argv[1], "box") == 0) { //BOX GENERATOR
			Box b = generateBox(atof(argv[2]), atoi(argv[3]));
			generateBoxFile(argv[4], b);

			std::ifstream fd; //debugging
			string line;
			fd.open(argv[4], ios::in);
			getline(fd, line);
			if (!line.compare("box")) {
				cout << "A ler box\n";
				b = *readBoxFromFile(fd);
				generateBoxFile("boxteste.3d", b);
			}//debugging
		}
	}
	else {
		//Write help code here
		cout << "Invalid arguments!\n";
	}
}