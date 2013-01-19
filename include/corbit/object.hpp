#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <boost/intrusive/list.hpp>

#include <corbit/globals.hpp>

class object_c : public boost::intrusive::list_base_hook<> {
public:
	const std::string	name;
	const var			mass,
						radius;

private:
	vector				_pos,
						_v,
						_acc;

public:
	var					x				() const				{return _pos(0,0);};
	var					y				() const				{return _pos(1,0);};
	vector				pos				() const				{return _pos;};
	var					Vx				() const				{return _v(0,0);};
	var					Vy				() const				{return _v(1,0);};
	vector				v				() const				{return _v;};
	var					accX			() const				{return _acc(0,0);};
	var					accY			() const				{return _acc(1,0);};
	vector				acc				() const				{return _acc;};
	void				set_x			(var x_)				{_pos(0,0) = x_;};
	void				set_y			(var y_)				{_pos(1,0) = y_;};
	void				set_pos			(var x_, var y_)		{_pos = vector(x_, y_);};
	void				set_Vx			(var Vx_)				{_v(0,0) = Vx_;};
	void				set_Vy			(var Vy_)				{_v(1,0) = Vy_;};
	void				set_V			(var Vx_, var Vy_)		{_v = vector(Vx_, Vy_);};
	void				set_accX		(var accX_)				{_acc(0,0) = accX_;};
	void				set_accY		(var accY_)				{_acc(1,0) = accY_;};
	void				set_acc			(var accX_, var accY_)	{_acc - vector(accX_, accY_);};

	void				move			(),
						accelerate		(var force, var radians);

	const col			color;

	object_c							(std::string name_, var m, var r,
										var X, var Y, var Vx, var Vy, var accX, var accY,
										col color_);
	object_c							();

	~object_c							();
};

#endif	//OBJECT_HPP
