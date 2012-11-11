#include "object.hpp"
#include <cmath>

long double object_c::totalmass() const {
	return mass;
}
long double object_c::x() const {
	return pos.x;
}
long double object_c::y() const {
	return pos.y;
}
long double object_c::Vx() const {
	return v.x;
}
long double object_c::Vy() const {
	return v.y;
}

void object_c::acc (long double force, long double radians) {

	v.x += cos(radians) * (force / totalmass() / FPS);
	v.y -= sin(radians) * (force / totalmass() / FPS);
}
long double object_c::distance2 (const object_c& targ) const {
	return (targ.x() - x())*(targ.x() - x()) + (targ.y() - y())*(targ.y() - y());
}
long double object_c::distance (const object_c& targ) const {
	return sqrt(distance2(targ));
}
long double object_c::gravity (const object_c& targ) const {
	return (6.74e-11 * mass * targ.mass) / (distance2(targ));
}
void object_c::gravitate (const object_c& targ) {

}
long double object_c::Vcen (const object_c* targ) const {

}
long double object_c::Vtan (const object_c* targ) const {

}
long double object_c::orbitV (const object_c* targ) const {

}


object_c::pos_t::pos_t (long double _x, long double _y)
	: x (_x), y (_y) {
}
object_c::v_t::v_t (long double _Vx, long double _Vy)
	: pos_t (_Vx, _Vy) {
}

object_c::object_c
(std::string _name, long double _mass, long long _radius,
 long double _x, long double _y, long double _Vx, long double _Vy,
 ALLEGRO_COLOR _color)
	: pos (_x, _y), v (_Vx, _Vy),
	name (_name), mass (_mass), radius (_radius),
	color (_color) {

}
object_c::~object_c() {
}
