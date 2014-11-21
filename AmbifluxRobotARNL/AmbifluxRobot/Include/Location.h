#ifndef DEF_LOCATION_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_LOCATION_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus


#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using namespace std;


//Location class
class Location
{
public:
	// Constructors
	  Location();
	  Location(ptree);
	  // Destructor, its just empty
	  ~Location(){};

	 //Getters
	 int getLocationId(void);
	 int getLocationTypeId(void);
	 char* getLocationName(void);

protected:
	int myLocationId;
	char myLocationName[14];	
	int myLocationTypeId;	
};
#endif