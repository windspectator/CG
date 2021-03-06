#include "input_editing.h"
#include "input_cutting.h"
#include <iostream>
using namespace std;

extern Memory memory;
extern Cg_state cg_state;
extern list<ellipse>::iterator idle_ins;
extern Cutting_graphic cutting_graphic;
extern Editing_state editing_state;

Editing_graphic editing_graphic;

void Editing_graphic::set(list<ellipse>::iterator chosen_ins, int x, int y)
{
	ins = chosen_ins;
	
	lastx = x;
	lasty = y;

	list<curve>::iterator c;
	list<line>::iterator l;
	list<ellipse>::iterator e;

	switch (ins->father_type) {
	case LINE:
		graphic_type = LINE;
		l = memory.lines.begin();
		while (l->no != ins->father)
			l++;
		can_line = l;

		if (ins->type == _TYPE_DELETE) {
			can_line->del();
			cg_state.changeto(IDLE);
			idle_ins = memory.ellipses.end();
			return;
		}

		can_line->editing_hide_all_ins();
		ins->isdisplayed = true;
		break;
	case ELLIPSE:
		graphic_type = ELLIPSE;
		e = memory.ellipses.begin();
		while (e->no != ins->father)
			e++;
		can_ellipse = e;

		if (ins->type == _TYPE_DELETE) {
			can_ellipse->del();
			cg_state.changeto(IDLE);
			idle_ins = memory.ellipses.end();
			return;
		}

		can_ellipse->editing_hide_all_ins();
		ins->isdisplayed = true;
		break;

	case CURVE:
		graphic_type = CURVE;
		c = memory.curves.begin();
		while (c->no != ins->father)
			c++;
		can_curve = c;

		if (ins->type == _TYPE_DELETE) {
			can_curve->del();
			cg_state.changeto(IDLE);
			idle_ins = memory.ellipses.end();
			return;
		}

		can_curve->editing_hide_all_ins();
		ins->isdisplayed = true; 
		break;

	case POLYGON:
		graphic_type = POLYGON;
		list<polygon>::iterator f = memory.polygons.begin();
		while (f->no != ins->father)
			f++;
		can_polygon = f;

		if (ins->type == _TYPE_DELETE) {
			can_polygon->del();
			cg_state.changeto(IDLE);
			idle_ins = memory.ellipses.end();
			return;
		}

		can_polygon->editing_hide_all_ins();
		ins->isdisplayed = true;
		switch (ins->type) {
		case _TYPE_INS_LEFT:
			can_polygon->ins[_D_RIGHT]->isdisplayed = true;
			break;
		case _TYPE_INS_RIGHT:
			can_polygon->ins[_D_LEFT]->isdisplayed = true;
			break;
		case _TYPE_INS_UP:
			can_polygon->ins[_D_DOWN]->isdisplayed = true;
			break;
		case _TYPE_INS_DOWN:
			can_polygon->ins[_D_UP]->isdisplayed = true;
			break;
		case _TYPE_INS_LEFT_UP:
			can_polygon->ins[_D_RIGHT_DOWN]->isdisplayed = true;
			break;
		case _TYPE_INS_RIGHT_UP:
			can_polygon->ins[_D_LEFT_DOWN]->isdisplayed = true;
			break;
		case _TYPE_INS_LEFT_DOWN:
			can_polygon->ins[_D_RIGHT_UP]->isdisplayed = true;
			break;
		case _TYPE_INS_RIGHT_DOWN:
			can_polygon->ins[_D_LEFT_UP]->isdisplayed = true;
			break;
		case _TYPE_ROTATE:
			can_polygon->ins[_D_CENTER_ASIDE]->o = can_polygon->ins[_D_CENTER]->o;
			can_polygon->ins[_D_CENTER_ASIDE]->isdisplayed = true;
			break;
		case _TYPE_INS_FILL:
			can_polygon->editing_fill();
			break;
		case _TYPE_INS_CUT:
			ins->isdisplayed = false;
			editing_state.changeto(EDIT_WAITING);
			cutting_graphic.cut_polygon = editing_graphic.can_polygon;
			cutting_graphic.state = CUT_WAITING;
			cg_state.changeto(CUTTING);
			break;
		}

		break;
	}
}

void Editing_graphic::moveto(int x, int y)
{
	switch (graphic_type) {
	case LINE:
		can_line->editing_drag(ins->dot_no, x - lastx, y - lasty);
		break;
	case ELLIPSE:
		switch (ins->type) {
		case _TYPE_INS_DOT:
			can_ellipse->editing_drag(ins->dot_no, x - lastx, y - lasty);
			break;
		case _TYPE_INS_MOVE:
			can_ellipse->editing_move(x - lastx, y - lasty);
			break;
		}
		break;
	case CURVE:
		switch (ins->type) {
		case _TYPE_INS_DOT:
			can_curve->editing_drag(ins->dot_no, x - lastx, y - lasty);
			break;
		case _TYPE_INS_MOVE:
			can_curve->editing_move(x - lastx, y - lasty);
			break;
		}
		break;
	case POLYGON:
		dot a, b, c;
		switch (ins->type) {
		case _TYPE_INS_DOT:
			can_polygon->editing_drag(ins->dot_no, x - lastx, y - lasty);
			break;
		case _TYPE_INS_LEFT:
			a = can_polygon->ins[_D_RIGHT]->o;
			b = can_polygon->ins[_D_LEFT]->o;
			c.x = can_polygon->ins[_D_LEFT]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_LEFT]->o.y;
			if (a.x == b.x || a.x == c.x)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				set(can_polygon->ins[_D_RIGHT], x, y);
			break;
		case _TYPE_INS_RIGHT:
			a = can_polygon->ins[_D_LEFT]->o;
			b = can_polygon->ins[_D_RIGHT]->o;
			c.x = can_polygon->ins[_D_RIGHT]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_RIGHT]->o.y;
			if (a.x == b.x || a.x == c.x)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				set(can_polygon->ins[_D_LEFT], x, y);
			break;
		case _TYPE_INS_UP:
			a = can_polygon->ins[_D_DOWN]->o;
			b = can_polygon->ins[_D_UP]->o;
			c.x = can_polygon->ins[_D_UP]->o.x;
			c.y = can_polygon->ins[_D_UP]->o.y + y - lasty;
			if (a.y == b.y || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.y - a.y) * (c.y - a.y) < 0)
				set(can_polygon->ins[_D_DOWN], x, y);
			break;
		case _TYPE_INS_DOWN:
			a = can_polygon->ins[_D_UP]->o;
			b = can_polygon->ins[_D_DOWN]->o;
			c.x = can_polygon->ins[_D_DOWN]->o.x;
			c.y = can_polygon->ins[_D_DOWN]->o.y + y - lasty;
			if (a.y == b.y || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.y - a.y) * (c.y - a.y) < 0)
				set(can_polygon->ins[_D_UP], x, y);
			break;
		case _TYPE_INS_LEFT_UP:
			a = can_polygon->ins[_D_RIGHT_DOWN]->o;
			b = can_polygon->ins[_D_LEFT_UP]->o;
			c.x = can_polygon->ins[_D_LEFT_UP]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_LEFT_UP]->o.y + y - lasty;
			if (a.x == b.x || a.y == b.y || a.x == c.x || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_RIGHT_DOWN], x, y);
				else
					set(can_polygon->ins[_D_RIGHT_UP], x, y);
			else
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_LEFT_DOWN], x, y);
			break;
		case _TYPE_INS_RIGHT_UP:
			a = can_polygon->ins[_D_LEFT_DOWN]->o;
			b = can_polygon->ins[_D_RIGHT_UP]->o;
			c.x = can_polygon->ins[_D_RIGHT_UP]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_RIGHT_UP]->o.y + y - lasty;
			if (a.x == b.x || a.y == b.y || a.x == c.x || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_LEFT_DOWN], x, y);
				else
					set(can_polygon->ins[_D_LEFT_UP], x, y);
			else
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_RIGHT_DOWN], x, y);
			break;
		case _TYPE_INS_LEFT_DOWN:
			a = can_polygon->ins[_D_RIGHT_UP]->o;
			b = can_polygon->ins[_D_LEFT_DOWN]->o;
			c.x = can_polygon->ins[_D_LEFT_DOWN]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_LEFT_DOWN]->o.y + y - lasty;
			if (a.x == b.x || a.y == b.y || a.x == c.x || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_RIGHT_UP], x, y);
				else
					set(can_polygon->ins[_D_RIGHT_DOWN], x, y);
			else
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_LEFT_UP], x, y);
			break;
		case _TYPE_INS_RIGHT_DOWN:
			a = can_polygon->ins[_D_LEFT_UP]->o;
			b = can_polygon->ins[_D_RIGHT_DOWN]->o;
			c.x = can_polygon->ins[_D_RIGHT_DOWN]->o.x + x - lastx;
			c.y = can_polygon->ins[_D_RIGHT_DOWN]->o.y + y - lasty;
			if (a.x == b.x || a.y == b.y || a.x == c.x || a.y == c.y)
				return;
			can_polygon->editing_resize(a, b, c);
			if ((b.x - a.x) * (c.x - a.x) < 0)
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_LEFT_UP], x, y);
				else
					set(can_polygon->ins[_D_LEFT_DOWN], x, y);
			else
				if ((b.y - a.y) * (c.y - a.y) < 0)
					set(can_polygon->ins[_D_RIGHT_UP], x, y);
			break;
		case _TYPE_INS_MOVE:
			can_polygon->editing_move(x - lastx, y - lasty);
			break;
		case _TYPE_ROTATE:
			if (lastx == x && lasty == y)
				return;
			can_polygon->editing_rotate(dot(lastx, lasty), dot(x, y));
			break;
		case _TYPE_INS_FILL:
			break;
		}
	}
	lastx = x;
	lasty = y;
}

void Editing_graphic::show_ins()
{
	switch (graphic_type)
	{
	case LINE:
		can_line->editing_show_all_ins();
		break;
	case ELLIPSE:
		can_ellipse->editing_show_all_ins();
		break;
	case CURVE:
		can_curve->editing_show_all_ins();
		break;
	case POLYGON:
		can_polygon->editing_show_all_ins();
		break;
	}
}

void Editing_graphic::hide_ins()
{
	switch (graphic_type)
	{
	case LINE:
		can_line->editing_hide_all_ins();
		break;
	case ELLIPSE:
		can_ellipse->editing_hide_all_ins();
		break;
	case CURVE:
		can_curve->editing_hide_all_ins();
		break;
	case POLYGON:
		can_polygon->editing_hide_all_ins();
		break;
	}
}
