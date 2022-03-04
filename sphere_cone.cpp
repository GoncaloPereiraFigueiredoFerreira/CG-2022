#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>

float angle_y = 0.0f;
float angle_x = 0.0f;
float angle_z = 0.0f;
float scale = 1.0f;

class MyPoint{
public:
	float cx;
	float cy;
	float cz;
	MyPoint(){}
	MyPoint(float x,float y,float z){
		cx=x;
		cy=y;
		cz=z;
	}
};



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y){
	switch (key){
		case 'y':
			angle_y += 5.0f;
			break;
		case 'x':
			angle_x += 5.0f;
			break;
		case 'z':
			angle_z += 5.0f;
			break;
		case 's':
			scale += 0.1f;
			break;
		case 'd':
			scale -= 0.1f;
			break;
	}
	glutPostRedisplay();
}

void drawSphere(float radius, int slices, int stacks) {
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;

    //Inicializacao da matriz onde serao guardados os pontos
    MyPoint** mat = (MyPoint**) malloc(sizeof(MyPoint*) * (stacks - 1));
    for (int i = 0; i < stacks - 1; i++)
        mat[i] = (MyPoint*) malloc(sizeof(MyPoint) * slices);

    float stacksAngInc = M_PI / stacks; 
    float slicesAngInc = 2 * M_PI / slices; //Valor do angulo a aumentar entre cada ponto que define uma slice
    float ang, height, r;

    for (int i = 0; i < stacks - 1; i++) {
        height = cy + radius * sin(-M_PI_2 + stacksAngInc * (i + 1));
        r = radius * cos(-M_PI_2 + stacksAngInc * (i + 1)); //raio da "circunferencia" atual

        for (int j = 0; j < slices; j++) {
            ang       = slicesAngInc * j;
            mat[i][j] = MyPoint(cx + r * cos(ang), height, cz + r * sin(ang));
        }
    }

    //Definicao do modo que mostra apenas a linha que define o limite dos triangulos
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);

    //Desenho da stack de baixo e de cima

        //Inicializacao do ponto cuja coordenada y é a mais baixa
        MyPoint lowestP = MyPoint(cx, cy - radius, cz);

        //Inicializacao do ponto cuja coordenada y é a mais alta
        MyPoint highestP = MyPoint(cx, cy + radius, cz);

        for (int j = 0; j < slices - 1; j++) {
            glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
            glVertex3f(mat[0][j].cx, mat[0][j].cy, mat[0][j].cz);
            glVertex3f(mat[0][j + 1].cx, mat[0][j + 1].cy, mat[0][j + 1].cz);

            glVertex3f(mat[stacks - 2][j].cx, mat[stacks - 2][j].cy, mat[stacks - 2][j].cz);
            glVertex3f(highestP.cx, highestP.cy, highestP.cz);
            glVertex3f(mat[stacks - 2][j + 1].cx, mat[stacks - 2][j + 1].cy, mat[stacks - 2][j + 1].cz);
        }

        //Desenha triangulos correspondentes à slice que completa uma volta
        glVertex3f(lowestP.cx, lowestP.cy, lowestP.cz);
        glVertex3f(mat[0][slices - 1].cx, mat[0][slices - 1].cy, mat[0][slices - 1].cz);
        glVertex3f(mat[0][0].cx, mat[0][0].cy, mat[0][0].cz);

        glVertex3f(mat[stacks - 2][slices - 1].cx, mat[stacks - 2][slices - 1].cy, mat[stacks - 2][slices - 1].cz);
        glVertex3f(highestP.cx, highestP.cy, highestP.cz);
        glVertex3f(mat[stacks - 2][0].cx, mat[stacks - 2][0].cy, mat[stacks - 2][0].cz);


    //Desenho das restantes stacks
    MyPoint *p1, *p2, *p3;

    for (int i = stacks - 2; i > 0; i--) {

        for (int j = 0; j < slices - 1; j++) {

            /*    Desenha os triangulos com forma |\    */

            p1 = &mat[i - 1][j + 1]; p2 = &mat[i][j + 1]; p3 = &mat[i - 1][j];

            glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
            glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
            glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);

            /*     Desenha os triangulos com forma \|    */

            p1 = &mat[i][j];

            glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
            glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
            glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
        }


        //Desenha triangulos correspondentes à slice que completa uma volta

            /*    Desenha os triangulos com forma |\    */

            p1 = &mat[i - 1][0]; p2 = &mat[i][0]; p3 = &mat[i - 1][slices - 1];

            glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
            glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
            glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);

            /*     Desenha os triangulos com forma \|    */

            p1 = &mat[i][slices - 1];

            glVertex3f((*p1).cx, (*p1).cy, (*p1).cz);
            glVertex3f((*p2).cx, (*p2).cy, (*p2).cz);
            glVertex3f((*p3).cx, (*p3).cy, (*p3).cz);
    }

    glEnd();


    //Liberta a memoria alocada
    for (int i = 0; i < stacks - 1; i++) free(mat[i]);
    free(mat);
}


void drawCone(float base, float height, int slices, int stacks){
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    MyPoint mat[stacks][slices];
    float angInc = 2 * M_PI / slices, //angle increment value
    heightInc = height / stacks,
    radiusDec = base / stacks;
    float ang, h, r;

    glColor3f(0.5f, 0.2f, 0.2f);

    for(int i = 0; i < stacks; i++){
        h = cz + heightInc * i;
        r = base - radiusDec * i;

        glBegin(GL_TRIANGLES);
        ang = 0;
        MyPoint p = MyPoint(cx + r * cos(ang), cy + r * sin(ang), h);
        mat[i][0] = p;

        for(int j = 1; j < slices; j++){
            ang = angInc * j;
            MyPoint p = MyPoint(cx + r * cos(ang), cy + r * sin(ang), h);
            mat[i][j] = p;

            if(i){
                glColor3f(0.5f, 0.2f, 0.2f);
                glVertex3f(mat[i-1][j].cx,mat[i-1][j].cy,mat[i-1][j].cz);
                glVertex3f(p.cx, p.cy, p.cz);
                glVertex3f(mat[i][j-1].cx,mat[i][j-1].cy,mat[i][j-1].cz);

                glColor3f(0.2f, 0.2f, 0.5f);

                glVertex3f(mat[i-1][j].cx,mat[i-1][j].cy,mat[i-1][j].cz);
                glVertex3f(mat[i][j-1].cx,mat[i][j-1].cy,mat[i][j-1].cz);
                glVertex3f(mat[i-1][j-1].cx,mat[i-1][j-1].cy,mat[i-1][j-1].cz);
            }
        }

            //dar a volta aos indices
        if(i){
            glColor3f(0.5f, 0.2f, 0.2f);
            glVertex3f(mat[i-1][0].cx,mat[i-1][0].cy,mat[i-1][0].cz);
            glVertex3f(mat[i][0].cx,mat[i][0].cy,mat[i][0].cz);
            glVertex3f(mat[i][slices-1].cx,mat[i][slices-1].cy,mat[i][slices-1].cz);

            glColor3f(0.2f, 0.2f, 0.5f);

            glVertex3f(mat[i-1][0].cx,mat[i-1][0].cy,mat[i-1][0].cz);
            glVertex3f(mat[i][slices-1].cx,mat[i][slices-1].cy,mat[i][slices-1].cz);
            glVertex3f(mat[i-1][slices-1].cx,mat[i-1][slices-1].cy,mat[i-1][slices-1].cz);
        }

        glEnd();
    }

    //faz pontas
    for(int i = 1; i < slices;i++){
        glBegin(GL_TRIANGLES);

        glColor3f(0.2f, 0.5f, 0.2f);

        glVertex3f(mat[0][i].cx,mat[0][i].cy,mat[0][i].cz);
        glVertex3f(mat[0][i-1].cx,mat[0][i-1].cy,mat[0][i-1].cz);
        glVertex3f(cx,cy,cz);

        glVertex3f(mat[stacks-1][i].cx,mat[stacks-1][i].cy,mat[stacks-1][i].cz);
        glVertex3f(cx,cy,cz + height);
        glVertex3f(mat[stacks-1][i-1].cx,mat[stacks-1][i-1].cy,mat[stacks-1][i-1].cz);

    }

    //da a volta aos indices
    glVertex3f(mat[0][0].cx,mat[0][0].cy,mat[0][0].cz);
    glVertex3f(mat[0][slices-1].cx,mat[0][slices-1].cy,mat[0][slices-1].cz);
    glVertex3f(cx,cy,cz);

    glVertex3f(mat[stacks-1][0].cx,mat[stacks-1][0].cy,mat[stacks-1][0].cz);
    glVertex3f(cx,cy,cz + height);
    glVertex3f(mat[stacks-1][slices-1].cx,mat[stacks-1][slices-1].cy,mat[stacks-1][slices-1].cz);

    glEnd();
}

void drawPlane(float lenght,int div){
    float cx = 0,cy = 0,cz = 0;
    float incr = lenght/div; //side increment value
    MyPoint mat[div+1][div+1];

    glBegin(GL_TRIANGLES);

    for(int i = 0; i <= div;i++){
        for(int j = 0;j <= div;j++){
            MyPoint p = MyPoint(cx,cy,cz);
            mat[i][j] = p;
            cx += incr;
            if(i && j){
                glColor3f(0.5f, 0.2f, 0.2f);
                
                glVertex3f(mat[i-1][j].cx,mat[i-1][j].cy,mat[i-1][j].cz);
                glVertex3f(mat[i][j-1].cx,mat[i][j-1].cy,mat[i][j-1].cz);
                glVertex3f(p.cx, p.cy, p.cz);

                glColor3f(0.2f, 0.2f, 0.5f);

                glVertex3f(mat[i-1][j].cx,mat[i-1][j].cy,mat[i-1][j].cz);
                glVertex3f(mat[i-1][j-1].cx,mat[i-1][j-1].cy,mat[i-1][j-1].cz);
                glVertex3f(mat[i][j-1].cx,mat[i][j-1].cy,mat[i][j-1].cz);
            }
        }
        cz += incr;
        cx = 0;
    }

    glEnd();
}

void drawCube(float lenght,int div){
    float cx = 0,cz = 0;
    float incr = lenght/div; //side increment value
    float cy_b = 0; //bottom y
    float cy_t = incr*div; //top y
    MyPoint mat_b[div+1][div+1];
    MyPoint mat_t[div+1][div+1];

    glBegin(GL_TRIANGLES);

    for(int i = 0; i <= div;i++){
        for(int j = 0;j <= div;j++){
            MyPoint p_b = MyPoint(cx,cy_b,cz);
            MyPoint p_t = MyPoint(cx,cy_t,cz);
            mat_b[i][j] = p_b;
            mat_t[i][j] = p_t;
            cx += incr;
            if(i && j){
                ///bottom
                glColor3f(0.5f, 0.2f, 0.2f);
                
                glVertex3f(mat_b[i-1][j].cx,mat_b[i-1][j].cy,mat_b[i-1][j].cz);
                glVertex3f(p_b.cx, p_b.cy, p_b.cz);
                glVertex3f(mat_b[i][j-1].cx,mat_b[i][j-1].cy,mat_b[i][j-1].cz);

                glColor3f(0.2f, 0.2f, 0.5f);

                glVertex3f(mat_b[i-1][j].cx,mat_b[i-1][j].cy,mat_b[i-1][j].cz);
                glVertex3f(mat_b[i][j-1].cx,mat_b[i][j-1].cy,mat_b[i][j-1].cz);
                glVertex3f(mat_b[i-1][j-1].cx,mat_b[i-1][j-1].cy,mat_b[i-1][j-1].cz);
                ///

                ///Right
                glColor3f(1.0f, 0.0f, 0.0f);

                glVertex3f(mat_b[i-1][j].cx,mat_b[i-1][j].cz,mat_b[i-1][j].cy);
                glVertex3f(mat_b[i][j-1].cx,mat_b[i][j-1].cz,mat_b[i][j-1].cy);
                glVertex3f(p_b.cx, p_b.cz, p_b.cy);

                glColor3f(0.0f, 1.0f, 0.0f);

                glVertex3f(mat_b[i-1][j].cx,mat_b[i-1][j].cz,mat_b[i-1][j].cy);
                glVertex3f(mat_b[i-1][j-1].cx,mat_b[i-1][j-1].cz,mat_b[i-1][j-1].cy);
                glVertex3f(mat_b[i][j-1].cx,mat_b[i][j-1].cz,mat_b[i][j-1].cy);
                ///

                ///front
                glColor3f(0.2f, 0.2f, 1.0f);
                
                glVertex3f(mat_b[i-1][j].cy,mat_b[i-1][j].cx,mat_b[i-1][j].cz);
                glVertex3f(mat_b[i][j-1].cy,mat_b[i][j-1].cx,mat_b[i][j-1].cz);
                glVertex3f(p_b.cy, p_b.cx, p_b.cz);

                glColor3f(0.2f, 0.5f, 0.2f);

                glVertex3f(mat_b[i-1][j].cy,mat_b[i-1][j].cx,mat_b[i-1][j].cz);
                glVertex3f(mat_b[i-1][j-1].cy,mat_b[i-1][j-1].cx,mat_b[i-1][j-1].cz);
                glVertex3f(mat_b[i][j-1].cy,mat_b[i][j-1].cx,mat_b[i][j-1].cz);
                ///


                ///Top
                glColor3f(0.5f, 0.2f, 0.2f);
                
                glVertex3f(mat_t[i-1][j].cx,mat_t[i-1][j].cy,mat_t[i-1][j].cz);
                glVertex3f(mat_t[i][j-1].cx,mat_t[i][j-1].cy,mat_t[i][j-1].cz);
                glVertex3f(p_t.cx, p_t.cy, p_t.cz);

                glColor3f(0.2f, 0.2f, 0.5f);

                glVertex3f(mat_t[i-1][j].cx,mat_t[i-1][j].cy,mat_t[i-1][j].cz);
                glVertex3f(mat_t[i-1][j-1].cx,mat_t[i-1][j-1].cy,mat_t[i-1][j-1].cz);
                glVertex3f(mat_t[i][j-1].cx,mat_t[i][j-1].cy,mat_t[i][j-1].cz);
                ///

                ///Left
                glColor3f(1.0f, 0.0f, 0.0f);

                glVertex3f(mat_t[i-1][j].cx,mat_t[i-1][j].cz,mat_t[i-1][j].cy);
                glVertex3f(p_t.cx, p_t.cz, p_t.cy);
                glVertex3f(mat_t[i][j-1].cx,mat_t[i][j-1].cz,mat_t[i][j-1].cy);

                glColor3f(0.0f, 1.0f, 0.0f);

                glVertex3f(mat_t[i-1][j].cx,mat_t[i-1][j].cz,mat_t[i-1][j].cy);
                glVertex3f(mat_t[i][j-1].cx,mat_t[i][j-1].cz,mat_t[i][j-1].cy);
                glVertex3f(mat_t[i-1][j-1].cx,mat_t[i-1][j-1].cz,mat_t[i-1][j-1].cy);
                ///

                ///Back
                glColor3f(0.2f, 0.2f, 1.0f);
                
                glVertex3f(mat_t[i-1][j].cy,mat_t[i-1][j].cx,mat_t[i-1][j].cz);
                glVertex3f(p_t.cy, p_t.cx, p_t.cz);
                glVertex3f(mat_t[i][j-1].cy,mat_t[i][j-1].cx,mat_t[i][j-1].cz);

                glColor3f(0.2f, 0.5f, 0.2f);

                glVertex3f(mat_t[i-1][j].cy,mat_t[i-1][j].cx,mat_t[i-1][j].cz);
                glVertex3f(mat_t[i][j-1].cy,mat_t[i][j-1].cx,mat_t[i][j-1].cz);
                glVertex3f(mat_t[i-1][j-1].cy,mat_t[i-1][j-1].cx,mat_t[i-1][j-1].cz);
                ///
            }
        }
        cz += incr;
        cx = 0;
    }

    glEnd();

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


	glBegin(GL_LINES);
	// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();


// put the geometric transformations here
	glRotatef(angle_y,0.0f,1.0f,0.0f);
	glRotatef(angle_x,1.0f,0.0f,0.0f);
	glRotatef(angle_z,0.0f,0.0f,1.0f);
	glScalef(scale,scale,scale);
	

// put drawing instructions here
	drawPlane(3,3);


	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events






int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboard);


//  OpenGL settings
	glPolygonMode(GL_FRONT,GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
