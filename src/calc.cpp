#include <corbit/calc.hpp>

#include <corbit/entity.hpp>

#include <eigen3/Eigen/Dense>
#define _USE_MATH_DEFINES

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

const float	G = 6.674e-11;

var		calc::distance2		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).squaredNorm();
}
var		calc::step_distance2(const entity_c &A, const entity_c &B) {
	return ((A.pos+ A.v*1/FPS) - (B.pos+ B.v*1/FPS)).squaredNorm();
}
var		calc::distance		(const entity_c &A, const entity_c &B) {
	return (A.pos - B.pos).norm();
}
var		calc::step_distance	(const entity_c &A, const entity_c &B) {
	return ((A.pos + (A.v + A.acc)/FPS) - (B.pos + (B.v + B.acc)/FPS)).norm();
}


var		calc::theta			(const entity_c &A, const entity_c &B) {
	return atan2 (B.pos[1]-A.pos[1], B.pos[0]-A.pos[0]);
}
var		calc::theta			(const entity_c &A, const entity_c &B, const entity_c &C) {
	return 100;	//TODO
}

var		calc::gravity		(const entity_c &A, const entity_c &B) {
	if(!std::isnormal(distance2(A,B)) || !distance2(A,B))	//if the distance is inf, NAN, or 0
		return 1;

	return G * (A.mass() * B.mass()) / distance2(A,B);
}

var		calc::v_cen			(const entity_c &A, const entity_c &B) {
	vect n (A.pos - B.pos);				//normal vector

	vect un (n / n.norm());				//unit vector of n

	return un.dot(A.v - B.v);
}
var		calc::v_tan			(const entity_c &A, const entity_c &B) {
	vect n (A.pos - B.pos);	//normal vector

	vect un (n / n.norm());				//unit vector of n
	vect unt (-un[1], un[0]);			//vector that is tangent to un

	return unt.dot(A.v - B.v);
}

var		calc::pitch			(const entity_c &A, const entity_c &B) {
//	return A.;
	return M_PI;	//TODO
}

var		calc::v_orbit		(const entity_c &A, const entity_c &B) {
	return sqrt(
		(B.mass()*B.mass() * G) /
		((A.mass() + B.mass()) * distance(A,B)));
}

var		calc::ecc			(const entity_c &A, const entity_c &B) {
	const var
	mu	= G * (A.mass() + B.mass()),	//G(m1+m2)
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

var		calc::semimajor_axis(const entity_c &A, const entity_c &B) {
	var u = G * (A.mass() + B.mass());
	var e = ((A.v - B.v).squaredNorm() / 2) -
			u / distance (A,B);

	return -u / 2 * e;
}

var		calc::periapsis		(const entity_c &A, const entity_c &B) {
	return (1 - ecc (A,B)) * semimajor_axis (A,B);
}
var		calc::apoapsis		(const entity_c &A, const entity_c &B) {
	return (1 + ecc (A,B)) * semimajor_axis (A,B);
}

var		calc::stopping_acc	(const entity_c &A, const entity_c &B) {
	return 200;	//TODO
}

vect	calc::position		(const entity_c &A, const entity_c &B) {
	return A.pos - B.pos;
}

vect	calc::velocity		(const entity_c &A, const entity_c &B) {
	return A.v - B.v;
}

void	calc::detect_collision(entity_c &A, entity_c &B, var time) {

	//general overview of this function:
	//1. find if the objects will collide in the given time
	//2. if yes, calculate collision:
	//2.1 represent velocities as normal velocity and tangential velocity
	//2.2 do a 1D collision using the normal veloctiy
	//2.3 add the normal and tangential velocities to get the new velocity

	//for this function I make one of the objects the frame of reference
	//which means my calculations are much simplified
	
	//for this, I just set up a quadratic equation, then solve for time. It all works out.
	var	a = velocity(A,B).squaredNorm(),	//a = |v|^2
		b = 2 * position(A,B).dot(velocity(A,B)),	//b = 2 * position . velocity
		c = position(A,B).squaredNorm() - (A.radius + B.radius)*(A.radius + B.radius);
		//c = |pos|^2 - sum of radii^2
		
	if (a == 0) {
		std::cerr << "Divide by 0 caught in collision detection routine (A.v = B.v)\n";
		return;
	}

	var t_to_impact = (-b - sqrt(b*b - 4*a*c)) / (2*a);	//finds when an impact will take place

	//return if the collision isn't this frame
	if (!std::isnormal(t_to_impact) ||
		t_to_impact > time ||
		t_to_impact < 0) {
		return;
	}

	std::clog << "Collision between " << A.name << " and " << B.name << std::endl;

	//move until the point of impact
	A.move (t_to_impact);
	B.move (t_to_impact);

	//for this section, basically turn the vectors into normal velocity and tangential velocity,
	//then do a 1D collision calculation, using the normal velocities
	//since a ' (prime symbol) wouldn't work, I've replaced it with a _ in variable names

	vect n (A.pos - B.pos);		//normal vector

	vect un (n / n.norm());		//unit vector of n
	vect unt (-un[1], un[0]);	//vector that is tangent to un

	var vAn = un.dot(A.v);		//A's velocity projected along un
	var vAt = unt.dot(A.v);		//A's velocity projected along unt

	var vBn = un.dot(B.v);		//same for B
	var vBt = unt.dot(B.v);

	//vAt and vBt will not change, so we don't do anything for them
	var vAt_ = vAt;
	var vBt_ = vBt;

	//vAn and vBt can be calculated with a simple 1D collision calculation
	var vAn_ = (vAn * (A.mass() - B.mass()) + 2 * B.mass() * vBn)
									/
							(A.mass() + B.mass());

	var vBn_ = (vBn * (B.mass() - A.mass()) + 2 * A.mass() * vAn)
									/
							(B.mass() + A.mass());

	//convert scalar normal and tangent velocities to vectors
	//also, apply coefficient of restitution
	const var R = 1;	//still working on this
	vect VAn = vAn_ * un * R;
	vect VAt = vAt_ * unt * R;

	vect VBn = vBn_ * un * R;
	vect VBt = vBt_ * unt * R;

	//add em up to get v'
	A.v = (VAn + VAt);
	B.v = (VBn + VBt);

	//move for the rest of the frame
	A.move (time - t_to_impact);
	B.move (time - t_to_impact);
}
