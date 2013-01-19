#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <corbit/globals.hpp>

class object_c;

class camera_c {
private:
	vector			_pos,
					_v,
					_acc;


	void			recenter		(),
					move			();

public:

	var				x				() const,
					y				() const,
					Vx				() const,
					Vy				() const,
					accX			() const,
					accY			() const;
	vector			pos				() const,
					v				() const,
					acc				() const;

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
