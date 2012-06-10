class viewpoint_t {

	const int zoomMagnitude;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
	const float zoomStep, panSpeed, maxZoom; //rate at which cameras zoom out, panning speed and how far you can zoom in
	const double minZoom; //the smaller this is, the farther you can zoom out

public:
	long double x, y, Vx, Vy;	//position of camera, and velocity

	long double zoomLevel, actualZoom();	//zoom level of camera. actualZoom returns zoomLevel ^ zoomMagnitude

	struct physical_t *target, *reference;	//target is center of camera. Once I get around to it, camera will autozoom to keep both target and reference in the screen
	bool track;	//toggles free camera mode. Note: free camera mode currentlly has not good pan speeds
	void updateSpeed(), shift(), autoZoom();	//shifts camera to keep it over target, eventually autoZoom will zoom to keep target and reference in the screen

	void zoom (short int direction), panX (short int direction), panY (short int direction);	//zoom function, and panning functions

	viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, float _panSpeed, long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed), x (0), y (0), Vx (0), Vy (0), zoomLevel (_zoomLevel), track (true)
	{}
} camera (	22,				0.05,		0.5,	1e-11,	1,			0);   //constructor initializes consts in the order they are declared, which is...
//			zoomMagnitude	zoomStep	maxZoom	minZoom	panSpeed    zoomLevel

class display_t {

	const unsigned short int gridSpace, lineSpace;	//space between each grid point and each line on the HUD
	const unsigned short int targVectorLength, vVectorLength;	//lengths of vector pointing to target and velocity vector
	const unsigned short int craftX, craftY;    //the x and y position of the craft displayed on the HUD, y position is craftY * lineSpace

public:
	struct physical_t *reference, *target;	//Vcen and Vtan calculations in relation to target, as well as target direction vector. reference doesn't do anything yet.
	void drawHUD(), drawGrid();	//draws HUD and grid to screen
	bool display, displayGrid;	//whether or not to draw the HUD, or to display grid

	display_t (short unsigned int _gridSpace, unsigned short int _lineSpace, unsigned short int _targVectorLength, unsigned short int _vVectorLength, unsigned short int _craftX, unsigned short int _craftY) :
		gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace), display (true), displayGrid (false) {
	}
} HUD (   18,		15,			110,				65,				140,	23);    //constructor initializes consts in the order they are declared, which is...
//        gridSpace	lineSpace	targVectorLength	vVectorLength	craftX	craftY


void display_t::drawGrid () {	//eventually I will make this a gravity grid, but not yet

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			_putpixel32 (buffer, x, y, makecol (255, 255, 255));

//	rectfill (buffer, 300, 320, 400, 420, makecol (0, 0, 255));
	fastline (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255,255,255));
	floodfill (buffer, 0, 100, 255);
}

void display_t::drawHUD () {

	rectfill (buffer, 0, 0, 330, 37 * lineSpace, 0);	//draws background for HUD
	rect (buffer, -1, -1, 330, 37 * lineSpace, makecol (255, 255, 255));	//draws outline for HUD

	textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbit V (m/s):"), textprintf_ex (buffer, font, 200, 1 * lineSpace, makecol (255, 255, 255), -1, "1337");
	textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Hab/Targ V diff:"), textprintf_ex (buffer, font, 200, 2 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg",
	        (entity[HAB]->Vx + entity[HAB]->Vy) - (target->Vx + target->Vy));
	textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centripetal V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel (kg):"), textprintf_ex (buffer, font, 200, 6 * lineSpace, makecol (255, 255, 255), -1, "%li", entity[HAB]->fuel);
	textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines (kg/s):");
	if (entity[HAB]->engine > 0)
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (150, 255, 150), -1, "%-10.1f", entity[HAB]->engine);
	else if (entity[HAB]->engine < 0)
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 150, 150), -1, "%-10.1f", fabs(entity[HAB]->engine));
	else
		textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 255, 255), -1, "%-10.1f", entity[HAB]->engine);
	textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 8 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lf", entity[HAB]->acceleration);
	textprintf_ex (buffer, font, lineSpace, 9 * lineSpace, makecol (200, 200, 200), -1, "Turning (degrees/s):"), textprintf_ex (buffer, font, 210, 9 * lineSpace, makecol (255, 255, 255), -1, "%.1Lf", fabs(entity[HAB]->turnRate) * 180 / PI);
	if (entity[HAB]->turnRate > 0)
		textprintf_ex (buffer, font, 240, 9 * lineSpace, makecol (50, 255, 50), -1, ">");
	else if (entity[HAB]->turnRate < 0)
		textprintf_ex (buffer, font, 200, 9 * lineSpace, makecol (50, 255, 50), -1, "<");
	textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Altitude (km):"), textprintf_ex (buffer, font, 200, 11 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg", entity[HAB]->distance (target->x, target->y) / 1000);
	textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Pitch (radians):");
	textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acc:"), textprintf_ex (buffer, font, 200, 13 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf",
	        entity[HAB]->distance (target->x, target->y) / (2 * entity[HAB]->distance (target->x, target->y) - target->radius) * cos (entity[HAB]->thetaV() - entity[HAB]->thetaToObject (*target)));
	textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Periapsis (m):");
	textprintf_ex (buffer, font, lineSpace, 15 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis (m):");

	entity[HAB]->draw (craftX, craftY, 1);	//draws the habitat onto the HUD

	line (buffer, craftX, craftY,	//draws velocity vector on habitat
	      craftX + (vVectorLength) * cos (entity[HAB]->thetaV()),
	      craftY + (vVectorLength) * sin (entity[HAB]->thetaV()),
	      makecol (255, 0, 0));
	textprintf_ex (buffer, font,	//draws target location in respect to habitat
	               craftX + (targVectorLength) * cos (entity[HAB]->thetaToObject (*target)),
	               craftY + (targVectorLength) * sin (entity[HAB]->thetaToObject (*target)),
	               makecol (255, 255, 255), -1, "%s", target->name.c_str());

	if (camera.track)
		textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 200, 31 * lineSpace, makecol (255, 255, 255), -1, "%s", camera.target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 200, 32 * lineSpace, makecol (255, 255, 255), -1, "%s", target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 33 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 200, 33 * lineSpace, makecol (255, 255, 255), -1, "%s", reference->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 34 * lineSpace, makecol (200, 200, 200), -1, "Autopilot:"), textprintf_ex (buffer, font, 200, 34 * lineSpace, makecol (255, 255, 255), -1, "%s", entity[HAB]->AI.descriptor[entity[HAB]->AI.navmode].c_str());
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

//	x += panSpeed / (actualZoom() * actualZoom() * direction);
	Vx += (panSpeed * direction) / actualZoom();
}

void viewpoint_t::panY (short int direction) {

//	y += panSpeed / (actualZoom() * actualZoom() * direction);
	Vy += (panSpeed * direction) / actualZoom();
}

void viewpoint_t::shift() {

	x += Vx;
	y -= Vy;
}

void viewpoint_t::updateSpeed() {

	Vx = target->Vx;
	Vy = target->Vy;

//	x = target->x;
//	y = target->y;
}
