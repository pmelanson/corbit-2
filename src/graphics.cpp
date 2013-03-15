#include <corbit/graphics.hpp>
#include <corbit/nav.hpp>
#include <corbit/calc.hpp>
#include <corbit/entity.hpp>
#include <corbit/hab.hpp>
#include <corbit/camera.hpp>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <sstream>
#include <iomanip>

extern ALLEGRO_DISPLAY *display;
camera_c default_camera(0,0, 0,0, 0,0, NULL, 4);

//#include <iostream>

namespace	graphics {
	camera_c	*camera = &default_camera;
	hud_c		hud (al_color_name("grey"));
}

void	graphics::draw_at	(const entity_c &obj, var zoom, var x, var y) {

	if (display) {
		al_draw_filled_circle(x, y, obj.radius * zoom, obj.color);
	}
}
void	graphics::draw		(const entity_c &obj) {
	draw_at(obj,
		camera->zoom(),
		(obj.pos[0] - camera->pos[0]) * camera->zoom() + camera->size[0]/2,
		(obj.pos[1] - camera->pos[1]) * camera->zoom() + camera->size[1]/2);
}

void	graphics::draw_at	(const hab_c &hab, var zoom, var x, var y) {

	if (display) {
		al_draw_filled_circle(x, y, hab.radius * zoom, hab.color);
	}
}
void	graphics::draw		(const hab_c &hab) {
	draw_at(hab, graphics::camera->zoom(), hab.pos[0], hab.pos[1]);
}

//
//void	graphics::hud_c::update() {
//	column_w	= graphics::camera->size[0] / columns;
//	text_start_x= padding;
//	text_start_y= 4 * graphics::camera->size[1] / 5;
//}

void	graphics::hud_c::new_column() {
	line = 0;
	++column;
}

void	graphics::hud_c::add_line (std::stringstream &text) {
	al_draw_text(font, text_col,
		text_start_x + column*column_w, text_start_y + padding*line++,
		ALLEGRO_ALIGN_LEFT, text.str().c_str());

	text.str("");
}

void	graphics::hud_c::draw () {

	column_w	= graphics::camera->size[0] / columns;
	text_start_x= padding;
	text_start_y= 4 * graphics::camera->size[1] / 5;

	al_draw_filled_rectangle
		(0, 4 * graphics::camera->size[1]/5 - padding,
			graphics::camera->size[0], graphics::camera->size[1],
			al_color_name("black"));
	al_draw_line
		(0, 4 * graphics::camera->size[1]/5 - padding,
			graphics::camera->size[0], 4 * graphics::camera->size[1]/5 - padding,
			al_color_name("white"), 1);

	std::stringstream text("");
	text << std::setprecision(2) << std::uppercase << std::fixed;
	line = 0;
	column = 0;

//	if(calc::distance(*nav::ship, *nav::ref) - (nav::ship->radius + nav::ref->radius) < 3)
//		system("import -window root /home/wopr/s.jpg");


	text << "Speed (" << nav::ref->name << "): "
		<< calc::velocity(*nav::ship, *nav::ref).norm();
	add_line(text);
	text << "Speed (" << nav::targ->name << "): "
		<< calc::velocity(*nav::ship, *nav::targ).norm();
	add_line(text);
	text << "Orbital Speed (" << nav::ref->name << "): "
		<< calc::v_orbit(*nav::ship, *nav::ref);
	add_line(text);
	text << "Centrifugal Speed (" << nav::ref->name << "): "
		<< calc::v_cen(*nav::ship, *nav::ref);
	add_line(text);
	text << "Tangential Speed (" << nav::ref->name << "): "
		<< calc::v_tan(*nav::ship, *nav::ref);
	add_line(text);
	text << "Acceleration (" << nav::ship->name << "): "
		<< nav::ship->acc.norm();
	add_line(text);
	text << "Stopping Acceleration (" << nav::ref->name << "): "
		<< calc::stopping_acc(*nav::ship, *nav::ref);
	add_line(text);
	text << "Drag: "
		<< "500000s";
	add_line(text);
	text << "Periapsis (" << nav::ref->name << "): "
		<< calc::periapsis(*nav::ship, *nav::ref);
	add_line(text);
	text << "Apoapsis (" << nav::ref->name << "): "
		<< calc::apoapsis(*nav::ship, *nav::ref);
	add_line(text);


	new_column();


	text << "Engine (" << nav::ship->name << "): "
		<< "100%";
	add_line(text);


	graphics::draw_at(*nav::ship, 50/nav::ship->radius,
					  graphics::camera->size[0]/2, 900);
//	add_line(text);
//	add_line(text);
//	add_line(text);
//	add_line(text);
//	add_line(text);
//	add_line(text);
//	add_line(text);
//	add_line(text);


	text << "Fuel (" << nav::ship->name << "): "
		<< "a billion";
	add_line(text);


	new_column();


	text << "Altitude (" << nav::ref->name << "): "
		<< calc::distance(*nav::ship, *nav::ref) - (nav::ship->radius + nav::ref->radius);
	add_line(text);
	text << "Time Dilation: "
		<< "1" << "x";
	add_line(text);
	text << "\u03B8 (\u2220" << nav::ship->name<<'/'<<nav::ref->name<<'/'<<nav::targ->name<< "): "
		<< calc::theta(*nav::ship, *nav::ref, *nav::targ);
	add_line(text);
	text << "Pitch (" << nav::ref->name << "): "
		<< calc::pitch(*nav::ship, *nav::ref);
	add_line(text);
	text << "Center: "
		<< "poop";
//		<< graphics::camera->center->name;
	add_line(text);
	text << "Ship: "
		<< nav::ship->name;
	add_line(text);
	text << "Target: "
		<< nav::targ->name;
	add_line(text);
	text << "Reference: "
		<< nav::ref->name;
	add_line(text);
	text << "Navigation: "
		<< "Manual";
	add_line(text);
}
