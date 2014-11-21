#include <string>
#include "OrderHeader.h"

using namespace std;

OrderHeader::OrderHeader():myOrderHeaderId(0)
{
	this->setObjetDemandeExpress("");
	this->setOrderHeaderNo("");
	this->setOrderHeaderType("");
	
}

OrderHeader::OrderHeader(ptree pt)
{
	try{
		strcpy_s(myOrderHeaderNo, (char*)(pt.get<string>("OrderNo").c_str()));
		myOrderHeaderId = pt.get<int>("OrderId");
		//strcpy_s(myType, (char*)(pt.get<string>("type").c_str()));
	}
	catch(std::exception const&  ex)
	{
		printf("JSON Error. %s", ex.what());
		//return false;
	}
}
OrderHeader::OrderHeader(int Id):myOrderHeaderId(Id)
{	
	this->setObjetDemandeExpress("");
	this->setOrderHeaderNo("");
	this->setOrderHeaderType("");
}
//Accesseurs get
char* OrderHeader::getOrderHeaderNo(void)
{return myOrderHeaderNo;}

int OrderHeader::getOrderHeaderId(void)
{return myOrderHeaderId;}

char* OrderHeader::getOrderHeaderType(void)
{return this->myOrderHeaderType;}

char* OrderHeader::getObjetDemandeExpress(void)
{return this->myObjetDemandeExpress;}

ptr_Location OrderHeader::getLocation(void){return myLocation;}
int OrderHeader::getLocationId(void){return myLocationId;}

//Accesseurs Set
void OrderHeader::setOrderHeaderId(int value)
{	
	myOrderHeaderId=value;
}
void OrderHeader::setOrderHeaderNo(char *value)
{
	strcpy_s(myOrderHeaderNo,value);
}

void OrderHeader::setOrderHeaderType(char *value)
{
	strcpy_s(myOrderHeaderType,value);
}

void OrderHeader::setObjetDemandeExpress(char *value)
{
	strcpy_s(myObjetDemandeExpress,value);
}

void OrderHeader::setLocation(ptr_Location value){this->myLocation=value;}
	void OrderHeader::setLocationId(int value){this->myLocationId=value;}


