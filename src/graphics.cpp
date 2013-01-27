#include <corbit/graphics.hpp>
#include <corbit/object.hpp>
#include <corbit/hab.hpp>
#include <corbit/camera.hpp>

#include <allegro5/allegro_primitives.h>

extern ALLEGRO_DISPLAY *display;
camera_c default_camera(0,0, 0,0, 0,0, NULL, 1);

#include <iostream>

namespace	graphics {
	camera_c *camera = &default_camera;
}

void		graphics::draw	(const object_c &obj) {

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

void		graphics::draw	(const hab_c &hab) {

	if (display) {
		al_draw_filled_circle(
			(hab.x() - camera->x()) * camera->zoom() + camera->size[0]/2,
			(hab.y() - camera->y()) * camera->zoom() + camera->size[1]/2,
			hab.radius * camera->zoom() * 500,
			hab.color
		);
	}

//	std::cout << hab.x() - camera->x() << ',' << hab.y() - camera->y() << std::endl;
}
