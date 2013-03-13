#include <corbit/graphics.hpp>
#include <corbit/entity.hpp>
#include <corbit/hab.hpp>
#include <corbit/camera.hpp>

#include <allegro5/allegro_primitives.h>

extern ALLEGRO_DISPLAY *display;
camera_c default_camera(0,0, 0,0, 0,0, NULL, 1);

#include <iostream>

namespace	graphics {
	camera_c *camera = &default_camera;
}

void		graphics::draw	(const entity_c &obj) {

	if (display) {
		al_draw_filled_circle(
			(obj.pos[0] - camera->pos[0]) * camera->zoom() + camera->size[0]/2,
			(obj.pos[1] - camera->pos[1]) * camera->zoom() + camera->size[1]/2,
			obj.radius * camera->zoom(),
			obj.color
		);
	}

//	std::cout << obj.pos[0] - camera->pos[0] << ',' << obj.pos[1] - camera->pos[1] << std::endl;
}

void		graphics::draw	(const hab_c &hab) {

	if (display) {
		al_draw_filled_circle(
			(hab.pos[0] - camera->pos[0]) * camera->zoom() + camera->size[0]/2,
			(hab.pos[1] - camera->pos[1]) * camera->zoom() + camera->size[1]/2,
			hab.radius * camera->zoom() * 500,
			hab.color
		);
	}

//	std::cout << hab.pos[0] - camera->pos[0] << ',' << hab.pos[1] - camera->pos[1] << std::endl;
}
