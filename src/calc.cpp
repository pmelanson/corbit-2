#include <corbit/calc.hpp>

				calc_c::calc_c			(class object_c* activeship_, class object_c* target_, class object_c* reference_)
	: activeship (activeship_), target (target_), reference (reference_) {}

				calc_c::~calc_c			() {}

/*
calc_c& calc_c::get_instance				(class object_c* activeship_, class object_c* target_, class object_c* reference_) {

	static calc_c instance (activeship_, target_, reference_);
	return instance;
}
*/

class object_c*	calc_c::getactiveship	() const						{return activeship;}
class object_c*	calc_c::gettarget		() const						{return target;}
class object_c*	calc_c::getreference		() const						{return reference;}
void 			calc_c::setactiveship	(class object_c* newship)		{activeship = newship;}
void 			calc_c::settarget		(class object_c* newtarget)		{target = newtarget;}
void 			calc_c::setreference		(class object_c* newreference)	{reference = newreference;}
