#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <corbit/globals.hpp>

class object_c;

class camera_c {
private:
	vector			_pos,
	_v,
	_acc;
	int				_screen_size[2];

	object_c		*_center;

	bool			_tracking;
	const unsigned	_inverse;
	float			_zoom_level;

	void 			recenter		(),
					move			();

					camera_c		(var X, var Y, var Vx, var Vy, var accX, var accY,
									object_c *center_, unsigned inverse_, float zoomlevel_);

public:
	void			update			();
	void			set_center		(object_c *center_);
	void 			track			(bool to_track),
					toggle_track	();
	void 			pan				(float X, float Y);
	void			set_dimensions	(int disp_w, int disp_h);

	int				screen_w		() const,
					screen_h		() const;
	float			zoom			() const;
	var				x				() const,
					y				() const,
					Vx				() const,
					Vy				() const,
					accX			() const,
					accY			() const;
	vector			pos				() const,
					v				() const,
					acc				() const;
	void			change_zoom		(float amount);

	static camera_c	&get_instance	(var X, var Y, var Vx, var Vy, var accX, var accY,
									object_c *center_, unsigned inverse_, float zoom_level_);

	~camera_c		();
};

#endif	//CAMERA_HPP
