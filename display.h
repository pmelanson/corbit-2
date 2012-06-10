#ifndef DISPLAY_H
#define DISPLAY_H
#include <allegro.h>

extern BITMAP* buffer;

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

	viewpoint_t::viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, float _panSpeed, long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), panSpeed (_panSpeed), minZoom (_minZoom), x (0), y (0), Vx (0), Vy (0), zoomLevel (_zoomLevel), track (true) {
	}

};

class display_t {

	const unsigned short int gridSpace, lineSpace;	//space between each grid point and each line on the HUD
	const unsigned short int targVectorLength, vVectorLength;	//lengths of vector pointing to target and velocity vector
	const unsigned short int craftX, craftY;    //the x and y position of the craft displayed on the HUD, y position is craftY * lineSpace

public:
	struct physical_t *craft, *target, *reference;	//craft is the ship the HUD is displaying info for. Vcen and Vtan calculations in relation to target, as well as target direction vector. reference doesn't do anything yet.
	void drawHUD(), drawGrid();	//draws HUD and grid to screen
	bool display, displayGrid;	//whether or not to draw the HUD, or to display grid

	display_t (short unsigned int _gridSpace, unsigned short int _lineSpace, unsigned short int _targVectorLength, unsigned short int _vVectorLength, unsigned short int _craftX, unsigned short int _craftY) :
		gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace), display (true), displayGrid (false) {
	}

};

#endif
