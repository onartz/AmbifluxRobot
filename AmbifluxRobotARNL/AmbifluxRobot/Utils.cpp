#include "Utils.h"

using namespace boost::random;

int Utils::randomInt(int inf, int sup)
{
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> dist(inf, sup);
	return dist(rng);



	

	/*boost::random::mt19937 rng;
	boost::uniform_int<> dist(inf, sup);
	boost::variate_generator<boost::random::mt19937 &,boost::uniform_int<>>dice(rng,dist);
	return(dice());	*/
}

int Utils::randomInt(int inf, int sup, int exclusion)
{
	int res;
	boost::random::mt19937 rng;
	boost::uniform_int<> dist(inf, sup);
	boost::variate_generator<boost::random::mt19937 &,boost::uniform_int<>>dice(rng,dist);
	while((res=dice())==exclusion);
	return(res);	
}