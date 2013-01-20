#include <corbit/camera.hpp>

#include <corbit/object.hpp>

#include <cmath>

var		camera_c::x			() const {return _pos(0,0) + size[0]/2;}
var		camera_c::y			() const {return _pos(1,0) + size[1]/2;}
var		camera_c::Vx		() const {return _v(0,0);}
var		camera_c::Vy		() const {return _v(1,0);}
var		camera_c::accX		() const {return _acc(0,0);}
var		camera_c::accY		() const {return _acc(1,0);}
vector	camera_c::pos		() const {return _pos;}
vector	camera_c::v			() const {return _v;}
vector	camera_c::acc		() const {return _acc;}

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
	: _pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	center (center_), tracking (true), zoom_level (zoom_level_),
	inverse (1), exp (2) {}

camera_c::~camera_c			() {}
