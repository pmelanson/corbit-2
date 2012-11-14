
struct	pos_s {
		data	x, y;
		pos_s	(data X, data Y)		: x (X), y (Y) {}
} pos;

struct	v_s : pos_s {
		v_s		(data Vx, data Vy)		: pos_s (Vx, Vy) {}
} v;

struct	acc_s : pos_s {
		acc_s	(data accX, data accY)	: pos_s (accX, accY) {}
} acc;

