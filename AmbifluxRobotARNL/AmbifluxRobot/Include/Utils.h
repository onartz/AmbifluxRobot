#ifndef DEF_UTILS_H
#define DEF_UTILS_H

#include "boost/random/uniform_int_distribution.hpp"
#include "boost/random/uniform_int.hpp"
#include "boost/random/variate_generator.hpp"
#include "boost/random/mersenne_twister.hpp"


class Utils
{
public:
	static int randomInt(int inf, int sup);
	static int randomInt(int inf, int sup, int exclusion);
};
#endif