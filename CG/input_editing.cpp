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
			if (editing_graphic.ins->type == _TYPE_ROTATE)
				editing_graphic.can_polygon->refresh_border();
			editing_state.changeto(EDIT_WAITING);
			editing_graphic.show_ins();
			return;
		}
		else {
			switch (editing_graphic.graphic_type)
			{
			case CURVE:
				for (auto &i : editing_graphic.can_curve->ins)
					if (distance(x, y, i->o.x, i->o.y) < _INS_R) {
						editing_state.changeto(EDIT_DRAGGING);
						editing_graphic.set(i, x, y);
						return;
					}
				for (auto &i : editing_graphic.can_curve->dot_ins)
					if (distance(x, y, i->o.x, i->o.y) < _INS_R) {
						editing_state.changeto(EDIT_DRAGGING);
						editing_graphic.set(i, x, y);
						return;
					}
				break;
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
				break;
			}
			return;
		}
	}

	if (button == GLUT_RIGHT_BUTTON) {
		cg_state.changeto(IDLE);
		editing_graphic.hide_ins();
	}
}