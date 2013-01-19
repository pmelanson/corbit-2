#include <corbit/object.hpp>

#include <allegro5/allegro_color.h>

void	object_c::move		() {
	_v	+=	_acc/FPS;

	_pos+=	_v	/FPS;

	_acc.setZero();
}

void	object_c::accelerate(var force, var radians) {
	if(!mass) return;

	set_accX (accX() + cos(radians) * (force / mass));
	set_accY (accY() + sin(radians) * (force / mass));
}

object_c::object_c			(std::string name_, var m, var r,
							var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
							col color_)
	: name (name_), mass (m ? m : 1), radius (r),
	_pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	color (color_) {

	using std::clog;

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

object_c::object_c			()
	: name ("the nameless"), mass (1e2), radius (1e2),
	_pos (vector::Random()), _v (vector::Random()), _acc (vector::Zero()),
	color (al_color_name("lightgoldenrodyellow")) {

	std::clog << "\nEmpty object created, placing default object at (" << x() << ", " << y() << ")" << std::endl;
}

object_c::~object_c			() {

	using std::clog;

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
