#include <corbit/drawer.hpp>

drawer_c::drawer_c () {}

drawer_c& drawer_c::getinstance	() {

	static drawer_c instance;
	return instance;
}
