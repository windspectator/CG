#include "state.h"
#include "conf.h"

Cg_state cg_state;
int window_height = _WINDOW_HEIGHT;
int window_width = _WINDOW_WIDTH;

void Cg_state::changeto(int new_state)
{
	state = new_state;
}