#include "object.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

const float	G = 6.674E-11;

data object_c::x() const {
	return pos.x;
}
data object_c::y() const {
	return pos.y;
}
data object_c::Vx() const {
	return v.x;
}
data object_c::Vy() const {
	return v.y;
}
data object_c::accX() const {
	return acc.x;
}
data object_c::accY() const {
	return acc.y;
}
data object_c::radius() const {
	return _radius;
}
data object_c::mass() const {
	return _mass;
}


void object_c::accelerate (data force, data radians) {
	acc.x += cos(radians) * (force / mass() / FPS);
	acc.y += sin(radians) * (force / mass() / FPS);
}

void object_c::move() {
	v.x += acc.x;
	v.y += acc.y;

	pos.x += v.x;
	pos.y -= v.y;
}

data object_c::distance2 (const object_c& targ) const {
	return (targ.x() - x())*(targ.x() - x()) + (targ.y() - y())*(targ.y() - y());
}
data object_c::distance (const object_c& targ) const {
	return sqrt(distance2(targ));
}

data object_c::thetaobject (const object_c& targ) const {
	return atan2f((targ.pos.y - pos.y), (targ.pos.x - pos.x));
}

data object_c::gravity (const object_c& targ) const {
	return (G * mass() * targ.mass()) / distance2(targ);
}

data object_c::Vcen (const object_c& targ) const {
	return (x() - targ.x()) * cos(thetaobject(targ)) +
		   (y() - targ.y()) * sin(thetaobject(targ));
}
data object_c::Vtan (const object_c& targ) const {
	return (x() - targ.x()) * cos(thetaobject(targ) + M_PI_2) +
		   (y() - targ.y()) * sin(thetaobject(targ) + M_PI_2);
}

data object_c::orbitV (const object_c& targ) const {
	return sqrt(
	           (targ.mass()*targ.mass() * G) /
	           ((mass() + targ.mass()) * distance(targ)));
}


object_c::pos_s::pos_s (data X, data Y) : x (X), y (Y) {}
object_c::v_s::v_s (data Vx, data Vy) : pos_s (Vx, Vy) {}
object_c::acc_s::acc_s (data accX, data accY) : pos_s (accX, accY) {}

object_c::object_c
(std::string name_, data m, data r,
 data X, data Y, data Vx, data Vy, data accX, data accY,
 ALLEGRO_COLOR color_)
	: name (name_), _mass (m), _radius (r),
	pos (X, Y), v (Vx, Vy), acc (accX, accY),
	color (color_) {}
object_c::~object_c() {}
