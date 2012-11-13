#include "hud.hpp"

hud_c::hud_c							(class object_c* activeship_, class object_c* target_, class object_c* reference_)
	: activeship (activeship_), target (target_), reference (reference_) {}
hud_c& hud_c::getinstance				(class object_c* activeship_, class object_c* target_, class object_c* reference_) {

	static hud_c instance (activeship_, target_, reference_);
	return instance;
}

class object_c*	hud_c::getactiveship	() const						{return activeship;}
class object_c*	hud_c::gettarget		() const						{return target;}
class object_c*	hud_c::getreference		() const						{return reference;}
void 			hud_c::setactiveship	(class object_c* newship)		{activeship = newship;}
void 			hud_c::settarget		(class object_c* newtarget)		{target = newtarget;}
void 			hud_c::setreference		(class object_c* newreference)	{reference = newreference;}
