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
	vect n (A.pos - B.pos);				//normal vector

	vect un (n / n.norm());				//unit vector of n

	return un.dot(A.v - B.v);
}
var			calc::v_tan			(const entity_c &A, const entity_c &B) {
	vect n (A.pos - B.pos);	//normal vector

	vect un (n / n.norm());				//unit vector of n
	vect unt (-un[1], un[0]);			//vector that is tangent to un

	return unt.dot(A.v - B.v);
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

//bool		calc::approaching	(const entity_c &A, const entity_c &B) {
//	return (B.pos[0] - A.pos[0]) * (A.v[0] - B.v[0]) + (B.pos[1] - A.pos[1]) * (A.v[1] - B.v[1]) > 0;
////	if (abs(A.pos
//}

void		calc::detect_collision(entity_c &A, entity_c &B) {

	if (distance(A,B) > A.radius + B.radius &&
		velocity(A,B).norm() < A.radius + B.radius) {
		return;
	}

	var	a = velocity(A,B).squaredNorm(),
		b = 2 * position(A,B).dot(velocity(A,B)),
		c = position(A,B).squaredNorm() - (A.radius + B.radius)*(A.radius + B.radius);

	if (b*b - 4*a*c < 0) {
		std::clog << "discriminant: -----\n";
	}
	else if (b*b - 4*a*c == 0) {
		std::clog << "discriminant: 00000\n";
	}
	else if (b*b - 4*a*c > 0) {
		std::clog << "discriminant: +++++\n";
	}
	std::clog << "-sqrt=" << (-b - sqrt(b*b -4*a*c))/(2*a) << '\n';
	std::clog << "+sqrt=" << (-b + sqrt(b*b -4*a*c))/(2*a) << '\n';

	var t_to_impact = (-b - sqrt(b*b -4*a*c))/(2*a);

	if (isnan(t_to_impact)) {
		return;
	}

	A.pos += A.v * t_to_impact;
	B.pos += B.v * t_to_impact;

	vect n (A.pos - B.pos);		//normal vector

	vect un (n / n.norm());		//unit vector of n
	vect unt (-un[1], un[0]);	//vector that is tangent to un

	var vAn = v_cen(A,B);		//A's velocity projected along un
	var vAt = v_tan(A,B);		//A's velocity projected along unt

	var vBn = v_cen(A,B);		//same for B
	var vBt = v_tan(A,B);

	//vAt and vBt will not change, so we don't do anything for them
	var vAt_ = v_tan(A,B);
	var vBt_ = v_tan(B,A);

	//under a certain threshold, just set them to zero
	std::clog << "vAt_ = " << vAt_ << '\t' << "vBt_ = " << vBt_ << '\n';

	//vAn and vBt can be calculated with a simple 1D collision calculation
	var vAn_ = (vAn * (A.mass - B.mass) + 2 * B.mass * vBn)
									/
							(A.mass + B.mass);

	var vBn_ = (vBn * (B.mass - A.mass) + 2 * A.mass * vAn)
									/
							(B.mass + A.mass);


	std::clog << "vAn_ = " << vAn_ << '\t' << "vBn_ = " << vBn_ << '\n';

	//convert scalar normal and tangent velocities to vectors
	//also, apply coefficient of restitution
	const var R = 1;
	vect VAn = vAn_ * un * R;
	vect VAt = vAt_ * unt * R;


	vect VBn = vBn_ * un * R;
	vect VBt = vBt_ * unt * R;

	//add em up
	A.v = VAn + VAt;
	B.v = VBn + VBt;
}
