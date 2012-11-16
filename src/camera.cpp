#include <corbit/camera.hpp>

void				camera_c::set_center	(class object_c& center_)	{center = &center_;}
void 				camera_c::recenter		(unsigned short dispw, unsigned short disph) {
	if (!tracking || !center) return;
	pos.x = center->x() -dispw/2;
	pos.y = center->y() -disph/2;

//	v.x = center->Vx();
//	v.y = center->Vy();
}
void				camera_c::move			() {
	pos.x += v.x /zoom();
	pos.y += v.y /zoom();
}
void 				camera_c::track			(bool totrack)				{tracking = totrack;}
void				camera_c::toggle_track	()							{tracking = !tracking;}
void 				camera_c::pan_x			(float amount)				{v.x += amount;}
void				camera_c::pan_y			(float amount)				{v.y += amount;}

data				camera_c::x				() const					{return pos.x;}
data				camera_c::y				() const					{return pos.y;}
float				camera_c::zoom			() const					{return inverse/zoom_level;}
void				camera_c::change_zoom	(float amount)				{zoom_level += amount;}


					camera_c::camera_c		(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoom_level_)
	: pos (X, Y), v (Vx, Vy), acc (accX, accY),
	inverse (inverse_), zoom_level (zoom_level_) {}

					camera_c::~camera_c		() {}

/*
camera_c& camera_c::get_instance			(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoom_level_) {

	static camera_c instance (X, Y, Vx, Vy, accX, accY, center_, inverse_, zoom_level_);
	return instance;
}*/

