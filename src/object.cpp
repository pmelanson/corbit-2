#include <corbit/object.hpp>
using namespace std;

string	object_c::name			() const		{return _name;}
data	object_c::mass			() const		{return _mass;}
data	object_c::radius		() const		{return _radius;}
data	object_c::x				() const		{return _pos(0,0);}
data	object_c::y				() const		{return _pos(1,0);}
data	object_c::Vx			() const		{return _v(0,0);}
data	object_c::Vy			() const		{return _v(1,0);}
data	object_c::accX			() const		{return _acc(0,0);}
data	object_c::accY			() const		{return _acc(1,0);}
vector	object_c::pos			() const		{return _pos;}
vector	object_c::v				() const		{return _v;}
vector	object_c::acc			() const		{return _acc;}

void	object_c::set_x			(data x_)		{_pos(0,0) = x_;}
void	object_c::set_y			(data y_)		{_pos(1,0) = y_;}
void	object_c::set_Vx		(data Vx_)		{_v(0,0) = Vx_;}
void	object_c::set_Vy		(data Vy_)		{_v(1,0) = Vy_;}
void	object_c::set_accX		(data accX_)	{_acc(0,0) = accX_;}
void	object_c::set_accY		(data accY_)	{_acc(1,0) = accY_;}
void	object_c::set_pos		(vector pos_)	{_pos = pos_;}
void	object_c::set_v			(vector v_)		{_v = v_;}
void	object_c::set_acc		(vector acc_)	{_acc = acc_;}


object_c::object_c				(std::string name_, data m, data r,
							 data x_, data y_, data Vx_, data Vy_, data accX_, data accY_,
							 ALLEGRO_COLOR color_)
	: _name (name_), _mass (m), _radius (r),
	_pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	color (color_) {

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
	al_unmap_rgba(color, &R, &G, &B, &A);
	clog << "\ncolor=" << hex << uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << dec << nouppercase;
	clog << endl;
}

object_c::object_c				()
	: _name ("the nameless"), _mass (1), _radius (1),
	_pos (9001, 9001), _v (0, 0), _acc (0, 0),
	color (al_color_name("magenta")) {

	clog << "Empty object created, placing default object at (" << x() << ", " << y() << ")" << endl;
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
	al_unmap_rgba(color, &R, &G, &B, &A);
	clog << "\ncolor=" << hex << uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << dec << nouppercase;
	clog << endl;
}
