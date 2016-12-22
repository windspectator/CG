#pragma once
#include <vector>
#include <list>
using namespace std;

enum { LINE, ELLIPSE, POLYGON };
enum { _TYPE_G, _TYPE_INS_CREATE, _TYPE_INS_LEFT, _TYPE_INS_RIGHT, _TYPE_INS_UP, _TYPE_INS_DOWN, _TYPE_INS_DOT, 
		_TYPE_INS_MOVE, _TYPE_INS_LEFT_UP, _TYPE_INS_RIGHT_UP, _TYPE_INS_LEFT_DOWN, _TYPE_INS_RIGHT_DOWN, _TYPE_ROTATE,
		_TYPE_DELETE };
enum { _D_LEFT, _D_RIGHT, _D_UP, _D_DOWN, _D_CENTER, _D_LEFT_UP, _D_RIGHT_UP, _D_LEFT_DOWN, _D_RIGHT_DOWN, _D_UP_UP,
		_D_RIGHT_UP_UP };

class dot
{
public:
	double x;
	double y;
	dot() :
		x(0), y(0) {}
	dot(int a, int b) :
		x(a), y(b) {}
};

class graphic
{
public:
	int no;
	int type;
	bool isdisplayed = true;

	void type_convert_to(int new_type);
};

class line : public graphic
{
public:
	dot a;
	dot b;
	line(dot d) :
		a(d), b(d) {}
	line(dot a, dot b) :
		a(a), b(b) {}
	void display();
	void drawing_modify(dot& d);
	void del();
};

class ellipse : public graphic
{
public:
	dot o;
	int a;
	int b;
	ellipse(dot o) :
		o(o), a(0), b(0) {}
	ellipse(dot o, int a, int b) :
		o(o), a(a), b(b) {}
	void display();
	void drawing_modify(dot & d);
	void del();

	int father_type;
	int father;
	int dot_no;
};

class polygon : public graphic
{
public:
	vector<line> lines;
	vector<int> borders;
	vector<list<ellipse>::iterator> ins;
	vector<list<ellipse>::iterator> dot_ins;
	polygon(dot d);

	void refresh_border();

	void display();
	void drawing_modify(dot& d);
	void drawing_add_line();
	void drawing_complete();
	void del();

	void editing_drag(int dot_no, int dx, int dy);
	void editing_resize(dot a, dot b, dot c);
	void editing_move(int dx, int dy);
	void editing_show_all_ins();
	void editing_hide_all_ins();
};