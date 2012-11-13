#include "globals.hpp"

object_c::pos_s::pos_s		(data X, data Y)		: x (X), y (Y) {}
object_c::v_s::v_s			(data Vx, data Vy)		: pos_s (Vx, Vy) {}
object_c::acc_s::acc_s		(data accX, data accY)	: pos_s (accX, accY) {}
