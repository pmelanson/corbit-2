/*******************
CORBIT Program

Designed by Patrick Melanson

Started on 23/03/2012


This program provides a realistic space flight simulation
The player's ship is called the "Hab"
Forces are as realistic as possible
See changelog.txt for changelog past 31/03/2012

*******************/

/*******************
     CHANGELOG

23/03/2012: Started program, we'll have some fun times with this yet
24/03/2012: Added momentum physics with WASD keys, and bouncy collision detection off sides. Collision is a bit glitchy
25/03/2012: Added turning physics, looked at http://www.helixsoft.nl/articles/circle/sincos.htm <--- SO COOL, fixed collision glitchiness
26/03/2012: Rewrote to include fixeds and ints, and tidy up code
29/03/2012: Rewrote to take out fixeds, they are silly
31/03/2012: Built project with AutoVersioning, will autogenerate changelog now, into changelog.txt. Smell ya later!

*******************/

#include <allegro.h>
#include <math.h>
#include "version.h"

//globals
const int screenWidth = 1240;
const int screenHeight = 950;
const float zoomMagnitude = 2;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
const float zoomStep = 0.02; //rate at which cameras zoom out
const double maxZoom = 20;
BITMAP *buffer = NULL;
volatile int timer = 0;
volatile int milliseconds = 0;
const float PI = 3.14159265;
const double G = 6.673e-11;
enum craft {HAB, CRAFTMAX};
enum planet {MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLANETMAX};


//prototypes
void timeStep();
void input();
void drawHab ();
void drawBuffer ();
void detectCollision ();
void debug();
void gravitate();
void drawGrid();

//beginning of class declarations
struct viewpoint {

    float x;
    float y;
    float zoom;
    float actualZoom();
};

struct entity { //stores data about any physical entity, such as mass and radius, acceleration, velocity, and angle from right

    char name[21];

    double mass;
    unsigned int radius;   //mass of entity, to be used in calculation F=ma, and radius of entity
    long double gravity;  //G * mass of object
    void gravitate();
    float x, y; //the center of the entity
    float a();
    float b();
    float turnRadians;
    float distance (float x, float y);
    void move ();   //moves entity

    void accelerate();
    float acc;  //net acceleration of entity
    float radians;    //the degree at which the entity is velocitying from the right, in radians
    void accX (float radians, float acc); //the entity's acceleration (m/s/s) along the x axis
    void accY (float radians, float acc); //''
    float Vx, Vy;   //the entity's speed (m/s) along each axis

    void turn ();   //turns the entity
    float turnRate; //rate at which the entity turns
    float degrees();  //normal degrees (360 in a circle) at which the entity is rotated from facing right

    void draw();    //draws entity
    int fillColour;
    void clearValues();
};

struct ship : entity {  //stores information about a pilotable ship, in addition to information already stored by an entity

    void fireEngine();
    float engine;
    unsigned int engineColour;

    void draw();
};

struct body : entity {   //stores information about an astronomical body, in addition to information already stored by an entity

    unsigned int atmosphereHeight;
    unsigned int atmosphereDrag;

    unsigned int atmosphereColour;
    void draw();
};

viewpoint camera;
ship craft[CRAFTMAX - 1]; //all ships. Default ship is the Hab, the first one declared
body planet[PLANETMAX - 1];    //all planets in our solar system, ordered in distance from sun. Earth is 3


int main () {

    //allegro initializations
    allegro_init();
    install_keyboard();
    set_color_depth (desktop_color_depth() );
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

    LOCK_VARIABLE (timer);
    LOCK_FUNCTION (timestep);
    install_int_ex (timeStep, BPS_TO_TIMER (60) );

    //bitmap initializations
    buffer = create_bitmap (screenWidth, screenHeight);

    //data initializations

    strcpy (planet[EARTH].name, "Earth");
    planet[EARTH].x = screenWidth / 2;
    planet[EARTH].y = screenHeight / 2;
    planet[EARTH].radius = 200;
    planet[EARTH].mass = 5.9742e8;
    planet[EARTH].fillColour = makecol (0, 255, 0);
    planet[EARTH].atmosphereColour = makecol (0, 0, 255);
    planet[EARTH].atmosphereHeight = 3;
    planet[EARTH].gravity = planet[EARTH].mass * G;

    strcpy (craft[HAB].name, "Habitat");
    craft[HAB].fillColour = makecol (211, 211, 211);
    craft[HAB].engineColour = makecol (139, 0, 0);
    craft[HAB].radius = 30;
    craft[HAB].x = screenWidth / 2 + planet[EARTH].radius + craft[HAB].radius;
    craft[HAB].y = screenHeight / 2;
    craft[HAB].mass = 35000;

    camera.zoom = pow (camera.actualZoom(), 1 / zoomMagnitude);
    camera.x = craft[HAB].x - (screenWidth / 4);
    camera.y = craft[HAB].y - (screenHeight / 4);

    while (!key[KEY_ESC]) {

        while (timer > 0) {

            input();

            craft[HAB].turn();
            craft[HAB].fireEngine();
            craft[HAB].gravitate();
            detectCollision();
            craft[HAB].move();


            timer--;
        }

        drawGrid();
        planet[EARTH].draw();
        craft[HAB].draw();
        craft[HAB].clearValues();

        debug();

        drawBuffer();


    }

    //end of program
    destroy_bitmap (buffer);
    return (0);
}
END_OF_MAIN();


void input () {

    if (key[KEY_A]) {
        craft[HAB].turnRate -= 0.1;
    }

    if (key[KEY_D]) {
        craft[HAB].turnRate += 0.1;
    }

    if (key[KEY_W]) {
        craft[HAB].engine ++;

    }

    if (key[KEY_S]) {
        craft[HAB].engine --;
    }

    if (key[KEY_BACKSPACE]) {
        if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
            craft[HAB].turnRate = 0;
        else
            craft[HAB].engine = 0;
    }

    if (key[KEY_ENTER])
        craft[HAB].engine = 100;

    if (key[KEY_LEFT])
        camera.x -= maxZoom + 1 - camera.actualZoom();

    if (key[KEY_RIGHT])
        camera.x += maxZoom + 1 - camera.actualZoom();

    if (key[KEY_UP])
        camera.y -= maxZoom + 1 - camera.actualZoom();

    if (key[KEY_DOWN])
        camera.y += maxZoom + 1 - camera.actualZoom();

    if (key[KEY_PLUS_PAD]) {
        if (camera.actualZoom() < maxZoom)
            camera.zoom += zoomStep;
    }

    if (key[KEY_MINUS] || key[KEY_MINUS_PAD]) {
        camera.zoom -= zoomStep;
    }

}

void drawBuffer () {

    textprintf_ex (buffer, font, 0, screenHeight - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

    draw_sprite (buffer, screen, screenHeight, screenWidth); // Draw the buffer to the screen
    draw_sprite (screen, buffer, 0, 0);
    clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void timeStep() {

    timer++;
}
END_OF_FUNCTION (timeStep);

void entity::move() {

    x += Vx;
    y += Vy;

    clearValues();
}

void ship::fireEngine() {

    accX (turnRadians, engine);
    accY (turnRadians, engine);
}

void debug() {

    textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %f", craft[HAB].x);
    textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %f", craft[HAB].y );
    textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: camera.x = %f", camera.x );
    textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: camera.y = %f", camera.y );
    textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: : %d", milliseconds);
    textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", craft[HAB].Vx);
    textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", craft[HAB].Vy);
    textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: G: %e", G);
    textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), -1, "DEBUG: arc tan: %f", atan2 (craft[HAB].x - planet[EARTH].x, craft[HAB].y - planet[EARTH].y) + PI * 0.5 );
    textprintf_ex (buffer, font, 0, 90, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %f", pow (zoomMagnitude, camera.zoom) );
    textprintf_ex (buffer, font, 0, 100, makecol (255, 255, 255), -1, "DEBUG: Camera zoom: %f", camera.zoom);
    textprintf_ex (buffer, font, 0, 110, makecol (255, 255, 255), -1, "DEBUG: Earth's gravity: %e", planet[EARTH].gravity);
}
END_OF_FUNCTION (entity::debug);

float entity::degrees() {

    return (radians * 180 / PI);
}
void entity::accX (float radians, float acc) {

    Vx += cos (radians) * acc / mass;
}
void entity::accY (float radians, float acc) {

    Vy += sin (radians) * acc / mass;
}

void entity::draw() {

    circlefill (buffer, x - camera.x, y - camera.y, radius * camera.zoom, fillColour ); // Draw the entity to the buffer
}

void body::draw() {

    circlefill (buffer, a(), b(), radius * camera.actualZoom() + atmosphereHeight, atmosphereColour);

    circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColour ); // Draw the entity to the buffer
}

void ship::draw() {

    float A = a();  //so that the program doesn't have to calculate a and b every time
    float B = b();

    circlefill (buffer, A, B, radius * camera.actualZoom(), fillColour); // Draw the picture to the buffer
    line (buffer, A, B, //draws the 'engine'
          A + radius * cos (turnRadians) * camera.actualZoom(),
          B + radius * sin (turnRadians) * camera.actualZoom(),
          engineColour);
}

void entity::turn () {

    turnRadians += turnRate * PI / 180;

    if (turnRadians < 0)
        turnRadians += 2 * PI;

    if (turnRadians > 2 * PI)
        turnRadians -= 2 * PI;
}

void entity::clearValues() {

    acc = 0;
}

void detectCollision () {

    if (craft[HAB].distance (planet[EARTH].x, planet[EARTH].y) + craft[HAB].Vx + craft[HAB].Vy < craft[HAB].radius + planet[EARTH].radius) {  //if the entity will touch the earth next move
//        craft[HAB].Vx = -craft[HAB].Vx;  //this will make the entity gradually slow down, instead of suddenly stopping
//        craft[HAB].Vy = sin ;
        craft[HAB].Vx = 0;
        craft[HAB].Vy = 0;
//        craft[HAB].engine = 0;
    }
}

float viewpoint::actualZoom() {

    return (pow (zoomMagnitude, zoom) );
}

float entity::a() {

    return ( (x - camera.x) * camera.actualZoom() );
}

float entity::b() {

    return ( (y - camera.y) * camera.actualZoom() );
}

void entity::gravitate() {

    float theta = atan2f (x - planet[EARTH].x, y - planet[EARTH].y);
    float gravity = G * ( (mass * planet[EARTH].mass) / (distance (planet[EARTH].x, planet[EARTH].y) * planet[EARTH].x, planet[EARTH].y) );
    theta += PI * 0.5;

    accX (theta, gravity);
    accY (-theta, gravity);
}

float entity::distance (float targetX, float targetY) {

    return (sqrtf ( ( (targetX - x) * (targetX - x) ) + ( (targetY - y) * (targetY - y) ) ) ); //finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

void drawGrid () {

    unsigned int n;

    for (n = 0; n < screenWidth; n++)
        line (buffer, n * 20 * fabs (camera.zoom), 0, n * 20 * fabs (camera.zoom), screenHeight, makecol (100, 100, 100) );

    for (n = 0; n < screenHeight * camera.actualZoom(); n++)
        line (buffer, 0, n * 20 * fabs (camera.zoom), screenWidth, n * 20 * fabs (camera.zoom), makecol (100, 100, 100) );


}
