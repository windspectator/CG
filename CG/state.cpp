#include "state.h"
#include "conf.h"
#include "gl/glut.h"

Cg_state cg_state;
int window_height = _WINDOW_HEIGHT;
int window_width = _WINDOW_WIDTH;

void Cg_state::changeto(int new_state)
{
	if (state == IDLE)
		glutDetachMenu(GLUT_RIGHT_BUTTON);

	state = new_state;

	if (state == IDLE)
		glutAttachMenu(GLUT_RIGHT_BUTTON);
}