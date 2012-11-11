#ifndef HUD_HPP
#define HUD_HPP

class hud_c {
private:

	class object_c	*activeship,	//ship currently controlled
					*target,
					*reference;

					hud_c			(class object_c* _activeship, class object_c* _target, class object_c* _reference);
public:

	class object_c	*getactiveship	(),
					*gettarget		(),
					*getreference	();
	void			setactiveship	(class object_c* newship),
					settarget		(class object_c* newtarget),
					setreference	(class object_c* newreference);

	void			drawHUD			();

	static hud_c&	getinstance		(class object_c* _activeship, class object_c* _target, class object_c* _reference);
};

#endif // HUD_HPP
