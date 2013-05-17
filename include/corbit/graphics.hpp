#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include <corbit/globals.hpp>
#include <sstream>
//#include <allegro5/allegro_font.h>
class entity_c;
class hab_c;
class camera_c;
class ALLEGRO_FONT;
class ALLEGRO_COLOR;
using std::stringstream;

namespace graphics {

	void		draw_at		(const entity_c &obj, var zoom, var x, var y);
	void		draw		(const entity_c &obj);

	class		hud_c {
	private:
		unsigned			line,
							column;
		const unsigned		columns;
		const unsigned		padding;
		unsigned			column_w,
							text_start_x,
							text_start_y;
		void				add_line	(std::stringstream &text);
		void				new_column	();

	public:
		const ALLEGRO_COLOR	text_col;
		ALLEGRO_FONT		*font;	//public because this can only be initialised once allegro has been initialised
		void				draw		();

		hud_c				(ALLEGRO_COLOR text_col_)
			: text_col(text_col_), line(0), column(0), columns(3),
			padding(15), font(0x0) {
		}
	};
	extern		hud_c		hud;

	extern		camera_c	*camera;
}


#endif	//GRAPHICS_HPP
