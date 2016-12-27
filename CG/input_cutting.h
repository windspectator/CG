#pragma once
#include "memory.h"
#include "state.h"
#include "gl/glut.h"

enum { CUT_WAITING, CUT_CUTTING };

class Cutting_graphic {
public:
	list<polygon>::iterator cutting_rectangle;
	list<polygon>::iterator cut_polygon;

	int state;

	void add_rectangle(int x, int y);
	void moveto(int x, int y);
	bool isin(dot d);
	dot get_intersection(line & l);
	void cut();
};