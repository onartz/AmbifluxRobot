//#include <string>
#include "Person.h"

//SRMA::SRMA():myLoad(SRMA::UNLOADED), myResourceId(9){};
//SRMA::SRMA(int resourceId):myLoad(UNLOADED), myResourceId(resourceId){};
Person::Person(){
	myCardId = "";
	myFirstName = "";
	myLastName = "";
	myEmailAddress = "";
}
//Appel du constructeur de la classe Resource
Person::Person(string str)
{
	ptree pt = JSONParser::parse(str);
	try{
		//myFirstName = pt.get_child("GetResourceByIdResult").get<int>("firstName");
		myFirstName = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("firstname").c_str()));
		myLastName = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("lastname").c_str()));
		myCardId = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("cardID").c_str()));
		myEmailAddress = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("emailAddress").c_str()));	
	}
	catch(std::exception const&  ex)
	{
		myCardId = "";
		myFirstName = "";
		myLastName = "";
		myEmailAddress = "";

		printf("JSON Error. %s", ex.what());
	}

}

string Person::getCardId(){return myCardId;}
string Person::getFirstName(){return myFirstName;}
string Person::getLastName(){return myLastName;}



