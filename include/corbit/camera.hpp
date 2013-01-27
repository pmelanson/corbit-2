#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <corbit/physical.hpp>

class object_c;

class camera_c : public physical_c {
private:

	void			recenter		();

public:

	float			zoom			() const;

	int				size[2];

	object_c		*center;

	bool			tracking;

	float			zoom_level;
	const float		exp;


	void 			pan				(float X, float Y);

	void			update			();

	camera_c						(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									object_c *center_, float zoomlevel_);

	~camera_c		();
};


#endif	//CAMERA_HPP
