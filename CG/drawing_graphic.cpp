#include "input_drawing.h"

extern Memory memory;

Drawing_graphic drawing_graphic;

void Drawing_graphic::add_graphic(int x, int y)
{
	switch (graphic_type) {
	case LINE:
		memory.add_graphic(line({ x, y }));
		can_line = prev(memory.lines.end());
		break;
	case ELLIPSE:
		memory.add_graphic(ellipse({ x, y }));
		can_ellipse = prev(memory.ellipses.end());
		break;
	case POLYGON:
		memory.add_graphic(polygon({ x, y }));
		can_polygon = prev(memory.polygons.end());
	}
}

void Drawing_graphic::modify(dot d)
{
	switch (graphic_type) {
	case LINE:
		can_line->drawing_modify(d);
		break;
	case ELLIPSE:
		can_ellipse->drawing_modify(d);
		break;
	case POLYGON:
		can_polygon->drawing_modify(d);
	}
}

void Drawing_graphic::polygon_add_line()
{
	can_polygon->drawing_add_line();
}

void Drawing_graphic::polygon_complete()
{
	can_polygon->drawing_complete();
}
