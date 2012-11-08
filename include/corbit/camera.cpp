#include "camera.h"




camera_c::pos_t::pos_t (long double _x, long double _y)
	: x (_x), y (_y) {
}
camera_c::v_t::v_t (long double _Vx, long double _Vy)
	: pos_t (_Vx, _Vy) {
}

camera_c::camera_c
(long double _x, long double _y, long double _Vx, long double _Vy, unsigned _inverse, float _zoomlevel)
	: pos (_x, _y), v (_Vx, _Vy), inverse (_inverse), zoomlevel (_zoomlevel) {
}
camera_c& camera_c::getinstance
(long double _x, long double _y, long double _Vx, long double _Vy, unsigned _inverse, float _zoomlevel) {

	static camera_c instance (_x, _y, _Vx, _Vy, _inverse, _zoomlevel);
	return instance;
}

