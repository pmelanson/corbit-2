#include <iostream>
using namespace std;
enum navmode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};
long double Pi = acos(-1.0L);

///class declarations///
class viewpoint_t {

	int zoomMagnitude;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
	float zoomStep, panSpeed, maxZoom; //rate at which cameras zoom out, panning speed and how far you can zoom in
	long double minZoom; //the smaller this is, the farther you can zoom out

public:
	long double x, y, Vx, Vy;	//position of camera, and velocity

	long double zoomLevel, actualZoom();	//zoom level of camera. actualZoom returns zoomLevel ^ zoomMagnitude

	struct physical_t *target, *reference;	//target is center of camera. Once I get around to it, camera will autozoom to keep both target and reference in the screen
	bool track;	//toggles free camera mode. Note: free camera mode currentlly has not good pan speeds
	void updateSpeed(), shift(), autoZoom();	//shifts camera to keep it over target, eventually autoZoom will zoom to keep target and reference in the screen

	void zoom (short direction), panX (short direction), panY (short direction);	//zoom function, and panning functions

	viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, float _panSpeed, long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), panSpeed (_panSpeed), minZoom (_minZoom), x (0), y (0), Vx (0), Vy (0), zoomLevel (_zoomLevel), track (true) {
	}

} camera (22,				0.05,		0.5,	1e-11,	1,			0);   //constructor initializes consts in the order they are declared, which is...
//					zoomMagnitude	zoomStep	maxZoom	minZoom	panSpeed    zoomLevel

class display_t {

	unsigned short gridSpace, lineSpace;	//space between each grid point and each line on the HUD
	unsigned short targVectorLength, vVectorLength;	//lengths of vector pointing to target and velocity vector
	unsigned short craftX, craftY;    //the x and y position of the craft displayed on the HUD, y position is craftY * lineSpace

public:
	struct physical_t *craft, *target, *reference;	//craft is the ship the HUD is displaying info for. Vcen and Vtan calculations in relation to target, as well as target direction vector. reference doesn't do anything yet.
	void drawHUD(), drawGrid();	//draws HUD and grid to screen
	bool display, displayGrid;	//whether or not to draw the HUD, or to display grid

	display_t (short unsigned _gridSpace, unsigned short _lineSpace, unsigned short _targVectorLength, unsigned short _vVectorLength, unsigned short _craftX, unsigned short _craftY) :
		gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace), display (true), displayGrid (false) {
	}

} HUD (	18,			15,			110,				65,				140,	23);   //constructor initializes consts in the order they are declared, which is...
//     			gridSpace	lineSpace	targVectorLength	vVectorLength	craftX	craftY

struct physical_t {	//stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	string name;  //I love C++ over C so much for this

	unsigned long long int radius;   //radius of physical, in m
	long double mass;	//mass of physical, in kg (easier to do gravity calculations with)
	virtual long double totalMass() {
		return mass;    //total mass of physical
	}
	long double x, y; //the center of the physical, compared to the origin (center of solar system)
	long double a(), b();  //on-screen position of physical
	float turnRadians;	//radians physical is rotated from right
	long double distance (long double targX, long double targY), stepDistance (long double targX, long double targY), thetaToObject (const physical_t &target);	//distance squared to point, distance squared next cycle, theta between physical and object
	virtual void move() {
		x += Vx, y -= Vy;    //moves physical
	}

	long double acceleration;    //total acceleration, no calculations are actually performed on this, just for printing purposes
	void acc (long double force, long double radians),	//just calls accX and accY, and converts from m/s/ms to m/s/s
	accX (long double force, long double radians), accY (long double force, long double radians);
	long double Vx, Vy, V(), thetaV();   //the physical's speed (m/s) along each axis and magnitude/theta of velocity vector
	long double Vcen (const physical_t &targ), Vtan (const physical_t &targ), Vtarg (long double targV);	//gets the vcen and vtan relative to a target, and velocity relative to target
	long double orbitV (const physical_t &targ);	//calculates the relative velocity needed to orbit at current height around target, NOT TAKING INTO ACCOUNT MASS OF CALLING PHYSICAL
	long double gravity (long double targX, long double targY, long double targMass);  //calculate gravitational acceleration to object
	long double apoapsis, periapsis;	//stores periapsis and apoapsis around targ, updated by calling eccentricity
	void eccentricity (physical_t &targ);	//eccentricity of orbit around object NOT ACCOUNTING FOR MASS OF CALLING PHYSICAL
	void gravitate (physical_t &targ), detectCollision (physical_t &targ);  //gravitate towards/detect collision with target

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns
	float turnRateStep, maxTurnRate;    //values are in degrees per second

	unsigned fillColor;
	virtual void draw (unsigned A, unsigned B, long double zoom);	//draws physical

	struct AI_t {

		navmode_enum navmode;       //enum for which navmode to use, e.g. MAN, CCW
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
		const unsigned long long atmosphereHeight;	//height of atmosphere, in m
	};

	union { //declares either fuel or atmosphereDrag

		unsigned long fuel;	//fuel in kg
		const unsigned atmosphereDrag;	//I'm going to get around to implementing this after exams
	};

	physical_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, float _engine_atmosphereHeight, unsigned long long int _fuel_atmosphereDrag) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (3.1), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * Pi / 180), maxTurnRate (41.5 * Pi / 180) {
	}
};


struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned atmosphereColor;	//color of atmosphere

	virtual void draw(unsigned A, unsigned B, long double zoom);	//draws planet with atmosphere

	solarBody_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _atmosphereColor, unsigned long long int _atmosphereHeight, unsigned long _atmosphereDrag) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _atmosphereHeight, _atmosphereDrag),
		atmosphereColor (_atmosphereColor) {

		cout << "\n" << name << " initialized, with data of\n";
		cout << "x = " << x << "\n";
		cout << "y = " << y << "\n";
		cout << "Vx = " << Vx << "\n";
		cout << "Vy = " << Vy << "\n";
		cout << "mass = " << mass << "\n";
		cout << "radius = " << radius << "\n";
		cout << hex << "fillColor = " << fillColor << "\n";
		cout << "atmosphereColor = " << atmosphereColor << "\n";
		cout << "atmosphereHeight = " << dec << atmosphereHeight << "\n";
		cout << "atmosphereDrag = " << atmosphereDrag << "\n";
	}
};

struct ship_t : physical_t {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void move(), fireEngine();	//updated move function, includes fireEngine, which fires engines
	long double totalMass();	//finds eccentricity of orbit around target, NOT accounting for other objects

	const unsigned engineColor;	//color of engine
	const unsigned short engineRadius;
	const float enginePower, burnRate;	//how powerful an engine is, in m/s/s, and how much fuel it burns

	virtual void draw(unsigned A, unsigned B, long double zoom);	//draws a circle with a line pointing the direction the ship is facing

	ship_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _engineColor, unsigned short _engineRadius, unsigned long _fuel) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, 0, _fuel),
		engineColor (_engineColor), engineRadius (_engineRadius), enginePower (1e6), burnRate (.0444) {
	}
};


struct habitat_t : ship_t {

	void draw(unsigned A, unsigned B, long double zoom);	//draws habitat

	habitat_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _engineColor, unsigned short _engineRadius, unsigned long _fuel) :
		ship_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius, _fuel) {

		cout << "\n" << name << " initialized, with data of\n";
		cout << "x = " << x << "\n";
		cout << "y = " << y << "\n";
		cout << "Vx = " << Vx << "\n";
		cout << "Vy = " << Vy << "\n";
		cout << "mass = " << mass << "\n";
		cout << "radius = " << radius << "\n";
		cout << hex << "fillColor = " << fillColor << "\n";
		cout << "engineColor = " << engineColor << "\n";
		cout << "engineRadius = " << dec << engineRadius << "\n";
		cout << "fuel = " << fuel << "\n";
	}
};
