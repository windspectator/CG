#pragma once
#include "graphics.h"

class Memory
{
public:
	list<line> lines;
	int sum_line = 0;
	list<ellipse> ellipses;
	int sum_ellipse = 0;
	list<polygon> polygons;
	int sum_polygon = 0;
	list<curve> curves;
	int sum_curve = 0;

	void add_graphic(line& g);
	void add_graphic(ellipse& g);
	void add_graphic(polygon& g);
	void add_graphic(curve & g);

	void display();
};