#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
//using namespace std;

enum navMode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};
const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;


struct physical_t {	//stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	const std::string name;  //I love C++ over C so much for this

	const unsigned long long int radius;   //radius of physical, in m
	const long double mass;	//mass of physical, in metric tonnes
	virtual long double totalMass();	//total mass of physical
	long double x, y; //the center of the physical, compared to the origin (center of solar system)
	unsigned int a(), b();  //on-screen position of physical
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
		std::string descriptor[NAVMAX];   //string describing the current nav mode

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

	physical_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long int _radius, unsigned int _fillColor, float _engine_atmosphereHeight, unsigned long long int _fuel_atmosphereDrag) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * PI / 180), maxTurnRate (41.5 * PI / 180) {
	}
};


struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned int atmosphereColor;	//color of atmosphere

	virtual void draw(unsigned int A, unsigned int B, long double zoom);	//draws planet with atmosphere

	solarBody_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, unsigned long long int _atmosphereHeight, unsigned long int _atmosphereDrag) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _atmosphereHeight, _atmosphereDrag),
		atmosphereColor (_atmosphereColor) {

		std::cout << "\n" << name << " initialized, with data of\n";
		std::cout << "x = " << x << "\n";
		std::cout << "y = " << y << "\n";
		std::cout << "Vx = " << Vx << "\n";
		std::cout << "Vy = " << Vy << "\n";
		std::cout << "mass = " << mass << "\n";
		std::cout << "radius = " << radius << "\n";
		std::cout << std::hex << "fillColor = " << fillColor << "\n";
		std::cout << "atmosphereColor = " << atmosphereColor << "\n";
		std::cout << "atmosphereHeight = " << std::dec << atmosphereHeight << "\n";
		std::cout << "atmosphereDrag = " << atmosphereDrag << "\n";
	}
};


struct ship_t : physical_t {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void move(), fireEngine();	//updated move function, includes fireEngine, which fires engines
	long double totalMass(), eccentricity (physical_t &targ);	//finds eccentricity of orbit around target, NOT accounting for other objects

	const unsigned int engineColor;	//color of engine
	const unsigned short int engineRadius;
	const float enginePower, burnRate;	//how powerful an engine is, in m/s/s, and how much fuel it burns

	virtual void draw(unsigned int A, unsigned int B, long double zoom);	//draws a circle with a line pointing the direction the ship is facing

	ship_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, unsigned long int _fuel) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, 0, _fuel),
		engineColor (_engineColor), engineRadius (_engineRadius), enginePower (15e3), burnRate (.0444) {
	}
};

struct habitat_t : ship_t {

	void draw(unsigned int A, unsigned int B, long double zoom);	//draws habitat

	habitat_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, unsigned long int _fuel) :
		ship_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius, _fuel) {

		std::cout << "\n" << name << " initialized, with data of\n";
		std::cout << "x = " << x << "\n";
		std::cout << "y = " << y << "\n";
		std::cout << "Vx = " << Vx << "\n";
		std::cout << "Vy = " << Vy << "\n";
		std::cout << "mass = " << mass << "\n";
		std::cout << "radius = " << radius << "\n";
		std::cout << std::hex << "fillColor = " << fillColor << "\n";
		std::cout << "engineColor = " << engineColor << "\n";
		std::cout << "engineRadius = " << std::dec << engineRadius << "\n";
		std::cout << "fuel = " << fuel << "\n";
	}
};

#endif
