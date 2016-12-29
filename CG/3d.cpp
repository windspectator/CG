#include <gl/glut.h>
using namespace std;
#include <iostream>

GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse2[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_position2[] = { 0.0, 0.0, 1.0, 0.0 };
GLfloat light_diffuse3[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_position3[] = { -1.0, 0.0, 0.0, 0.0 };

GLfloat n[6][3] = {
	{ -1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, -1.0 }
};

GLint faces[6][4] = {
	{ 0, 1, 2, 3 },{ 3, 2, 6, 7 },{ 7, 6, 5, 4 },
	{ 4, 5, 1, 0 },{ 5, 6, 2, 1 },{ 7, 4, 0, 3 }
};

GLfloat v[8][3];

void drawBox(void)
{
	int i;

	for (i = 0; i <6; i++) {
		glBegin(GL_QUADS);
		glNormal3fv(&n[i][0]);
		glVertex3fv(v[faces[i][0]]);
		glVertex3fv(v[faces[i][1]]);
		glVertex3fv(v[faces[i][2]]);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}

void display_cube(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawBox();
	glutSwapBuffers();
}

void init_cube(void)
{
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse3);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(20, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 0.0, 1.0);

	glClearColor(1, 1, 1, 0.0);
}

int lastx;
int lasty;

void process_motion_cube(int x, int y)
{
	glRotatef(lastx - x, 1.0, 0.0, 0.0);
	glRotatef(lasty - y, 0.0, 1.0, 0.0);

	glutPostRedisplay();

	lastx = x;
	lasty = y;
}

void process_mouse_cube(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		lastx = x;
		lasty = y;
	}
	glutPostRedisplay();
}

void trans_to_3d()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("cube");
	glutDisplayFunc(display_cube);
	init_cube();
	glutMotionFunc(process_motion_cube);
	glutMouseFunc(process_mouse_cube);
	glutMainLoop();
}
