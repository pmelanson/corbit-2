enum navMode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};

struct physical_t {	//stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	const string name;  //I love C++ over C so much for this

	const unsigned long long int radius;   //radius of physical, in m
	const long double mass;	//mass of physical, in metric tonnes
	virtual long double totalMass();	//total mass of physical
	long double x, y; //the center of the physical, compared to the origin (center of solar system)
	long int a(), b();  //on-screen position of physical
	float turnRadians;	//radians physical is rotated from right
	long double distance (long double x, long double y), thetaToObject (physical_t &target);	//distance to point, theta between physical and object
	virtual void move();   //moves physical

	long double acceleration;    //total acceleration, no calculations are actually performed on this, just for printing purposes
	void acc (long double radians, long double acc),	//just calls accX and accY, and converts from m/s/ms to m/s/s
	accX (long double radians, long double acc), accY (long double radians, long double acc);
	long double Vx, Vy;   //the physical's speed (m/s) along each axis
	long double Vcen (physical_t &targ), Vtan (physical_t &targ), thetaV();	//gets the vcen and vtan relative to a target, as well as the theta of the velocity vector
	long double gravity (long double _x, long double _y, long double _mass);  //calculate gravitational acceleration to point
	void gravitate (physical_t &targ), detectCollision (physical_t &targ);  //gravitate towards/detect collision with target

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns
	float turnRateStep, maxTurnRate;    //values are in degrees per second

	const unsigned int fillColor;
	virtual void draw (unsigned int A, unsigned int B, long double zoom);	//draws physical

	struct AI_t {

		navMode_enum navmode;       //enum for which navmode to use, e.g. MAN, CCW
		string descriptor[NAVMAX];   //string describing the current nav mode

		AI_t () :
			navmode (MAN) {

			descriptor[MAN] = "Manual";
			descriptor[APP_TARG] = "App Targ";
			descriptor[DEPART_REF] = "Depart Ref";
			descriptor[CCW] = "CCW Prograde";
			descriptor[CW] = "CW Retrograde";
		}
	} AI;

	union { //declares either engine or atmosphereHeight

		float engine;	//percentage of engines that is engaged. In orbit, and in corbit (this program) there is no upper limit, and engines can go negative
		const unsigned long long int atmosphereHeight;	//height of atmosphere, in m
	};

	union { //declares either fuel or atmosphereDrag

		unsigned long int fuel;	//fuel in kg
		const unsigned int atmosphereDrag;	//I'm going to get around to implementing this after exams
	};

	physical_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long int _radius, unsigned int _fillColor, float _engine_atmosphereHeight, unsigned long long int _fuel_atmosphereDrag) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * PI / 180), maxTurnRate (41.5 * PI / 180) {
	}
};


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

	return (mass + fuel);
}

void physical_t::acc (long double radians, long double acc) {

	accX (radians, acc / totalMass() / 60);
	accY (radians, -acc / totalMass() / 60);
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

	return (sqrtf( ((x - targX) * (x - targX)) + ((y - targY) * (y - targY)) ));	//finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical_t::gravity (long double targX, long double targY, long double targMass) {

	return (G * mass * targMass / distance (targX, targY) );	//G * mass1 * mass2 / r^2
}

long int physical_t::a() { //on-screen x position of physical

	return ( (x - camera.x) * camera.actualZoom() + SCREEN_W / 2);
}

long int physical_t::b() { //on-screen y position of physical

	return ( (y - camera.y) * camera.actualZoom() + SCREEN_H / 2);
}

void physical_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor); //draws a circle.
}

void physical_t::gravitate (physical_t &targ) { //calculates gravitational acceleration, calling and target entity, then accelerates them

	long double force = (G * 1000 * mass * targ.mass) / (distance (targ.x, targ.y) * distance (targ.x, targ.y));
	long double theta = atan2f( -(y - targ.y), -(x - targ.x) );

	acc (force, theta);
	targ.acc (force, theta);
	entity[HAB]->acc (100, PI);
//		cout << name << "/" << targ.name << ": " << force << " " << theta << endl;
}

void physical_t::detectCollision (physical_t &targ) {

	if (distance (targ.x, targ.y) < radius + targ.radius) {
		long double VxNorm, VyNorm;
//		VxNorm
	}
}


struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned int atmosphereColor;	//color of atmosphere

	virtual void draw(unsigned int A, unsigned int B, long double zoom);	//draws planet with atmosphere

	solarBody_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, unsigned long long int _atmosphereHeight, unsigned long int _atmosphereDrag) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _atmosphereHeight, _atmosphereDrag),
		atmosphereColor (_atmosphereColor) {

		cout << endl << name << " initialized, with data of\n";
		cout << "x = " << x << endl;
		cout << "y = " << y << endl;
		cout << "Vx = " << Vx << endl;
		cout << "Vy = " << Vy << endl;
		cout << "mass = " << mass << endl;
		cout << "radius = " << radius << endl;
		cout << hex << "fillColor = " << hex << fillColor << endl;
		cout << hex << "atmosphereColor = " << atmosphereColor << endl;
		cout << "atmosphereHeight = " << dec << atmosphereHeight << endl;
		cout << "atmosphereDrag = " << atmosphereDrag << endl;
	}
};

void solarBody_t::draw(unsigned int A, unsigned int B, long double zoom) {

	if (zoom < 0.005){
	circlefill (buffer, A, B, zoom * (atmosphereHeight + radius), atmosphereColor);	//draws the atmosphere to the buffer

	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColor);	//draws the planet body to the buffer
	}
//	rectfill (buffer, A, B, zoom * (atmosphereHeight + radius), atmosphereColor);	//draws the atmosphere to the buffer
//
//	rectfill (buffer, A, B, radius * camera.actualZoom(), fillColor);	//draws the planet body to the buffer

	textprintf_ex (buffer, font, A, B, makecol (200, 200, 200), -1, "%s", name.c_str() );	//draws name of planet
}



struct ship_t : physical_t {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void move(), fireEngine();	//updated move function, includes fireEngine, which fires engines
	long double totalMass(), eccentricity (physical_t &targ);	//finds eccentricity of orbit around target, NOT accounting for other objects

	const unsigned int engineColor;	//color of engine
	const unsigned short int engineRadius;
	const float enginePower, burnRate;	//how powerful an engine is, in m/s/s, and how much fuel it burns

	virtual void draw(unsigned int A, unsigned int B, long double zoom);	//draws a circle with a line pointing the direction the ship is facing

	ship_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, unsigned long int _fuel) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, 0, _fuel),
		engineColor (_engineColor), engineRadius (_engineRadius), enginePower (15e4), burnRate (.444) {
	}
};

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

void ship_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the picture to the buffer
	line (buffer, A, B,	//draws the 'engine'
	      A + radius * cos (turnRadians) * zoom,
	      B + radius * sin (turnRadians) * zoom,
	      engineColor);
}


struct habitat_t : ship_t {

	void draw(unsigned int A, unsigned int B, long double zoom);	//draws habitat

	habitat_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, unsigned long int _fuel) :
		ship_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius, _fuel) {

		cout << endl << name << " initialized, with data of\n";
		cout << "x = " << x << endl;
		cout << "y = " << y << endl;
		cout << "Vx = " << Vx << endl;
		cout << "Vy = " << Vy << endl;
		cout << "mass = " << mass << endl;
		cout << "radius = " << radius << endl;
		cout << hex << "fillColor = " << hex << fillColor << endl;
		cout << hex << "engineColor = " << engineColor << endl;
		cout << "engineRadius = " << dec << engineRadius << endl;
		cout << "fuel = " << fuel << endl;
	}
};

void habitat_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the hull to the buffer

	if (engine == 0) {	//idle engines

		circlefill (buffer,	//draws the center 'engine'
		            A + (radius - engineRadius) * cos (turnRadians - (PI) ) * zoom,
		            B + (radius - engineRadius) * sin (turnRadians - (PI) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
		circlefill (buffer,	//draws the left 'engine'
		            A + radius * cos (turnRadians - (PI * .75) ) * zoom,
		            B + radius * sin (turnRadians - (PI * .75) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
		circlefill (buffer,	//draws the right 'engine'
		            A + radius * cos (turnRadians - (PI * 1.25) ) * zoom,
		            B + radius * sin (turnRadians - (PI * 1.25) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
	}

	else {

		circlefill (buffer,	//draws the center 'engine'
		            A + (radius - engineRadius) * cos (turnRadians - (PI) ) * zoom,
		            B + (radius - engineRadius) * sin (turnRadians - (PI) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
		circlefill (buffer,	//draws the left 'engine'
		            A + radius * cos (turnRadians - (PI * .75) ) * zoom,
		            B + radius * sin (turnRadians - (PI * .75) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
		circlefill (buffer,	//draws the right 'engine'
		            A + radius * cos (turnRadians - (PI * 1.25) ) * zoom,
		            B + radius * sin (turnRadians - (PI * 1.25) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
	}
}
