#include <corbit/graphics.hpp>
#include <corbit/object.hpp>
#include <corbit/camera.hpp>

#include <allegro5/allegro_primitives.h>

extern ALLEGRO_DISPLAY *display;
camera_c default_camera(0,0, 0,0, 0,0, NULL, 1);

#include <iostream>

namespace	graphics {
	camera_c *camera = &default_camera;
}

void		graphics::draw	(object_c &obj) {

	if (display) {
		al_draw_filled_circle(
			(obj.x() - camera->x()) * camera->zoom() + camera->size[0]/2,
			(obj.y() - camera->y()) * camera->zoom() + camera->size[1]/2,
			obj.radius * camera->zoom(),
			obj.color
		);
	}

//	std::cout << obj.x() - camera->x() << ',' << obj.y() - camera->y() << std::endl;
}
