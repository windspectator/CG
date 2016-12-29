#pragma once
enum { IDLE, DRAWING, EDITING, CUTTING, _3D };

class Cg_state
{
public:
	int state = IDLE;
	void changeto(int new_state);
};