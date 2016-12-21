#include "input.h"
#include <iostream>
using namespace std;

extern Memory memory;
extern Cg_state cg_state;
extern int window_height;

void process_menu(int value)
{
	start_drawing(value);
}

void process_motion(int x, int y)
{
	y = window_height - y;
	switch (cg_state.state)
	{
	case IDLE:
		idle_motion_process(x, y);
		break;
	case DRAWING:
		drawing_motion_process(x, y);
		break;
	case EDITING:
		editing_motion_process(x, y);
		break;
	}
	glutPostRedisplay();
}

void process_no_motion(int x, int y)
{
	y = window_height - y;

	switch (cg_state.state)
	{
	case IDLE:
		idle_motion_process(x, y);
		break;
	case DRAWING:
		drawing_motion_process(x, y);
		break;
	case EDITING:
		editing_motion_process(x, y);
		break;
	}
	glutPostRedisplay();
}

void process_mouse(int button, int state, int x, int y)
{
	y = window_height - y;
	switch (cg_state.state)
	{
	case IDLE:
		idle_mouse_process(button, state, x, y);
		break;
	case DRAWING:
		drawing_mouse_process(button, state, x, y);
		break;
	case EDITING:
		editing_mouse_process(button, state, x, y);
	}
	glutPostRedisplay();
}