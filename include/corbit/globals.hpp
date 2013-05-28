#ifndef GLOBALS_HPP
#define GLOBALS_HPP


#include <eigen3/Eigen/Dense>
#include <allegro5/color.h>

typedef				long double				var;
typedef				Eigen::Matrix<var, 1, 2>	vect;
const int				FPS							=30;
enum					ENTITY_TYPE					{ENTITY, HAB, PLANET, ENTITY_TYPE_MAX};


#endif	//GLOBALS_HPP
