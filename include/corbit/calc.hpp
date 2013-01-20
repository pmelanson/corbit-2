#ifndef CALC_HPP
#define CALC_HPP
#include <corbit/globals.hpp>

class object_c;

namespace calc {

	var				distance2	(const object_c &A, const object_c &B),
					distance	(const object_c &A, const object_c &B),
					theta		(const object_c &A, const object_c &B),
					gravity		(const object_c &A, const object_c &B),
					orbitV		(const object_c &A, const object_c &B),
					Vcen		(const object_c &A, const object_c &B),
					Vtan		(const object_c &A, const object_c &B),
					ecc			(const object_c &A, const object_c &B);
	vector			position	(const object_c &A, const object_c &B),
					velocity	(const object_c &A, const object_c &B);
};

#endif	//CALC_HPP
