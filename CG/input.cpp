#include "input.h"
#include <iostream>
using namespace std;

void trans_to_3d();
void output();
extern Memory memory;
extern Cg_state cg_state;
extern int window_height;

void process_menu(int value)
{
	if (value == 4) {
		trans_to_3d();
		cg_state.changeto(_3D);
		return;
	}

	if (value == 5) {
		output();
		return;
	}

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
	case CUTTING:
		cutting_motion_process(x, y);
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
		break;
	case CUTTING:
		cutting_mouse_process(button, state, x, y);
		break;
	}
	glutPostRedisplay();
}