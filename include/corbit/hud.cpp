#include "hud.h"

hud_c::hud_c (class object_c* _activeship, class object_c* _target, class object_c* _reference)
: activeship (_activeship), target (_target), reference (_reference) {
}
hud_c& hud_c::getinstance (class object_c* _activeship, class object_c* _target, class object_c* _reference) {

	static hud_c instance (_activeship, _target, _reference);
	return instance;
}

class object_c*	hud_c::getactiveship	()								{return activeship;}
class object_c*	hud_c::gettarget		()								{return target;}
class object_c*	hud_c::getreference		()								{return reference;}
void 			hud_c::setactiveship	(class object_c* newship)		{activeship = newship;}
void 			hud_c::settarget		(class object_c* newtarget)		{target = newtarget;}
void 			hud_c::setreference		(class object_c* newreference)	{reference = newreference;}
