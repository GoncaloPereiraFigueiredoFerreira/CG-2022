#include "BenzierGenerator.h"
using namespace std;


inline long long unsigned int combination(int n, int r){
    if (r == 0) return 1;

    /*
     Extra computation saving for large R,
     using property:
     N choose R = N choose (N-R)
    */
    if (r > n / 2) return combination(n, n - r); 

    long res = 1; 

    for (int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }

    return res;
}

int readfile_benzier(char *path,vector<Point> &points,vector<vector<int>> &index){
    std::ifstream fd;
    fd.open(path, ios::in);

    if (fd.fail()) return -1;
    else {
        int size = 0;
        string line;
        string delimiter = ",";

        getline(fd, line);
        int nlines = stoi(line);

        index.reserve(nlines);
        for(int i = 0; i < nlines;i++){
            getline(fd, line);
            vector<int> aux;
            vector<string> fields = parseLine(line, delimiter);
            size = fields.size();
            aux.reserve(size);
            for(int j = 0;j < size;j++){
                aux.push_back(stoi(fields[j]));
            }
            index.push_back(aux);
        }

        getline(fd, line);
        nlines = stoi(line);


        points.reserve(nlines);
        for(int i = 0; i < nlines;i++){
            getline(fd, line);
            vector<Point> aux;
            vector<string> fields = parseLine(line, delimiter);
            points.push_back(Point(stof(fields[0]),stof(fields[1]),stof(fields[2])));
        }
        return 0;
    }
}

void benzier(char *path,int tessellation,vector<float> &vertex_VBO,vector<unsigned int> &index_VBO){
    vector<Point> points;
    vector<vector<int>> index;

    vector<int> aux;

    readfile_benzier(path,points,index);
    //for(int i = 0; i < points.size();i++) printf("%f,%f,%f\n",points[i].cx,points[i].cy,points[i].cz);

    int size_x = index.size();
    int size_y = index[0].size();
    long unsigned int coef[size_x][size_y];


    for(int i = 0; i < 4;i++){
        for(int j = 0; j < 4;j++){
            coef[i][j] = combination(size_x-1,i) * combination(size_y-1,j);
        }
    }

    double inc = 1.0f/(tessellation);


    for(int p_x = 0; p_x < size_x;p_x+=4){
        for(int p_y = 0; p_y < size_y;p_y+=4){


            double t_x = 0;
            for(int i = 0; i < tessellation;i++){
                double t_y = 0;
                for(int j = 0; j < tessellation;j++){
                    double x = 0,y = 0,z = 0;
                    for(int aux_x = 0;aux_x < 4;aux_x++){
                        double val_x = pow((double)t_x,aux_x) * (double)pow(1-t_x,3-aux_x);      
                        for(int aux_y = 0;aux_y < 4;aux_y++){
                            double val = (double)(coef[aux_x][aux_y]) * val_x;
                            val *= pow((double)t_y,aux_y) * (double)pow(1-t_y,3-aux_y);
                            //printf("val_y:%lf\n",val);
                            x += ((double)points[index[p_x+aux_x][p_y+aux_y]].cx) * (double)val;
                            y += ((double)points[index[p_x+aux_x][p_y+aux_y]].cy) * (double)val;
                            z += ((double)points[index[p_x+aux_x][p_y+aux_y]].cz) * (double)val;
                        }
                    }
                    vertex_VBO.push_back((float)x);
                    vertex_VBO.push_back((float)y);
                    vertex_VBO.push_back((float)z);

                    t_y += inc;
                }
                t_x += inc;
            }

        }
    }



    for (int i = 1; i <= tessellation; i++) {
        for (int j = 1; j <= tessellation; j++) {

            index_VBO.push_back((i - 1) * (tessellation + 1) + (j - 1));
            index_VBO.push_back(i * (tessellation + 1) + (j - 1));
            index_VBO.push_back(i * (tessellation + 1) + j);

            index_VBO.push_back((i - 1) * (tessellation + 1) + (j - 1));
            index_VBO.push_back(i * (tessellation + 1) + j);
            index_VBO.push_back((i - 1) * (tessellation + 1) + j);
        }
    }
}