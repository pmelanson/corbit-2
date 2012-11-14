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
	float				zoomlevel;

						camera_c	(data X, data Y, data Vx, data Vy, data accX, data accY,
									 class object_c* center_, unsigned inverse_, float zoomlevel_);
public:
	void				setcenter	(class object_c& center_);
	void 				recenter	(unsigned short dispw, unsigned short disph),
						move		();
	void 				track		(bool totrack),
						toggletrack	();
	void 				panx		(float amount),
						pany		(float amount);

	float				zoom		() const;
	data				x			() const,
						y			() const;
	void				changezoom	(float amount);

	static camera_c&	getinstance (data X, data Y, data Vx, data Vy, data accX, data accY,
									 class object_c* center_, unsigned inverse_, float zoomlevel_);
};

#endif	//CAMERA_HPP
