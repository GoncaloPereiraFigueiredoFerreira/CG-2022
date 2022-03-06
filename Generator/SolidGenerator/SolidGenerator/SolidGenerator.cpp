// SolidGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "ConeGenerator.h"
#include "SphereGenerator.h"
#include "PlaneGenerator.h"
#include "BoxGenerator.h"
#include <vector>


using namespace std;

int main(int argc, char**argv)
{    
    if (argc == 7 && strcmp(argv[1], "cone") == 0) { //CONE GENERATOR
        Cone c = generateCone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
        generateConeFile(argv[6], c);

        std::ifstream fd; //debugging
        string line;
        fd.open(argv[6], ios::in);
        getline(fd, line);
        if (!line.compare("cone")) {
            cout << "Reading cone\n";
            c = *readConeFromFile(fd);
            generateConeFile("coneTeste.3d", c);
        } //debugging
    }
    else if (argc == 6 && strcmp(argv[1], "sphere") == 0) { //SPHERE GENERATOR
        Sphere s = generateSphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
        generateSphereFile(argv[5], s);

        std::ifstream fd; //debugging
        string line;
        fd.open(argv[5], ios::in);
        getline(fd, line);
        if (!line.compare("sphere")) {
            cout << "Entrou\n";
            s = *readSphereFromFile(fd);
        } //debugging
    }
    else if (argc == 5){
        if (strcmp(argv[1], "plane") == 0) { //PLANE GENERATOR
            Plane p = generatePlane(atof(argv[2]), atoi(argv[3]));
            generatePlaneFile(argv[4], p);

            std::ifstream fd; //debugging
            string line;
            fd.open(argv[4], ios::in);
            getline(fd, line);
            if (!line.compare("plane")) {
                cout << "A ler plano\n";
                p = *readPlaneFromFile(fd);
                generatePlaneFile("planoteste.3d", p);
            }//debugging
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

