#pragma once
#include <vector>
#include <list>
using namespace std;

enum { LINE, ELLIPSE, POLYGON, CURVE };
enum { _TYPE_G, _TYPE_INS_CREATE, _TYPE_INS_LEFT, _TYPE_INS_RIGHT, _TYPE_INS_UP, _TYPE_INS_DOWN, _TYPE_INS_DOT, 
		_TYPE_INS_MOVE, _TYPE_INS_LEFT_UP, _TYPE_INS_RIGHT_UP, _TYPE_INS_LEFT_DOWN, _TYPE_INS_RIGHT_DOWN, _TYPE_ROTATE,
		_TYPE_DELETE, _TYPE_INS_ROTATE_CENTER, _TYPE_INS_FILL, _TYPE_INS_CUT };
enum { _D_LEFT, _D_RIGHT, _D_UP, _D_DOWN, _D_CENTER, _D_LEFT_UP, _D_RIGHT_UP, _D_LEFT_DOWN, _D_RIGHT_DOWN, _D_UP_UP,
		_D_RIGHT_UP_UP, _D_CENTER_ASIDE, _D_LEFT_UP_UP, _D_DOWN_DOWN };

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

class int_dot
{
public:
	int x;
	int y;
	int_dot() :
		x(0), y(0) {}
	int_dot(int a, int b) :
		x(a), y(b) {}
	int_dot(dot d) :
		x(d.x), y(d.y) {}
};

class graphic
{
public:
	int no;
	int type;
	bool isdisplayed;
	bool isfilt;

	void type_convert_to(int new_type);
};

class ellipse;

class line : public graphic
{
public:
	dot a;
	dot b;
	bool ispartofp;
	line(dot d, bool isp);

	vector<list<ellipse>::iterator> ins;
	list<ellipse>::iterator del_ins;

	void display();
	void drawing_modify(dot& d);
	void editing_drag(int dot_no, int dx, int dy);
	void del();
	void editing_show_all_ins();
	void editing_hide_all_ins();
};

class ellipse : public graphic
{
public:
	dot o;
	double a;
	double b;
	ellipse(dot o);
	ellipse(dot o, int a, int b) :
		o(o), a(a), b(b) {}
	void display();
	void drawing_modify(dot & d);
	void del();

	vector<list<ellipse>::iterator> ins;
	vector<list<ellipse>::iterator> dot_ins;
	void editing_drag(int dot_no, int dx, int dy);
	void editing_move(int dx, int dy);
	void refresh_border();
	void editing_show_all_ins();
	void editing_hide_all_ins();

	int father_type;
	int father;
	int dot_no;
};


class curve : public graphic
{
public:
	vector<dot> dots;
	vector<int> borders;
	curve(dot d);
	vector<list<ellipse>::iterator> ins;
	vector<list<ellipse>::iterator> dot_ins;

	void display();
	void drawing_add_dot(dot d);
	void drawing_modify(dot d);
	void refresh_border();
	void drawing_complete();
	void editing_show_all_ins();
	void editing_hide_all_ins();

	void editing_drag(int dot_no, int dx, int dy);
	void editing_move(int dx, int dy);
	void del();
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
	void editing_rotate(dot b, dot c);
	void editing_fill();
	void editing_show_all_ins();
	void editing_hide_all_ins();
};