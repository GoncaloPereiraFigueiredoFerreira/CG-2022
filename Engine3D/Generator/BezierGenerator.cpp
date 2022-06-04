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

inline void multVecVec(float *v_x,float *v_y,float *v_aux){

    for(int i = 0; i < 4;i++){
                v_aux[0] += v_x[i] * v_y[i*3];

                v_aux[1] += v_x[i] * v_y[i*3+1];

                v_aux[2] += v_x[i] * v_y[i*3+2];

            }
}

inline void multMatrixVec(float *m_points,float *v_y,float *v_aux){

    for(int i = 0; i < 4;i++){
                v_aux[i*3] = m_points[i*4*3    ] * v_y[0] + 
                             m_points[i*4*3 + 3] * v_y[1] +
                             m_points[i*4*3 + 6] * v_y[2] +
                             m_points[i*4*3 + 9] * v_y[3];

                v_aux[i*3+1] = m_points[i*4*3 + 1 ] * v_y[0] + 
                               m_points[i*4*3 + 4 ] * v_y[1] +
                               m_points[i*4*3 + 7 ] * v_y[2] +
                               m_points[i*4*3 + 10] * v_y[3];

                v_aux[i*3+2] = m_points[i*4*3 + 2 ] * v_y[0] + 
                               m_points[i*4*3 + 5 ] * v_y[1] +
                               m_points[i*4*3 + 8 ] * v_y[2] +
                               m_points[i*4*3 + 11] * v_y[3];
            }
}

void bezier(char *path,int tessellation,vector<float> &vertex_VBO,vector<unsigned int> &index_VBO,vector<float> &normalB, vector<float>& textB){
    vector<Point> points;
    vector<vector<int>> index;

    readfile_bezier(path,points,index);

    int patchs = index.size();

    vertex_VBO.reserve(patchs * (tessellation + 1) * (tessellation + 1) * 16 * 3);

    float inc = 1.0f/tessellation;
    float m[16] = {-1, 3,-3, 1,
                    3,-6, 3, 0,
                   -3, 3, 0, 0,
                    1, 0, 0, 0};
    float m_aux[16 * 3];
    float m_points[16 * 3];

    for(int p = 0; p < patchs; p++){
        int *index_patch = index[p].data(); // carrega index dos pontos do patch

        //preenche m_points com os pontos da patch matrix 4*4 de pontos

        for(int i = 0; i < 16;i++) {
            m_points[i * 3]     = points[index_patch[i]].cx;
            m_points[i * 3 + 1] = points[index_patch[i]].cy;
            m_points[i * 3 + 2] = points[index_patch[i]].cz;
        } 

        //primeira multiplicação de matrizes m*points

        for(int i = 0; i < 16;i++){
            int c = i % 4;
            int l = i / 4;

            m_aux[i*3]   = 0;
            m_aux[i*3+1] = 0;
            m_aux[i*3+2] = 0;

            for(int j = 0;j < 4;j++) {
                m_aux[i*3]   += m[l*4+j] * m_points[(j*4+c) * 3];
                m_aux[i*3+1] += m[l*4+j] * m_points[(j*4+c) * 3+1];
                m_aux[i*3+2] += m[l*4+j] * m_points[(j*4+c) * 3+2];
            }
        }

        //segunda multiplicação de matrizes points*mt

        for(int i = 0; i < 16;i++){
            int c = i % 4;
            int l = i / 4;

            m_points[i*3]   = 0;
            m_points[i*3+1] = 0;
            m_points[i*3+2] = 0;

            for(int j = 0;j < 4;j++) {
                m_points[i*3]   += m_aux[(l*4+j) * 3  ] * m[j*4+c];
                m_points[i*3+1] += m_aux[(l*4+j) * 3+1] * m[j*4+c];
                m_points[i*3+2] += m_aux[(l*4+j) * 3+2] * m[j*4+c];
            }
        }

        float t_y = 0;

        for(int y = 0; y <= tessellation;y++){
            float vy_aux[12] = {0};
            float vy_aux_d[12] = {0};
            float v_y[4] = {pow(t_y,3.0f),pow(t_y,2.0f),t_y,1};
            float v_y_d[4] = {3 * pow(t_y,2.0f),2 * t_y,1,0};

            multMatrixVec(m_points,v_y,vy_aux);
            multMatrixVec(m_points,v_y_d,vy_aux_d);

            float t_x = 0;

            for(int x = 0; x <= tessellation;x++){
                float p[3] = {0};
                float p_d_x[3] = {0};
                float p_d_y[3] = {0};
                float p_d[3] = {0};
                float v_x[4] = {pow(t_x,3.0f),pow(t_x,2.0f),t_x,1};
                float v_x_d[4] = {3 * pow(t_x,2.0f),2 * t_x,1,0};

                multVecVec(v_x,vy_aux,p);
                multVecVec(v_x,vy_aux_d,p_d_x); //derivada segundo x
                multVecVec(v_x_d,vy_aux,p_d_y); //derivada segundo y

                getNormal(p_d_x,p_d_y,p_d); 

                for(int i = 0;i<3;i++){vertex_VBO.push_back(p[i]);normalB.push_back(p_d[i]);}

                textB.push_back(float((tessellation) - y)/float(tessellation));
                textB.push_back(float((tessellation) - x)/float(tessellation));

                t_x += inc;
            }

            t_y += inc;

        }



    }

    index_VBO.reserve((tessellation + 1) * patchs * (tessellation + 1) );

    for (int i = 0; i <  patchs; i++) {
            for(int i_p = 1; i_p <= tessellation;i_p++){
                for(int j_p = 1; j_p <= tessellation;j_p++){
                    int patch = i * (tessellation + 1) * (tessellation + 1);
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + j_p);
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + (j_p - 1));
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + (j_p - 1));

                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + (j_p - 1));
                    index_VBO.push_back(patch + i_p * (tessellation + 1) + j_p);
                    index_VBO.push_back(patch + (i_p - 1) * (tessellation + 1) + j_p);
                }
            }
        
    }

}