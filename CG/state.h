#pragma once
enum { IDLE, DRAWING, EDITING };

class Cg_state
{
public:
	int state = IDLE;
	void changeto(int new_state);
};