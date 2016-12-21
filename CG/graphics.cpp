#include "graphics.h"
#include <cmath>
#include "memory.h"
#include "conf.h"
#include <iostream>
using namespace std;

extern Memory memory;

void graphic::type_convert_to(int new_type)
{
	type = new_type;
}

void line::drawing_modify(dot& d)
{
	b = d;
}

void ellipse::drawing_modify(dot & d)
{
	a = round(sqrt((d.x - o.x) * (d.x - o.x) + (d.y - o.y) * (d.y - o.y)));
	b = a;
}

polygon::polygon(dot d)
{
	lines.push_back(line(d));
	borders.resize(4);

	for (int i = 0; i < 9; i++) {
		memory.ellipses.push_back(ellipse(d, _INS_R, _INS_R));
		ins.push_back(prev(memory.ellipses.end()));
		ins[i]->isdisplayed = false;
		ins[i]->father_type = POLYGON;
		ins[i]->father = memory.sum_polygon;
		switch (i) {
		case _D_LEFT:
			ins[i]->type_convert_to(_TYPE_INS_LEFT);
			break;
		case _D_RIGHT:
			ins[i]->type_convert_to(_TYPE_INS_RIGHT);
			break;
		case _D_UP:
			ins[i]->type_convert_to(_TYPE_INS_UP);
			break;
		case _D_DOWN:
			ins[i]->type_convert_to(_TYPE_INS_DOWN);
			break;
		case _D_CENTER:
			ins[i]->type_convert_to(_TYPE_INS_MOVE);
			break;
		case _D_LEFT_UP:
			ins[i]->type_convert_to(_TYPE_INS_LEFT_UP);
			break;
		case _D_RIGHT_UP:
			ins[i]->type_convert_to(_TYPE_INS_RIGHT_UP);
			break;
		case _D_LEFT_DOWN:
			ins[i]->type_convert_to(_TYPE_INS_LEFT_DOWN);
			break;
		case _D_RIGHT_DOWN:
			ins[i]->type_convert_to(_TYPE_INS_RIGHT_DOWN);
			break;
		}
	}

	memory.ellipses.push_back(ellipse(d, _INS_R, _INS_R));
	dot_ins.push_back(prev(memory.ellipses.end()));
	dot_ins[0]->type_convert_to(_TYPE_INS_DOT);
	dot_ins[0]->isdisplayed = false;
	dot_ins[0]->father_type = POLYGON;
	dot_ins[0]->father = memory.sum_polygon;
	dot_ins[0]->dot_no = 0;
}

void polygon::refresh_border()
{
	borders[_D_LEFT] = lines[0].a.x;
	borders[_D_RIGHT] = lines[0].a.x;
	borders[_D_UP] = lines[0].a.y;
	borders[_D_DOWN] = lines[0].a.y;
	for (auto &i : lines) {
		if (i.a.x < borders[_D_LEFT])
			borders[_D_LEFT] = i.a.x;
		else
			if (i.a.x > borders[_D_RIGHT])
				borders[_D_RIGHT] = i.a.x;
		if (i.a.y > borders[_D_UP])
			borders[_D_UP] = i.a.y;
		else
			if (i.a.y < borders[_D_DOWN])
				borders[_D_DOWN] = i.a.y;
	}

	int center_x = (borders[_D_LEFT] + borders[_D_RIGHT]) / 2;
	int center_y = (borders[_D_UP] + borders[_D_DOWN]) / 2;

	ins[_D_LEFT]->o.x = borders[_D_LEFT];
	ins[_D_LEFT]->o.y = center_y;
	ins[_D_RIGHT]->o.x = borders[_D_RIGHT];
	ins[_D_RIGHT]->o.y = center_y;
	ins[_D_UP]->o.x = center_x;
	ins[_D_UP]->o.y = borders[_D_UP];
	ins[_D_DOWN]->o.x = center_x;
	ins[_D_DOWN]->o.y = borders[_D_DOWN];
	ins[_D_CENTER]->o.x = center_x;
	ins[_D_CENTER]->o.y = center_y;
	ins[_D_LEFT_UP]->o.x = borders[_D_LEFT];
	ins[_D_LEFT_UP]->o.y = borders[_D_UP];
	ins[_D_RIGHT_UP]->o.x = borders[_D_RIGHT];
	ins[_D_RIGHT_UP]->o.y = borders[_D_UP];
	ins[_D_LEFT_DOWN]->o.x = borders[_D_LEFT];
	ins[_D_LEFT_DOWN]->o.y = borders[_D_DOWN];
	ins[_D_RIGHT_DOWN]->o.x = borders[_D_RIGHT];
	ins[_D_RIGHT_DOWN]->o.y = borders[_D_DOWN];

	for (unsigned i = 0; i < dot_ins.size(); i++)
		dot_ins[i]->o = lines[i].a;
}

void polygon::drawing_modify(dot& d)
{
	prev(lines.end())->drawing_modify(d);
}

void polygon::drawing_add_line()
{
	dot last_dot = prev(lines.end())->b;
	lines.push_back(line(last_dot));			// add a new line whose a and b are the same as last line's b

	memory.ellipses.push_back(ellipse(last_dot, _INS_R, _INS_R));
	dot_ins.push_back(prev(memory.ellipses.end()));
	list<ellipse>::iterator current_ins = *prev(dot_ins.end());
	current_ins->type_convert_to(_TYPE_INS_DOT);
	current_ins->isdisplayed = false;
	current_ins->father_type = POLYGON;
	current_ins->father = no;
	current_ins->dot_no = dot_ins.size() - 1;
}

void polygon::drawing_complete()
{
	drawing_modify(lines.begin()->a);
	refresh_border();
}

void polygon::editing_drag(int dot_no, int dx, int dy)
{
	lines[dot_no].a.x += dx;
	lines[dot_no].a.y += dy;
	int last_line_no = (dot_no == 0) ? lines.size() - 1 : dot_no - 1;
	lines[last_line_no].b.x += dx;
	lines[last_line_no].b.y += dy;
	dot_ins[dot_no]->o.x += dx;
	dot_ins[dot_no]->o.y += dy;

	refresh_border();
}

void polygon::editing_resize(dot a, dot b, dot c)
{
	if (b.x == c.x && b.y == c.y)
		return;

	if (b.x == c.x)
		for (auto &i : lines) {
			i.a.y = a.y + (i.a.y - a.y) * (c.y - a.y) / (b.y - a.y);
			i.b.y = a.y + (i.b.y - a.y) * (c.y - a.y) / (b.y - a.y);
		}
	else
		if (b.y == c.y)
			for (auto &i : lines) {
				i.a.x = a.x + (i.a.x - a.x) * (c.x - a.x) / (b.x - a.x);
				i.b.x = a.x + (i.b.x - a.x) * (c.x - a.x) / (b.x - a.x);
			}
		else
			for (auto &i : lines) {
				i.a.x = a.x + (i.a.x - a.x) * (c.x - a.x) / (b.x - a.x);
				i.b.x = a.x + (i.b.x - a.x) * (c.x - a.x) / (b.x - a.x);
				i.a.y = a.y + (i.a.y - a.y) * (c.y - a.y) / (b.y - a.y);
				i.b.y = a.y + (i.b.y - a.y) * (c.y - a.y) / (b.y - a.y);
			}

	refresh_border();
}

void polygon::editing_show_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = true;
	for (auto &i : dot_ins)
		i->isdisplayed = true;
}

void polygon::editing_hide_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = false;
	for (auto &i : dot_ins)
		i->isdisplayed = false;
}
