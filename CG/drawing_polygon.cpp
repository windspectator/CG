#include "input_drawing.h"
#include "conf.h"

extern Memory memory;
extern Drawing_graphic drawing_graphic;
extern Cg_state cg_state;
Drawing_polygon drawing_polygon;

void Drawing_polygon::init(int x, int y)
{
	start_point = { x, y };
	is_last_in = true;
	is_now_in = true;
	is_ins_on = false;
}

bool Drawing_polygon::is_in(int x, int y)
{
	int d2 = (x - start_point.x) * (x - start_point.x) + (y - start_point.y) * (y - start_point.y);
	return d2 <= _POLYGON_INS_R * _POLYGON_INS_R;
}

void Drawing_polygon::motion_process(int x, int y)
{
	is_now_in = is_in(x, y);
	if ((!is_last_in && !is_now_in) || ((is_last_in && is_now_in))) {
		is_last_in = is_now_in;
		return;
	}

	if (is_now_in)
	{
		if (is_ins_on)
		{
			memory.add_graphic(ellipse(start_point, _POLYGON_INS_R, _POLYGON_INS_R));		// add the instruction
			instruct = prev(memory.ellipses.end());
			instruct->type_convert_to(_TYPE_INS_CREATE);
		}
	}
	else
	{
		if (is_ins_on)
		{
			memory.ellipses.erase(instruct);			// remove the instruction
		}
		is_ins_on = !is_ins_on;
	}

	is_last_in = is_now_in;
}

bool Drawing_polygon::left_click(int x, int y)			// return if the drawing has completed
{
	if (is_now_in && is_ins_on)
	{
		drawing_graphic.polygon_complete();
		memory.ellipses.erase(instruct);
		return 1;
	}
	else
	{
		drawing_graphic.polygon_add_line();
		return 0;
	}
}