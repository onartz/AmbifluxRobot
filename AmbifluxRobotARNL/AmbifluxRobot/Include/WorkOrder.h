#ifndef DEF_WORKORDER_H
#define DEF_WORKORDER_H 

//Forward dependencie
//La classe WorkorderRouting include déjà Workorder.h
//class WorkorderRouting;

#include <vector>
#include <list>
#include <string>
#include "WorkorderRouting.h"
#include "OrderHeader.h"
//#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;


//#include "OrderHeader.h"

class OrderHeader;

typedef OrderHeader * ptr_OrderHeader;
typedef WorkorderRouting * ptr_WorkorderRouting;

class Workorder
{
public:

enum WORKORDER_STATE
    {
        WO_COMPLETE=1,
        WO_PARTIAL,
        WO_FAILURE
    };
	

enum WORKORDER_STATUS
    {
        NEW=1,
        STARTED,
        CLOSE,
		CANCELLED
    };

  Workorder(ptree);
    // Destructor, its just empty
  ~Workorder(void){};

  //WORKORDER_STATE wostate;

//Méthodes publiques
  void start(void);
  void close(void);
  void cancel(void);

  bool isClosed(void);
  bool isCancelled(void);
  bool isNew(void);
  bool isStarted(void);

 /* void addWorkorderRouting(WorkorderRouting*);
  void addWorkorderRouting(WorkorderRouting);*/


 //Accesseurs get
 int getWorkorderId(void);
 int getOrderHeaderId(void);
 int getResourceId(void);
 char* getWorkorderStartDate(void);
 char* getWorkorderEndDate(void);
 OrderHeader getOrderHeader(void);
 char* getWorkorderNo(void);
 WORKORDER_STATUS getStatusId(void);
 WORKORDER_STATE getFinishedStateId(void);
 char* getWorkorderType(void);
 //std::vector<WorkorderRouting*> getWorkorderRoutingList(void);
 //std::list<WorkorderRouting> getWorkorderRoutingList(void);
 std::list<WorkorderRouting> getWorkorderRoutingList(void);

 //Accesseurs Set
 //void setWorkorderId(int);
 //void setWorkorderStartDate(char *);
 //void setWorkorderStartDate();
 //void setWorkorderEndDate(char *);
 ////void setOrderHeader(ptr_OrderHeader);
 //void setOrderHeaderId(int);
 //void setResourceId(int);
 //void setWorkorderNo(char *);
 //void setStatusId(WORKORDER_STATUS);
 //void setFinishedStateId(WORKORDER_STATE);
 //void setWorkorderType(char *);
 //void setModifiedDate(char *);
 //void setDateCreated(char *);
 /*void setWorkorderRoutingList(std::vector<ptr_WorkorderRouting>);
 void setWorkorderRoutingList(std::list<WorkorderRouting>);*/
 
protected:
	int workOrderId;
	char startDate[23];
	char endDate[23];
	char myModifiedDate[23];
	char dateCreated[23];
	OrderHeader myOrderHeader;
	int orderHeaderId;
	int resourceId;
	char myWorkorderNo[12];
	WORKORDER_STATUS myStatusId;
	WORKORDER_STATE finishedStateId;
	char myType[4];	
	
	ptr_WorkorderRouting currentWorkorderRouting;
	//std::vector<WorkorderRouting*> workorderRoutingList;
	std::list<WorkorderRouting> myWorkorderRoutingList;
};
#endif