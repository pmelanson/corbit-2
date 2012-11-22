#include <corbit/object.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

const float	G = 6.674E-11;

string	object_c::name			() const {return _name;}
data	object_c::x				() const {return _pos(0,0);}
data	object_c::y				() const {return _pos(1,0);}
data	object_c::Vx			() const {return _v(0,0);}
data	object_c::Vy			() const {return _v(1,0);}
data	object_c::accX			() const {return _acc(0,0);}
data	object_c::accY			() const {return _acc(1,0);}
vector	object_c::pos			() const {return _pos;}
vector	object_c::v				() const {return _v;}
vector	object_c::acc			() const {return _acc;}
data	object_c::radius		() const {return _radius;}
data	object_c::mass			() const {return _mass;}


void	object_c::accelerate	(data force, data radians) {
	_acc(0,0) += cos(radians) * (force / mass() / FPS);
	_acc(1,0) += sin(radians) * (force / mass() / FPS);
}

void	object_c::move			() {
	_v += acc();

	_pos += v();
}

data	object_c::distance2		(const object_c& targ) const {
	return (targ.x()-x())*(targ.x()-x()) + (targ.y()-y())*(targ.y()-y());
}
data	object_c::distance		(const object_c& targ) const {return sqrt(distance2(targ));}

data	object_c::thetaobject	(const object_c& targ) const {
	return atan2f((targ.y()-y()), (targ.x()-x()) );
}

data	object_c::gravity		(const object_c& targ) const {
	return G * (mass() * targ.mass()) / distance2(targ);
}

data	object_c::Vcen			(const object_c& targ) const {
	return (x() - targ.x()) * cos(thetaobject(targ)) +
		   (y() - targ.y()) * sin(thetaobject(targ));
}
data	object_c::Vtan			(const object_c& targ) const {
	return (x() - targ.x()) * cos(thetaobject(targ) + M_PI_2) +
		   (y() - targ.y()) * sin(thetaobject(targ) + M_PI_2);
}

data	object_c::orbitv		(const object_c& targ) const {
	return sqrt(
			   (targ.mass()*targ.mass() * G) /
			   ((mass() + targ.mass()) * distance(targ)));
}

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
