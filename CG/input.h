#pragma once
#include "memory.h"
#include "gl/glut.h"
#include "state.h"
#include "input_drawing.h"

void idle_motion_process(int x, int y);
void idle_mouse_process(int button, int state, int x, int y);

void editing_motion_process(int x, int y);
void editing_mouse_process(int button, int state, int x, int y);

void cutting_motion_process(int x, int y);
void cutting_mouse_process(int button, int state, int x, int y);

void process_menu(int value);
void process_motion(int x, int y);
void process_no_motion(int x, int y);
void process_mouse(int button, int state, int x, int y);
