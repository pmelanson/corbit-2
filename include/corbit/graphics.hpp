#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include <corbit/globals.hpp>

class entity_c;
class hab_c;
class camera_c;

namespace graphics {

	void		draw		(const entity_c &obj);
	void		draw		(const hab_c &hab);

	extern		camera_c	*camera;
}


#endif	//GRAPHICS_HPP
