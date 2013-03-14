#ifndef NAV_HPP
#define NAV_HPP


#include <corbit/globals.hpp>
class entity_c;

namespace nav {

	extern entity_c	*ship,
					*targ,
					*ref;

	enum			navmode		{MAN, NAVMAX};

	void			stabilise	();


	void			update		();
}


#endif	//NAV_HPP
