/*******************

 .d8888b.                   888      d8b 888
d88P  Y88b                  888      Y8P 888
888    888                  888          888
888         .d88b.  888d888 88888b.  888 888888
888        d88""88b 888P"   888 "88b 888 888
888    888 888  888 888     888  888 888 888
Y88b  d88P Y88..88P 888     888 d88P 888 Y88b.
 "Y8888P"   "Y88P"  888     88888P"  888  "Y888

Designed by Patrick Melanson (patrick.melanstone@gmail.com)

Started on 23/03/2012


This program provides a realistic space flight simulation
The player's ship_t is called the "Hab"
Forces are as realistic as possible
Each pixel at camera.actualzoomLevel() = 0 level is equivalent the distance light travels in a vacumn in 1/299,792,458th of a second (one metre)
See changelog.txt for changelog past 31/03/2012

*******************/

/*******************

       CREDITS

-OCESS/Smamule Blatz/OCESS people
Baltz specifically, for helping me put gravity calculations into maths.
Just, be cool spacesim. Don't lose your swagger.
Without spacesim, there would be no space simulation, and thus this project would be, like, a 2D platformer game or something like that.

-Max J.
One of the bestest programmers I know IRL, and also a pretty good spacesimmer.
Helped me with 'good programming practice', whatever that is (next project I'll implement calling functions and classes, promise!)
Also a pretty good guy to bounce ideas off of. My int main is only 9 lines because of him.

-reddit and stackoverflow
For helping me with programming structure, practices, and methods
This project has definitely benefited from them,
I don't know how I could've made this as cool as it was without 'em

-Dr. Admiral Magwood
The Admiral of OCESS. Can't get much better than that. Maybe Robert Thirsk.
Made an orbit simulator in Quick Basic, which this is based off of (the concept, more than the code)
Good with orbital physics. Orbital projection algorithms wouldn't exist in corbit without him.

-zzyzzyxx
Technically also redditor, but was the most deserving of my upvotes.
Helped me with some neat programming tricks (isstream reading was his suggestion)
Definitely one of the most constructive members that I've been able to work with.

-Mrs. Odecki
One of the most chill teachers I've had.
Gotten me through some of my silly moments (eg array[x][y] array[y][x] mixups), as well as some opaque vector manipulation
Corbit wouldn't exist without her giving me time to complete it, and lots of it

*******************/

/*******************

Copyright (C) 2012 Patrick Melanson <patrick.melanstone@gmail.com>

This software is licensed under the WTFPL license 2012, as follows:




            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.




Isn't that an awesome license? I like it.

*******************/

/*******************

	  TODO LIST

-Implement atmospheric resistance
-Do more math stuff
-Add SRBs
-Add more objects (AYSE, ISS)
-Make framerate close up to planets acceptable
-Greater time acceleration with fewer CPU cycles (i.e. not calculating every step, only every 60th, then extrapolating)
-Background stars

	 KNOWN BUGS

-Program hangs when you set timestep too high
Not actually a bug, just don't do it.
-After pressing a mod key, escape key doesn't work
It looks like sticky keys, probably an Allegro thing. No fix, just press another not-mod key.

*******************/

/*******************

     CHANGELOG

23/03/2012: Started program, we'll have some fun times with this yet
24/03/2012: Added momentum physics with WASD keys, and bouncy collision detection off sides. Collision is a bit glitchy
25/03/2012: Added turning physics, looked at http://www.helixsoft.nl/articles/circle/sincos.htm <--- SO COOL, fixed collision glitchiness
26/03/2012: Rewrote to include fixeds and ints, and tidy up code
29/03/2012: Rewrote to take out fixeds, they are silly
31/03/2012: Built project with AutoVersioning, will autogenerate changelog now, into changelog.txt

*******************/

#include <iostream>
#include <allegro.h>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include "classDeclarations.h"
#include "parseDeclarations.h"
#include "version.h"
using namespace std;

///global and constant declarations///
const unsigned short int screenWidth = 1280, screenHeight = 980;    //my computer's resolution
bool printDebug = false;

BITMAP *buffer = NULL;
volatile unsigned int cycle = 0, cps = 0, cycleCounter = 0,	//used for running calculation loop and getting calculations performed per second
                                       fps = 0, fpsCounter = 0,	//used for getting frams per second
                                               inputTimer = 0;	//used for getting input
unsigned long long cycleRate = 60;	//how many times per second all calculations are performed
const unsigned short int FPS_COUNT_BPS = 2,	//how many times per second FPS and CPS (cycles per second) are calculated. Larger numbers result in faster refresh rates, but also larger extrapolations
        INPUT_BPS = 10;	//how many times per second keyboard is checked for input

const long double AU = 1495978707e2, G = 6.673e-11, PI = acos(-1.0L);

enum entity_enum {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, HAB, ENTITYMAX};


vector <physical_t*> entity;

//vector iterators are initialized here so they aren't redeclared thousands of times a millisecond
vector <physical_t*>::iterator it = entity.begin(), itX = entity.begin(), itY = entity.begin();	//it stands for iterator

///prototypes///
long double dot (long double x, long double y, long double a, long double b);	//calculates dot product of two 2D vectors
void CYCLE(), FPS(), INPUT();   //timer functions for running calculations, getting FPS, and getting input, respectively
void input(), drawBuffer(), drawDisplay(), debug();	//gets input, draws buffer to screen, draw stuff to buffer, draws debug information, respectively
void changeTimeStep (float step);	//changes how many calculations are performed per second (60 default)
void calculate();	//calculates velocities, accelerations, new positions, turn, etc. for all objects
void initialize(), initializeAllegro(), initializeFromFile();
void cleanup();

void CYCLE() {

    cycle++;
}
END_OF_FUNCTION (CYCLE);

void FPS() {

    cps = cycleCounter * FPS_COUNT_BPS;
    cycleCounter = 0;

    fps = fpsCounter * FPS_COUNT_BPS;
    fpsCounter = 0;
}
END_OF_FUNCTION (FPS);

void INPUT() {

    inputTimer++;
};
END_OF_FUNCTION (INPUT);

void drawBuffer() {

    textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%li%li%li.%li", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);	//prints version information

    draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
    draw_sprite (screen, buffer, 0, 0);
    clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void input() {

    if (key[KEY_W])	//throttle up, hold shift for fine adjustments. There is no cap on how high you can go, like in original version of orbit
        if (key_shifts & KB_SHIFT_FLAG)
            entity[HAB]->engine += 0.1;
        else
            entity[HAB]->engine += 1;

    if (key[KEY_S])	//ditto, for throttle down. Yes, I know you can go negative engines, in the original orbit version you can as well
        if (key_shifts & KB_SHIFT_FLAG)
            entity[HAB]->engine -= 0.1;
        else
            entity[HAB]->engine -= 1;

    if (key[KEY_A] && entity[HAB]->turnRate > -entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)	//turns hab, if hab is under the maximum turn rate, and the navmode is set to manual
        entity[HAB]->turnRate -= entity[HAB]->turnRateStep;

    if (key[KEY_D] && entity[HAB]->turnRate < entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)	//ditto, but for CW turning
        entity[HAB]->turnRate += entity[HAB]->turnRateStep;

    if (key[KEY_TAB]) {	//toggles camera tracking
        camera.track = !camera.track;
        camera.x = camera.target->x;
        camera.y = camera.target->y;
    }

    if (key[KEY_LEFT])	//pans camera left
        camera.panX (-1);

    if (key[KEY_RIGHT])	//pans camera right
        camera.panX (1);

    if (key[KEY_UP])	//pans camera up
        camera.panY (1);

    if (key[KEY_DOWN])	//you get the idea
        camera.panY (-1);

    if (key[KEY_PLUS_PAD])	//zooms camera in, holding alt increases calculations performed per second
        if (key_shifts & KB_ALT_FLAG)
        	changeTimeStep (1.1);
        else
        camera.zoom (1);

    if (key[KEY_MINUS_PAD])	//ditto, zooming out, decreases calculations
        if (key_shifts & KB_ALT_FLAG)
        	changeTimeStep (0.9);
        else
        camera.zoom (-1);

    if (key[KEY_TILDE])
        HUD.display = !HUD.display;

    if (key[KEY_Z])	//toggles printing of debug information
        printDebug = !printDebug;

    if (key[KEY_G])	//toggles grid drawing
        HUD.displayGrid = !HUD.displayGrid;

    if (key[KEY_BACKSPACE]) {	//cuts engines, holding down ctrl stops turning
        if (key_shifts & KB_CTRL_FLAG) {   //all those if statements there just slow the turnRate gradually
            if (fabs (entity[HAB]->turnRate) < entity[HAB]->turnRateStep)
                entity[HAB]->turnRate = 0;
            else if (entity[HAB]->turnRate > 0 && !key[KEY_A])   //you can't turn in the opposite direction of turning, and use the stop key at the same time
                entity[HAB]->turnRate -= entity[HAB]->turnRateStep;
            else if (entity[HAB]->turnRate < 0 && !key[KEY_D])   //ditto, but for the other key
                entity[HAB]->turnRate += entity[HAB]->turnRateStep;
        } else
            entity[HAB]->engine = 0;
    }

    if (key[KEY_ENTER])	//engines to 100%, RAMMING SPEED!
        if (key_shifts & KB_SHIFT_FLAG)
            entity[HAB]->engine *= -1;
        else
            entity[HAB]->engine = 100;

    ///set targ/ref to planet n from sun when holding down shift/ctrl, alt sets navmode for hab
    if (key[KEY_0])
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[SUN];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[SUN];
        else {
            camera.target = entity[SUN];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_1])	//AI to manual
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[MERCURY];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[MERCURY];
        else if (key_shifts & KB_ALT_FLAG)
            entity[HAB]->AI.navmode = MAN;
        else {
            camera.target = entity[MERCURY];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_2])	//AI to approach targ
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[VENUS];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[VENUS];
        else if (key_shifts & KB_ALT_FLAG)
            entity[HAB]->AI.navmode = APP_TARG;
        else {
            camera.target = entity[VENUS];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_3])	//AI to depart ref
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[EARTH];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[EARTH];
        else if (key_shifts & KB_ALT_FLAG)
            entity[HAB]->AI.navmode = DEPART_REF;
        else {
            camera.target = entity[EARTH];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_4])	//AI to ccw orientation around target
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[MARS];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[MARS];
        else if (key_shifts & KB_ALT_FLAG)
            entity[HAB]->AI.navmode = CCW;
        else {
            camera.target = entity[MARS];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_5])	//AI to cw orientation around target
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[JUPITER];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[JUPITER];
        else if (key_shifts & KB_ALT_FLAG)
            entity[HAB]->AI.navmode = CW;
        else {
            camera.target = entity[JUPITER];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_6])
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[SATURN];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[SATURN];
        else {
            camera.target = entity[SATURN];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_7])
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[URANUS];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[URANUS];
        else {
            camera.target = entity[URANUS];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_8])
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[NEPTUNE];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[NEPTUNE];
        else {
            camera.target = entity[NEPTUNE];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_9])
        if (key_shifts & KB_SHIFT_FLAG)
            HUD.target = entity[PLUTO];
        else if (key_shifts & KB_CTRL_FLAG)
            HUD.reference = entity[PLUTO];
        else {
            camera.target = entity[PLUTO];
            camera.x = camera.target->x;
            camera.y = camera.target->y;
        }

    if (key[KEY_H]) {	//sets center to hab
        camera.target = entity[HAB];
        camera.x = camera.target->x;
        camera.y = camera.target->y;
    }


    inputTimer--;
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

long double physical_t::a() { //on-screen x position of physical

    return ( (x - camera.x) * camera.actualZoom() + SCREEN_W / 2);
}

long double physical_t::b() { //on-screen y position of physical

    return ( (y - camera.y) * camera.actualZoom() + SCREEN_H / 2);
}

void physical_t::draw(unsigned int A, unsigned int B, long double zoom) {

    circlefill (buffer, A, B, radius * zoom, fillColor); //draws the physical to the buffer
}

void solarBody_t::draw(unsigned int A, unsigned int B, long double zoom) {

    if (zoom < 0.05) {
        circlefill (buffer, A, B, zoom * (atmosphereHeight + radius), atmosphereColor);	//draws the atmosphere to the buffer
        circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the planet body to the buffer
    }

    textprintf_ex (buffer, font, A, B + 10, makecol (200, 200, 200), -1, "%s", name.c_str() );
}

void ship_t::draw(unsigned int A, unsigned int B, long double zoom) {

    circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the picture to the buffer
    line (buffer, A, B,	//draws the 'engine'
          A + radius * cos (turnRadians) * zoom,
          B + radius * sin (turnRadians) * zoom,
          engineColor);
}

void habitat_t::draw(unsigned int A, unsigned int B, long double zoom) {

    circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the hull to the buffer

    if (engine == 0) {	//draws engine grayer than normal

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

    else {				//draws the engine as engineColor

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


void display_t::drawGrid () {	//eventually I will make this a gravity grid, but not yet

    unsigned short int x, y;

    for (x = 0; x < SCREEN_W; x += gridSpace)
        for (y = 0; y < SCREEN_H; y += gridSpace)
            putpixel (buffer, x, y, makecol (255, 255, 255));

//	rectfill (buffer, 300, 320, 400, 420, makecol (0, 0, 255));
//	fastline (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255, 255, 255));
//	floodfill (buffer, 0, 100, 255);
}

void display_t::drawHUD () {

    //330 is the width of the HUD, 38 * lineSpace is the height
    hline (buffer, 0, 38 * lineSpace, 330, makecol (255, 255, 255));	//draws bottom border of HUD
    vline (buffer, 330, 0, 38 * lineSpace,  makecol (255, 255, 255));	//draws right border of HUD
    rect (buffer, -1, -1, 330, 38 * lineSpace, makecol (255, 255, 255));	//draws outline for HUD

    textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbit V (m/s):"), textprintf_ex (buffer, font, 200, 1 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg", craft->orbitV(*target));
    textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Hab/Targ V (m/s):"), textprintf_ex (buffer, font, 200, 2 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lf", craft->Vtarg(target->V()));	//velocity relative to target
    textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centripetal V (m/s):"), textprintf_ex (buffer, font, 200, 3 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lf", craft->Vcen(*target));
    textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential V (m/s):"), textprintf_ex (buffer, font, 200, 4 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lf", craft->Vtan(*target));
    hline (buffer, 0, 5.25 * lineSpace, 330 - 1, makecol (150, 150, 150));
    ///end of Velocity Information section

    textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel (kg):"), textprintf_ex (buffer, font, 200, 6 * lineSpace, makecol (255, 255, 255), -1, "%li", craft->fuel);
    textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines (kg/s):");

    if (craft->engine > 0)
        textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (150, 255, 150), -1, "%-10.1f", craft->engine);
    else if (craft->engine < 0)
        textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 150, 150), -1, "%-10.1f", fabs(craft->engine));
    else
        textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 255, 255), -1, "%-10.1f", craft->engine);
    textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 8 * lineSpace, makecol (255, 255, 255), -1, "%-10.8Lf", craft->acceleration);
    textprintf_ex (buffer, font, lineSpace, 9 * lineSpace, makecol (200, 200, 200), -1, "Turning (degrees/s):"), textprintf_ex (buffer, font, 210, 9 * lineSpace, makecol (255, 255, 255), -1, "%.1Lf", fabs(craft->turnRate) * 180/PI);
    if (craft->turnRate > 0)
        textprintf_ex (buffer, font, 240, 9 * lineSpace, makecol (50, 255, 50), -1, ">");
    else if (craft->turnRate < 0)
        textprintf_ex (buffer, font, 200, 9 * lineSpace, makecol (50, 255, 50), -1, "<");
    hline (buffer, 0, 10.25 * lineSpace, 330 - 1, makecol (150, 150, 150));
    ///end of Ship Information section

    textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Altitude (km):"), textprintf_ex (buffer, font, 200, 11 * lineSpace, makecol (255, 255, 255), -1, "%-10.8g",
            (sqrtf(craft->distance (target->x, target->y)) - (craft->radius + target->radius) )/ 1000);
    textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Pitch (radians):");
    textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 13 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf",
            craft->distance (target->x, target->y) / (2 * craft->distance (target->x, target->y) - target->radius) * cos (craft->thetaV() - craft->thetaToObject (*target)));
    craft->eccentricity(*target);
    textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Periapsis (km):"), textprintf_ex (buffer, font, 200, 14 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lg", (craft->periapsis - (craft->radius + target->radius) )/1000);
    textprintf_ex (buffer, font, lineSpace, 15 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis (km):"), textprintf_ex (buffer, font, 200, 15 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lg", (craft->apoapsis - (craft->radius + target->radius) )/1000);
    ///end of Orbit Information section

    craft->draw (craftX, craftY, 1);	//draws the habitat onto the HUD

    line (buffer, craftX, craftY,	//draws velocity vector on habitat
          craftX + (vVectorLength) * cos (craft->thetaV() - target->thetaV()),
          craftY + (vVectorLength) * sin (craft->thetaV() - target->thetaV()),
          makecol (255, 0, 0));
    textprintf_ex (buffer, font,	//draws target location in respect to habitat
                   craftX + (targVectorLength) * cos (craft->thetaToObject (*target)),
                   craftY + (targVectorLength) * sin (craft->thetaToObject (*target)),
                   makecol (255, 255, 255), -1, "%s", target->name.c_str());
    ///end of Ship Visualization section

    if (camera.track)
        textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 200, 31 * lineSpace, makecol (255, 255, 255), -1, "%s", camera.target->name.c_str());
    textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 200, 32 * lineSpace, makecol (255, 255, 255), -1, "%s", target->name.c_str());
    textprintf_ex (buffer, font, lineSpace, 33 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 200, 33 * lineSpace, makecol (255, 255, 255), -1, "%s", reference->name.c_str());
    textprintf_ex (buffer, font, lineSpace, 34 * lineSpace, makecol (200, 200, 200), -1, "Autopilot:"), textprintf_ex (buffer, font, 200, 34 * lineSpace, makecol (255, 255, 255), -1, "%s", craft->AI.descriptor[craft->AI.navmode].c_str());
    textprintf_ex (buffer, font, lineSpace, 35 * lineSpace, makecol (200, 200, 200), -1, "FPS:"), textprintf_ex (buffer, font, 200, 35 * lineSpace, makecol (255, 255, 255), -1, "%d", fps);
    textprintf_ex (buffer, font, lineSpace, 36 * lineSpace, makecol (200, 200, 200), -1, "Timestep:"), textprintf_ex (buffer, font, 200, 36 * lineSpace, makecol (255, 255, 255), -1, "%.3f", float(cps) / 60);
    ///end of HUD Information section
}


void physical_t::turn () {

    turnRadians += turnRate / 60;

    if (turnRadians < 0)
        turnRadians += 2 * PI;

    else if (turnRadians > 2 * PI)
        turnRadians -= 2 * PI;
}

long double ship_t::totalMass() {

    return (mass + fuel);
}

void physical_t::acc (long double force, long double radians) {

    accX (force / totalMass() / 60, radians);
    accY (force / totalMass() / 60, radians);
    acceleration += fabs(force) / totalMass() / 60;
}

void physical_t::accX (long double force, long double radians) {	//takes angle at which to accelerate, and takes F/m for force

    Vx += cos (radians) * force;
}

void physical_t::accY (long double force, long double radians) {	//takes angle at which to accelerate, and takes F/m for force

    Vy -= sin (radians) * force;	//negative y acceleration because the zero point of a cartesian graph is top left corner of screen, with positive y values being underneath x axis (top of screen)
}

long double physical_t::orbitV (const physical_t &targ) {

    return sqrtf(
               (G * targ.mass) /
               sqrtf(distance (targ.x, targ.y))
           );
}

long double physical_t::V() {	//total magnitude of velocity vector

    return sqrtf( (Vx * Vx) + (Vy * Vy));	//basic pythagorean theorem here
}

long double physical_t::Vtarg (long double targV) {	//velocity relative to target, takes total velocity of target (i.e. when calling this, pass V() as the parameter)

    /*return sqrtf(	//basic pythagorean theorem to find magnitude of relative velocity vector
               (Vx - targ.Vx) * (Vx - targ.Vx) +
               (Vy - targ.Vy) * (Vy - targ.Vy) );*/

    return fabs(V() - targV);	//simple difference in velocities
}

long double physical_t::thetaV() {	//returns theta of velocity vector, relative to target's velocity

    return (-atan2f(Vy, Vx));
}

long double physical_t::Vcen (const physical_t &targ) {	//centripetal force

    /*return sqrtf(   //sqrt (V) * cos(theta)
               (Vx - targ.Vx) * (Vx - targ.Vx) +
               (Vy - targ.Vy) * (Vy - targ.Vy) )
           * cos(thetaToObject(targ) );
    first prototype, uses expensive square roots, as well as a taylor polynomial
    */

    return (Vx - targ.Vx) * cos (thetaToObject(targ)) + (Vy - targ.Vy) * sin (thetaToObject(targ));	//how fast calling physical is moving away from targ, positive values mean moving away from target (it is really centripetal velocity, but it makes more sense like this)
}

long double physical_t::Vtan (const physical_t &targ) {

    /*return fabs(sqrtf(   //sqrt (V) * sin(theta)
                    (Vx - targ.Vx) * (Vx - targ.Vx) +
                    (Vy - targ.Vy) * (Vy - targ.Vy) )
                * sin(thetaToObject(targ) ) );
    first prototype, uses expensive square roots, as well as a taylor polynomial
    */

    return (Vx - targ.Vx) * cos (thetaToObject(targ) + PI/2) + (Vy - targ.Vy) * sin (thetaToObject(targ) + PI/2);	//how fast calling physical is travelling tangentially, positive values mean CCW movement (normal orbit orientation for just about everything)
}

long double physical_t::thetaToObject (const physical_t &targ) {	//returns theta of angle made by the intersection of a line from the physical and the -x axis, at the target (e.g. when calling physical is directly to the right of targ, this will return PI)

    return  (atan2f( -(y - targ.y), -(x - targ.x)) );
}

long double physical_t::distance (long double targX, long double targY) {	//finds squared distance from physical to target

    return ((x - targX) * (x - targX)) + ((y - targY) * (y - targY));	//finds the distance between two entities, not squared
}

long double physical_t::stepDistance (long double targX, long double targY) {	//finds distance squared from physical to target next step, takes targ.pos + targ.v as parameters

    return (((x + Vx) - targX) * ((x + Vx) - targX)) + (((y + Vy) - targY) * ((y + Vy) - targY));	//finds the distance squared between two entities next cycle, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical_t::gravity (long double targX, long double targY, long double targMass) {

    return (G * mass * targMass / distance (targX, targY) );	//G * mass1 * mass2 / r^2
}

void physical_t::gravitate (physical_t &targ) { //calculates gravitational acceleration, calling and target entity, then accelerates them

    long double force = G * totalMass() * targ.totalMass() / distance (targ.x, targ.y);	//G((m1)(m2) / r^2)

    acc (force, thetaToObject (targ));
    targ.acc (-force, thetaToObject (targ));
}

void physical_t::detectCollision (physical_t &targ) {

    if (distance (targ.x, targ.y) < (radius + targ.radius)*(radius + targ.radius) ) {

        //I'll keep my previous attempts at this, just so that I don't end up having to rewrite all of this, even if I end up just keeping it for a bit

        /*
        cout << name << ", " << targ.name << endl;

        long double
        impact[2] = {Vx - targ.Vx, Vy - targ.Vy},	//this.V - targ.V
                    impactSpeed,
                    impulse[2] = {x - targ.x, y - targ.y};	//normalise (this.center - targ.center)

        impulse[0] /= sqrtf (impulse[0] * impulse[0] + impulse[1] * impulse[1]);	//normalising
        impulse[1] /= sqrtf (impulse[0] * impulse[0] + impulse[1] * impulse[1]);

        impactSpeed = -impulse[0] * impact[0] + impulse[0] * impact[1];	//dot product(impulse, impact)
        //		cout << "impactspeed: " << impactSpeed << endl;
        //		cout << "sqrtf" << impactSpeed << " * " << mass << " * " << targ.mass << endl;
        impulse[0] *= sqrtf(impactSpeed);	//impulse *= sqrt(impactSpeed)
        impulse[1] *= sqrtf(impactSpeed);
        //		cout << "impulse: " << impulse[0] << ", " << impulse[1] << endl;

        cout << name << ".V: " << Vx << ", " << Vy << endl;
        cout << targ.name << ".V: " << targ.Vx << ", " << targ.Vy << endl;

        Vx += impulse[0] / totalMass();	//this.Vx += impulse / this.mass
        Vy += impulse[1] / totalMass();
        targ.Vx += impulse[0] / targ.totalMass();	//targ.Vx += impulse / targ.mass
        targ.Vy += impulse[1] / targ.totalMass();

        cout << "new\n";
        cout << name << "V: " << Vx << ", " << Vy << endl;
        cout << targ.name << ".V: " << targ.Vx << ", " << targ.Vy << endl;

        first prototype, didn't work as intended (when hab collided with earth, it was accelerated at ridiculous speeds to the top left)
        */

        /*Vx = (Vx * (mass - targ.mass) + 2 * targ.mass * targ.Vx) /
        	(mass + targ.mass),
        Vy = (Vy * (mass - targ.mass) + 2 * targ.mass * targ.Vy) /
        	(mass + targ.mass);

        targ.Vx = (targ.Vx * (targ.mass - mass) + 2 * mass * Vx) /
        	(mass + targ.mass),
        targ.Vy = (targ.Vy * (targ.mass - mass) + 2 * mass * Vy) /
        	(mass + targ.mass);

        second prototype, was actually 1 dimensional collision, didn't work
        */
        /*
        //http://www.vobarian.com/collisions/2dcollisions2.pdf for how I got this algorithm

        //find normalUnit and tangentUnit between the two physicals
        long double normalUnit[2] = {x - targ.x, y - targ.y};
        normalUnit[0] /= sqrtf (normalUnit[0] * normalUnit[0] + normalUnit[1] * normalUnit[1]),	//finds unit vector of normal
                         normalUnit[1] /= sqrtf (normalUnit[0] * normalUnit[0] + normalUnit[1] * normalUnit[1]);
        long double tangentUnit[2] = {-normalUnit[1], normalUnit[0]};	//tangent unit is just the negative reciprocal of normal unit

        long double Vprojected[2][2] = {	//the velocity of each object, projected onto the tangentV and normalUnit vectors (scalar)
        	{
        		normalUnit[0] * Vx + normalUnit[1] * Vy,	//normal velocity for calling physical
        		tangentUnit[0] * Vx + tangentUnit[1] * Vy
        	},	//tangent velocity for calling physical
        	{
        		normalUnit[0] * targ.Vx + normalUnit[1] * targ.Vy,	//'' for targ physical, just doing the dot product between the respective unit here
        		tangentUnit[0] * targ.Vx + tangentUnit[1] * Vy
        	}
        };

        long double Vprime[2][2] = {	//the resulting velocity along the normal for each mass (scalar)

        	{
        		(Vprojected[0][0] * (mass - targ.mass) + 2 * targ.mass * Vprojected[1][0])	//calculates prime normal velocity, using 1D collision maths (wikipedia)
        		/ (mass + targ.mass),
        		Vprojected[0][1]
        	},	//prime tangent velocity is the same

        	{
        		(Vprojected[1][0] * (targ.mass - mass) + 2 * mass * Vprojected[0][0])	//'', but for the targ physical
        		/ (targ.mass + mass),
        		Vprojected[1][1]
        	}	//prime tangent velocity is still the same

        };

        Vprime[0][1] *= 0.8;	//prime tangent velocity shouldn't change with a perfectly elastic collision, but temporarily I'm going to approximate this degree of elasticity, for extra realism
        Vprime[1][1] *= 0.8;

        //calculate the final normal and tangential velocity vectors for both physicals, by multiplying the normalV (scalar) with normalUnit (vector), and same thing for tangent
        long double VnormPrime[2][2] = {
        	{
        		Vprime[0][0] * normalUnit[0],
        		Vprime[0][0] * normalUnit[1]
        	},
        	{
        		Vprime[1][0] * normalUnit[0],
        		Vprime[1][0] * normalUnit[1]
        	}

        },

        	VtanPrime[2][2] = {
        	{
        		Vprime[0][1] * tangentUnit[0],
        		Vprime[0][1] * tangentUnit[1]
        	},
        	{
        		Vprime[1][1] * tangentUnit[0],
        		Vprime[1][1] * tangentUnit[1]
        	}
        };

        //get final velocities of each vector, by V = Vnorm + Vtan (which are all vectors)
        Vx = VnormPrime[0][0] + VtanPrime[0][0],
        Vy = VnormPrime[0][1] + VtanPrime[0][1];

        targ.Vx = VnormPrime[1][0] + VtanPrime[1][0],
        targ.Vy = VnormPrime[1][1] + VtanPrime[1][1];
        another prototype, using projections and 1D collisions for JUST Vcen. Slowed down hab, but didn't bounce it
        */
        //just gets the Vcen and Vtan, then projects the new velocities onto the Vx and Vy axes
        /*Vx = -Vcen (targ) * cos (thetaToObject(targ)) +	//Vx = Vcen projected onto x axis + Vtan projected onto x axis
        	Vtan (targ) * cos (thetaToObject(targ));
        Vy = -Vcen (targ) * sin (thetaToObject(targ)) +	//'', but for Vy and y axis
        	Vtan (targ) * sin (thetaToObject(targ));

        targ.Vx = -targ.Vcen (*this) * cos (targ.thetaToObject(*this)) +	//same as above, but for target
        	targ.Vtan (*this) * cos (targ.thetaToObject(*this));
        targ.Vy = -targ.Vcen (*this) * sin (targ.thetaToObject(*this)) +
        	targ.Vtan (*this) * sin (targ.thetaToObject(*this));

        another prototype. The material I got this from was obscure, and I didn't really like the math
        */

        /*
        //there is a 'bug' in this wherein if you are on the earth, you cannot fire your engines, but in the real world you're not going to be firing your engines when you're sitting on them, or you'll explode
        engine = 0,
                 targ.engine = 0;

        long double normV = Vcen (targ),
                            tanV = Vtan (targ),
                                   thetaNorm = thetaToObject (targ),	//theta component of polar normal vector
                                               thetaTan = thetaNorm + PI/2;	//perpindicular to thetaNorm

        cout << "normV: " << normV << endl;
        cout << "tanV: " << tanV << endl;
        cout << "thetaNorm: " << thetaNorm << endl;
        cout << "thetaTan: " << thetaTan << endl;

        //find prime normV, using 1D collision equations
        long double primeNormV = (Vcen(targ) * (mass - targ.mass) + 2 * targ.mass * targ.Vcen(*this) ) /	//targ.Vcen(*this) = -Vcen(targ), but this looks more like the normal 1D collision equation
        				(mass + targ.mass);

        Vx = -primeNormV * cos (thetaNorm) + tanV * cos (thetaTan);detectCollision
        Vy = -primeNormV * sin (thetaNorm) + tanV * sin (thetaTan);

        targ.Vx = primeNormV * cos (thetaNorm) + tanV * cos (thetaTan);
        targ.Vy = primeNormV * sin (thetaNorm) + tanV * sin (thetaTan);

        another prototype. I have a lot of prototypes. This one in particular didn't work.
        */

        /*
        cout << endl << name << "/" << targ.name << " collision" << endl;
        //		engine = 0;
        //		targ.engine = 0;

        //Here, I take the Vcen and Vtan unit vectors, then project the post-collision velocities onto them, after using 1D collision equation to find out post-collision Vcen
        //note: instead of calling Vcen(targ) and targ.Vcen(*this), I instead call Vcen(targ) and -Vcen(targ), because that is the same thing, but more efficient as it is stored in the processor cache

        //finding Vcen prime:
        long double VcenPrime[2] = {	//VcenPrime[0] is attached to calling physical, VcenPrime[1] is for targ
            //for calling physical...
        //            (Vcen(targ) * (mass - targ.mass) + 2 * targ.mass * Vcen(targ) )
        //            / (mass + targ.mass),
            Vcen(targ),
            //for targ
        //            (Vcen(targ) * (targ.mass - mass) + 2 * mass * Vcen(targ) )
        //            / (targ.mass + mass)
            -Vcen(targ)
        };

        //and Vtan remains the same in a perfectly elastic collision, so we can leave that

        cout << "pre-Vcen:\n" << name << ": " << Vcen(targ) << "\t" << targ.name << ": " << Vcen(targ) << endl;
        cout << "post-Vcen:\n" << name << ": " << VcenPrime[0] << "\t" << targ.name << ": " << VcenPrime[1] << endl;
        cout << "\npre-collision\n";
        cout << name << ": " << Vx << ", " << Vy << endl;
        cout << targ.name << ": " << targ.Vx << ", " << targ.Vy << endl;

        long double cenV[2] = {Vx * cos(thetaToObject(targ)), Vy * sin(thetaToObject(targ))};
        long double tanV[2] = {Vx * cos(thetaToObject(targ)+PI/2), Vy * sin(thetaToObject(targ)+PI/2)};
        cout << "Vtan(targ) = " << Vtan(targ) << "\ntarg.Vtan(*this) = " << targ.Vtan(*this) << endl;

        Vx = VcenPrime[0] * cos(thetaToObject(targ)) + Vtan(targ) * cos(thetaToObject(targ) + PI/2);
        Vy = VcenPrime[0] * sin(thetaToObject(targ)) + Vtan(targ) * sin(thetaToObject(targ) + PI/2);
        //        again, targ.Vtan(*this) == -Vtan(targ)
        targ.Vx = Vcen(targ) * cos(thetaToObject(targ)) + Vtan(targ) * cos(thetaToObject(targ) + PI/2);
        targ.Vy = Vcen(targ) * sin(thetaToObject(targ)) + Vtan(targ) * sin(thetaToObject(targ) + PI/2);

        targ.Vx = cenV[0] * -cos(thetaToObject(targ)) + tanV[0] * cos(thetaToObject(targ) + PI/2);
        targ.Vy = cenV[1] * -sin(thetaToObject(targ)) + tanV[1] * sin(thetaToObject(targ) + PI/2);

        //        if (targ.name == "Habitat") {
        //            targ.Vx = Vx;
        //            targ.Vy = Vy;
        //            targ.x += cos(thetaToObject(targ));
        //            targ.y += sin(thetaToObject(targ));
        //        }
        //if you were to fire your engines while on earth (and not on the launchpad) you would blow up. I make sure you don't fire your engines here.
        targ.engine = 0;

        cout << "post-collision\n";
        cout << name << ": " << Vx << ", " << Vy << endl;
        cout << targ.name << ": " << targ.Vx << ", " << targ.Vy << endl;
            */

        /*if (targ.name == "Habitat"){
        	targ.Vx = Vx, targ.Vy = Vy;
        	targ.Vx -= cos(targ.thetaToObject(*this));
        	targ.Vy -= sin(targ.thetaToObject(*this));
        }
        else if (name == "Habitat"){
        	Vx = targ.Vx, Vy = targ.Vy;
        	Vx -= cos(thetaToObject(targ));
        	Vy -= sin(thetaToObject(targ));*/

        if (targ.name == "Habitat") {
            targ.Vx = Vx, targ.Vy = Vy;
            targ.Vx += cos(thetaToObject(targ));
            targ.Vy += sin(thetaToObject(targ));
        } else if (name == "Habitat") {
            Vx = targ.Vx, Vy = targ.Vy;
            Vx += cos(targ.thetaToObject(*this));
            Vy += sin(targ.thetaToObject(*this));
        }

    }
}

void ship_t::move() {

    fireEngine();

    x += Vx;
    y -= Vy;	//since the larger a y value is, the lower on the screen it appears
}

void ship_t::fireEngine() {

    if (fuel > 0) {
        acc (engine * enginePower, turnRadians);
        fuel -= (fabs(engine) * burnRate) / 60;
    }
}

void physical_t::eccentricity (physical_t &targ) {

    long double
    Ek = Vtarg (targ.V()),
         Ep = (-G * targ.mass) / sqrtf(distance(targ.x, targ.y)),
              E = Ek + Ep,
                  L2 = (sqrtf(distance(targ.x, targ.y)) * Vtan(targ)) * (sqrtf(distance(targ.x, targ.y)) * Vtan(targ)),	//(r*Vtan)^2
                       N = targ.mass * G,
                           e = sqrtf(1+ (2*E * L2) / (N * N)),
                               A = sqrtf(distance(targ.x, targ.y)) / fabs(2 * E);

    apoapsis = A * (1+e);

    if (e == 1)
        periapsis = A;
    else if (e > 1)
        periapsis = A * (e-1);
    else
        periapsis = A * (1-e);
}


void initializeAllegro() {

    allegro_init();
    install_keyboard();
    set_keyboard_rate(100, 100);
    set_color_depth (desktop_color_depth());
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);
    set_display_switch_mode(SWITCH_BACKGROUND);

    LOCK_VARIABLE (timer);
    LOCK_VARIABLE (fpsCounter);
    LOCK_VARIABLE (fps);
    LOCK_VARIABLE (inputTimer);
    LOCK_FUNCTION (INPUT);
    LOCK_FUNCTION (CYCLE);
    LOCK_FUNCTION (FPS);
    install_int_ex (CYCLE, BPS_TO_TIMER (60) );
    install_int_ex (FPS, BPS_TO_TIMER (FPS_COUNT_BPS));
    install_int_ex (INPUT, BPS_TO_TIMER (INPUT_BPS));
    buffer = create_bitmap (SCREEN_W, SCREEN_H);
}


void changeTimeStep (float step) {

    if (cycleRate * step <= 18446744073709551615ULL && cycleRate * step > 1) {   //making sure that cycleRate does not go too small, or beyond the limits of an unsigned long long int (but frankly, you'd better have a crazy computer to do this many cycles per second, you silly person)
        cycleRate *= step;	//in/decrements calculations performed per second
        install_int_ex (CYCLE, BPS_TO_TIMER (cycleRate) );	//applies changes
    }
}

void debug() {

    const unsigned short int spacing = SCREEN_H - SCREEN_H / 4;	//how low on the screen to print debug information

    textprintf_ex (buffer, font, 0, 0 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.x: %Lf", camera.target->x);
    textprintf_ex (buffer, font, 0, 10 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.y = %Lf", camera.target->y );
    textprintf_ex (buffer, font, 0, 20 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.a: %Lf", camera.target->a() );
    textprintf_ex (buffer, font, 0, 30 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.b: %Lf", camera.target->b() );
    textprintf_ex (buffer, font, 0, 40 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vx: %Lf", camera.target->Vx);
    textprintf_ex (buffer, font, 0, 50 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vy: %Lf", camera.target->Vy);
    textprintf_ex (buffer, font, 0, 60 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.turnRate: %Lf", camera.target->turnRate);
    textprintf_ex (buffer, font, 0, 70 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.radius: %llu", camera.target->radius);
    textprintf_ex (buffer, font, 0, 80 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.mass: %Lf", camera.target->mass);
    textprintf_ex (buffer, font, 0, 90 + spacing, makecol (200, 200, 200), -1, "DEBUG: arc tan: %Lf", atan2f (entity[HAB]->x - entity[EARTH]->x, entity[HAB]->y - entity[EARTH]->y) + PI * 0.5 );
    textprintf_ex (buffer, font, 0, 100 + spacing, makecol (200, 200, 200), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
    textprintf_ex (buffer, font, 0, 110 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera X: %Lf", camera.x);
    textprintf_ex (buffer, font, 0, 120 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera Y: %Lf", camera.y);
    textprintf_ex (buffer, font, 0, 150 + spacing, makecol (200, 200, 200), -1, "DEBUG: earth atmosphere: %llu", entity[EARTH]->atmosphereHeight);
    textprintf_ex (buffer, font, 0, 160 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.x: %Lf", camera.x);
    textprintf_ex (buffer, font, 0, 170 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.y: %Lf", camera.y);
    textprintf_ex (buffer, font, 0, 180 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.Vx: %Lf", camera.Vx);
    textprintf_ex (buffer, font, 0, 190 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.Vy: %Lf", camera.Vy);
}

void calculate() {

    for (itX = entity.begin(); itX != entity.end(); ++itX)
        for (itY = itX, ++itY; itY != entity.end(); ++itY) {
            (*itX)->gravitate (**itY);
            (*itX)->detectCollision (**itY);
        }

    for (it = entity.begin(); it != entity.end(); ++it) {
        (*it)->turn();
        (*it)->acceleration = 0;
        (*it)->move();
    }

    if (camera.track == true)
        camera.updateSpeed();
    camera.shift();

    cycle--;
    cycleCounter++;
}

void drawDisplay() {

    if (HUD.displayGrid)
        HUD.drawGrid();

    for (it = entity.begin(); it != entity.end(); ++it)
//	    if ((*it)->a() + (*it)->radius > 0 && (*it)->a() - (*it)->radius < SCREEN_W &&  //if entity is on screen
//            (*it)->b() + (*it)->radius > 0 && (*it)->b() - (*it)->radius < SCREEN_H)
        (*it)->draw((*it)->a(), (*it)->b(), camera.actualZoom());

    if (HUD.display)
        HUD.drawHUD();

    if (printDebug)
        debug();

    drawBuffer();

    fpsCounter++;
}

void cleanup() {

    destroy_bitmap (buffer);
    release_screen();

    for (it = entity.begin(); it != entity.end(); ++it)
        delete *it;
    entity.clear();
}

void initializeFromFile() {

    //file initialization
    ifstream datafile;
    datafile.open ("entities.txt");
    if (datafile.is_open())
        cout << "datafile open\n";
    if (datafile.good())
        cout << "datafile good\n";

    //data initializations
    string container = "", name = "";
    long double x = 1337, y = 1337, Vx = 0, Vy = 0;
    long double mass = 1337, radius = 1337, specialRadius = 413;
    unsigned fillColor = 255, specialColor = 255;
    short unsigned R = 255, G = 255, B = 255;
    float specialFloat = 612;
    string line = "";

    datafile.ignore (4096, '!');
    cout << uppercase;

    while (getline (datafile, line)) { //each loop through this reads in an entity

        container = "", name = "";
        x = 1337, y = 1337, Vx = 0, Vy = 0;
        mass = 1337, radius = 1337, specialRadius = 413;
        specialFloat = 612;
        fillColor = 255, specialColor = 255;

        stringstream iss (line);
        iss >> container;
        cout << endl << container;


        if (parse (datafile, name));
        else {
            name = "N/A";
            cout << "could not determine name, set to " << name << endl;
        }

        if (parse (datafile, x)) {
            x *= AU;
            if (x == 0)
                x = 1;
        } else
            cout << "x read fail for " << name << endl;

        if (parse (datafile, y)) {
            y *= AU;
            if (y == 0)
                y = 1;
        } else
            cout << "y read fail for " << name << endl;

        if (parse (datafile, Vx));
        else
            cout << "Vx read fail for " << name << endl;

        if (parse (datafile, Vy));
        else
            cout << "Vy read fail for " << name << endl;

        if (parse (datafile, mass))
            if (mass == 0)
                mass == 1;
            else
                cout << "mass read fail for " << name << endl;

        if (parse (datafile, radius))
            radius *= 2;
        else
            cout << "radius read fail for " << name << endl;

        if (parseColor (datafile, R, G, B))
            fillColor = makecol (R, G, B);
        else
            cout << "fillColor read fail for " << name << endl;

        if (parseColor (datafile, R, G, B))
            specialColor = makecol (R, G, B);
        else
            cout << "specialColor read fail for " << name << endl;

        if (parse (datafile, specialRadius));
        else
            cout << "specialRadius read fail for " << name << endl;

        if (parse (datafile, specialFloat));
        else
            cout << "specialFloat read fail for " << name << endl;

        if (container == "solarBody") {
//			specialRadius *= 100;

//            body.push_back (new solarBody_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
            entity.push_back (new solarBody_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
//			body_eg.push_back (boost::make_shared <solarBody_t> (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
        }

        if (container == "craft")
            if (name == "Habitat") {
                specialRadius *= 2;

//				craft.push_back (new habitat_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
                entity.push_back (new habitat_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
//				body_eg.push_back (boost::make_shared<habitat_t>(name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
            }

        if (container == "N/A") {

            cout << endl << name << " was not constructed with data of\n";
            cout << "x = " << x << endl;
            cout << "y = " << y << endl;
            cout << "Vx = " << Vx << endl;
            cout << "Vy = " << Vy << endl;
            cout << "mass = " << mass << endl;
            cout << "radius = " << radius << endl;
            cout << hex << "fillColor = " << hex << fillColor << endl;
            cout << hex << "specialColor = " << specialColor << endl;
            cout << "specialRadius = " << dec << specialRadius << endl;
            cout << "specialFloat = " << specialFloat << endl;
        }

        datafile.ignore (4096, '!');
    }


    /*long double xPos = 0, yPos = 0;

    datafile.ignore (4096, '<');

    while (getline (datafile, line, '>')) { //loops, getting the position of each background star

    	istringstream iss (line);
    }*/
    datafile.close();
    cout << nouppercase;
}

void initialize() {

    initializeAllegro();
    initializeFromFile();

    camera.target = entity[HAB];
    camera.x = camera.target->x;
    camera.y = camera.target->y;
    camera.reference = entity[EARTH];
    HUD.craft = entity[HAB];
    HUD.target = entity[EARTH];
    HUD.reference = entity[MARS];
}


int main (int argc, char *argv[]) {

    initialize();

    while (!key[KEY_ESC]) {

        while (inputTimer > 0)
            input();
        while (cycle > 0)
            calculate();
        drawDisplay();
    }

    cleanup();
    return 0;
}
END_OF_MAIN();  //5 lines of code in the main function. Now that is pretty sweet.
