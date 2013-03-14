#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include <corbit/globals.hpp>
#include <allegro5/allegro_font.h>
#include <iostream>
class entity_c;
class hab_c;
class camera_c;
class ALLEGRO_FONT;
class ALLEGRO_COLOR;

namespace graphics {

	void		draw		(const entity_c &obj);
	void		draw		(const hab_c &hab);

	class		hud_c {

	private:
		const ALLEGRO_COLOR	text_col;
		unsigned			lines;
		void				add_line	(std::stringstream &text);

	public:
		ALLEGRO_FONT		*font;	//public because this can only be initialised once allegro has been initialised
		void				draw		();

		hud_c				(ALLEGRO_COLOR text_col_)
			: text_col(text_col_), lines(1), font(0x0) {
		}
	};

	extern		hud_c		hud;

	extern		camera_c	*camera;
}


#endif	//GRAPHICS_HPP
