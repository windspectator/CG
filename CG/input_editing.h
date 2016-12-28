#pragma once
#include "graphics.h"
#include "memory.h"
#include "state.h"
#include "gl/glut.h"

enum { EDIT_WAITING, EDIT_DRAGGING };

class Editing_state
{
public:
	int state;
	void changeto(int new_state);
};

class Editing_graphic
{
public:
	int graphic_type;
	list<line>::iterator can_line;
	list<ellipse>::iterator can_ellipse;
	list<polygon>::iterator can_polygon;
	list<curve>::iterator can_curve;
	list<ellipse>::iterator ins;
	int lastx;
	int lasty;

	void set(list<ellipse>::iterator chosen_ins, int x, int y);
	void moveto(int x, int y);
	void show_ins();
	void hide_ins();
};