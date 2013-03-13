#include <corbit/physical.hpp>

void	physical_c::move() {
	v	+=	acc/FPS;
	acc.setZero();
	pos	+=	v/FPS;
}
