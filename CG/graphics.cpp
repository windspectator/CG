#include "graphics.h"
#include <cmath>
#include "memory.h"
#include "conf.h"
#include <iostream>
using namespace std;

extern Memory memory;
double cosine_thoerem(dot a, dot b, dot c);

void graphic::type_convert_to(int new_type)
{
	type = new_type;
}

line::line(dot d, bool isp) :
	a(d), b(d)
{
	ispartofp = isp;
	if (isp)
		return;
	memory.ellipses.push_back(ellipse(d, _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[0]->isdisplayed = false;
	ins[0]->father_type = LINE;
	ins[0]->father = memory.sum_line;
	ins[0]->dot_no = 0;
	ins[0]->type_convert_to(_TYPE_INS_DOT);

	memory.ellipses.push_back(ellipse(d, _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[1]->isdisplayed = false;
	ins[1]->father_type = LINE;
	ins[1]->father = memory.sum_line;
	ins[1]->dot_no = 1;
	ins[1]->type_convert_to(_TYPE_INS_DOT);

	memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
	del_ins = prev(memory.ellipses.end());
	del_ins->isdisplayed = false;
	del_ins->father_type = LINE;
	del_ins->father = memory.sum_line;
	del_ins->type_convert_to(_TYPE_DELETE);
}

void line::drawing_modify(dot& d)
{
	b = d;
	if (ispartofp)
		return;
	ins[1]->o = d;
	del_ins->o.x = (b.x > a.x) ? b.x : a.x;
	del_ins->o.y = ((b.y > a.y) ? b.y : a.y) + 2 * _INS_R;
}

void line::editing_drag(int dot_no, int dx, int dy)
{
	if (dot_no == 0) {
		a.x += dx;
		a.y += dy;
		ins[0]->o = a;
	}
	else {
		b.x += dx;
		b.y += dy;
		ins[1]->o = b;
	}
	del_ins->o.x = (b.x > a.x) ? b.x : a.x;
	del_ins->o.y = ((b.y > a.y) ? b.y : a.y) + 2 * _INS_R;
}

void line::del()
{
	for (auto &i : ins)
		memory.ellipses.erase(i);
	memory.ellipses.erase(del_ins);

	auto i = memory.lines.begin();
	while (i->no != no)
		i++;
	memory.lines.erase(i);
}

void line::editing_show_all_ins()
{
	ins[0]->isdisplayed = true;
	ins[1]->isdisplayed = true;
	del_ins->isdisplayed = true;
}

void line::editing_hide_all_ins()
{
	ins[0]->isdisplayed = false;
	ins[1]->isdisplayed = false;
	del_ins->isdisplayed = false;
}

ellipse::ellipse(dot o) :
	o(o), a(0), b(0)
{
	for (int i = 0; i < 4; i++) {
		memory.ellipses.push_back(ellipse(o, _INS_R, _INS_R));
		dot_ins.push_back(prev(memory.ellipses.end()));
		list<ellipse>::iterator current_ins = *prev(dot_ins.end());
		current_ins->type_convert_to(_TYPE_INS_DOT);
		current_ins->isdisplayed = false;
		current_ins->father_type = ELLIPSE;
		current_ins->father = memory.sum_ellipse;
		current_ins->dot_no = dot_ins.size() - 1;
	}

	memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[0]->isdisplayed = false;
	ins[0]->father_type = ELLIPSE;
	ins[0]->father = memory.sum_ellipse;
	ins[0]->type_convert_to(_TYPE_INS_MOVE);

	memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[1]->isdisplayed = false;
	ins[1]->father_type = ELLIPSE;
	ins[1]->father = memory.sum_ellipse;
	ins[1]->type_convert_to(_TYPE_DELETE);
}

void ellipse::drawing_modify(dot & d)
{
	a = (int)round(sqrt((d.x - o.x) * (d.x - o.x) + (d.y - o.y) * (d.y - o.y)));
	b = a;
	refresh_border();
}

void ellipse::del()
{
	for (auto &i : ins)
		memory.ellipses.erase(i);
	for (auto &i : dot_ins)
		memory.ellipses.erase(i);

	auto i = memory.ellipses.begin();
	while (i->no != no)
		i++;
	memory.ellipses.erase(i);
}

void ellipse::editing_drag(int dot_no, int dx, int dy)
{
	switch (dot_no) {
	case 0:
		o.x += dx / 2.0;
		a -= dx / 2.0;
		break;
	case 1:
		o.x += dx / 2.0;
		a += dx / 2.0;
		break;
	case 2:
		o.y += dy / 2.0;
		b += dy / 2.0;
		break;
	case 3:
		o.y += dy / 2.0;
		b -= dy / 2.0;
		break;
	}

	refresh_border();
}

void ellipse::editing_move(int dx, int dy)
{
	o.x += dx;
	o.y += dy;
	refresh_border();
}

void ellipse::refresh_border()
{
	dot_ins[0]->o = dot(o.x - a, o.y);
	dot_ins[1]->o = dot(o.x + a, o.y);
	dot_ins[2]->o = dot(o.x, o.y + b);
	dot_ins[3]->o = dot(o.x, o.y - b);

	ins[0]->o = dot(o.x, o.y);
	ins[1]->o = dot(o.x + a, o.y + b);
}

void ellipse::editing_show_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = true;
	for (auto &i : dot_ins)
		i->isdisplayed = true;
}

void ellipse::editing_hide_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = false;
	for (auto &i : dot_ins)
		i->isdisplayed = false;
}

curve::curve(dot d)
{
	isdisplayed = true;
	dots.push_back(d);
	dots.push_back(d);
}

void curve::drawing_add_dot(dot d)
{
	dots.push_back(d);
}

void curve::drawing_modify(dot d)
{
	*prev(dots.end()) = d;
}

void curve::refresh_border()
{
	borders[_D_LEFT] = (int)dots[0].x;
	borders[_D_RIGHT] = (int)dots[0].x;
	borders[_D_UP] = (int)dots[0].y;
	borders[_D_DOWN] = (int)dots[0].y;

	for (auto &i : dots) {
		if (i.x < borders[_D_LEFT])
			borders[_D_LEFT] = (int)i.x;
		else
			if (i.x > borders[_D_RIGHT])
				borders[_D_RIGHT] = (int)i.x;
		if (i.y > borders[_D_UP])
			borders[_D_UP] = (int)i.y;
		else
			if (i.y < borders[_D_DOWN])
				borders[_D_DOWN] = (int)i.y;
	}

	ins[0]->o.x = (borders[_D_LEFT] + borders[_D_RIGHT]) / 2;
	ins[0]->o.y = (borders[_D_UP] + borders[_D_DOWN]) / 2;
	ins[1]->o.x = borders[_D_RIGHT];
	ins[1]->o.y = borders[_D_UP] + 2 * _INS_R;

	for (unsigned i = 0; i < dot_ins.size(); i++)
		dot_ins[i]->o = dots[i];
}

void curve::drawing_complete()
{
	for (auto &i : dots) {
		memory.ellipses.push_back(ellipse(i, _INS_R, _INS_R));
		dot_ins.push_back(prev(memory.ellipses.end()));
		list<ellipse>::iterator current_ins = *prev(dot_ins.end());
		current_ins->type_convert_to(_TYPE_INS_DOT);
		current_ins->isdisplayed = false;
		current_ins->father_type = CURVE;
		current_ins->father = no;
		current_ins->dot_no = dot_ins.size() - 1;
	}

	memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[0]->isdisplayed = false;
	ins[0]->father_type = CURVE;
	ins[0]->father = no;
	ins[0]->type_convert_to(_TYPE_INS_MOVE);

	memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
	ins.push_back(prev(memory.ellipses.end()));
	ins[1]->isdisplayed = false;
	ins[1]->father_type = CURVE;
	ins[1]->father = no;
	ins[1]->type_convert_to(_TYPE_DELETE);

	borders.resize(4);
	refresh_border();
}

void curve::editing_show_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = true;
	for (auto &i : dot_ins)
		i->isdisplayed = true;
}

void curve::editing_hide_all_ins()
{
	for (auto &i : ins)
		i->isdisplayed = false;
	for (auto &i : dot_ins)
		i->isdisplayed = false;
}

void curve::editing_drag(int dot_no, int dx, int dy)
{
	dots[dot_no].x += dx;
	dots[dot_no].y += dy;

	refresh_border();
}

void curve::editing_move(int dx, int dy)
{
	for (auto &i : dots) {
		i.x += dx;
		i.y += dy;
	}

	refresh_border();
}

void curve::del()
{
	for (auto &i : ins)
		memory.ellipses.erase(i);
	for (auto &i : dot_ins)
		memory.ellipses.erase(i);

	auto i = memory.curves.begin();
	while (i->no != no)
		i++;
	memory.curves.erase(i);
}

polygon::polygon(dot d)
{
	isdisplayed = true;
	isfilt = false;
	lines.push_back(line(d, true));
}

void polygon::refresh_border()
{
	borders[_D_LEFT] = (int)lines[0].a.x;
	borders[_D_RIGHT] = (int)lines[0].a.x;
	borders[_D_UP] = (int)lines[0].a.y;
	borders[_D_DOWN] = (int)lines[0].a.y;
	for (auto &i : lines) {
		if (i.a.x < borders[_D_LEFT])
			borders[_D_LEFT] = (int)i.a.x;
		else
			if (i.a.x > borders[_D_RIGHT])
				borders[_D_RIGHT] = (int)i.a.x;
		if (i.a.y > borders[_D_UP])
			borders[_D_UP] = (int)i.a.y;
		else
			if (i.a.y < borders[_D_DOWN])
				borders[_D_DOWN] = (int)i.a.y;
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
	ins[_D_UP_UP]->o.x = center_x;
	ins[_D_UP_UP]->o.y = borders[_D_UP] + 2 * _INS_R;
	ins[_D_RIGHT_UP_UP]->o.x = borders[_D_RIGHT];
	ins[_D_RIGHT_UP_UP]->o.y = borders[_D_UP] + 2 * _INS_R;
	ins[_D_LEFT_UP_UP]->o.x = borders[_D_LEFT];
	ins[_D_LEFT_UP_UP]->o.y = borders[_D_UP] + 2 * _INS_R;
	ins[_D_DOWN_DOWN]->o.x = center_x;
	ins[_D_DOWN_DOWN]->o.y = borders[_D_DOWN] - 2 * _INS_R;

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
	lines.push_back(line(last_dot, true));			// add a new line whose a and b are the same as last line's b
}

void polygon::drawing_complete()
{
	drawing_modify(lines.begin()->a);

	for (auto &i : lines) {
		memory.ellipses.push_back(ellipse(i.a, _INS_R, _INS_R));
		dot_ins.push_back(prev(memory.ellipses.end()));
		list<ellipse>::iterator current_ins = *prev(dot_ins.end());
		current_ins->type_convert_to(_TYPE_INS_DOT);
		current_ins->isdisplayed = false;
		current_ins->father_type = POLYGON;
		current_ins->father = no;
		current_ins->dot_no = dot_ins.size() - 1;
	}

	borders.resize(4);
	for (int i = 0; i < 14; i++) {
		memory.ellipses.push_back(ellipse(dot(0, 0), _INS_R, _INS_R));
		ins.push_back(prev(memory.ellipses.end()));
		ins[i]->isdisplayed = false;
		ins[i]->father_type = POLYGON;
		ins[i]->father = no;
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
		case _D_UP_UP:
			ins[i]->type_convert_to(_TYPE_ROTATE);
			break;
		case _D_RIGHT_UP_UP:
			ins[i]->type_convert_to(_TYPE_DELETE);
			break;
		case _D_CENTER_ASIDE:
			ins[i]->type_convert_to(_TYPE_INS_ROTATE_CENTER);
			break;
		case _D_LEFT_UP_UP:
			ins[i]->type_convert_to(_TYPE_INS_FILL);
			break;
		case _D_DOWN_DOWN:
			ins[i]->type_convert_to(_TYPE_INS_CUT);
			break;
		}
	}

	refresh_border();
}

void polygon::del()
{
	for (auto &i : ins)
		memory.ellipses.erase(i);
	for (auto &i : dot_ins)
		memory.ellipses.erase(i);

	auto i = memory.polygons.begin();
	while (i->no != no)
		i++;
	memory.polygons.erase(i);
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

void polygon::editing_move(int dx, int dy)
{
	for (auto &i : lines) {
		i.a.x += dx;
		i.a.y += dy;
		i.b.x += dx;
		i.b.y += dy;
	}

	refresh_border();
}

void polygon::editing_rotate(dot b, dot c)
{
	dot a = ins[_D_CENTER_ASIDE]->o;
	double cos�� = cosine_thoerem(a, b, c);
	double sin�� = pow(1 - cos�� * cos��, 0.5);
		 
	for (auto &i : lines) {
		double new_ax = a.x + (i.a.x - a.x) * cos�� - (i.a.y - a.y) * sin��;
		double new_bx = a.x + (i.b.x - a.x) * cos�� - (i.b.y - a.y) * sin��;
		i.a.y = a.y + (i.a.x - a.x) * sin�� + (i.a.y - a.y) * cos��;
		i.b.y = a.y + (i.b.x - a.x) * sin�� + (i.b.y - a.y) * cos��;
		i.a.x = new_ax;
		i.b.x = new_bx;
	}
}

void polygon::editing_fill()
{
	isfilt = !isfilt;
}

void polygon::editing_show_all_ins()
{
	for (auto &i : ins)
		if (i->type != _TYPE_INS_ROTATE_CENTER)
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
