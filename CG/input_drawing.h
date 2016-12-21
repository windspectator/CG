#pragma once
#include "graphics.h"
#include "memory.h"
#include "state.h"
#include "gl/glut.h"

enum { DRAW_WAITING, DRAW_DRAWING };

class Drawing_state
{
public:
	int state = DRAW_WAITING;
	void changeto(int new_state);
};

class Drawing_graphic
{
public:
	int graphic_type;
	list<line>::iterator can_line;
	list<ellipse>::iterator can_ellipse;
	list<polygon>::iterator can_polygon;

	void add_graphic(int x, int y);
	void modify(dot d);
	void polygon_add_line();
	void polygon_complete();
};

class Drawing_polygon
{
public:
	dot start_point;
	list<ellipse>::iterator instruct;
	bool is_last_in;
	bool is_now_in;
	bool is_ins_on;

	void init(int x, int y);
	bool is_in(int x, int y);
	void motion_process(int x, int y);
	bool left_click(int x, int y);
};

void start_drawing(int type);
void drawing_motion_process(int x, int y);
void drawing_mouse_process(int button, int state, int x, int y);

