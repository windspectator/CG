#include "input_editing.h"
#include "conf.h"
#include <iostream>

Editing_state editing_state;
extern Editing_graphic editing_graphic;
extern Cg_state cg_state;

double distance(int x1, int y1, int x2, int y2);

void Editing_state::changeto(int new_state)
{
	state = new_state;
}

void editing_motion_process(int x, int y)
{
	if (editing_state.state == EDIT_WAITING)
		return;

	editing_graphic.moveto(x, y);
}

void editing_mouse_process(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			editing_state.changeto(EDIT_WAITING);
			return;
		}
		else {
			switch (editing_graphic.graphic_type)
			{
			case POLYGON:
				for (auto &i : editing_graphic.can_polygon->ins)
					if (distance(x, y, i->o.x, i->o.y) < _INS_R) {
						editing_state.changeto(EDIT_DRAGGING);
						editing_graphic.set(i, x, y);
						return;
					}
				for (auto &i : editing_graphic.can_polygon->dot_ins)
					if (distance(x, y, i->o.x, i->o.y) < _INS_R) {
						editing_state.changeto(EDIT_DRAGGING);
						editing_graphic.set(i, x, y);
						return;
					}
			}
			cg_state.changeto(IDLE);
			editing_graphic.hide_ins();
		}
	}
}