#ifndef DEF_WORKORDERROUTINGTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_WORKORDERROUTINGTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
#include "strutil.h"
//#include "Aria.h"
#include "ArNetworking.h"
//#include "Arnl.h"
//#include "ArLocalizationTask.h"
//#include "ArDocking.h"
#include "ArServerMode.h"
#include "ArDocking.h"
//#include "ArAsyncTask.h"
//#include "ArCondition.h"
//#include "ArMutex.h"
#include "IhmCommunicationThread.h"

#include "WorkOrderRouting.h"
#include "Location.h"
//#include "DAL.h"
//#include "Robot.h"
//#include "Tablette.h"
//#include "SRMA.h"
#include "LecteurCarte.h"
#include "Resource.h"
#include "IdentificationSM.h"
#include "ArServerClasses.h"
//#include "A_ArNetServer.h"
//#include "ClientTCP.h"

//

//class SRMA;

#define TIMEOUT_DUREE_CHARGEMENT 300
#define TIMEOUT_DUREE_LIVRAISON 300

class WorkOrderRoutingTask : public ArASyncTask
{
public:

//Différents états
 enum State { 
	 STATE_TRANSPORT_START,
	 STATE_TRANSPORT_DEPLACEMENT,
	 STATE_TRANSPORT_FIN,
	 STATE_CHARGEMENT_START,
	 STATE_CHARGEMENT_IDENTIFICATION,
	 STATE_DEM_AFFICHAGE_CHARGEMENT,
	 STATE_ATTENTE_FERMETURE_CHARGEMENT,
	 STATE_DEM_FERMETURE_CHARGEMENT,
	 STATE_ATTENTE_CHARGEMENT_TERMINE,
	 STATE_CHARGEMENT_FIN,
	 STATE_LIVRAISON_START,
	 STATE_DEM_AFFICHAGE_LIVRAISON,
	 STATE_DEM_FERMETURE_LIVRAISON,
	 STATE_ATTENTE_FERMETURE_LIVRAISON,
	 STATE_ATTENTE_LIVRAISON_TERMINE,
	 STATE_LIVRAISON_FIN,
	 STATE_DECHARGEMENT_START,
	 STATE_TRANSFERTA_START,
	 STATE_TRANSFERTB_START
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

	//WorkOrderRoutingTask(WORKORDERROUTING_TYPE, SRMA*, WorkOrderRouting*);
 //WorkOrderRoutingTask(WORKORDERROUTING_TYPE, pilotage::Pilotage *);
	//WorkOrderRoutingTask(WorkOrderRouting *, Robot *, TabletteSM *, SRMA *);
	WorkOrderRoutingTask();
	WorkOrderRoutingTask(ArServerModeGoto *, ArServerModeDock *, WorkOrderRouting *, LecteurCarte *, IhmCommunicationThread *);

	
	
	//void Start();
	//void Start(WorkOrderRouting *, SRMA *);
	//void Start(WorkOrderRouting *, SRMA *, A_ArNetServer *, A_ArNetServer *);
	void Stop();
	Resultat getResultat(void);

	//Accesseurs Get
	//WorkOrderRouting::WORKORDERROUTING_TYPE getType();

protected:
	ArServerModeGoto *myModeGoto;
	ArServerModeDock *myModeDock;
	State myState;
	Resultat myResult;
	bool myNewState;
	int cptEchec;
	//ArSocket *serverSocket;
	//ClientTCP *clientTCP;

	void start();

	//Statemachine pour l'identification
	IdentificationSM identificationSM;

	Resource operateur;;

	ArTime myStateStartTime;
	ArTime myCycleStartTime;
	//ArTime myStateStartTimeIdentification;

	Location *nextLocation;
	char *nextLocationName;
	
	int retCode;
	//int retCodeIdentification;

	bool fin;

	//SRMA *srma;
	//A_ArNetServer* server;
	//A_ArNetServer* serverEvt;

	//Robot* robot;
	WorkOrderRouting* workorderRouting;
	//TabletteSM* tabletteSM;
	LecteurCarte *myLecteurCarte;

	WorkOrderRouting::WORKORDERROUTING_TYPE type;


	ArCondition condition;
	ArMutex mutex;
	char bufETCPServerEvt[256];

	//Tache asynchrone
	virtual void* runThread(void*);
	bool isStateFailedToGetTo(std::string);
	bool isStateFailedToGetTo(char *);
	bool isStateArrivedAt(char *);

	/*void traiterWorkOrderRoutingTransport();
	void traiterWorkOrderRoutingChargement();
	void traiterWorkOrderRoutingDechargement();
	void traiterWorkOrderRoutingLivraison();
	void traiterHelpTask();
	void traiterWorkOrderRoutingTransfertB();*/
	//int identifierParCarte(Resource &);	
};
#endif