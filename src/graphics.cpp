#include <iostream>

#include <allegro5/allegro_primitives.h>

#include <corbit/graphics.hpp>
using namespace std;

extern ALLEGRO_DISPLAY* display;

void		graphics_c::draw				(const object_c& obj) const {

	if (display && &obj)
		al_draw_filled_circle (obj.x() - camera.x(), obj.y() - camera.y(),
						obj.radius() * camera.zoom(),
						obj.color);
}

void		graphics_c::draw_all			(boost::intrusive::list<object_c> &list) const {

	static boost::intrusive::list<object_c>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		draw(*it);
}


graphics_c::graphics_c						(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_)
	: camera (X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_) {}

graphics_c::~graphics_c						() {}

graphics_c&	graphics_c::get_instance		(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoom_level_) {

	static graphics_c instance(X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_);
	return instance;
}
