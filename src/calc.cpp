#include <corbit/calc.hpp>

#include <corbit/entity.hpp>

#include <eigen3/Eigen/Dense>
#define _USE_MATH_DEFINES
#include <cmath>
const float	G	=	6.674e-11;

var			calc::distance2		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).squaredNorm();
}
var			calc::step_distance2	(const entity_c &A, const entity_c &B) {
	return ((A.pos+ A.v*1/FPS) - (B.pos+ B.v*1/FPS)).squaredNorm();
}
var			calc::distance		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).norm();
}
var			calc::step_distance		(const entity_c &A, const entity_c &B) {
	return ((A.pos+ A.v*1/FPS) - (B.pos+ B.v*1/FPS)).norm();
}


var			calc::theta			(const entity_c &A, const entity_c &B) {
	return atan2f(B.pos[1]-A.pos[1], B.pos[0]-A.pos[0]);
}
var			calc::theta			(const entity_c &A, const entity_c &B, const entity_c &C) {
	return 100;	//TODO
}

var			calc::gravity		(const entity_c &A, const entity_c &B) {
	if(!std::isnormal(distance2(A,B)) || !distance2(A,B))	//if the distance is inf, NAN, or 0
		return 1;

	return G * (A.mass * B.mass) / distance2(A,B);
}

var			calc::v_cen			(const entity_c &A, const entity_c &B) {
	vect n (A.pos - B.pos);	//normal vector

	vect un (n / n.norm());				//unit vector of n

	return un.dot(A.v);
}
var			calc::v_tan			(const entity_c &A, const entity_c &B) {
	vect n (A.pos - B.pos);	//normal vector

	vect un (n / n.norm());				//unit vector of n
	vect unt (-un[1], un[0]);			//vector that is tangent to un

	return unt.dot(A.v);
}

var			calc::pitch			(const entity_c &A, const entity_c &B) {
	return 300;	//TODO
}

var			calc::v_orbit		(const entity_c &A, const entity_c &B) {
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

	h	= (distance(A,B) * v_tan(A,B)) * (distance(A,B) * v_tan(A,B));	//(r * v_tan)^2

	return std::sqrt(	//sqrt of
		1 +				//1 +
		(2 * E * h)		//2Eh^2
		/
		(mu)			//G(m1+m2)
	);
}

var			calc::periapsis		(const entity_c &A, const entity_c &B) {
	return 20;	//TODO
}
var			calc::apoapsis		(const entity_c &A, const entity_c &B) {
	return 40;	//TODO
}

var			calc::stopping_acc	(const entity_c &A, const entity_c &B) {
	return 200;	//TODO
}

vect		calc::position		(const entity_c &A, const entity_c &B) {
	return A.pos - B.pos;
}

vect		calc::velocity		(const entity_c &A, const entity_c &B) {
	return A.v - B.v;
}

bool		calc::approaching	(const entity_c &A, const entity_c &B) {
//	return (B.pos[0] - A.pos[0]) * (A.v[0] - B.v[0]) + (B.pos[1] - A.pos[1]) * (A.v[1] - B.v[1]) > 0;
//	std::cout << "dotting: " << (B.pos[0] - A.pos[0]) * (A.v[0] - B.v[0]) + (B.pos[1] - A.pos[1]) * (A.v[1] - B.v[1]) << '\n';
	return (B.pos[0] - A.pos[0]) * (A.v[0] - B.v[0]) + (B.pos[1] - A.pos[1]) * (A.v[1] - B.v[1]) > 0;
}

void		calc::detect_collision(entity_c &A, entity_c &B) {
	if(distance(A,B) > A.radius + B.radius)
		return;
//	std::cout << A.name << " approaching " << B.name << "?\t" << approaching(A,B) << '\n';
	if(!approaching(A,B))
		return;

	vect n (A.pos - B.pos);				//normal vector

	vect un (n / n.norm());				//unit vector of n
	vect unt (-un[1], un[0]);			//vector that is tangent to un

	var vAn = un.dot(A.v);				//A's velocity projected along un
	var vAt = unt.dot(A.v);				//A's velocity projected along unt

	var vBn = un.dot(B.v);				//same for B
	var vBt = unt.dot(B.v);

	//vAt and vBt will not change, so we don't do anything for them
	var vAt_ = vAt;
	var vBt_ = vBt;

	//vAn and vBt can be calculated with a simple 1D collision calculation
	var vAn_ = (vAn * (A.mass - B.mass) + 2 * B.mass * vBn)
									/
							(A.mass + B.mass);

	var vBn_ = (vBn * (B.mass - A.mass) + 2 * A.mass * vAn)
									/
							(B.mass + A.mass);

	//convert scalar normal and tangent velocities to vectors
	//also, apply coefficient of restitution
	static const var R = 0.5;
	vect VAn = vAn_ * un * R;
	vect VAt = vAt_ * unt * R;


	vect VBn = vBn_ * un * R;
	vect VBt = vBt_ * unt * R;

	//add em up
	A.v = VAn + VAt;
	B.v = VBn + VBt;
}
