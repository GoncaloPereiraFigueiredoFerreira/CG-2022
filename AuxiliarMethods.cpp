#include "AuxiliarMethods.h"

using namespace std;

Point** mallocMatrix(int linhas, int colunas) {
    Point** mat = (Point**)malloc(sizeof(Point*) * linhas);

    if(mat != NULL)
        for (int i = 0; i < linhas; i++)
            mat[i] = (Point*)malloc(sizeof(Point) * colunas);

    return mat;
}

void freeMatrix(Point** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) free(matriz[i]);
    free(matriz);
}

vector<string> parseLine(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}