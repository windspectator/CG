#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include "graphics.h"
#include <iostream>
using namespace std;

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

	vector<vector<int>> intersections;
	for (int i = 0; i < window_height; i++)
		intersections.push_back(vector<int>());

	glBegin(GL_POINTS);

	int_dot begin, end;
	int_dot a, b;
	for (auto i = lines.begin(); i != lines.end(); i++) {
		a = i->a;
		b = i->b;
		if (b.x - a.x + b.y - a.y < 0) {
			begin = b;
			end = a;
		}
		else {
			begin = a;
			end = b;
		}

		if (end.x - begin.x >= abs(begin.y - end.y)) {
			int last_y = begin.y;
			int dx = end.x - begin.x;
			int dy = abs(end.y - begin.y);
			int d = 2 * dy - dx;
			int y = begin.y;
			int dirt_y = (end.y >= begin.y) ? 1 : -1;
			for (int x = begin.x; x < end.x; x++) {
				glVertex2i(x, y);
				if (0 <= y && y < window_height && y != last_y && y != end.y) {
					intersections[y].push_back(x);
					last_y = y;
				}
				if (d > 0) {
					y += dirt_y;
					d -= dx;
				}
				d += dy;
			}
			intersections[a.y].push_back(a.x);
			if (0 <= b.y && b.y < window_height) {
				auto next_line = (i == prev(lines.end())) ? lines.begin() : next(i);
				if ((b.y - a.y) * ((int)next_line->b.y - (int)next_line->a.y) < 0)
					intersections[b.y].push_back(b.x);
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
				if (0 <= y && y < window_height && y != begin.y)
					intersections[y].push_back(x);
				if (d > 0) {
					x += dirt_x;
					d -= dy;
				}
				d += dx;
			}
			intersections[a.y].push_back(a.x);
			if (0 <= b.y && b.y < window_height) {
				auto next_line = (i == prev(lines.end())) ? lines.begin() : next(i);
				if ((b.y - a.y) * ((int)next_line->b.y - (int)next_line->a.y) < 0)
					intersections[b.y].push_back(b.x);
			}
		}

		glVertex2i(end.x, end.y);
	}

	if (!isfilt) {
		glEnd();
		return;
	}
	
	for (int j = 0; j < window_height; j++) {
		sort(intersections[j].begin(), intersections[j].end());
		if (intersections[j].size() % 2 == 1) {
			cout << "error" << endl;
			for (auto &i : lines)
				cout << i.a.x << " " << i.a.y << " " << i.b.x << " " << i.b.y << " " << endl;
		}
	}
	
	for (int j = 0; j < window_height; j++) {
		for (int k = 0; k < intersections[j].size(); k += 2) {
			for (int i = intersections[j][k]; i < intersections[j][k + 1]; i++)
				glVertex2i(i, j);
		}
	}

	glEnd();
}