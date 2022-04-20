#include "BezierGenerator.h"
using namespace std;

int readfile_bezier(char *path,vector<Point> &points,vector<vector<int>> &index){
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

void bezier(char *path,int tessellation,vector<float> &vertex_VBO,vector<unsigned int> &index_VBO){
    vector<Point> points;
    vector<vector<int>> index;
    float *vertex_arr;

    readfile_bezier(path,points,index);

    int patchs = index.size();
    int coef[4] = {1,3,3,1}; //combinaçoes de 3 a index a index

    vertex_VBO.reserve(patchs * (tessellation + 1) * (tessellation + 1) * 16 * 3);

    float inc = 1.0f/tessellation;

    for(int p = 0; p < patchs; p++){
        int *index_patch = index[p].data(); // carrega index dos pontos do patch
        float t_x = 0;
        for(int i = 0; i <= tessellation; i++){
            float t_y = 0;
            for(int j = 0; j <= tessellation; j++){
                float x = 0,y = 0,z = 0;
                for(int aux_x = 0; aux_x < 4; aux_x++){
                    float val_x = pow((double)t_x,aux_x) * (double)pow(1-t_x,3-aux_x) * coef[aux_x]; //calculo das potencias do eixo x
                    for(int aux_y = 0; aux_y < 4; aux_y++){
                        float val = pow((double)t_y,aux_y) * (double)pow(1-t_y,3-aux_y) * coef[aux_y] * val_x; //calculo das potencias do eixo y
                        x += val * points[index_patch[aux_x * 4 + aux_y]].cx;
                        y += val * points[index_patch[aux_x * 4 + aux_y]].cy;
                        z += val * points[index_patch[aux_x * 4 + aux_y]].cz;

                    }
                }
                vertex_VBO.push_back(x);
                vertex_VBO.push_back(y);
                vertex_VBO.push_back(z);

                t_y += inc;
            }

            t_x += inc;
        }

    }

    index_VBO.reserve((tessellation + 1) * patchs * (tessellation + 1) );

    for (int i = 0; i <  patchs; i++) {
            for(int i_p = 1; i_p <= tessellation;i_p++){
                for(int j_p = 1; j_p <= tessellation;j_p++){
                    int patch = i * (tessellation + 1) * (tessellation + 1);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + j_p);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + (j_p - 1));
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + (j_p - 1));

                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + (j_p - 1));
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + j_p);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + j_p);
                }
            }
        
    }

}