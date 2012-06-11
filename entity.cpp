#include "entity.h"
#include <math.h>
const long double G = 6.673e-11;

/*physical_t::physical_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long int _radius, unsigned int _fillColor, float _engine_atmosphereHeight, unsigned long long int _fuel_atmosphereDrag) :
	name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * PI / 180), maxTurnRate (41.5 * PI / 180) {
}*/

/*physical_t::AI_t::AI_t () :
	navmode (MAN) {

	descriptor[MAN] = "Manual";
	descriptor[APP_TARG] = "App Targ";
	descriptor[DEPART_REF] = "Depart Ref";
	descriptor[CCW] = "CCW Prograde";
	descriptor[CW] = "CW Retrograde";
}*/

void physical_t::move() {

	x += Vx;
	y -= Vy;
}

void physical_t::turn () {

	turnRadians += turnRate / 60;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	else if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;

	if (AI.navmode == APP_TARG) {

//		if (entity[HAB]->thetaToObject (HUD.target) == PI);
	}
}

long double physical_t::totalMass() {

	return (mass);
}

long double ship_t::totalMass() {

	return (mass + fuel / 1000);
}

void physical_t::acc (long double radians, long double acc) {

	accX (radians, acc / totalMass() / 60);
	accY (radians, -acc / totalMass() / 60);	//negative acceleration because of the way the computer draws on the screen
	acceleration += fabs(acc) / totalMass() / 60;
}

void physical_t::accX (long double radians, long double acc) {	//takes angle at which to accelerate, and takes F/m for acc

	Vx += cos (radians) * acc;
}

void physical_t::accY (long double radians, long double acc) {	//takes angle at which to accelerate, and takes F/m for acc

	Vy += sin (radians) * acc;
}

long double physical_t::Vcen (physical_t &targ) {	//centripetal force


}

long double physical_t::Vtan (physical_t &targ) {


}

long double physical_t::thetaV() {	//returns theta of velocity vector

	return (-atan2f(Vy, Vx));
}

long double physical_t::thetaToObject (physical_t &targ) {	//returns theta of angle made by the physical and the target

	return (atan2f( -(y - targ.y), -(x - targ.x)) );
}

long double physical_t::distance (long double targX, long double targY) {	//finds distance from physical to target

	return (sqrtf( (((x + Vx) - targX) * ((x + Vx) - targX)) + (((y + Vy) - targY) * ((y + Vy) - targY)) ));	//finds the distance between two entities next cycle, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical_t::gravity (long double targX, long double targY, long double targMass) {

	return (G * mass * targMass / distance (targX, targY) );	//G * mass1 * mass2 / r^2
}

void physical_t::gravitate (physical_t &targ) { //calculates gravitational acceleration, calling and target entity, then accelerates them

	long double force = (G * 1000 * mass * targ.mass) / (distance (targ.x, targ.y) * distance (targ.x, targ.y));
	long double theta = atan2f( -(y - targ.y), -(x - targ.x) );

	acc (force, theta);
	targ.acc (force, theta);
//		cout << name << "/" << targ.name << ": " << force << " " << theta << endl;
}

void physical_t::detectCollision (physical_t &targ) {

	if (distance (targ.x + targ.Vx, targ.y + targ.Vy) < radius + targ.radius) {	//This line of code is responsible for me knowing about dot products, scalars, normalisation. Kind of.
		long double
		impact[2] = {Vx - targ.Vx, Vy - targ.Vy},	//this.V - targ.V
		impactSpeed,
		impulse[2] = {x - targ.x, y - targ.y};	//normalise (this.center - targ.center)

		impulse[0] /= sqrtf (impulse[0] * impulse[0] + impulse[1] * impulse[1]);	//normalising
		impulse[1] /= sqrtf (impulse[0] * impulse[0] + impulse[1] * impulse[1]);
		impactSpeed = impulse[0] * impact[0] + impulse[0] * impact[1];	//dot product(impulse, impact)
		impulse[0] *= sqrtf(impactSpeed * mass * targ.mass);	//impulse *= sqrt(impactSpeed * this.mass * targ.mss)
		impulse[1] *= sqrtf(impactSpeed * mass * targ.mass);

		Vx += impulse[0] / mass;	//this.Vx += impulse / this.mass
		Vy += impulse[1] / mass;
		targ.Vx += impulse[0] / targ.mass;	//targ.Vx += impulse / targ.mass
		targ.Vy += impulse[1] / targ.mass;
	}
}

void ship_t::move() {

	fireEngine();

	x += Vx;
	y -= Vy;
}

void ship_t::fireEngine() {

	if (fuel > 0) {
		acc (turnRadians, engine * enginePower);
		fuel -= (fabs(engine) * burnRate) / 60;
	}
}

long double ship_t::eccentricity (physical_t &targ) {

	/*long double e, E, h, u;
	E = total energy / targ.mass;

	term2 = 2 * orbEnergy * AngularVelocity^2 / (G * targ.mass)^2

	e = sqrtf(++
	          (2 * E * h * h) /
	          (u * u));*/
}
