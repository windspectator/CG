#include "memory.h"
#include "conf.h"
#include "gl/glut.h"
#include "state.h"
#include "input_editing.h"

#include <iostream>
using namespace std;

extern Memory memory;
extern Cg_state cg_state;
extern Editing_state editing_state;
extern Editing_graphic editing_graphic;

double distance(int x1, int y1, int x2, int y2);

list<ellipse>::iterator idle_ins;

void input_idle_init()
{
	idle_ins = memory.ellipses.end();
}

void idle_motion_process(int x, int y)
{
	if (idle_ins != memory.ellipses.end() && idle_ins->isdisplayed == true) {
		if (distance(x, y, idle_ins->o.x, idle_ins->o.y) <= _INS_R)
			return;
		else
			idle_ins->isdisplayed = false;
	}

	for (auto i = memory.ellipses.begin(); i != memory.ellipses.end(); i++) {
		if (i->type != _TYPE_INS_DOT)
			continue;

		if (distance(x, y, i->o.x, i->o.y) <= _INS_R) {
			idle_ins = i;
			idle_ins->isdisplayed = true;
			break;
		}
	}
}

void idle_mouse_process(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && idle_ins != memory.ellipses.end() && 
		idle_ins->isdisplayed == true) {
		cg_state.changeto(EDITING);
		editing_state.changeto(EDIT_DRAGGING);
		editing_graphic.set(idle_ins, x, y);
		editing_graphic.show_ins();
	}
}