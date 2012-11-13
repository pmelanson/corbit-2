#ifndef HUD_HPP
#define HUD_HPP

class hud_c {
private:

	class object_c	*activeship,	//ship currently controlled
					*target,
					*reference;

					hud_c			(class object_c* activeship_, class object_c* target_, class object_c* reference_);
public:

	class object_c	*getactiveship	() const,
					*gettarget		() const,
					*getreference	() const;
	void			setactiveship	(class object_c* newship),
					settarget		(class object_c* newtarget),
					setreference	(class object_c* newreference);

	void			drawHUD			() const;

	static hud_c&	getinstance		(class object_c* activeship_, class object_c* target_, class object_c* reference_);
};

#endif	//HUD_HPP
