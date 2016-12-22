#include "input.h"
#include "conf.h"
#include <iostream>

extern Memory memory;
extern int window_width;
extern int window_height;

void input_idle_init();

void init()
{
	input_idle_init();
}

void init_window()
{
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);
	glutInitWindowPosition(_WIN_INIT_X, _WIN_INIT_Y);
	glutCreateWindow(_WINDOW_TITLE);
	glClearColor(_WIN_BACKG_R, _WIN_BACKG_G, _WIN_BACKG_B, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, _WINDOW_WIDTH, 0.0, _WINDOW_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}

void init_nemu()
{
	glutCreateMenu(process_menu);

	glutAddMenuEntry("line", LINE);
	glutAddMenuEntry("ellipse", ELLIPSE);
	glutAddMenuEntry("polygon", POLYGON);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	memory.display();

	glFlush();
}

void process_reshape(int width, int height)
{
	window_width = width;
	window_height = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	init_window();
	glutDisplayFunc(display);
	init_nemu();

	glutMotionFunc(process_motion);
	glutPassiveMotionFunc(process_no_motion);
	glutMouseFunc(process_mouse);
	glutReshapeFunc(process_reshape);

	memory.add_graphic(polygon({ 300, 200 }));
	memory.polygons.begin()->drawing_modify(dot(400, 300));
	memory.polygons.begin()->drawing_add_line();
	memory.polygons.begin()->drawing_modify(dot(300, 400));
	memory.polygons.begin()->drawing_add_line();
	memory.polygons.begin()->drawing_modify(dot(100, 300));
	memory.polygons.begin()->drawing_add_line();
	memory.polygons.begin()->drawing_complete();
	memory.polygons.begin()->isfilt = true;

	init();

	glutMainLoop();
}