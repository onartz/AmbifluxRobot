#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "DateTime.h"
#include "WorkorderRouting.h"

using namespace std;
	
/*WorkorderRouting::WorkorderRouting():
//location(NULL),
//resource(NULL),
//workOrder(NULL),
myStateId(WorkorderRouting::NEW)
{
}*/

WorkorderRouting::WorkorderRouting(Workorder* workorder, ptree pt):
myWorkorder(workorder)
{		
	try {
		strcpy_s(myWorkorderRoutingNo, (char*)(pt.get<string>("WorkOrderRoutingNo").c_str()));
		strcpy_s(myType, (char*)(pt.get<string>("Type").c_str()));
		switch(*(char*)myType)
			{
			case 'T':
				myTypeLong = TRANSPORT;
				break;
			case 'C':
				myTypeLong = CHARGEMENT;
				break;
			case 'L':
				myTypeLong = LIVRAISON;
				break;
			}
		//Création de Location
	try {
		ptree ptLocation = pt.get_child("Location");
		myLocation = Location(ptLocation);		
	}
	catch(std::exception const&  ex)
	{
		printf("JSON Error. %s", ex.what());
		//return false;
	}
		//Todo : modifier pour faire la lecture ds le ptree
		//Modifier le wcf pour générer Status
		myStatusId = NEW;	
		myStateId = UNKNOWN;
	}
	catch (exception& e)
	{
		cout << e.what() << '\n';
	}
	
}

/*WorkorderRouting::WorkorderRouting(Workorder *workorder, Location &location, Resource *resource):
myWorkorder(workorder),
myLocation(location),
myResource(resource),
myStatusId(WorkorderRouting::NEW)
{

}*/

/*WorkorderRouting::WorkorderRouting(Workorder *workorder, Location &location):
myWorkorder(workorder),
myLocation(location),
myResource(NULL),
myStatusId(WorkorderRouting::NEW)
{

}*/


void WorkorderRouting::start()
{
	if(myStatusId == NEW)
	{
		printf("WorkorderRouting %s started\n",myWorkorderRoutingNo);
		myStatusId = STARTED;	
		strcpy_s(myActualStartDate,aipllib::currentDateTime());
	}
}

//Close
void WorkorderRouting::close()
{
	if(myStatusId==STARTED)
	{
		printf("WorkorderRouting %s closed\n",myWorkorderRoutingNo);
		myStatusId=CLOSE;
		strcpy_s(myActualEndDate,aipllib::currentDateTime());
	}
}

//Cancel
void WorkorderRouting::cancel()
{
	printf("WorkorderRouting %s annule\n",myWorkorderRoutingNo);
	myStatusId=CANCELLED;
}

bool WorkorderRouting::isCancelled(void){return myStatusId==CANCELLED;}
bool WorkorderRouting::isStarted(void){return myStatusId==STARTED;}
bool WorkorderRouting::isClosed(void){return myStatusId==CLOSE;}
bool WorkorderRouting::isNew(void){return myStatusId==NEW;}


//Accesseurs get
	Workorder* WorkorderRouting::getWorkorder(void)	{return myWorkorder;}
	int WorkorderRouting::getOperationSequence(void) {return myOperationSequence;}
	Resource* WorkorderRouting::getResource(void) const{return myResource;}
	Location WorkorderRouting::getLocation(void) const{return myLocation;};
	//ptr_Location WorkorderRouting::getLocation(void){return location;}
	//int WorkorderRouting::getLocationId(void){return locationId;}
	//int WorkorderRouting::getResourceId(void){return resourceId;}
	char* WorkorderRouting::getScheduledStartDate(void){return myScheduledStartDate;}
	char* WorkorderRouting::getScheduledEndDate(void){return myScheduledEndDate;}
	char* WorkorderRouting::getActualStartDate(void){return myActualStartDate;}
	char* WorkorderRouting::getActualEndDate(void){return myActualEndDate;}
	char* WorkorderRouting::getModifiedDate(void){return myModifiedDate;}
	WorkorderRouting::WORKORDERROUTING_STATUS WorkorderRouting::getStatusId(void){return myStatusId;}
	WorkorderRouting::WORKORDERROUTING_STATE WorkorderRouting::getStateId(void){return myStateId;}
	char* WorkorderRouting::getType(void){return myType;}
	WorkorderRouting::WORKORDERROUTING_TYPE WorkorderRouting::getTypeLong(void){return myTypeLong;}
	const char* WorkorderRouting::getWorkorderRoutingNo(void) const {return myWorkorderRoutingNo;}
	int WorkorderRouting::getErrorCodeId(void){return myErrorCodeId;}

	//Accesseurs Set
	//void WorkorderRouting::setWorkorder(Workorder& value){myWorkorder = value;}
	void WorkorderRouting::setOperationSequence(int value){myOperationSequence=value;}
	void WorkorderRouting::setLocation(Location& value)
	{
		//this->locationId=value->getLocationId();
		myLocation = value;
	}
	//void WorkorderRouting::setLocation(Location value){myLocation = value;}
	//void WorkorderRouting::setLocationId(int value){this->locationId=value;}
	//void WorkorderRouting::setResourceId(int value){this->resourceId=value;}
	void WorkorderRouting::setResource(Resource* value ){myResource=value;}
	void WorkorderRouting::setScheduledStartDate(char *value){strcpy_s(myScheduledStartDate,value);}
	void WorkorderRouting::setScheduledEndDate(char *value){strcpy_s(myScheduledEndDate,value);}
	void WorkorderRouting::setActualStartDate(char *value){strcpy_s(myActualStartDate,value);}
	void WorkorderRouting::setActualEndDate(char *value){strcpy_s(myActualEndDate,value);}
	void WorkorderRouting::setModifiedDate(char *value){strcpy_s(myModifiedDate,value);}
	void WorkorderRouting::setStatusId(WORKORDERROUTING_STATUS value){myStatusId=value;}
	void WorkorderRouting::setStateId(WORKORDERROUTING_STATE value){myStateId=value;}
	void WorkorderRouting::setType(char *value){strcpy_s(myType,value);}
	void WorkorderRouting::setTypeLong(WORKORDERROUTING_TYPE value){myTypeLong=value;}
	void WorkorderRouting::setWorkorderRoutingNo(char *value){strcpy_s(myWorkorderRoutingNo,value);}
	void WorkorderRouting::setErrorCodeId(int value){myErrorCodeId=value;}


#endif