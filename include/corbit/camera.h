#ifndef CAMERA_H
#define CAMERA_H

class camera_c {
private:

	struct pos_t {

		long double x, y;
		pos_t (long double _x, long double _y);
	} pos;
	struct v_t : pos_t {

		v_t (long double _Vx, long double _Vy);
	} v;

	bool tracking;
	const unsigned inverse;
	float zoomlevel;

	camera_c (long double _x, long double _y, long double _Vx, long double _Vy, unsigned _inverse, float _zoomlevel);
public:
	void setcenter (class object_c& center);
	void recenter();
	void track (bool totrack);
	void panX (int amount), panY (int amount);

	float zoom();
	void changezoom (float amount);

	static camera_c& getinstance (long double _x, long double _y, long double _Vx, long double _Vy, unsigned _inverse, float _zoomlevel);
};

#endif // CAMERA_H
