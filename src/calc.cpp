#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <corbit/calc.hpp>
#include <corbit/object.hpp>
using namespace std;
const float	G = 6.674e-11;

object_c*	calc_c::active_ship		() const						{return _active_ship;}
object_c*	calc_c::target			() const						{return _target;}
object_c*	calc_c::reference		() const						{return _reference;}
void 		calc_c::set_active_ship	(class object_c* new_ship)		{_active_ship = new_ship;}
void 		calc_c::set_target		(class object_c* new_target)	{_target = new_target;}
void 		calc_c::set_reference	(class object_c* new_reference)	{_reference = new_reference;}


void		calc_c::accelerate				(object_c& A, var force, var radians) {
	A.set_accX (A.accX() + cos(radians) * (force / A.mass() / FPS) );
	A.set_accY (A.accY() + sin(radians) * (force / A.mass() / FPS) );
}

void		calc_c::move					(object_c& A) {
	A.set_v (A.v() + A.acc());
	A.set_pos (A.pos() + A.v());
	vector zero(0,0);
	A.set_acc (zero);
}

var			calc_c::distance2				(const object_c& A, const object_c& B) const {
	return (A.x()-B.x())*(A.x()-B.x()) + (A.y()-B.y())*(A.y()-B.y());
}
var			calc_c::distance				(const object_c& A, const object_c& B) const {return sqrt(distance2(A,B));}

var			calc_c::theta_object			(const object_c& A, const object_c& B) const {
	return atan2f(B.y()-A.y(), B.x()-A.x());
}

var			calc_c::gravity					(const object_c& A, const object_c& B) const {
	return G * (A.mass() * B.mass()) / distance2(A,B);
}

var			calc_c::Vcen					(const object_c& A, const object_c& B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B)) +
		   (A.y() - B.y()) * sin(theta_object(A,B));
}
var			calc_c::Vtan					(const object_c& A, const object_c& B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B) + M_PI_2) +
		   (A.y() - B.y()) * sin(theta_object(A,B) + M_PI_2);
}

var			calc_c::orbitV					(const object_c& A, const object_c& B) const {
	return sqrt(
			   (B.mass()*B.mass() * G) /
			   ((A.mass() + B.mass()) * distance(A,B)));
}


void		calc_c::update					(boost::intrusive::list<object_c> &list) {

	static boost::intrusive::list<object_c>::iterator
	itX, itY;

//	std::cout << list.begin()->name();
//	itX = list.begin();
//	std::cout << list.begin()->name();

	for (itX = list.begin(); itX != list.end(); ++itX) {
		itY = list.begin();
		while (itY != itX) {
//			cout << itX->name() << '\t' << itY->name() << '\t' << gravity(*itX, *itY) << '\t' << theta_object(*itX, *itY) << endl;
			accelerate(*itX, gravity(*itX, *itY), theta_object(*itX, *itY));
			accelerate(*itY, gravity(*itX, *itY), theta_object(*itX, *itY));
			++itY;
		}
	}

//	cout.precision(2);
//	cout << fixed;

	for (itX = list.begin(); itX!=list.end(); ++itX) {
//		cout << itX->x() << '\t' << itX->y() << '\t' << '\t';
		move(*itX);
	}
//	cout << endl;
}

calc_c::calc_c							(class object_c* active_ship_, class object_c* target_, class object_c* reference_)
	: _active_ship (active_ship_), _target (target_), _reference (reference_) {}

calc_c::~calc_c							() {}

calc_c& calc_c::get_instance			(class object_c* active_ship_, class object_c* target_, class object_c* reference_) {

	static calc_c instance (active_ship_, target_, reference_);
	return instance;
}
