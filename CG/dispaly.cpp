#include <GL/glut.h>
#include <cmath>
#include "graphics.h"

extern int window_width;
extern int window_height;

enum { _EMPTY, _VERTEX_BYPASS, _VERTEX_THROUGH, _EDGE_BYPASS, _EDGE_THROUGH };

void line::display()
{
	if (!isdisplayed)
		return;

	glBegin(GL_POINTS);

	dot begin, end;

	if (b.x - a.x + b.y - a.y < 0) {
		begin = b;
		end = a;
	}
	else {
		begin = a;
		end = b;
	}

	if (end.x - begin.x >= abs(begin.y - end.y)) {
		int dx = end.x - begin.x;
		int dy = abs(end.y - begin.y);
		int d = 2 * dy - dx;
		int y = begin.y;
		int dirt_y = (end.y >= begin.y) ? 1 : -1;
		for (int x = begin.x; x < end.x; x++) {
			glVertex2i(x, y);
			if (d > 0) {
				y += dirt_y;
				d -= dx;
			}
			d += dy;
		}
	}
	else {
		int dx = abs(end.x - begin.x);
		int dy = end.y - begin.y;
		int d = 2 * dx - dy;
		int x = begin.x;
		int dirt_x = (end.x >= begin.x) ? 1 : -1;
		for (int y = begin.y; y < end.y; y++) {
			glVertex2i(x, y);
			if (d > 0) {
				x += dirt_x;
				d -= dy;
			}
			d += dx;
		}
	}
	glVertex2i(end.x, end.y);

	glEnd();
}

void ellipse::display()
{
	if (!isdisplayed)
		return;

	glBegin(GL_POINTS);

	if (a == 0 || b == 0)
	{
		glVertex2i(o.x, o.y);
		glEnd();
		return;
	}

	int a2 = a*a;
	int b2 = b*b;
	int fa2 = 4 * a2;
	int fb2 = 4 * b2;

	for (int x = 0, y = b, s = 2 * b2 + a2 * (1 - 2 * b); b2 * x <= a2 * y; x++) {
		glVertex2i(o.x + x, o.y + y);
		glVertex2i(o.x - x, o.y + y);
		glVertex2i(o.x + x, o.y - y);
		glVertex2i(o.x - x, o.y - y);
		if (s >= 0)
		{
			s += fa2 * (1 - y);
			y--;
		}
		s += b2 * ((4 * x) + 6);
	}

	for (int x = a, y = 0, s = 2 * a2 + b2 * (1 - 2 * a); a2 * y <= b2 * x; y++) {
		glVertex2i(o.x + x, o.y + y);
		glVertex2i(o.x - x, o.y + y);
		glVertex2i(o.x + x, o.y - y);
		glVertex2i(o.x - x, o.y - y);
		if (s >= 0)
		{
			s += fb2 * (1 - x);
			x--;
		}
		s += a2 * ((4 * y) + 6);
	}

	glEnd();
}

void polygon::display()
{
	if (!isdisplayed)
		return;

	vector<vector<int>> screen;
	for (int i = 0; i < window_width; i++)
		screen.push_back(vector<int>());
	for (auto &i : screen)
		i.resize(window_height, _EMPTY);

	glBegin(GL_POINTS);

	int_dot begin, end;
	for (auto i = lines.begin(); i != lines.end(); i++) {
		if (i->b.x - i->a.x + i->b.y - i->a.y < 0) {
			begin = i->b;
			end = i->a;
		}
		else {
			begin = i->a;
			end = i->b;
		}

		if (end.x - begin.x >= abs(begin.y - end.y)) {
			bool fill_is_through = (begin.y == end.y) ? false : true;
			int dx = end.x - begin.x;
			int dy = abs(end.y - begin.y);
			int d = 2 * dy - dx;
			int y = begin.y;
			int dirt_y = (end.y >= begin.y) ? 1 : -1;
			for (int x = begin.x; x < end.x; x++) {
				glVertex2i(x, y);
				if (x != begin.x && 0 <= x && x < window_width && 0 <= y && y < window_height && fill_is_through) 
					screen[x][y] = (screen[x][y] == _EDGE_THROUGH) ? _EDGE_BYPASS : _EDGE_THROUGH;
				if (d > 0) {
					y += dirt_y;
					d -= dx;
				}
				d += dy;
			}
		}
		else {
			bool fill_is_through = (begin.y == end.y) ? false : true;
			int dx = abs(end.x - begin.x);
			int dy = end.y - begin.y;
			int d = 2 * dx - dy;
			int x = begin.x;
			int dirt_x = (end.x >= begin.x) ? 1 : -1;
			for (int y = begin.y; y < end.y; y++) {
				glVertex2i(x, y);
				if (x != begin.x && 0 <= x && x < window_width && 0 <= y && y < window_height&& !fill_is_through)
					screen[x][y] = (screen[x][y] == _EDGE_THROUGH) ? _EDGE_BYPASS : _EDGE_THROUGH;
				if (d > 0) {
					x += dirt_x;
					d -= dy;
				}
				d += dx;
			}
		}

		glVertex2i(end.x, end.y);
		if (0 <= end.x && end.x < window_width && 0 <= end.y && end.y < window_height) {
			auto next_line = (i == prev(lines.end())) ? lines.begin() : next(i);
			double product = (next_line->b.y - next_line->a.y)*(i->b.y - i->a.y);
			if (product > 0)
				screen[end.x][end.y] = _VERTEX_THROUGH;
			else if (product < 0)
				screen[end.x][end.y] = _VERTEX_BYPASS;
			else {
				if (next_line->b.y - next_line->a.y < 0 || i->b.y - i->a.y > 0)
					screen[end.x][end.y] = _VERTEX_THROUGH;
				else
					screen[end.x][end.y] = _VERTEX_BYPASS;
			}
		}
	}

	if (!isfilt) {
		glEnd();
		return;
	}

	
	for (int j = 0; j < window_height; j++) {
		bool is_on = false;
		for (int i = 0; i < window_width; i++) {
			switch (screen[i][j])
			{
			case _EMPTY:
				if (is_on)
					glVertex2i(i, j);
				break;
			case _VERTEX_THROUGH:
				is_on = !is_on;
				break;
			case _EDGE_THROUGH:
				is_on = !is_on;
				break;
			default:
				break;
			}
		}
	}

	glEnd();
}