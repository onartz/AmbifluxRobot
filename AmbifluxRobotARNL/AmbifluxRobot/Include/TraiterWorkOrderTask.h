#ifndef DEF_TRAITERWORKORDERTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_TRAITERWORKORDERTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "ArNetworking.h"
#include "Arnl.h"
#include "ArLocalizationTask.h"
#include "ArDocking.h"
//#include "ArAsyncTask.h"
//#include "ArCondition.h"
//#include "ArMutex.h"
#include "IhmCommunicationThread.h"
#include "WorkOrder.h"
#include "WorkOrderRouting.h"
#include "WorkOrderRoutingTask.h"
//#include "Robot.h"
//#include "CppEvent.h"
#include "DALRest.h"


class TraiterWorkorderTask : public ArASyncTask
{

//Diff�rents �tats
 enum State { 
	 STATE_START,
	 STATE_LECTURE_WORS,
	 STATE_TRAITEMENT_WOR,
	 STATE_ATTENTE_FIN_WOR,
	 STATE_FIN_WO,
	 STATE_CANCELLED
 };

 void handler(void);
 void setState(State);


public:
	//Constructeurs
	TraiterWorkorderTask();
	TraiterWorkorderTask(ArServerModeGoto *modeGoto, ArServerModeDock *modeDock, WorkOrder *, IhmCommunicationThread *);

	void Stop();


protected:
	ArServerModeGoto *myModeGoto;
	ArServerModeDock *myModeDock;
	IhmCommunicationThread *myIhm;

	State myState;
	bool myNewState;
	ArTime myStateStartTime;
	ArTime myCycleStartTime;
	bool fin;

	/*Robot* robot;
	TabletteSM* tabletteSM;*/
	//LecteurCarte *lecteurCarte;


	WorkOrderRoutingTask worTask;
	WorkOrder *workOrder;
	WorkOrderRouting* workorderRouting;
	//std::vector<WorkOrderRouting*> worList;
	std::list<WorkOrderRouting*> worList;

	ArCondition condition;
	ArMutex myMutex;
	char bufETCPServerEvt[256];

	int retCode;
	int indexWor;

	//Tache asynchrone
	virtual void* runThread(void*);

};
#endif