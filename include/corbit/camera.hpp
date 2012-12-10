#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <corbit/globals.hpp>

#include <corbit/object.hpp>

class camera_c {
//private:
public:
	vector			_pos,
					_v,
					_acc;
	vector			_screen_size;

	object_c*		_center;

	bool			_tracking;
	const unsigned	_inverse;
	float			_zoom_level;

	void 			recenter		(),
					move			();


public:
	void			update			();
	void			set_center		(class object_c& center_);
	void 			track			(bool to_track),
					toggle_track	();
	void 			pan				(float X, float Y);
	void			set_dimensions	(int disp_w, int disp_h);

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

					camera_c		(var X, var Y, var Vx, var Vy, var accX, var accY,
									object_c* center_, unsigned inverse_, float zoomlevel_);

					~camera_c		();
};

#endif	//CAMERA_HPP
