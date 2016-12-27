#include "input_cutting.h"
#include "input_editing.h"

Cutting_graphic cutting_graphic;

extern Cg_state cg_state;
extern Editing_graphic editing_graphic;
extern Memory memory;
extern list<ellipse>::iterator idle_ins;

void cutting_motion_process(int x, int y)
{
	if (cutting_graphic.state == CUT_WAITING)
		return;

	cutting_graphic.moveto(x, y);
}

void cutting_mouse_process(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	if (cutting_graphic.state == CUT_WAITING) {
		cutting_graphic.add_rectangle(x, y);
		cutting_graphic.state = CUT_CUTTING;
		return;
	}

	cutting_graphic.cut();
}

void Cutting_graphic::add_rectangle(int x, int y)
{
	memory.add_graphic(polygon(dot(x, y)));
	cutting_rectangle = prev(memory.polygons.end());
	cutting_rectangle->drawing_add_line();
	cutting_rectangle->drawing_add_line();
	cutting_rectangle->drawing_add_line();
	cutting_rectangle->drawing_complete();
}

void Cutting_graphic::moveto(int x, int y)
{
	cutting_rectangle->lines[0].b.y = y;
	cutting_rectangle->lines[1].a = cutting_rectangle->lines[0].b;
	cutting_rectangle->lines[1].b = dot(x, y);
	cutting_rectangle->lines[2].a = cutting_rectangle->lines[1].b;
	cutting_rectangle->lines[3].a.x = x;
	cutting_rectangle->lines[2].b = cutting_rectangle->lines[3].a;
}

bool Cutting_graphic::isin(dot d)
{
	if (cutting_rectangle->borders[_D_LEFT] > d.x || cutting_rectangle->borders[_D_RIGHT] < d.x ||
		cutting_rectangle->borders[_D_UP] < d.y || cutting_rectangle->borders[_D_DOWN] > d.y)
		return false;
	return true;
}

dot Cutting_graphic::get_intersection(line & l)
{
	if (l.a.x == l.b.x) {
		if ((l.a.y > l.b.y && isin(l.a)) || (l.a.y < l.b.y && isin(l.b)))
			return dot(l.a.x, cutting_rectangle->borders[_D_DOWN]);
		else
			return dot(l.a.x, cutting_rectangle->borders[_D_UP]);
	}

	if (l.a.y == l.b.y) {
		if ((l.a.x > l.b.x && isin(l.a)) || (l.a.x < l.b.x && isin(l.b)))
			return dot(cutting_rectangle->borders[_D_RIGHT], l.a.y);
		else
			return dot(cutting_rectangle->borders[_D_LEFT], l.a.y);
	}

	double k = (l.b.y - l.a.y) / (l.b.x - l.a.x);
	double y = k * (cutting_rectangle->borders[_D_LEFT] - l.a.x) + l.a.y;
	if (cutting_rectangle->borders[_D_DOWN] <= y && y <= cutting_rectangle->borders[_D_UP])
		return dot(cutting_rectangle->borders[_D_LEFT], y);

	y = k * (cutting_rectangle->borders[_D_RIGHT] - l.a.x) + l.a.y;
	if (cutting_rectangle->borders[_D_DOWN] <= y && y <= cutting_rectangle->borders[_D_UP])
		return dot(cutting_rectangle->borders[_D_RIGHT], y);

	double x = (cutting_rectangle->borders[_D_UP] - l.a.y) / k + l.a.x;
	if (cutting_rectangle->borders[_D_LEFT] <= x && x <= cutting_rectangle->borders[_D_RIGHT])
		return dot(x, cutting_rectangle->borders[_D_UP]);

	x = (cutting_rectangle->borders[_D_DOWN] - l.a.y) / k + l.a.x;
	//if (cut_polygon->borders[_D_LEFT] <= x && x <= cut_polygon->borders[_D_RIGHT])
	return dot(x, cutting_rectangle->borders[_D_DOWN]);
}

void Cutting_graphic::cut()
{
	cutting_rectangle->refresh_border();
	vector<dot> dots;

	for (auto &i : cut_polygon->lines) {
		if (isin(i.a)) {
			if (isin(i.b))
				dots.push_back(i.b);
			else
				dots.push_back(get_intersection(i));
		}
		else {
			if (isin(i.b)) {
				dots.push_back(get_intersection(i));
				dots.push_back(i.b);
			}
		}
	}

	if (dots.size() == 0) {
		cg_state.changeto(EDITING);
		return;
	}

	memory.polygons.push_back(polygon(dots[0]));
	list<polygon>::iterator new_polygon = prev(memory.polygons.end());
	for (int i = 1; i < dots.size(); i++) {
		new_polygon->drawing_modify(dots[i]);
		new_polygon->drawing_add_line();
	}
	new_polygon->drawing_complete();
	new_polygon->editing_show_all_ins();
	cutting_graphic.cutting_rectangle->del();
	cutting_graphic.cut_polygon->del();
	editing_graphic.set(new_polygon->ins[_D_CENTER], 0, 0);
	idle_ins = new_polygon->ins[_D_CENTER];
	cg_state.changeto(EDITING);
}
