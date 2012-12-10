#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <boost/intrusive/list.hpp>

#include <corbit/camera.hpp>


class graphics_c {
private:

	void				draw			(const object_c& obj) const;

						graphics_c		(var X, var Y, var Vx, var Vy, var accX, var accY,
										class object_c* center_, unsigned inverse_, float zoom_level_);

public:

	camera_c			camera;

	void				draw_all		(boost::intrusive::list<object_c> &list) const;

	static graphics_c&	get_instance	(var X, var Y, var Vx, var Vy, var accX, var accY,
										class object_c* center_, unsigned inverse_, float zoom_level_);
						~graphics_c		();
};

#endif	//DRAWER_HPP

