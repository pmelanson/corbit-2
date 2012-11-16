#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <corbit/object.hpp>
#include <corbit/camera.hpp>

#include <boost/intrusive/list.hpp>

class graphics_c {
public:

	void				draw			(const object_c& obj) const;

						graphics_c		(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_);

public:

	void				draw_all		(const boost::intrusive::list<object_c> &list) const;

	camera_c			camera;

	static graphics_c&	get_instance	(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_);
						~graphics_c		();
};

#endif	//DRAWER_HPP

