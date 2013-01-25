#ifndef PHYSICAL_HPP
#define PHYSICAL_HPP


#include <corbit/globals.hpp>

class physical_c {
protected:
	vect	_pos,
			_v,
			_acc;

public:
	var		x		() const	{return _pos(0,0);};
	var		y		() const	{return _pos(1,0);};
	vect	pos		() const	{return _pos;};
	var		Vx		() const	{return _v(0,0);};
	var		Vy		() const	{return _v(1,0);};
	vect	v		() const	{return _v;};
	var		accX	() const	{return _acc(0,0);};
	var		accY	() const	{return _acc(1,0);};
	vect	acc		() const	{return _acc;};

	physical_c		(var x_,var y_, var Vx_,var Vy_, var accX_,var accY_) :
		_pos(x_,y_), _v(Vx_,Vy_), _acc(accX_,accY_) {
	}
};

#endif	//PHYSICAL_HPP
