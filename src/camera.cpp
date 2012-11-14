#include <corbit/camera.hpp>

void				camera_c::setcenter		(class object_c& center_)	{center = &center_;}
void 				camera_c::recenter		(unsigned short dispw, unsigned short disph) {
	if (!tracking || !center) return;
	pos.x = center->x() -dispw/2;
	pos.y = center->y() -disph/2;

	v.x = center->Vx();
	v.y = center->Vy();
}
void				camera_c::move			() {
	pos.x += v.x;
	pos.y += v.y;
}
void 				camera_c::track			(bool totrack)				{tracking = totrack;}
void				camera_c::toggletrack	()							{tracking = !tracking;}
void 				camera_c::panx			(float amount)				{v.x += amount;}
void				camera_c::pany			(float amount)				{v.y += amount;}

data				camera_c::x				() const					{return pos.x;}
data				camera_c::y				() const					{return pos.y;}
float				camera_c::zoom			() const					{return inverse/zoomlevel;}
void				camera_c::changezoom	(float amount)				{zoomlevel += amount;}


camera_c::camera_c							(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoomlevel_)
	: pos (X, Y), v (Vx, Vy), acc (accX, accY),
	inverse (inverse_), zoomlevel (zoomlevel_) {}

camera_c& camera_c::getinstance				(data X, data Y, data Vx, data Vy, data accX, data accY,
											 class object_c* center_, unsigned inverse_, float zoomlevel_) {

	static camera_c instance (X, Y, Vx, Vy, accX, accY, center_, inverse_, zoomlevel_);
	return instance;
}

