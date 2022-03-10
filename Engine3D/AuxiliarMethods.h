#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Point.h"

Point** mallocMatrix(int linhas, int colunas);

void freeMatrix(Point** matriz, int linhas);

std::vector<std::string> parseLine(std::string s, std::string delimiter);