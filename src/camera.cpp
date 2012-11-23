#include <corbit/camera.hpp>

data	camera_c::x							() const {return _pos(0,0);}
data	camera_c::y							() const {return _pos(1,0);}
data	camera_c::Vx						() const {return _v(0,0);}
data	camera_c::Vy						() const {return _v(1,0);}
data	camera_c::accX						() const {return _acc(0,0);}
data	camera_c::accY						() const {return _acc(1,0);}
vector	camera_c::pos						() const {return _pos;}
vector	camera_c::v							() const {return _v;}
vector	camera_c::acc						() const {return _acc;}

void				camera_c::update		(unsigned short dispw, unsigned short disph) {move(); recenter(dispw, disph);}
void				camera_c::set_center	(class object_c& center_)	{_center = &center_;}
void 				camera_c::recenter		(unsigned short dispw, unsigned short disph) {
	if (!_tracking || !_center) return;
	_pos(0,0) = _center->x() -dispw/2;
	_pos(1,0) = _center->y() -disph/2;
}
void				camera_c::move			() {
	_pos += v() /zoom();
}
void 				camera_c::track			(bool totrack)				{_tracking = totrack;}
void				camera_c::toggle_track	()							{_tracking = !_tracking;}
void 				camera_c::pan			(float X, float Y)			{_v(0,0) += X; _v(1,0) += Y;}

float				camera_c::zoom			() const					{return _inverse/_zoom_level;}
void				camera_c::change_zoom	(float amount)				{_zoom_level += amount;}


camera_c::camera_c							(data x_, data y_, data Vx_, data Vy_, data accX_, data accY_,
											 class object_c* center_, unsigned inverse_, float zoom_level_)
	: _pos (x_, y_), _v (Vx_, Vy_), _acc (accX_, accY_),
	_inverse (inverse_), _zoom_level (zoom_level_) {}

camera_c::~camera_c							() {}

/*
camera_c& camera_c::get_instance			(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoom_level_) {

	static camera_c instance (X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_);
	return instance;
}*/

