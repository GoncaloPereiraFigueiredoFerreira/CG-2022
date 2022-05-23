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

inline void calc_point(vector<Point> points,int *index_patch,float t_x,float t_y,float *x){
    int coef[4] = {1,3,3,1}; //combinaçoes de 3 a index a index

    x[0] = 0;
    x[1] = 0;
    x[2] = 0;

    for(int aux_x = 0; aux_x < 4; aux_x++){
        float val_x = pow((double)t_x,aux_x) * (double)pow(1-t_x,3-aux_x) * coef[aux_x]; //calculo das potencias do eixo x
        float dev_x = 1;
        for(int aux_y = 0; aux_y < 4; aux_y++){
            float val_y = pow((double)t_y,aux_y) * (double)pow(1-t_y,3-aux_y) * coef[aux_y]; //calculo das potencias do eixo y
           
            x[0] += val_x * val_y * points[index_patch[aux_x * 4 + aux_y]].cx;
            x[1] += val_x * val_y * points[index_patch[aux_x * 4 + aux_y]].cy;
            x[2] += val_x * val_y * points[index_patch[aux_x * 4 + aux_y]].cz;

        }
    }
}

inline void dif_points(float *a,float *b,float *res){
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[2] = a[2] - b[2];
}

void bezier(char *path,int tessellation,vector<float> &vertex_VBO,vector<unsigned int> &index_VBO,vector<float> &normalB, vector<float>& textB){
    vector<Point> points;
    vector<vector<int>> index;
    float *vertex_arr;

    float x_ant[(tessellation + 1) * 3];
    float x_atu[(tessellation + 1) * 3];
    float t_y;

    readfile_bezier(path,points,index);

    int patchs = index.size();

    vertex_VBO.reserve(patchs * (tessellation + 1) * (tessellation + 1) * 16 * 3);

    float inc = 1.0f/tessellation;

    for(int p = 0; p < patchs; p++){
        int *index_patch = index[p].data(); // carrega index dos pontos do patch

        t_y = 0;
        for(int i = 0; i <= tessellation;i++){
            calc_point(points,index_patch,-inc,t_y,&x_ant[i*3]);
            calc_point(points,index_patch,0,t_y,&x_atu[i*3]);
            t_y += inc;
        }

        float t_x = inc;
        for(int i = 0; i <= tessellation; i++){
            float point_atu[3],point_ant[3];
            calc_point(points,index_patch,t_x,-inc,point_ant);
            calc_point(points,index_patch,t_x,0,point_atu);
            t_y = inc;
            for(int j = 0; j <= tessellation; j++){
                float point_aft[3];
                float points_dev_x[3] = {0},points_dev_y[3] = {0},points_normal[3] = {0};

                
                calc_point(points,index_patch,t_x,t_y,point_aft); 

                dif_points(point_aft,&x_ant[j*3],points_dev_x);
                dif_points(point_ant,point_aft,points_dev_y);

                getNormal(points_dev_x,points_dev_y,points_normal);

                vertex_VBO.push_back(x_atu[j*3+0]);
                vertex_VBO.push_back(x_atu[j*3+1]);
                vertex_VBO.push_back(x_atu[j*3+2]);

                textB.push_back(t_x - inc);
                textB.push_back(t_y - inc);

                normalB.push_back(points_normal[0]);
                normalB.push_back(points_normal[1]);
                normalB.push_back(points_normal[2]);

                point_ant[0] = point_atu[0];
                point_ant[1] = point_atu[1];
                point_ant[2] = point_atu[2];

                x_ant[j*3+0] = x_atu[j*3+0];
                x_ant[j*3+1] = x_atu[j*3+1];
                x_ant[j*3+2] = x_atu[j*3+2];

                x_atu[j*3+0] = point_atu[0] = point_aft[0];
                x_atu[j*3+1] = point_atu[1] = point_aft[1];
                x_atu[j*3+2] = point_atu[2] = point_aft[2];

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