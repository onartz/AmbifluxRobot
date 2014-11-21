#ifndef DEF_WORKORDERROUTING_H
#define DEF_WORKORDERROUTING_H 

#include "Location.h"
#include "Resource.h"
//#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;

class Workorder;

//myTypedef Workorder *ptr_Workorder;
//myTypedef Location *ptr_Location ;
//myTypedef Resource *ptr_Resource;

class WorkorderRouting
{
public:
	enum WORKORDERROUTING_STATE
    {
		UNKNOWN=0,
        COMPLETE,
        PARTIAL,
        FAILURE
    };

//Type de workorderRouting
enum WORKORDERROUTING_TYPE
    {
        TRANSPORT=1,
		CHARGEMENT,
		DECHARGEMENT,
		LIVRAISON,
		TRANSFERTA,
		TRANSFERTB
    };

    //WORKORDERROUTING_STATE WorkorderRoutingState;

	enum WORKORDERROUTING_STATUS
    {
        NEW=1,
        STARTED,
        CLOSE,
		CANCELLED
    };
    //WORKORDERROUTING_STATUS WorkorderRoutingStatus;

	enum WORKORDERROUTING_ERRORCODE
    {
        ERR_TRANSPORT_FAILED=1,
        ERR_IDENTIFICATION_FAILED,
		ERR_TIMEOUT_ARRIVEE_OPERATEUR,
		ERR_TIMEOUT_OPERATION
    };
    //WORKORDERROUTING_ERRORCODE WorkorderRoutingErrorCode;

	
// constructor
	//WorkorderRouting();
	//WorkorderRouting(Workorder &, ptree);
	WorkorderRouting(Workorder *, ptree);
  //WorkorderRouting(Workorder &, Location &);
  //WorkorderRouting(Workorder &, Location &, Resource *);
    // Destructor, its just empty
  //~WorkorderRouting(){};

//Méthodes publiques
  void start(void);
  void close(void);
  void cancel(void);
  bool isClosed(void);
  bool isCancelled(void);
  bool isNew(void);
  bool isStarted(void);

 //Accesseurs get
	//int getWorkorderId(void);
	Workorder* getWorkorder(void);
	int getOperationSequence(void);
	//ptr_Resource getResource(void);
	//ptr_Location getLocation(void);
	Resource* getResource(void) const;
	Location getLocation(void) const;
	//Location getLocation(void);
	//int getLocationId(void);
	//int getResourceId(void);
	char* getScheduledStartDate(void);
	char* getScheduledEndDate(void);
	char* getActualStartDate(void);
	char* getActualEndDate(void);
	char* getModifiedDate(void);
	WORKORDERROUTING_STATUS getStatusId(void);
	WORKORDERROUTING_STATE getStateId(void);
	char* getType(void);
	WORKORDERROUTING_TYPE getTypeLong(void);
	const char* getWorkorderRoutingNo(void) const;
	int getErrorCodeId(void);

 //Accesseurs Set

	//void setWorkorder(Workorder&);
	void setOperationSequence(int);
	void setLocation(Location&);
	//void setLocation(Location);
	//void setLocationId(int);
	//void setResourceId(int);
	void setResource(Resource*);
	void setScheduledStartDate(char *);
	void setScheduledEndDate(char *);
	void setActualStartDate(char *);
	void setActualEndDate(char *);
	void setModifiedDate(char *);
	void setStatusId(WORKORDERROUTING_STATUS);
	void setStateId(WORKORDERROUTING_STATE);
	void setType(char *);
	void setTypeLong(WORKORDERROUTING_TYPE);
	void setWorkorderRoutingNo(char *);
	void setErrorCodeId(int);

 
private:
	Workorder* myWorkorder;
	//Agrégation externe, Location existe avant et initialidsé dans le constructeur
	Location myLocation;
	//Resource peut exister avant
	Resource *myResource;
	int myOperationSequence;
	
	//Location myLocation;
	//int locationId;
	//int resourceId;
	
	char myScheduledStartDate[23];
	char myScheduledEndDate[23];
	char myActualStartDate[23];
	char myActualEndDate[23];
	char myModifiedDate[23];
	//Complet Partial Failure
	WORKORDERROUTING_STATE myStateId;
	//New Started Close Cancel
	WORKORDERROUTING_STATUS myStatusId;
	//Chargement Livraison Transport
	char myType[2];
	WORKORDERROUTING_TYPE myTypeLong;
	char myWorkorderRoutingNo[23];
	int myErrorCodeId;
	
};
#endif