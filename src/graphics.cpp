#include <corbit/graphics.hpp>
#include <corbit/object.hpp>
#include <corbit/camera.hpp>

extern ALLEGRO_DISPLAY *display;

bool		graphics_c::set_camera			(camera_c *camera) {
	if(!camera)
		return false;

	_camera = camera;
	return true;
}


void		graphics_c::change_zoom			(float amount)			{_camera->change_zoom(amount);}
void 		graphics_c::track				(bool to_track)			{_camera->track(to_track);}
void		graphics_c::toggle_track		()						{_camera->toggle_track();}
void 		graphics_c::pan					(float X, float Y)		{_camera->pan(X, Y);}
void		graphics_c::update				()						{_camera->update();}

void		graphics_c::set_dimensions		(int disp_w, int disp_h){_camera->set_dimensions(disp_w, disp_h);}
void		graphics_c::set_center			(object_c *center_) 	{_camera->set_center(center_);}

void		graphics_c::_draw				(object_c &obj) const {

	if (display)
		al_draw_filled_circle	(
								(obj.x() - _camera->x()) * _camera->zoom() + _camera->screen_w()/2,
								(obj.y() - _camera->y()) * _camera->zoom() - _camera->screen_h()/2,
								obj.radius() * _camera->zoom(),
								obj.color()
								);
}

void		graphics_c::draw_all			(boost::intrusive::list<object_c>& list) const {

	static boost::intrusive::list<object_c>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		_draw(*it);
}


graphics_c::graphics_c						() {}
graphics_c::graphics_c						(camera_c *camera_)
	: _camera (camera_) {}

graphics_c	&graphics_c::get_instance		() {

	static graphics_c instance;
	return instance;
}

graphics_c	&graphics_c::get_instance		(camera_c *camera_) {

	static graphics_c instance(camera_);
	return instance;
}

graphics_c::~graphics_c						() {}
