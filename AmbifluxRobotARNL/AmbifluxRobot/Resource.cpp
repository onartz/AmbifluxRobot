#include <string>
#include "Resource.h"

using namespace std;
Resource::Resource(){
	myId = 0;
	myName = "";
}

Resource::Resource(ptree pt)
{
	try{
		myId = pt.get<int>("ResourceId");
		myName = string((char*)(pt.get<std::string>("Name").c_str()));	
	}
	catch(std::exception const&  ex)
	{
		myId = 0;
		myName = "";
		printf("JSON Error. %s", ex.what());
	}
}

Resource::Resource(string resource)
{
	ptree pt = JSONParser::parse(resource);
	try{
		myId = pt.get_child("GetResourceByIdResult").get<int>("ResourceId");
		myName = string((char*)(pt.get_child("GetResourceByIdResult").get<std::string>("Name").c_str()));
	}
	catch(std::exception const&  ex)
	{
		myId = 0;
		myName = "";
		printf("JSON Error. %s", ex.what());
	}
}
//Accesseurs get
string getName(void);
int Resource::getId(void){return myId;}
string Resource::getName(void){return myName;}

