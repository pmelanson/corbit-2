#include <corbit/object.hpp>

#include <cmath>

#include <allegro5/allegro_color.h>

using std::clog;

void	object_c::move() {
	_v	+=	_acc/FPS;
	_acc.setZero();
	_pos+=	_v	/FPS;
}

void	object_c::accelerate(var force, var radians) {
	if(!mass) return;

	_acc(0,0) += std::cos(radians) * (force / mass);
	_acc(1,0) += std::sin(radians) * (force / mass);
}

object_c::object_c(std::string name_, var m, var r,
				   var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
				   col color_)
	: name(name_), mass(m>0 ? m : 1), radius(r),
	physical_c(x_,y_, Vx_,Vy_, accX_,accY_),
	color(color_) {

	clog << "\n[" << name << "]";
	clog << "\nmass=" << mass;
	clog << "\nradius=" << radius;
	clog << "\nx=" << x();
	clog << "\ny=" << y();
	clog << "\nVx=" << Vx();
	clog << "\nVy=" << Vy();
	clog << "\naccX=" << accX();
	clog << "\naccY=" << accY();
	unsigned char R, G, B, A;
	al_unmap_rgba(color, &R, &G, &B, &A);
	clog << "\ncolor=" << std::hex << std::uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << std::dec << std::nouppercase;
	clog << std::endl;
}

object_c::object_c()
	: name("the nameless"), mass(1e2), radius(1e2),
	physical_c(vect::Random()(0,0),vect::Random()(1,0), vect::Random()(0,0),vect::Random()(1,0), 0,0),
	color(al_color_name("lightgoldenrodyellow")) {

	std::clog << "\nEmpty object created, placing default object at (" << x() << ", " << y() << ")" << std::endl;
}

object_c::~object_c() {

	clog << "\n[" << name << "]";
	clog << "\nmass=" << mass;
	clog << "\nradius=" << radius;
	clog << "\nx=" << x();
	clog << "\ny=" << y();
	clog << "\nVx=" << Vx();
	clog << "\nVy=" << Vy();
	clog << "\naccX=" << accX();
	clog << "\naccY=" << accY();
	unsigned char R, G, B, A;
	al_unmap_rgba(color, &R, &G, &B, &A);
	clog << "\ncolor=" << std::hex << std::uppercase << unsigned(R) << unsigned(G) << unsigned(B) << unsigned(A) << std::dec << std::nouppercase;
	clog << std::endl;
}
