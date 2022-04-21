#include "catmull-rom.h"

void buildRotMatrix(float *x, float *y, float *z, float *m) {
	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {
	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {
	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};

	float aux_res[4];
	float t_vect[4] = {pow(t,3.0f),pow(t,2.0f),t,1.0f};
	float t_line_vect[4] = {3.0f*pow(t,2.0f),2.0f*t,1.0f,0.0f}; 
	
	for(int i = 0;i < 3;i++){
		float aux[4] = {p0[i],p1[i],p2[i],p3[i]};
		multMatrixVector((float*)m,(float*)aux,(float*)aux_res);

		pos[i] = 0;
		deriv[i] = 0;

		for(int j = 0; j < 4; j++){
			pos[i] += aux_res[j] * t_vect[j];
			deriv[i] += aux_res[j] * t_line_vect[j];
		}

	}	

}

void CatmullRomPoint(float gt, std::vector<Point> p,float *pos, float *deriv) {
	int p_size = p.size();
	float t = gt * p_size; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + p_size-1)%p_size;	
	indices[1] = (indices[0]+1)%p_size;
	indices[2] = (indices[1]+1)%p_size; 
	indices[3] = (indices[2]+1)%p_size;

	Point aux = p[indices[0]];
	float p0[3] = {aux.cx,aux.cy,aux.cz};
	aux = p[indices[1]];
	float p1[3] = {aux.cx,aux.cy,aux.cz};
	aux = p[indices[2]];
	float p2[3] = {aux.cx,aux.cy,aux.cz};
	aux = p[indices[3]];
	float p3[3] = {aux.cx,aux.cy,aux.cz};

	getCatmullRomPoint(t, p0, p1, p2, p3, pos, deriv);
}
