#include <iostream>
#include <corbit/graphics.hpp>

#include <allegro5/allegro_primitives.h>

extern ALLEGRO_DISPLAY* display;

void		graphics_c::draw				(const object_c& obj) const {

	if (display)
		if (&obj)
		al_draw_circle (obj.x() -camera.x(), obj.y() -camera.y(), obj.radius() *camera.zoom(), obj.color, 0);

	std::cout << "poo\n";
}

void		graphics_c::draw_all			(const boost::intrusive::list<object_c> &list) const {

	static boost::intrusive::list<object_c>::iterator it;
	while (it != list.end())
		draw(*it);

	std::cout << "\nyea\n";
}


			graphics_c::graphics_c			(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_)
	: camera (X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_) {}

			graphics_c::~graphics_c			() {}

graphics_c&	graphics_c::get_instance		(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoom_level_) {

	static graphics_c instance(X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_);
	return instance;
}
