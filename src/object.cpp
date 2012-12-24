#include <corbit/object.hpp>
using namespace std;

string	object_c::name		() const		{return _name;}
var		object_c::mass		() const		{return _mass;}
var		object_c::radius	() const		{return _radius;}
var		object_c::x			() const		{return _pos(0,0);}
var		object_c::y			() const		{return _pos(1,0);}
var		object_c::Vx		() const		{return _v(0,0);}
var		object_c::Vy		() const		{return _v(1,0);}
var		object_c::accX		() const		{return _acc(0,0);}
var		object_c::accY		() const		{return _acc(1,0);}
vector	object_c::pos		() const		{return _pos;}
vector	object_c::v			() const		{return _v;}
vector	object_c::acc		() const		{return _acc;}
col		object_c::color		() const		{return _color;}

void	object_c::set_x		(var x_)		{_pos(0,0) = x_;}
void	object_c::set_y		(var y_)		{_pos(1,0) = y_;}
void	object_c::set_Vx	(var Vx_)		{_v(0,0) = Vx_;}
void	object_c::set_Vy	(var Vy_)		{_v(1,0) = Vy_;}
void	object_c::set_accX	(var accX_)		{_acc(0,0) = accX_;}
void	object_c::set_accY	(var accY_)		{_acc(1,0) = accY_;}
void	object_c::set_acc0	()				{_acc.setZero();}
void	object_c::set_pos	(vector pos_)	{_pos = pos_;}
void	object_c::set_v		(vector v_)		{_v = v_;}
void	object_c::set_acc	(vector acc_)	{_acc = acc_;}


object_c::object_c			(string name_, var m, var r,
							var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
							col color_)
	: _name (name_), _mass (m ? m : 1), _radius (r),
	_pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	_color (color_) {

	clog << "\n[" << name() << "]";
	clog << "\nmass=" << mass();
	clog << "\nradius=" << radius();
	clog << "\nx=" << x();
	clog << "\ny=" << y();
	clog << "\nVx=" << Vx();
	clog << "\nVy=" << Vy();
	clog << "\naccX=" << accX();
	clog << "\naccY=" << accY();
	unsigned char R, G, B, A;
	al_unmap_rgba(_color, &R, &G, &B, &A);
	clog << "\ncolor=" << hex << uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << dec << nouppercase;
	clog << endl;
}

object_c::object_c			()
	: _name ("the nameless"), _mass (1e2), _radius (1e2),
	_pos (vector::Random()), _v (vector::Random()), _acc (vector::Zero()),
	_color (al_color_name("lightgoldenrodyellow")) {

	clog << "\nEmpty object created, placing default object at (" << x() << ", " << y() << ")" << endl;
}

object_c::~object_c			() {

	clog << "\n[" << name() << "]";
	clog << "\nmass=" << mass();
	clog << "\nradius=" << radius();
	clog << "\nx=" << x();
	clog << "\ny=" << y();
	clog << "\nVx=" << Vx();
	clog << "\nVy=" << Vy();
	clog << "\naccX=" << accX();
	clog << "\naccY=" << accY();
	unsigned char R, G, B, A;
	al_unmap_rgba(_color, &R, &G, &B, &A);
	clog << "\ncolor=" << hex << uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << dec << nouppercase;
	clog << endl;
}
