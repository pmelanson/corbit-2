#include <iostream>

enum navMode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};
const long double Pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;

class viewpoint_t {

	const int zoomMagnitude;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
	const float zoomStep, panSpeed, maxZoom; //rate at which cameras zoom out, panning speed and how far you can zoom in
	const long double minZoom; //the smaller this is, the farther you can zoom out

public:
	long double x, y, Vx, Vy;	//position of camera, and velocity

	long double zoomLevel, actualZoom();	//zoom level of camera. actualZoom returns zoomLevel ^ zoomMagnitude

	struct physical_t *target, *reference;	//target is center of camera. Once I get around to it, camera will autozoom to keep both target and reference in the screen
	bool track;	//toggles free camera mode. Note: free camera mode currentlly has not good pan speeds
	void updateSpeed(), shift(), autoZoom();	//shifts camera to keep it over target, eventually autoZoom will zoom to keep target and reference in the screen

	void zoom (short direction), panX (short direction), panY (short direction);	//zoom function, and panning functions

	viewpoint_t::viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, float _panSpeed, long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), panSpeed (_panSpeed), minZoom (_minZoom), x (0), y (0), Vx (0), Vy (0), zoomLevel (_zoomLevel), track (true) {
	}

};

class display_t {

	const unsigned short gridSpace, lineSpace;	//space between each grid point and each line on the HUD
	const unsigned short targVectorLength, vVectorLength;	//lengths of vector pointing to target and velocity vector
	const unsigned short craftX, craftY;    //the x and y position of the craft displayed on the HUD, y position is craftY * lineSpace

public:
	struct physical_t *craft, *target, *reference;	//craft is the ship the HUD is displaying info for. Vcen and Vtan calculations in relation to target, as well as target direction vector. reference doesn't do anything yet.
	void drawHUD(), drawGrid();	//draws HUD and grid to screen
	bool display, displayGrid;	//whether or not to draw the HUD, or to display grid

	display_t (short unsigned _gridSpace, unsigned short _lineSpace, unsigned short _targVectorLength, unsigned short _vVectorLength, unsigned short _craftX, unsigned short _craftY) :
		gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace), display (true), displayGrid (false) {
	}

};


struct physical_t {	//stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	const std::string name;  //I love C++ over C so much for this

	const unsigned long long int radius;   //radius of physical, in m
	const long double mass;	//mass of physical, in metric tonnes
	virtual long double totalMass();	//total mass of physical
	long double x, y; //the center of the physical, compared to the origin (center of solar system)
	unsigned a(), b();  //on-screen position of physical
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

	const unsigned fillColor;
	virtual void draw (unsigned A, unsigned B, long double zoom);	//draws physical

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

		unsigned long fuel;	//fuel in kg
		const unsigned atmosphereDrag;	//I'm going to get around to implementing this after exams
	};

	physical_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, float _engine_atmosphereHeight, unsigned long long int _fuel_atmosphereDrag) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * Pi / 180), maxTurnRate (41.5 * Pi / 180) {
	}
};


struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned atmosphereColor;	//color of atmosphere

	virtual void draw(unsigned A, unsigned B, long double zoom);	//draws planet with atmosphere

	solarBody_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _atmosphereColor, unsigned long long int _atmosphereHeight, unsigned long _atmosphereDrag) :
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

	const unsigned engineColor;	//color of engine
	const unsigned short engineRadius;
	const float enginePower, burnRate;	//how powerful an engine is, in m/s/s, and how much fuel it burns

	virtual void draw(unsigned A, unsigned B, long double zoom);	//draws a circle with a line pointing the direction the ship is facing

	ship_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _engineColor, unsigned short _engineRadius, unsigned long _fuel) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, 0, _fuel),
		engineColor (_engineColor), engineRadius (_engineRadius), enginePower (15e3), burnRate (.0444) {
	}
};

struct habitat_t : ship_t {

	void draw(unsigned A, unsigned B, long double zoom);	//draws habitat

	habitat_t (std::string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long _radius, unsigned _fillColor, unsigned _engineColor, unsigned short _engineRadius, unsigned long _fuel) :
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

#ifndef VIEWPOINT_T
#define VIEWPOINT_T

void display_t::drawGrid () {	//eventually I will make this a gravity grid, but not yet

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			_putpixel32 (buffer, x, y, makecol (255, 255, 255));

//	rectfill (buffer, 300, 320, 400, 420, makecol (0, 0, 255));
	fastline (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255, 255, 255));
	floodfill (buffer, 0, 100, 255);
}

void display_t::drawHUD () {

	rectfill (buffer, 0, 0, 330, 37 * lineSpace, 0);	//draws background for HUD
	rect (buffer, -1, -1, 330, 37 * lineSpace, makecol (255, 255, 255));	//draws outline for HUD

	textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbit V (m/s):"), textprintf_ex (buffer, font, 200, 1 * lineSpace, makecol (255, 255, 255), -1, "1337");
	textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Hab/Targ V diff:"), textprintf_ex (buffer, font, 200, 2 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg",
	        (craft->Vx + craft->Vy) - (target->Vx + target->Vy));
	textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centripetal V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel (kg):"), textprintf_ex (buffer, font, 200, 6 * lineSpace, makecol (255, 255, 255), -1, "%li", craft->fuel);
	textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines (kg/s):");
	if (craft->engine > 0)
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (150, 255, 150), -1, "%-10.1f", craft->engine);
	else if (craft->engine < 0)
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 150, 150), -1, "%-10.1f", fabs(craft->engine));
	else
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 255, 255), -1, "%-10.1f", craft->engine);
	textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 8 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lf", craft->acceleration);
	textprintf_ex (buffer, font, lineSpace, 9 * lineSpace, makecol (200, 200, 200), -1, "Turning (degrees/s):"), textprintf_ex (buffer, font, 210, 9 * lineSpace, makecol (255, 255, 255), -1, "%.1Lf", fabs(craft->turnRate) * 180 / Pi);
	if (craft->turnRate > 0)
		textprintf_ex (buffer, font, 240, 9 * lineSpace, makecol (50, 255, 50), -1, ">");
	else if (craft->turnRate < 0)
		textprintf_ex (buffer, font, 200, 9 * lineSpace, makecol (50, 255, 50), -1, "<");
	textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Altitude (km):"), textprintf_ex (buffer, font, 200, 11 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg", craft->distance (target->x, target->y) / 1000);
	textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Pitch (radians):");
	textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acc:"), textprintf_ex (buffer, font, 200, 13 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf",
	        craft->distance (target->x, target->y) / (2 * craft->distance (target->x, target->y) - target->radius) * cos (craft->thetaV() - craft->thetaToObject (*target)));
	textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Periapsis (m):");
	textprintf_ex (buffer, font, lineSpace, 15 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis (m):");

	craft->draw (craftX, craftY, 1);	//draws the habitat onto the HUD

	line (buffer, craftX, craftY,	//draws velocity vector on habitat
	      craftX + (vVectorLength) * cos (craft->thetaV()),
	      craftY + (vVectorLength) * sin (craft->thetaV()),
	      makecol (255, 0, 0));
	textprintf_ex (buffer, font,	//draws target location in respect to habitat
	               craftX + (targVectorLength) * cos (craft->thetaToObject (*target)),
	               craftY + (targVectorLength) * sin (craft->thetaToObject (*target)),
	               makecol (255, 255, 255), -1, "%s", target->name.c_str());

	if (camera.track)
		textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 200, 31 * lineSpace, makecol (255, 255, 255), -1, "%s", camera.target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 200, 32 * lineSpace, makecol (255, 255, 255), -1, "%s", target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 33 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 200, 33 * lineSpace, makecol (255, 255, 255), -1, "%s", reference->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 34 * lineSpace, makecol (200, 200, 200), -1, "Autopilot:"), textprintf_ex (buffer, font, 200, 34 * lineSpace, makecol (255, 255, 255), -1, "%s", craft->AI.descriptor[craft->AI.navmode].c_str());
	textprintf_ex (buffer, font, lineSpace, 35 * lineSpace, makecol (200, 200, 200), -1, "FPS:"), textprintf_ex (buffer, font, 200, 35 * lineSpace, makecol (255, 255, 255), -1, "%d", fps);
	textprintf_ex (buffer, font, lineSpace, 36 * lineSpace, makecol (200, 200, 200), -1, "Timestep:"), textprintf_ex (buffer, font, 200, 36 * lineSpace, makecol (255, 255, 255), -1, "%f", float(cps) / 60);
}

void viewpoint_t::zoom (short int direction) {

	if (zoomLevel < maxZoom)
		zoomLevel += zoomStep * direction;
	else
		zoomLevel -= zoomStep;
}

long double viewpoint_t::actualZoom() {

	return (pow (zoomMagnitude, zoomLevel) + minZoom);
}

void viewpoint_t::panX (short int direction) {

	Vx += (panSpeed * direction) / actualZoom();
}

void viewpoint_t::panY (short int direction) {

	Vy += (panSpeed * direction) / actualZoom();
}

void viewpoint_t::shift() {

	x += Vx;
	y -= Vy;
}

void viewpoint_t::updateSpeed() {

	Vx = target->Vx;
	Vy = target->Vy;
}

#endif
