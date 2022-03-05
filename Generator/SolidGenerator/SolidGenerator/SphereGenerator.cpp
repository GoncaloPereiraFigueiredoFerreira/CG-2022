#include "SphereGenerator.h"
#include "AuxiliarMethods.h"

using namespace std;

Sphere generateSphere(float radius, int slices, int stacks) {
    //Inicializacao da matriz onde serao guardados os pontos
    Point** mat = mallocMatrix(stacks - 1, slices);
    float stacksAngInc = M_PI / stacks;
    float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
    float ang, height, r;

    for (int i = 0; i < stacks - 1; i++) {
        height = radius * sin(-M_PI_2 + stacksAngInc * (float) (i + 1));
        r = radius * cos(-M_PI_2 + stacksAngInc * (float) (i + 1)); //raio da "circunferencia" atual

        for (int j = 0; j < slices; j++) {
            ang = slicesAngInc * j;
            mat[i][j] = Point(r * cos(ang), height, r * sin(ang));
        }
    }

    //Ponto cuja coordenada y é a mais baixa
    Point lowestP = Point(0, - radius, 0);

    //Ponto cuja coordenada y é a mais alta
    Point highestP = Point(0, radius, 0);

    return Sphere(radius, slices, stacks, lowestP, highestP, mat);
}


void generateSphereFile(string filename, Sphere sphere) {
    ofstream fich;
    fich.open(filename, ios::out);
    /* 
        Default separator of fields: ';'

        1st line: type
        2nd line: [;argument]
            - [;argument] is optional, and can be repeated, in this case for sphere, we should write the radius, number of stacks and the number of slices
        
        Points format: "<coord x>,<coord y>,<coord z>
    */

    fich << "sphere" << "\n";

    fich << sphere.radius << ";" << sphere.slices << ";" << sphere.stacks << "\n";

    //3rd line is the point with lowest y
    fich << sphere.lowestP.cx << "," << sphere.lowestP.cy << "," << sphere.lowestP.cz << "\n";

    //4th line is the point with highest z
    fich << sphere.highestP.cx << "," << sphere.highestP.cy << "," << sphere.highestP.cz << "\n";

    //Each row has the all the points that divide 2 different stacks
    Point** mat = sphere.mat; Point p;

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

Sphere* readSphereFromFile(std::ifstream& fd) {
    float radius;
    int slices, stacks;
    Point lowestP, highestP;
    Point** mat;

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
        mat = mallocMatrix(stacks - 1, slices);

        for (int i = 0; getline(fd, line); i++) {
            tokens = parseLine(line, delimiter);

            for (int j = 0; j < tokens.size(); j++) {
                coords = parseLine(tokens[j], ",");
                mat[i][j] = Point(stof(coords[0]), stof(coords[1]), stof(coords[2]));
            }
        }

        Sphere* s = (Sphere*)malloc(sizeof(Sphere));
        if (s != NULL)
            (*s) = Sphere(radius, slices, stacks, lowestP, highestP, mat);

        return s;
    }

    return NULL;
}