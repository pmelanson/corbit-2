#include <corbit/physical.hpp>

#include <cmath>

void	physical_c::move() {
	_v	+=	_acc/FPS;
	_acc.setZero();
	_pos+=	_v	/FPS;
}
