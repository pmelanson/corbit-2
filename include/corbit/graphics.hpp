#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <boost/intrusive/list.hpp>
#include <allegro5/allegro_primitives.h>

#include <corbit/globals.hpp>

class camera_c;
class object_c;


class graphics_c {
private:

	void				draw			(object_c &obj) const;

	camera_c			*_camera;

						graphics_c		();
						graphics_c		(camera_c *camera_);

public:

	bool				set_camera		(camera_c *camera);
	void				set_dimensions	(int disp_w, int disp_h);
	void				set_center		(object_c *center_);

	void				draw_all		(boost::intrusive::list<object_c> &list) const;

	static graphics_c	&get_instance	();
	static graphics_c	&get_instance	(camera_c *camera_);
						~graphics_c		();
};

#endif	//GRAPHICS_HPP

