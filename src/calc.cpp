#include <corbit/calc.hpp>

#include <corbit/entity.hpp>

#include <eigen3/Eigen/Dense>
#define _USE_MATH_DEFINES
#include <cmath>
const float	G	=	6.674e-11;

var			calc::distance2		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).squaredNorm();
}
var			calc::distance		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).norm();
}

var			calc::theta			(const entity_c &A, const entity_c &B) {
	return atan2f(B.y()-A.y(), B.x()-A.x());
}

var			calc::gravity		(const entity_c &A, const entity_c &B) {
	if(!std::isnormal(distance2(A,B)) || !distance2(A,B))	//if the distance is inf, NAN, or 0
		return 1;

	return G * (A.mass * B.mass) / distance2(A,B);
}

var			calc::Vcen			(const entity_c &A, const entity_c &B) {
	return (A.x() - B.x()) * cos(theta(A,B)) +
		   (A.y() - B.y()) * sin(theta(A,B));
}
var			calc::Vtan			(const entity_c &A, const entity_c &B) {
	return (A.x() - B.x()) * cos(theta(A,B) + M_PI_2) +
		   (A.y() - B.y()) * sin(theta(A,B) + M_PI_2);
}

var			calc::orbitV		(const entity_c &A, const entity_c &B) {
	return sqrt(
			   (B.mass*B.mass * G) /
			   ((A.mass + B.mass) * distance(A,B)));
}

var			calc::ecc			(const entity_c &A, const entity_c &B) {
	const var
	mu	= G * (A.mass + B.mass),	//G(m1+m2)
	E	=
		(velocity(A,B).squaredNorm())	/ 2	//v^2 / 2
		-									//-
		(mu / distance(A,B)),				//mu / r

	h	= (distance(A,B) * Vtan(A,B)) * (distance(A,B) * Vtan(A,B));	//(r * Vtan)^2

	return std::sqrt(	//sqrt of
		1 +				//1 +
		(2 * E * h)		//2Eh^2
		/
		(mu)			//G(m1+m2)
	);
}

vect		calc::position		(const entity_c &A, const entity_c &B) {
	return A.pos - B.pos;
}

vect		calc::velocity		(const entity_c &A, const entity_c &B) {
	return A.v - B.v;
}

#include <iostream>
using std::cout;
void		calc::detect_collision(entity_c &A, entity_c &B) {
	if(distance2(A,B) > (A.radius + B.radius)*(A.radius + B.radius))
		return;	//distance^2 > (A.r + B.r)^2 is a faster calcaulation

	vect n (A.x()-B.x(), A.y()-B.y());	//normal vector
	cout << "\nn:\n" << n;

	vect un (n / n.norm());				//unit vector of n
	cout << "\nun:\n" << un;
	vect unt (-un[1], un[0]);		//vector that is tangent to un
	cout << "\nunt:\n" << unt;

	var vAn = un.dot(A.v);			//A's velocity projected along un
	var vAt = unt.dot(A.v);			//A's velocity projected along ut

	var vBn = un.dot(B.v);			//same for B
	var vBt = unt.dot(B.v);

	//vAt and vBt will not change, so we don't do anything for them
	var vAt_ = vAt;
	var vBt_ = vBt;

	//vAn and vBt can be calculated with a simple 1D collision calculation

	//calculate vAn prime
	var vAn_ = (vAn * (A.mass - B.mass) + 2 * B.mass * vBn)
									/
							(A.mass + B.mass);

	//calculate vBn prime
	var vBn_ = (vBn * (B.mass - A.mass) + 2 * A.mass * vAn)
									/
							(B.mass + A.mass);

	//convert scalar normal and tangent velocities to vectors
	vect VAn = vAn_ * un;
	vect VAt = vAt_ * unt;

	vect VBn = vBn_ * un;
	vect VBt = vBt_ * unt;

	//add em up
	A.v = VAn + VAt;
	B.v = VBn + VBt;
}
