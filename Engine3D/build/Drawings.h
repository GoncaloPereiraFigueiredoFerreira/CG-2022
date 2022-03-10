#pragma once
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "../AuxiliarMethods.h"
#include "Solids.h"


void drawSphere(Sphere sphere);

void drawCone(Cone cone);

void drawPlane(Plane plane);

void drawBox(Box box);