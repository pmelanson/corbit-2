#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <corbit/physical.hpp>

class entity_c;

class camera_c : public physical_c {
private:

	void			recenter		();

public:

	float			zoom			() const;

	int				size[2];

	entity_c		*center;

	bool			tracking;

	float			zoom_level;


	void 			pan				(float X, float Y);

	void			update			();

	camera_c						(var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									entity_c *center_, float zoomlevel_);

	~camera_c		();
};


#endif	//CAMERA_HPP
