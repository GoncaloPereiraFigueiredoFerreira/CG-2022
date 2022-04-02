#include "Generator.h"
using namespace std;

int main(int argc, char** argv)
{
	if (argc == 7) { 
		if(strcmp(argv[1], "cone") == 0 && atoi(argv[5]) >= 1 && atoi(argv[4]) >= 1){//CONE GENERATOR
			Cone c = Cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
			generateConeFile(argv[6], c);
		}
		else if(strcmp(argv[1], "torus") == 0 && atoi(argv[5]) >= 1 && atoi(argv[4]) >= 1){//TORUS GENERATOR
			Torus c = Torus(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
			generateTorusFile(argv[6], c);
		}else printf("Invalid arguments!\n");
	}
	else if (argc == 6 && strcmp(argv[1], "sphere") == 0 && atoi(argv[3]) >= 1 && atoi(argv[4]) >= 1) { //SPHERE GENERATOR
		Sphere s = Sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
		generateSphereFile(argv[5], s);
	}
	else if (argc == 5) {
		if (strcmp(argv[1], "plane") == 0 && atoi(argv[3]) >= 1) { //PLANE GENERATOR
			Plane p = Plane(atof(argv[2]), atoi(argv[3]));
			generatePlaneFile(argv[4], p);
		}
		else if (strcmp(argv[1], "box") == 0 && atoi(argv[3]) >= 1) { //BOX GENERATOR
			Box b = Box(atof(argv[2]), atoi(argv[3]));
			generateBoxFile(argv[4], b);

			/*std::ifstream fd; //debugging
			string line;
			fd.open(argv[4], ios::in);
			getline(fd, line);
			if (!line.compare("box")) {
				cout << "A ler box\n";
				b = *readBoxFromFile(fd);
				generateBoxFile("boxteste.3d", b);
			}//debugging*/
		}else printf("Invalid arguments!\n");
	}
	else {
		//Write help code here
		cout << "Invalid arguments!\n";
	}
}