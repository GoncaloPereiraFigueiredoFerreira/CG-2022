// SolidGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "SphereGenerator.h"
#include <vector>

using namespace std;

int main(int argc, char**argv)
{    
    if (argc > 2) {
        Sphere s = generateSphere(2.0f, 10, 10);
        generateSphereFile("sphere.3d", s);

        std::ifstream fd;
        string line;
        fd.open("sphere.3d", ios::in);
        getline(fd, line);
        if(!line.compare("sphere")){
            cout << "Entrou\n";
            s = *readSphereFromFile(fd);
        }
    }
    else {
        //Write help code here
        cout << "Invalid arguments!\n";
    }
}

