#include <corbit/camera.hpp>

#include <corbit/object.hpp>

#include <cmath>

void	camera_c::update	() {
	move();
	recenter();
}

void	camera_c::recenter	() {
	if (!tracking || !center) return;
	_pos = center->pos();
	_v = center->v();
	_acc = center->acc();
}

void	camera_c::pan		(float X, float Y)	{
	_acc(0,0) += X;
	_acc(1,0) += Y;
}
float	camera_c::zoom		() const			{
	return 1 / std::pow(std::abs(zoom_level), exp);
}


camera_c::camera_c			(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
							object_c *center_, float zoom_level_)
	: physical_c(x_,y_, Vx_,Vy_, accX_,accY_),
	center (center_), tracking (true), zoom_level (zoom_level_), exp (2) {}

camera_c::~camera_c			() {}
