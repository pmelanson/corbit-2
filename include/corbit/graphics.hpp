#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <boost/intrusive/list.hpp>

#include <corbit/camera.hpp>


class graphics_c {
private:

	void				draw			(const object_c& obj) const;

						graphics_c		(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_);

public:

	camera_c			camera;

	void				draw_all		(boost::intrusive::list<object_c> &list) const;

	static graphics_c&	get_instance	(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoom_level_);
						~graphics_c		();
};

#endif	//DRAWER_HPP

