#include <cmath>
#include "graphics.h"

using namespace std;

double distance(int x1, int y1, int x2, int y2)
{
	return pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2), 0.5);
}

double cosine_thoerem(dot a, dot b, dot c)
{
	int a2 = (c.x - b.x)*(c.x - b.x) + (c.y - b.y)*(c.y - b.y);
	int b2 = (c.x - a.x)*(c.x - a.x) + (c.y - a.y)*(c.y - a.y);
	int c2 = (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
	return (b2 + c2 - a2) / (2 * distance(a.x, a.y, c.x, c.y) * distance(a.x, a.y, b.x, b.y));
}