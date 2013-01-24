#include <corbit/calc.hpp>

#include <corbit/object.hpp>

#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <cmath>
const float	G	=	6.674e-11;

var			calc::distance2		(const object_c &A, const object_c &B) {
	return (A.pos() - B.pos()).squaredNorm();
}
var			calc::distance		(const object_c &A, const object_c &B) {
	return (A.pos() - B.pos()).norm();
}

var			calc::theta			(const object_c &A, const object_c &B) {
	return atan2f(B.y()-A.y(), B.x()-A.x());
}

var			calc::gravity		(const object_c &A, const object_c &B) {
	if(!std::isnormal(distance2(A,B)) || !distance2(A,B))	//if the distance is inf, NAN, or 0
		return 1;

	return G * (A.mass * B.mass) / distance2(A,B);
}

var			calc::Vcen			(const object_c &A, const object_c &B) {
	return (A.x() - B.x()) * cos(theta(A,B)) +
		   (A.y() - B.y()) * sin(theta(A,B));
}
var			calc::Vtan			(const object_c &A, const object_c &B) {
	return (A.x() - B.x()) * cos(theta(A,B) + M_PI_2) +
		   (A.y() - B.y()) * sin(theta(A,B) + M_PI_2);
}

var			calc::orbitV		(const object_c &A, const object_c &B) {
	return sqrt(
			   (B.mass*B.mass * G) /
			   ((A.mass + B.mass) * distance(A,B)));
}

var			calc::ecc			(const object_c &A, const object_c &B) {
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

vect		calc::position		(const object_c &A, const object_c &B) {
	return A.pos() - B.pos();
}

vect		calc::velocity		(const object_c &A, const object_c &B) {
	return A.v() - B.v();
}
