#include "camera.hpp"

void				camera_c::setcenter		(class object_c* center_)	{center = center_;}
void 				camera_c::recenter		() {
	pos.x = center->x();
	pos.y = center->y();

	v.x = center->x();
	v.y = center->y();
}
void 				camera_c::track			(bool totrack)				{tracking = totrack;}
void 				camera_c::panX			(float amount)				{v.x += amount;}
void				camera_c::panY			(float amount)				{v.y += amount;}

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

