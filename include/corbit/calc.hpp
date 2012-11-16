#ifndef CALC_HPP
#define CALC_HPP

class calc_c {
private:

	class object_c	*activeship,	//ship currently controlled
					*target,
					*reference;
public:

	class object_c	*getactiveship	() const,
					*gettarget		() const,
					*getreference	() const;
	void			setactiveship	(class object_c* newship),
					settarget		(class object_c* newtarget),
					setreference	(class object_c* newreference);

	void			drawHUD			() const;

					calc_c			(class object_c* activeship_, class object_c* target_, class object_c* reference_);
					~calc_c			();

//	static calc_c&	get_instance	(class object_c* activeship_, class object_c* target_, class object_c* reference_);
};

#endif	//CALC_HPP
