#include "memory.h"
#include <iostream>
using namespace std;

Memory memory;

void Memory::add_graphic(line & g)
{
	g.no = sum_line;
	sum_line++;
	lines.push_back(g);
}

void Memory::add_graphic(ellipse & g)
{
	g.no = sum_ellipse;
	sum_ellipse++;
	ellipses.push_back(g);
}

void Memory::add_graphic(polygon & g)
{
	g.no = sum_polygon;
	sum_polygon++;
	polygons.push_back(g);
}

void Memory::add_graphic(curve & g)
{
	g.no = sum_curve;
	sum_curve++;
	curves.push_back(g);
}

void Memory::display()
{
	for (auto &i : lines)
		i.display();
	for (auto &i : ellipses)
		i.display();
	for (auto &i : polygons)
		i.display();
	for (auto &i : curves)
		i.display();
}
