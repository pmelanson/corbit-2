#include <iostream>
#include <corbit/calc.hpp>

#include <corbit/object.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
const float	G = 6.674e-11;

object_c	*calc_c::active_ship	() const						{return _active_ship;}
object_c	*calc_c::target			() const						{return _targ;}
object_c	*calc_c::reference		() const						{return _ref;}
void 		calc_c::set_active_ship	(class object_c *new_ship)		{_active_ship = new_ship;}
void 		calc_c::set_target		(class object_c *new_target)	{_targ = new_target;}
void 		calc_c::set_reference	(class object_c *new_reference)	{_ref = new_reference;}


var			calc_c::distance2		(const object_c &A, const object_c &B) const {
	return (A.pos() - B.pos()).squaredNorm();
}
var			calc_c::distance		(const object_c &A, const object_c &B) const {
	return (A.pos() - B.pos()).norm();
}

var			calc_c::theta_object	(const object_c &A, const object_c &B) const {
	return atan2f(B.y()-A.y(), B.x()-A.x());
}

var			calc_c::gravity			(const object_c &A, const object_c &B) const {
	if(!std::isnormal(distance2(A,B)) || !distance2(A,B))	//if the distance is inf, nan, or 0
		return 1;

	return G * (A.mass() * B.mass()) / distance2(A,B);
}

var			calc_c::Vcen			(const object_c &A, const object_c &B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B)) +
		   (A.y() - B.y()) * sin(theta_object(A,B));
}
var			calc_c::Vtan			(const object_c &A, const object_c &B) const {
	return (A.x() - B.x()) * cos(theta_object(A,B) + M_PI_2) +
		   (A.y() - B.y()) * sin(theta_object(A,B) + M_PI_2);
}

var			calc_c::orbitV			(const object_c &A, const object_c &B) const {
	return sqrt(
			   (B.mass()*B.mass() * G) /
			   ((A.mass() + B.mass()) * distance(A,B)));
}
var			calc_c::ship_targ_orbitV() const {
	if(!_active_ship || !_targ)
		return 1./0.;	//NaN
	return orbitV(*_active_ship, *_targ);
}
var			calc_c::ship_ref_orbitV	() const {
	if(!_active_ship || !_targ)
		return 1./0.;	//NaN
	return orbitV(*_active_ship, *_ref);
}

var			calc_c::eccentricity	(const object_c &A, const object_c &B) const {
	const var E =
		(relative_v(A,B).squaredNorm())	// v^2 / 2
		/ 2
		-								// -
		G * (A.mass() + B.mass())	/	// G(m1 + m2) / r
		distance(A,B),

	h = (distance(A,B) * Vtan(A,B)) * (distance(A,B) * Vtan(A,B));	//(r * Vtan)^2

//	std::cout.precision(3);
//	std::cout << h << '\t' << E << std::endl;
//	h = 50;

	return std::sqrt(				//sqrt of
		1 +							//1 +
		(2 * E * h * 10000)					//2Eh^2
		/
		(G * (A.mass() + B.mass()) )//G(m1+m2)
	);
}

var			calc_c::ship_ecc		(const object_c &A) const {
	if(!_active_ship)
		return 12./0.;	//NaN
	return eccentricity(*_active_ship, A);
}
var			calc_c::ship_targ_ecc	() const {return ship_ecc(*_targ);}
var			calc_c::ship_ref_ecc	() const {return ship_ecc(*_ref);}


vector		calc_c::relative_pos	(const object_c &A, const object_c &B) const {
	return A.pos() - B.pos();
}

vector		calc_c::relative_v		(const object_c &A, const object_c &B) const {
	return A.v() - B.v();
}

vector		calc_c::ship_relative_v	(const object_c &A) const {
	return relative_v(*_active_ship, A);
}
vector		calc_c::ship_targ_v		() const {
	if(!_active_ship || !_targ)
		return vector(1./0., 1./0.);	//NaN
	return ship_relative_v(*_targ);
}
vector		calc_c::ship_ref_v		() const {
	if(!_active_ship || !_ref)
		return vector(1./0., 1./0.);	//NaN
	return ship_relative_v(*_ref);
}

void		calc_c::_accelerate		(object_c &A, var force, var radians) {
	if(!A.mass())
		return;

	A.set_accX (A.accX() + cos(radians) * (force / A.mass()) );
	A.set_accY (A.accY() + sin(radians) * (force / A.mass()) );
}
void		calc_c::ship_accelerate	(var force, var radians) {
	_accelerate(*_active_ship, force, radians);
}

void		calc_c::_move			(object_c &A) {
	A.set_v(A.v() + A.acc()/FPS);
	A.set_pos(A.pos() + A.v()/FPS);
	A.set_acc0();
}

void		calc_c::update			(boost::intrusive::list<object_c> &list) {

	static boost::intrusive::list<object_c>::iterator
	itX, itY;

	for (itX = list.begin(); itX != list.end(); ++itX) {
		itY = list.begin();
		for (itY = list.begin(); itY != itX; ++itY) {	//gravitation
			_accelerate(*itX, gravity(*itX, *itY), theta_object(*itX, *itY));
			_accelerate(*itY, -gravity(*itX, *itY), theta_object(*itX, *itY));
		}
	}
	for (itX = list.begin(); itX!=list.end(); ++itX) {	//update positions and velocity
		_move(*itX);
	}
}

calc_c::calc_c								(class object_c *active_ship_, class object_c *target_, class object_c *reference_)
	: _active_ship (active_ship_), _targ (target_), _ref (reference_) {}

calc_c::~calc_c						() {}

calc_c &calc_c::get_instance		(class object_c *active_ship_, class object_c *target_, class object_c *reference_) {

	static calc_c instance (active_ship_, target_, reference_);
	return instance;
}
