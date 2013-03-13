#ifndef PHYSICAL_HPP
#define PHYSICAL_HPP


#include <corbit/globals.hpp>

class physical_c {
public:
			vect	pos,
					v,
					acc;

	virtual var		x			() const	{return pos[0];}
	virtual var		y			() const	{return pos[1];}
	virtual var		Vx			() const	{return v[0];}
	virtual var		Vy			() const	{return v[1];}
	virtual var		accX		() const	{return acc[0];}
	virtual var		accY		() const	{return acc[1];}

	virtual void	move		();



	physical_c		(var x_,var y_, var Vx_,var Vy_, var accX_,var accY_) :
		pos(x_,y_), v(Vx_,Vy_), acc(accX_,accY_) {
	}
};

#endif	//PHYSICAL_HPP
