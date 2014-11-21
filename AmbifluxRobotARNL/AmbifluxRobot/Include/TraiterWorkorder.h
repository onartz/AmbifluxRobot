#ifndef DEF_TRAITERWORKORDER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TRAITERWORKORDER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus


//#include "Arnl.h"
//#include "ArDocking.h"
//Ne pas enlever
#include "Winsock2.h"
#include "Workorder.h"
//#include "IhmCommunicationThread.h"
//#include "DALRest.h"
//#include "TraiterWorkorderRouting.h"
#include "Srma.h"


class TraiterWorkorder
{

//Différents états
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
	//TraiterWorkorder();
	TraiterWorkorder(Workorder &, SRMA &);
	
	//TraiterWorkorder(ArServerModeGoto &modeGoto, ArServerModeDock *modeDock, Workorder &, IhmCommunicationThread &, SRMA &);
	//void Start(ArServerModeGoto *modeGoto, ArServerModeDock *modeDock, Workorder *, IhmCommunicationThread *);

private:

	SRMA &mySrma;

	State myState;
	bool myNewState;
	ArTime myStateStartTime;
	ArTime myCycleStartTime;

	//LecteurCarte *lecteurCarte;

	//TraiterWorkorderRouting traiterWorkorderRouting.
	//Current Workorder, une référence sur un workorder
	Workorder& myWorkorder;
	//Active workorderRouting
	WorkorderRouting* myWorkorderRouting;
	//WorkorderList
	std::list<WorkorderRouting> worList;

	char bufRequestIhm[256];

	int retCode;
	int indexWor;

	//Cycling
	bool myRunning;
	//Setup before start
	void setup();
	//Start cycle
	void start();
	//Stop cycle
	void stop();
};

#endif