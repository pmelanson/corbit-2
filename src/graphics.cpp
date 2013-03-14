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

extern ALLEGRO_DISPLAY *display;
camera_c default_camera(0,0, 0,0, 0,0, NULL, 1);

//#include <iostream>

namespace	graphics {
	camera_c	*camera = &default_camera;
	hud_c		hud (al_color_name("grey"));
}

void	graphics::draw	(const entity_c &obj) {

	if (display) {
		al_draw_filled_circle(
			(obj.pos[0] - camera->pos[0]) * camera->zoom() + camera->size[0]/2,
			(obj.pos[1] - camera->pos[1]) * camera->zoom() + camera->size[1]/2,
			obj.radius * camera->zoom(),
			obj.color
		);
	}
}

void	graphics::draw	(const hab_c &hab) {

	if (display) {
		al_draw_filled_circle(
			(hab.pos[0] - camera->pos[0]) * camera->zoom() + camera->size[0]/2,
			(hab.pos[1] - camera->pos[1]) * camera->zoom() + camera->size[1]/2,
			hab.radius * camera->zoom() * 500,
			hab.color
		);
	}
}

void	graphics::hud_c::add_line (std::stringstream &text) {
	al_draw_text(font, text_col, 10, 20 * lines++, ALLEGRO_ALIGN_LEFT, text.str().c_str());
	text.str("");
}

void	graphics::hud_c::draw () {

	al_draw_filled_rectangle(0, 0, 500, 800, al_color_name("black"));
	al_draw_rectangle		(0, 0, 500, 800, al_color_name("white"), 1);

	std::stringstream text("");

	//ref Vo
	text << "Orbital Speed (" << nav::ref->name << "): "
		<< calc::v_orbit(*nav::ship, *nav::ref);
	add_line(text);
	//V hab-ref
	text << "Speed (" << nav::ref->name << "): "
		<< calc::velocity(*nav::ship, *nav::ref).norm();
	add_line(text);
	//Vtarg-ref
	text << "Speed (" << nav::targ->name << "): "
		<< calc::velocity(*nav::ship, *nav::targ).norm();
	add_line(text);
	//Engine
	text << "Engine(" << nav::ship->name << "): "
		<< "100%";
	add_line(text);
	///
	text << "---------------";
	add_line(text);
	//vectors
//	graphics::draw(
	//drag
	text << "Drag: "
		<< "500000s";
	add_line(text);
	//acceleration
	text << "Acceleration (" << nav::ship->name << "): "
		<< nav::ship->acc.norm();
	add_line(text);
	//timestep
	text << "Time Dilation: "
		<< "1" << "x";
	add_line(text);
	///
	text << "---------------";
	add_line(text);
	//fuel
	text << "Fuel (" << nav::ship->name << "): "
		<< "a billion";
	add_line(text);
	///
	text << "---------------";
	add_line(text);
	//alt
	text << "Altitude (" << nav::ref->name << "): "
		<< calc::distance(*nav::ship, *nav::ref) - (nav::ship->radius + nav::ref->radius);
	add_line(text);
	//Acc
	text << "Stopping Acceleration (" << nav::ref->name << "): "
		<< calc::stopping_acc(*nav::ship, *nav::ref);
	add_line(text);
	//theta Hrt
	text << "\u03B8 (\u2220" << nav::ship->name<<'/'<<nav::ref->name<<'/'<<nav::targ->name<< "): "
		<< calc::theta(*nav::ship, *nav::ref, *nav::targ);
	add_line(text);
	//v_cen
	text << "Centrifugal Speed (" << nav::ref->name << "): "
		<< calc::v_cen(*nav::ship, *nav::ref);
	add_line(text);
	//v_tan
	text << "Tangential Speed (" << nav::ref->name << "): "
		<< calc::v_tan(*nav::ship, *nav::ref);
	add_line(text);
	//theta pch
	text << "Pitch (" << nav::ref->name << "): "
		<< calc::pitch(*nav::ship, *nav::ref);
	add_line(text);
	//Peri
	text << "Periapsis (" << nav::ref->name << "): "
		<< calc::periapsis(*nav::ship, *nav::ref);
	add_line(text);
	//Apo
	text << "Apoapsis (" << nav::ref->name << "): "
		<< calc::apoapsis(*nav::ship, *nav::ref);
	add_line(text);
	///
	text << "---------------";
	add_line(text);
	//center
//	text << "Center: "
//		<< graphics::camera->center->name;
//	add_line(text);
	//ship
	text << "Ship: "
		<< nav::ship->name;
	add_line(text);
	//target
	text << "Target: "
		<< nav::targ->name;
	add_line(text);
	//ref
	text << "Reference: "
		<< nav::ref->name;
	add_line(text);
	//NAVmode
	text << "Navigation: "
		<< "Manual";
	add_line(text);

	lines = 0;
//	al_draw_text(font, text_col, 10,10, ALLEGRO_ALIGN_LEFT, text.str().c_str());
//al_draw_textf(font, al_map_rgb(200,200,200), 200,0, ALLEGRO_ALIGN_LEFT, "zoom: %Lf zoom_level: %Lf", graphics::camera->zoom(), graphics::camera->zoom_level);
}
