#include "object.h"
#include <cmath>
extern int FPS;

long double object_t::totalmass() const {
	return mass;
}
long double object_t::x() const {
	return pos.x;
}
long double object_t::y() const {
	return pos.y;
}
long double object_t::Vx() const {
	return v.x;
}
long double object_t::Vy() const {
	return v.y;
}

void object_t::acc (long double force, long double radians) {

	v.x += cos(radians) * (force / totalmass() / FPS);
	v.y -= sin(radians) * (force / totalmass() / FPS);
}
long double object_t::distance2 (const object_t& targ) const {
	return (targ.x() - x())*(targ.x() - x()) + (targ.y() - y())*(targ.y() - y());
}
long double object_t::distance (const object_t& targ) const {
	return sqrt(distance2(targ));
}
long double object_t::gravity (const object_t& targ) const {
	return (6.74e-11 * mass * targ.mass) / (distance2(targ));
}
void object_t::gravitate (const object_t& targ) {

}
long double object_t::Vcen (const object_t* targ) const {

}
long double object_t::Vtan (const object_t* targ) const {

}
long double object_t::orbitV (const object_t* targ) const {

}


object_t::pos_t::pos_t
(long double _x, long double _y)
	: x (_x), y (_y) {

}
object_t::v_t::v_t
(long double _Vx, long double _Vy)
	: pos_t (_Vx, _Vy) {

}

object_t::object_t
(std::string _name, long double _mass, long long _radius,
 long double _x, long double _y, long double _Vx, long double _Vy)
	: name (_name), mass (_mass), radius (_radius),
	pos (_x, _y), v (_Vx, _Vy) {

}
object_t::~object_t() {

}
