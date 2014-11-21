#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

//#include <string>
//#include <list>
#include "DateTime.h"
#include "Workorder.h"

using namespace std;
	
//constructeur par défaut
/*Workorder::Workorder()
{
	workOrderId=0;
	strcpy_s(startDate,"");
	strcpy_s(endDate,"");
	orderHeader=NULL;
	strcpy_s(myWorkorderNo,"");
	strcpy_s(myType,"");
	//this->myWorkorderRoutingList();
	//this->myWorkorderRoutingList=NULL;

}*/

Workorder::Workorder(ptree pt)
{
	try{
		strcpy_s(myWorkorderNo, (char*)(pt.get<string>("WorkorderNo").c_str()));
		strcpy_s(myType, (char*)(pt.get<string>("type").c_str()));
		workOrderId = pt.get<int>("WorkOrderID");
		myStatusId = NEW;
		//WorkorderRoutings	
		BOOST_FOREACH(ptree::value_type &json_workorderRouting, pt.get_child("WorkorderRoutings"))
		{
			assert(json_workorderRouting.first.empty());
			WorkorderRouting wor(&(*this), json_workorderRouting.second);
			myWorkorderRoutingList.push_back(wor);
		}

		//Création de OrderHeader
		try {
			ptree ptoh = pt.get_child("OrderHeader");
			myOrderHeader = OrderHeader(ptoh);		
		}
		catch(std::exception const&  ex)
		{
			printf("JSON Error. %s", ex.what());
			//return false;
		}
	}
	catch(std::exception const&  ex)
	{
		workOrderId = 0;
		strcpy_s(startDate, "");
		strcpy_s(endDate, "");
		strcpy_s(myModifiedDate, "");
		strcpy_s(dateCreated, "");

		orderHeaderId = 0;
		resourceId = 0;
		strcpy_s(myWorkorderNo, "");
		myStatusId = Workorder::NEW;
		finishedStateId = Workorder::WO_FAILURE;
		strcpy_s(myType, "");	
	
		currentWorkorderRouting = NULL;
		//std::vector<WorkorderRouting*> workorderRoutingList;
		printf("JSON Error. %s", ex.what());
		//return false;
	}
	
}

/*Workorder::Workorder(list<WorkorderRouting>& worList):
orderHeader(NULL),
myWorkorderRoutingList(worList)
{}*/

//Démarrage
void Workorder::start()
{
	if(myStatusId==NEW)
{
	printf("Workorder %s started\n",myWorkorderNo);
	myStatusId=STARTED;	
	strcpy_s(startDate,aipllib::currentDateTime());

}

}

//Close
void Workorder::close()
{
	if(myStatusId==STARTED)
	{
		myStatusId=CLOSE;
		printf("Workorder %s closed\n",myWorkorderNo);
		strcpy_s(endDate,aipllib::currentDateTime());
	}
}

//Cancel
void Workorder::cancel()
{
	printf("Workorder %s annule\n",myWorkorderNo);
	myStatusId=CANCELLED;


}

//void Workorder::addWorkorderRouting(WorkorderRouting* pWor){
//	myWorkorderRoutingList.push_back(*pWor);
//}
//
//
//
//void Workorder::addWorkorderRouting(WorkorderRouting pWor){
//	myWorkorderRoutingList.push_back(pWor);
//}

//Obtient la liste des workorderRoutings
/*std::vector<ptr_WorkorderRouting> Workorder::getWorkorderRoutings(void)
{
	std::vector<ptr_WorkorderRouting> vecWorkorderRoutings;


	return vecWorkorderRoutings;

}*/
//Accesseurs get
int Workorder::getWorkorderId(void){return workOrderId;}
int Workorder::getOrderHeaderId(void){return orderHeaderId;}
int Workorder::getResourceId(void){return resourceId;}
char* Workorder::getWorkorderStartDate(void){return(startDate);}
char* Workorder::getWorkorderEndDate(void){return(endDate);}
OrderHeader Workorder::getOrderHeader(void){return(myOrderHeader);}
char* Workorder::getWorkorderNo(void){return(myWorkorderNo);}
Workorder::WORKORDER_STATUS Workorder::getStatusId(void){return(myStatusId);}
Workorder::WORKORDER_STATE Workorder::getFinishedStateId(void){return(finishedStateId);}
char* Workorder::getWorkorderType(void){return(myType);}
//std::vector<WorkorderRouting*> Workorder::getWorkorderRoutingList(){return(this->myWorkorderRoutingList);}
 //std::list<WorkorderRouting> Workorder::getWorkorderRoutingList(){return(myWorkorderRoutingList);}
 std::list<WorkorderRouting> Workorder::getWorkorderRoutingList(void){return(myWorkorderRoutingList);}
 //std::list<WorkorderRouting> getWorkorderRoutingList(void) const;
bool Workorder::isCancelled(void){return myStatusId==CANCELLED;}
bool Workorder::isStarted(void){return myStatusId==STARTED;}
bool Workorder::isClosed(void){return myStatusId==CLOSE;}
bool Workorder::isNew(void){return myStatusId==NEW;}


//Accesseurs Set
//void Workorder::setWorkorderId(int value){workOrderId=value;}
//void Workorder::setOrderHeaderId(int value){orderHeaderId=value;}
//void Workorder::setResourceId(int value){resourceId=value;}
//
//void Workorder::setWorkorderStartDate(char *value)
//{strcpy_s(startDate,value);}
//
//void Workorder::setWorkorderStartDate()
//{strcpy_s(startDate,aipllib::currentDateTime());}
//
//void Workorder::setWorkorderEndDate(char *value)
//{strcpy_s(endDate,value);}
//
//void Workorder::setModifiedDate(char *value)
//{strcpy_s(myModifiedDate,value);}
//
//void Workorder::setDateCreated(char *value)
//{strcpy_s(dateCreated,value);}

/*void Workorder::setOrderHeader(ptr_OrderHeader orderHeader)
{this->orderHeader=orderHeader;}*


void Workorder::setWorkorderNo(char *value)
{
	strcpy_s(myWorkorderNo,value);
}

void Workorder::setWorkorderType(char *value)
{
	strcpy_s(myType,value);
}

void Workorder::setStatusId(WORKORDER_STATUS value)
{
	myStatusId=value;
}

void Workorder::setFinishedStateId(WORKORDER_STATE value)
{
	this->finishedStateId=value;
}
/*void Workorder::setWorkorderRoutingList(std::vector<ptr_WorkorderRouting> value)
{
	myWorkorderRoutingList=myWorkorderRoutingList(
}*/
/*void Workorder::setWorkorderRoutingList(std::list<WorkorderRouting> value)
 {
	 myWorkorderRoutingList = value;
 }*/


/*void Workorder::setWorkorderRoutingList(std::vector<ptr_WorkorderRouting> value)
{
	this->myWorkorderRoutingList=value;
}*/
#endif