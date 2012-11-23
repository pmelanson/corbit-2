#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <corbit/globals.hpp>

#include <corbit/object.hpp>

class camera_c {
//private:
public:
	vector				_pos,
						_v,
						_acc;

	class object_c*		_center;

	bool				_tracking;
	const unsigned		_inverse;
	float				_zoom_level;

	void 				recenter		(unsigned short dispw, unsigned short disph),
						move			();


public:
	void				update			(unsigned short dispw, unsigned short disph);
	void				set_center		(class object_c& center_);
	void 				track			(bool totrack),
						toggle_track	();
	void 				pan				(float X, float Y);

	float				zoom			() const;
	data				x				() const,
						y				() const,
						Vx				() const,
						Vy				() const,
						accX			() const,
						accY			() const;
	vector				pos				() const,
						v				() const,
						acc				() const;
	void				change_zoom		(float amount);

						camera_c		(data X, data Y, data Vx, data Vy, data accX, data accY,
										 class object_c* center_, unsigned inverse_, float zoomlevel_);

						~camera_c		();

//	static camera_c&	get_instance	(data X, data Y, data Vx, data Vy, data accX, data accY,
//										 class object_c* center_, unsigned inverse_, float zoomlevel_);
};

#endif	//CAMERA_HPP
