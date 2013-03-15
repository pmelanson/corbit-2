#include <corbit/camera.hpp>

#include <corbit/entity.hpp>

#include <cmath>

void	camera_c::update	() {
	move();
	recenter();
}

void	camera_c::recenter	() {
	if (!tracking || !center) return;
	pos = center->pos;
	v = center->v;
	acc = center->acc;
}

void	camera_c::pan		(float X, float Y)	{
	acc[0] += X;
	acc[1] += Y;
}
float	camera_c::zoom		() const			{
	return 20 / std::pow(std::abs(zoom_level), 4);
}


camera_c::camera_c			(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
							entity_c *center_, float zoom_level_)
	: physical_c(x_,y_, Vx_,Vy_, accX_,accY_),
	center (center_), tracking (true), zoom_level (zoom_level_) {}

camera_c::~camera_c			() {}
