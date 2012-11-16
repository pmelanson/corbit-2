#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <corbit/globals.hpp>

#include <corbit/object.hpp>

class camera_c {
private:

	#include <corbit/dynamics.hpp>

	class object_c*		center;

	bool				tracking;
	const unsigned		inverse;
	float				zoom_level;


public:
	void				set_center		(class object_c& center_);
	void 				recenter		(unsigned short dispw, unsigned short disph),
						move			();
	void 				track			(bool totrack),
						toggle_track	();
	void 				pan_x			(float amount),
						pan_y			(float amount);

	float				zoom			() const;
	data				x				() const,
						y				() const;
	void				change_zoom		(float amount);

						camera_c		(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoomlevel_);

						~camera_c		();

//	static camera_c&	get_instance	(data X, data Y, data Vx, data Vy, data accX, data accY,
//										 class object_c* center_, unsigned inverse_, float zoomlevel_);
};

#endif	//CAMERA_HPP
