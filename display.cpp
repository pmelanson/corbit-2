#ifndef ENTITY_H
#include "entity.h"
#endif

#include "display.h"

#include <allegro.h>
#include <math.h>
volatile unsigned int fps, cps;
const long double Pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;

BITMAP* buffer = NULL;

extern viewpoint_t camera;

/*viewpoint_t camera (22,				0.05,		0.5,	1e-11,	1,			0);   //constructor initializes consts in the order they are declared, which is...
//					zoomMagnitude	zoomStep	maxZoom	minZoom	panSpeed    zoomLevel

display_t HUD (	18,			15,			110,				65,				140,	23);    //constructor initializes consts in the order they are declared, which is...
//     			gridSpace	lineSpace	targVectorLength	vVectorLength	craftX	craftY*/


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



/*viewpoint_t::viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, float _panSpeed, long double _zoomLevel) :
	zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed), x (0), y (0), Vx (0), Vy (0), zoomLevel (_zoomLevel), track (true) {
}

viewpoint_t camera (22,				0.05,		0.5,	1e-11,	1,			0);   //constructor initializes consts in the order they are declared, which is...
//					zoomMagnitude	zoomStep	maxZoom	minZoom	panSpeed    zoomLevel*/


/*display_t::display_t (short unsigned int _gridSpace, unsigned short int _lineSpace, unsigned short int _targVectorLength, unsigned short int _vVectorLength, unsigned short int _craftX, unsigned short int _craftY) :
	gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace), display (true), displayGrid (false) {
}

display_t HUD (	18,			15,			110,				65,				140,	23);    //constructor initializes consts in the order they are declared, which is...
//        		gridSpace	lineSpace	targVectorLength	vVectorLength	craftX	craftY
*/
