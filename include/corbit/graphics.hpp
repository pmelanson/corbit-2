#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <corbit/globals.hpp>

class object_c;
class camera_c;

namespace graphics {

	void		draw		(object_c &obj);

	extern		camera_c	*camera;
};

#endif	//GRAPHICS_HPP
