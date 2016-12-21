#include <cmath>

using namespace std;

double distance(int x1, int y1, int x2, int y2)
{
	return pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2), 0.5);
}