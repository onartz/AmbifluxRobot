#include <string>
#include "Location.h"

Location::Location():myLocationId(0),myLocationTypeId(0){
	strcpy_s(myLocationName,"");	
}

Location::Location(ptree pt)
{
	try{
		strcpy_s(myLocationName, (char*)(pt.get<std::string>("LocationName").c_str()));
	}
	catch(std::exception const&  ex)
	{
		myLocationId = 0;
		strcpy_s(myLocationName,"");	
		myLocationTypeId = 0;	
		printf("JSON Error. %s", ex.what());
	}

}

//Accesseurs get

char* Location::getLocationName(void)
{return myLocationName;}

int Location::getLocationId(void)
{return myLocationId;}

int Location::getLocationTypeId(void)
{return myLocationTypeId;}


//Accesseurs Set
/*void Location::setLocationName(char *value)
{
	strcpy_s(myLocationName,value);
}

void Location::setLocationTypeId(int value)
{
	myLocationTypeId=value;
}

void Location::setLocationId(int value)
{	
	myLocationId=value;
}*/

