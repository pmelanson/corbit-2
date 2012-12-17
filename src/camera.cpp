#include <corbit/camera.hpp>

#include <corbit/object.hpp>

#include <cmath>

var		camera_c::x				() const {return _pos(0,0) + _screen_size[0]/2;}
var		camera_c::y				() const {return _pos(1,0) + _screen_size[1]/2;}
var		camera_c::Vx			() const {return _v(0,0);}
var		camera_c::Vy			() const {return _v(1,0);}
var		camera_c::accX			() const {return _acc(0,0);}
var		camera_c::accY			() const {return _acc(1,0);}
vector	camera_c::pos			() const {return _pos;}
vector	camera_c::v				() const {return _v;}
vector	camera_c::acc			() const {return _acc;}

void	camera_c::set_dimensions(int disp_w, int disp_h) {
	_screen_size[0] = disp_w;
	_screen_size[1] = disp_h;
}

int		camera_c::screen_w		() const {return _screen_size[0];}
int		camera_c::screen_h		() const {return -_screen_size[1];}

void	camera_c::update		() {
	_move();
	_recenter();
}
void	camera_c::set_center	(object_c *center_)	{
	_center = center_;
}

void 	camera_c::_recenter		() {
	if (!_tracking || !_center) return;
	_pos(0,0) = _center->x() - _screen_size[0]/2;
	_pos(1,0) = _center->y() - _screen_size[1]/2;
	_v = _center->v();
}
void	camera_c::_move			() {
	_pos += v()/FPS;
}
void	camera_c::track			(bool to_track)		{_tracking = to_track;}
void	camera_c::toggle_track	()					{_tracking = !_tracking;}
void	camera_c::pan			(float X, float Y)	{
	_acc(0,0) += X;
	_acc(1,0) += Y;
}
float	camera_c::zoom			() const			{return _inverse/std::pow(_zoom_level, _exp);}
void	camera_c::change_zoom	(float amount)		{_zoom_level += amount;}


camera_c::camera_c					(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									object_c *center_, float zoom_level_)
	: _pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	_tracking (false), _zoom_level (zoom_level_),
	_inverse (1), _exp (2) {}

camera_c	&camera_c::get_instance	(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									object_c *center_, float zoom_level_) {

	static camera_c instance (x_, y_, Vx_, Vy_, accX_, accY_, center_, zoom_level_);
	return instance;
}

camera_c::~camera_c					() {}
