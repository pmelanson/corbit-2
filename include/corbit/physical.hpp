#ifndef PHYSICAL_HPP
#define PHYSICAL_HPP


#include <corbit/globals.hpp>

class physical_c {
public:
			vect	pos,
					v,
					acc;

	virtual void	move		();



	physical_c		(var x_,var y_, var Vx_,var Vy_, var accX_,var accY_) :
		pos(x_,y_), v(Vx_,Vy_), acc(accX_,accY_) {
	}
};

#endif	//PHYSICAL_HPP
