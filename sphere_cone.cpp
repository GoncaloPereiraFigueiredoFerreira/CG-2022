#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

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

void drawSphere3(float radius, int slices, int stacks){
    float cx = 0.0f, cy = 0.0f, cz = 0.0f;
    MyPoint mat[stacks + 1][slices] = {MyPoint(cx,cy,cz)};
    float angInc = 2 * M_PI / slices; //angle increment value
    float ang, h, r;

    

    for(int i = 1; i < stacks; i++){
        h = cz + radius * sin(- M_PI_2 + i * M_PI / stacks);
        r = radius * sin(i * M_PI / stacks);

        //glBegin(GL_LINE_LOOP);
        glBegin(GL_TRIANGLES);
        ang = 0;
        MyPoint p = MyPoint(cx + r * cos(ang), cy + r * sin(ang), h);
        mat[i][0] = p;

        for(int j = 1; j < slices; j++){
            ang = angInc * j;
            MyPoint p = MyPoint(cx + r * cos(ang), cy + r * sin(ang), h);
            mat[i][j] = p;

            glColor3f(0.5f, 0.2f, 0.2f);

            if( i != 1){           
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
        if( i != 1){
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
        glColor3f(0.5f, 0.2f, 0.2f);


    }

    //desenha as pontas

    for(int i = 1; i < slices;i++){
        glBegin(GL_TRIANGLES);

        glColor3f(0.2f, 0.5f, 0.2f);

        glVertex3f(mat[1][i].cx,mat[1][i].cy,mat[1][i].cz);
        glVertex3f(mat[1][i-1].cx,mat[1][i-1].cy,mat[1][i-1].cz);
        glVertex3f(cx,cy,cz - radius);

        glVertex3f(mat[stacks-1][i].cx,mat[stacks-1][i].cy,mat[stacks-1][i].cz);
        glVertex3f(cx,cy,cz + radius);
        glVertex3f(mat[stacks-1][i-1].cx,mat[stacks-1][i-1].cy,mat[stacks-1][i-1].cz);

    }

        //da a volta aos indices
    glVertex3f(mat[1][0].cx,mat[1][0].cy,mat[1][0].cz);
    glVertex3f(mat[1][slices-1].cx,mat[1][slices-1].cy,mat[1][slices-1].cz);
    glVertex3f(cx,cy,cz - radius);

    glVertex3f(mat[stacks-1][0].cx,mat[stacks-1][0].cy,mat[stacks-1][0].cz);
    glVertex3f(cx,cy,cz + radius);
    glVertex3f(mat[stacks-1][slices-1].cx,mat[stacks-1][slices-1].cy,mat[stacks-1][slices-1].cz);

    glEnd();
    
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
	drawCone(1,3,5,5);


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
