#ifndef HUD_H
#define HUD_H

class hud_c {
private:

	class object_c* activeship, *target, *reference;

	hud_c (class object_c* _activeship, class object_c* _target, class object_c* _reference);
public:

	class object_c*	getactiveship	();
	class object_c*	gettarget		();
	class object_c*	getreference	();
	void			setactiveship	(class object_c* newship);
	void			settarget		(class object_c* newtarget);
	void			setreference	(class object_c* newreference);

	void drawHUD();

	static hud_c& getinstance (class object_c* _activeship, class object_c* _target, class object_c* _reference);
};

#endif // HUD_H
