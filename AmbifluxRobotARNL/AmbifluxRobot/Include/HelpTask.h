#ifndef DEF_HELPTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_HELPTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

#include "Location.h"
#include "DAL.h"
#include "CppEvent.h"

#include "SRMA.h"
#include "Resource.h"
#include "ArSocket.h"

#include "strutil.h"

//class SRMA;

#define TIMEOUT_DUREE_CHARGEMENT 300
#define TIMEOUT_DUREE_DECHARGEMENT 300

class HelpTask : public ArASyncTask
{
public:

//Différents états
 enum State { 
	 STATE_TRANSPORT_START,
	STATE_TRANSPORT,
	STATE_TRANSPORT_FIN,
	STATE_ATTENTE,
	STATE_DEM_AFFICHAGE_INSTRUCTION,
	STATE_ATTENTE_AFFICHAGE_INSTRUCTION,
	STATE_FIN,
	STATE_DEM_FERMETURE_INSTRUCTION
	
 };

 enum Resultat
	{
		RES_UNKNOWN,
		RES_FAILED,
		RES_OK,
		RES_TIMEOUT,
		RES_ERROR
	};

 

 void handler(void);
 void setState(State);

	//Constructeurs

	//WorkorderRoutingTask(WORKORDERROUTING_TYPE, SRMA*, WorkorderRouting*);
 //WorkorderRoutingTask(WORKORDERROUTING_TYPE, pilotage::Pilotage *);
	//WorkorderRoutingTask(WorkorderRouting *, Robot *, TabletteSM *, SRMA *);
	HelpTask();
	HelpTask(Location*);
	CppEvent1<bool,int> evtTaskEnded;
	ArFunctor3C<HelpTask, char **, int, ArSocket *> myHandleGotoGoalReceivedCB;
	
	
	void Start();
	//void Start(WorkorderRouting *, SRMA *);
	void Start(SRMA *, A_ArNetServer *, A_ArNetServer *);
	void Start(Location *, SRMA *, A_ArNetServer *, A_ArNetServer *);
	void Start(Workorder *, SRMA *, A_ArNetServer *, A_ArNetServer *);
	void Stop();
	Resultat getResultat(void);

protected:
	State myState;
	Resultat myResult;
	bool myNewState;
	int cptEchec;
	Workorder *workOrder;

	void start();
	void handleGotoGoalReceived(char **, int, ArSocket *);

	ArTime myStateStartTime;
	ArTime myCycleStartTime;
	//Premier endroit ou se rendre
	Location *firstLocation;
	//Endroit on l'on est
	Location location;
	//Prochain endroit ou se rendre
	Location nextLocation;
	
	char nextLocationName[32];
	char firstLocationName[32];
	char locationName[32];

	
	int retCode;
	//int retCodeIdentification;

	bool fin;

	SRMA *srma;
	A_ArNetServer* server;
	A_ArNetServer* serverEvt;

	Robot* robot;
	TabletteSM* tabletteSM;


	ArCondition condition;
	ArMutex mutex;
	char bufRequestIhm[256];

	//Tache asynchrone
	virtual void* runThread(void*);

};
#endif