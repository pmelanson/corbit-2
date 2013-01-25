#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <corbit/physical.hpp>

class object_c;

class camera_c : public physical_c {
private:

	void			recenter		(),
					move			();

public:

	float			zoom			() const;

	int				size[2];

	object_c		*center;

	bool			tracking;

	float			zoom_level;
	const float		inverse,
					exp;


	void 			pan				(float X, float Y);

	void			update			();

	camera_c						(var X, var Y, var Vx, var Vy, var accX, var accY,
									object_c *center_, float zoomlevel_);

	~camera_c		();
};


#endif	//CAMERA_HPP
