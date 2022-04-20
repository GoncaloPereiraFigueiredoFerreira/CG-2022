#include "BenzierGenerator.h"
using namespace std;

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
    float *vertex_arr;

    vector<int> aux;

    readfile_benzier(path,points,index);
    //for(int i = 0; i < points.size();i++) printf("%f,%f,%f\n",points[i].cx,points[i].cy,points[i].cz);

    int size_x = index.size();
    int size_y = index[0].size();
    int patchs_x = size_x/4;
    int patchs_y = size_y/4;
    int coef[4] = {1,3,3,1};

    /*cout << "tam:" <<patchs_x * patchs_y * (tessellation + 1) * (tessellation + 1) * 3 << endl;
    vertex_VBO.resize(patchs_x * patchs_y * (tessellation + 1) * (tessellation + 1) * 3);
    vertex_arr = vertex_VBO.data();*/

    vertex_VBO.reserve(patchs_x * patchs_y * (tessellation + 1) * (tessellation + 1) * 3);

    float inc = 1.0f/tessellation;

    for(int p_x = 0; p_x < size_x; p_x += 4){
        for(int p_y = 0; p_y < size_y;p_y += 4){
            float t_x = 0;
            for(int i = 0; i <= tessellation; i++){
                float t_y = 0;
                for(int j = 0; j <= tessellation; j++){
                    float x = 0,y = 0,z = 0;
                    for(int aux_x = 0; aux_x < 4; aux_x++){
                        float val_x = pow((double)t_x,aux_x) * (double)pow(1-t_x,3-aux_x) * coef[aux_x];
                        for(int aux_y = 0; aux_y < 4; aux_y++){
                            float val = pow((double)t_y,aux_y) * (double)pow(1-t_y,3-aux_y) * coef[aux_y] * val_x;

                            x += val * points[index[p_x + aux_x][p_y + aux_y]].cx;
                            y += val * points[index[p_x + aux_x][p_y + aux_y]].cy;
                            z += val * points[index[p_x + aux_x][p_y + aux_y]].cz;

                        }
                    }
                    /*int ind_VBO = (p_x/4) * patchs_y + (p_y/4);
                    ind_VBO = ind_VBO * (tessellation + 1) * (tessellation + 1) + i * (tessellation + 1) + j;
                    cout << "ind:" << ind_VBO  << endl;
                    vertex_arr[ind_VBO * 3] = x;
                    vertex_arr[ind_VBO * 3 + 1] = y;
                    vertex_arr[ind_VBO * 3 + 2] = z;*/

                    vertex_VBO.push_back(x);
                    vertex_VBO.push_back(y);
                    vertex_VBO.push_back(z);

                    t_y += inc;
                }

                t_x += inc;
            }
        }
    }

    cout << "ola" << endl;
    cout << (tessellation + 1) * patchs_x * (tessellation + 1) * patchs_y << endl;
    index_VBO.reserve((tessellation + 1) * patchs_x * (tessellation + 1) * patchs_y);

    for (int i = 0; i <  patchs_x; i++) {
        for (int j = 0; j <  patchs_y; j++) {
            for(int i_p = 1; i_p <= tessellation;i_p++){
                for(int j_p = 1; j_p <= tessellation;j_p++){
                    int patch = i * j * (tessellation + 1) * (tessellation + 1);
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) * patchs_y + (j_p - 1));
                    index_VBO.push_back(patch + i_p * (tessellation + 1) * patchs_y + j_p);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) * patchs_y + (j_p - 1));

                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) * patchs_y + (j_p - 1));
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) * patchs_y + j_p);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) * patchs_y + j_p);
                }
            }
        
        }
    }

}