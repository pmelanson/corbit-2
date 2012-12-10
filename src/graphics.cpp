#include <corbit/graphics.hpp>
#include <corbit/object.hpp>
#include <corbit/camera.hpp>

extern ALLEGRO_DISPLAY *display;

//void		graphics_c::set_dimensions		(int disp_w, int disp_h) {camera->set_dimensions(disp_w, disp_h);}

void		graphics_c::draw				(object_c &obj) const {

//	if (display && obj)
//		al_draw_filled_circle (obj->x() - camera->x(), obj->y() - camera->y(),
//							   obj->radius() * camera->zoom(),
//							   obj->color);
}

void		graphics_c::draw_all			(boost::intrusive::list<object_c>& list) const {

	static boost::intrusive::list<object_c>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		draw(*it);
}


graphics_c::graphics_c						() {}
graphics_c::graphics_c						(camera_c *camera_)
	: _camera (camera_) {}

graphics_c	&graphics_c::get_instance		() {

	static graphics_c instance;
	return instance;
}

graphics_c	&graphics_c::get_instance		(camera_c *camera_) {

	static graphics_c instance(camera_);
	return instance;
}

graphics_c::~graphics_c						() {}
