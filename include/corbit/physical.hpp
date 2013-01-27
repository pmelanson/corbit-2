#ifndef PHYSICAL_HPP
#define PHYSICAL_HPP


#include <corbit/globals.hpp>

class physical_c {
protected:
			vect	_pos,
					_v,
					_acc;

public:
	virtual var		x			() const	{return _pos(0,0);};
	virtual var		y			() const	{return _pos(1,0);};
	virtual vect	pos			() const	{return _pos;};
	virtual var		Vx			() const	{return _v(0,0);};
	virtual var		Vy			() const	{return _v(1,0);};
	virtual vect	v			() const	{return _v;};
	virtual var		accX		() const	{return _acc(0,0);};
	virtual var		accY		() const	{return _acc(1,0);};
	virtual vect	acc			() const	{return _acc;};

	virtual void	move		();



	physical_c		(var x_,var y_, var Vx_,var Vy_, var accX_,var accY_) :
		_pos(x_,y_), _v(Vx_,Vy_), _acc(accX_,accY_) {
	}
};

#endif	//PHYSICAL_HPP
