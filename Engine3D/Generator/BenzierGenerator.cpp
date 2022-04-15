#include "BenzierGenerator.h"
using namespace std;

//se n for negativo da problema 
inline int factorial(int n){
    int ret = 1;
    for(int i = 2; i <= n;i++){
        ret *= i;
    }
    return ret;
}

inline int combination(int n,int m){
    return factorial(n)/(factorial(m) * factorial(n-m));
}

int readfile_benzier(char *path,vector<vector<Point>> &ret){
    std::ifstream fd;
    fd.open(path, ios::in);

    if (fd.fail()) return -1;
    else {
        int size = 0;
        string line;
        string delimiter = ";";

        getline(fd, line);
        int tessellation = stoi(line);

        while(getline(fd, line)){
            vector<Point> aux;
            vector<string> points = parseLine(line, delimiter);
            aux.reserve(points.size());
            for(int i = 0; i < points.size() ; i++){
                vector<string> cord = parseLine(points[i],",");
                aux.push_back(Point(stof(cord[0]),stof(cord[1]),stof(cord[2])));
            }
            ret.push_back(aux);
        }

        return tessellation;
    }
}

void benzier_build(vector<vector<Point>> points,int tessellation,vector<float> &vertex,vector<unsigned int> &index){
    int size_x = points.size();
    int size_y = points[0].size();
    int coef[size_x][size_y];

    for(int i = 0; i < size_x;i++){
        for(int j = 0; j < size_y;j++){
            coef[i][j] = combination(size_x-1,i) * combination(size_y-1,j);
        }
    }

    double inc = 1.0f/(tessellation);
    double t_x = 0;


    for(int i = 0;i <= tessellation;i++){
        double t_y = 0;
        for(int j = 0; j <= tessellation;j++){ 
            double x = 0,y = 0,z = 0;

            for(int p_x = 0; p_x < size_x;p_x++){
                for(int p_y = 0; p_y < size_y;p_y++){
                    double val = pow((double)t_x,p_x) * pow(1-t_x,size_x-1-p_x);
                    val *= pow((double)t_y,p_y) * pow(1-t_y,size_y-1-p_y);
                    val *= (double)coef[p_x][p_y];
                    x += points[p_x][p_y].cx * val;
                    y += points[p_x][p_y].cy * val;
                    z += points[p_x][p_y].cz * val;
                }
            }

            vertex.push_back(x);
            vertex.push_back(y);
            vertex.push_back(z);

            t_y += inc;
        }
        t_x += inc;
    }


    for (int i = 1; i <= tessellation; i++) {
        for (int j = 1; j <= tessellation; j++) {

            index.push_back((i - 1) * (tessellation + 1) + (j - 1));
            index.push_back(i * (tessellation + 1) + (j - 1));
            index.push_back(i * (tessellation + 1) + j);

            index.push_back((i - 1) * (tessellation + 1) + (j - 1));
            index.push_back(i * (tessellation + 1) + j);
            index.push_back((i - 1) * (tessellation + 1) + j);
        }
    }
}

void benzier(char* path, std::vector<float>& vertexB, std::vector<unsigned int>& indexB){
    vector<vector<Point>> points;
    int tessellation = readfile_benzier(path,points);
    benzier_build(points,tessellation,vertexB,indexB);
}