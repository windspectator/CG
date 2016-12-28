#include "input_drawing.h"

Drawing_state drawing_state;
extern Memory memory;
extern Cg_state cg_state;
extern Drawing_graphic drawing_graphic;
extern Drawing_polygon drawing_polygon;

dot drawing_last_left_click;

void Drawing_state::changeto(int new_state)
{
	state = new_state;
}

void start_drawing(int type)
{
	cg_state.changeto(DRAWING);
	drawing_state.changeto(DRAW_WAITING);
	drawing_graphic.graphic_type = type;
}

void drawing_motion_process(int x, int y)
{
	switch (drawing_state.state)
	{
	case DRAW_WAITING:
		return;
	case DRAW_DRAWING:
		if (drawing_graphic.graphic_type == POLYGON)
			drawing_polygon.motion_process(x, y);
		drawing_graphic.modify({ x, y });
	}
}

void drawing_mouse_process(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (drawing_state.state == DRAW_WAITING)
		{
			if (drawing_graphic.graphic_type == POLYGON)
				drawing_polygon.init(x, y);
			drawing_state.changeto(DRAW_DRAWING);
			drawing_graphic.add_graphic(x, y);
			drawing_last_left_click = { x, y };
			return;
		}
		else
		{
			drawing_last_left_click = { -1, -1 };
			return;
		}
	}
	if (state == GLUT_DOWN)
		return;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (drawing_last_left_click.x == x && drawing_last_left_click.y == y)
			return;
		if (drawing_graphic.graphic_type == CURVE) {
			drawing_graphic.can_curve->drawing_add_dot({ x, y });
			return;
		}
		if ((drawing_graphic.graphic_type != POLYGON || drawing_polygon.left_click(x, y)))
			cg_state.changeto(IDLE);
		return;
	}
	
	if (button == GLUT_RIGHT_BUTTON)
	{
		switch (drawing_state.state)
		{
		case DRAW_WAITING:
			cg_state.changeto(IDLE);
			break;
		case DRAW_DRAWING:
			if (drawing_graphic.graphic_type == POLYGON && drawing_graphic.can_polygon->lines.size() > 1) {
				drawing_graphic.can_polygon->lines.pop_back();
				return;
			}
			if (drawing_graphic.graphic_type == CURVE && drawing_graphic.can_curve->dots.size() > 1) {
				drawing_graphic.can_curve->drawing_complete();
				cg_state.changeto(IDLE);
				return;
			}
			else {
				switch (drawing_graphic.graphic_type) {
				case LINE:
					drawing_graphic.can_line->del();
					break;
				case ELLIPSE:
					drawing_graphic.can_ellipse->del();
					break;
				case POLYGON:
					drawing_graphic.can_polygon->del();
					break;
				case CURVE:
					drawing_graphic.can_curve->del();
					break;
				}
				drawing_state.changeto(DRAW_WAITING);
			}
			return;
		}
	}
}
