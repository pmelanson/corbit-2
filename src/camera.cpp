#include <corbit/camera.hpp>

#include <corbit/object.hpp>

#include <cmath>

void	camera_c::update	() {
	move();
	recenter();
}

void	camera_c::recenter	() {
	if (!tracking || !center) return;
	_pos(0,0) = center->x() - size[0]/2;
	_pos(1,0) = center->x() - size[1]/2;
	_v = center->v();
}
void	camera_c::move		() {
	_pos += v()/FPS;
}
void	camera_c::pan		(float X, float Y)	{
	_acc(0,0) += X;
	_acc(1,0) += Y;
}
float	camera_c::zoom		() const			{return inverse/std::pow(zoom_level, exp);}


camera_c::camera_c			(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
							object_c *center_, float zoom_level_)
	: physical_c(x_,y_, Vx_,Vy_, accX_,accY_),
	center (center_), tracking (true), zoom_level (zoom_level_),
	inverse (1), exp (2) {}

camera_c::~camera_c			() {}
