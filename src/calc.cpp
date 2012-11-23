#define _USE_MATH_DEFINES
#include <cmath>

#include <corbit/calc.hpp>
#include <corbit/object.hpp>
using namespace std;
const float	G = 6.674E-11;

class object_c*	calc_c::active_ship		() const						{return _active_ship;}
class object_c*	calc_c::target			() const						{return _target;}
class object_c*	calc_c::reference		() const						{return _reference;}
void 			calc_c::set_active_ship	(class object_c* new_ship)		{_active_ship = new_ship;}
void 			calc_c::set_target		(class object_c* new_target)	{_target = new_target;}
void 			calc_c::set_reference	(class object_c* new_reference)	{_reference = new_reference;}


void	calc_c::accelerate				(object_c& A, data force, data radians) {
	A.set_accX (A.accX() + cos(radians) * (force / A.mass() / FPS) );
	A.set_accY (A.accY() + sin(radians) * (force / A.mass() / FPS) );
}

void	calc_c::move					(object_c& A) {
	A.set_v (A.v() + A.acc());
	A.set_pos (A.pos() + A.v());
	vector zero(0,0);
	A.set_acc (zero);
}

data	calc_c::distance2				(const object_c& A, const object_c& B) const {
	return (A.x()-B.x())*(A.x()-B.x()) + (A.y()-B.y())*(A.y()-B.y());
}
data	calc_c::distance				(const object_c& A, const object_c& B) const {return sqrt(distance2(A,B));}

data	calc_c::theta_object			(const object_c& A, const object_c& B) const {
	return atan2f((A.y()-B.y()), (A.x()-B.x()) );
}

data	calc_c::gravity					(const object_c& A, const object_c& B) const {
	return G * (A.mass() * B.mass()) / distance2(A,B);
}

data	calc_c::Vcen					(const object_c& A, const object_c& B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B)) +
		   (A.y() - B.y()) * sin(theta_object(A,B));
}
data	calc_c::Vtan					(const object_c& A, const object_c& B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B) + M_PI_2) +
		   (A.y() - B.y()) * sin(theta_object(A,B) + M_PI_2);
}

data	calc_c::orbitV					(const object_c& A, const object_c& B) const {
	return sqrt(
			   (B.mass()*B.mass() * G) /
			   ((A.mass() + B.mass()) * distance(A,B)));
}


void	calc_c::update					(boost::intrusive::list<object_c> &list) {

	static boost::intrusive::list<object_c>::iterator itX;
	for (itX = list.begin(); itX != list.end(); ++itX) {
		move(*itX);
	}
}

calc_c::calc_c							(class object_c* active_ship_, class object_c* target_, class object_c* reference_)
	: _active_ship (active_ship_), _target (target_), _reference (reference_) {}

calc_c::~calc_c							() {}

calc_c& calc_c::get_instance				(class object_c* active_ship_, class object_c* target_, class object_c* reference_) {

	static calc_c instance (active_ship_, target_, reference_);
	return instance;
}
