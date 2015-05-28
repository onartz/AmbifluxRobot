#ifndef DEF_TRAITERWORKORDERROUTING_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TRAITERWORKORDERROUTING_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

//#include "Arnl.h"
//#include "ArDocking.h"

//#include "Winsock2.h"
#include "Srma.h"
#include "Identification.h"
#include "MessagePool.h"
#include "Globals.h"

#define TIMEOUT_DUREE_CHARGEMENT 1000
#define TIMEOUT_DUREE_LIVRAISON 1000


class TraiterWorkorderRouting
{
public:
		//Constructeurs
	 TraiterWorkorderRouting(SRMA &, WorkorderRouting &, Pool<TCPReceivedRequest>*);
	//~TraiterWorkorderRouting();// destructeur 


//Différents états
 enum State { 
	 STATE_TRANSPORT_START,
	 STATE_TRANSPORT_PREPARATION,
	 STATE_TRANSPORT_DEPLACEMENT,
	 STATE_TRANSPORT_FIN,
	 STATE_CHARGEMENT_START,
	 STATE_CHARGEMENT_IDENTIFICATION, //Ouverture Form et attente fin identification par lecteur
	 STATE_CHARGEMENT_FIN_IDENTIFICATION, //Fermeture Form
	 STATE_CHARGEMENT_NOIHM_IDENTIFICATION,
	 STATE_ATTENTE_FIN_CHARGEMENT_NOIHM,
	 STATE_DEM_AFFICHAGE_CHARGEMENT,
	 STATE_ATTENTE_FERMETURE_CHARGEMENT,
	 STATE_DEM_FERMETURE_CHARGEMENT,
	 STATE_ATTENTE_CHARGEMENT_TERMINE,
	 STATE_CHARGEMENT_FIN,
	 STATE_LIVRAISON_START,
	 STATE_ATTENTE_FIN_LIVRAISON_NOIHM,
	 STATE_LIVRAISON_NOIHM_IDENTIFICATION,
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


	Resultat getResultat(void);

protected:
	SRMA &mySrma;
	//Pour communiquer avec la tablette
	IhmCommunicationThread &myIhm;
	//Pointeur vers messages venant de la tablette;
	Pool<TCPReceivedRequest> * myMessagePool;
	//Pour connaitre le status et mode du robot
	OutputHandler &myOutputHandler;
	//WorkorderRouting a traiter
	WorkorderRouting & myWorkorderRouting;

	//Etat courant
	State myState;

	Resultat myResult;
	bool myNewState;
	int cptEchec;
	

	//Todo remplacer par ressource pour etre generique qqsoit le type de WorrkorderRuoting
	Person * myOperateur;

	ArTime myStateStartTime;
	ArTime myCycleStartTime;

	//Toutes ces variables sont obligatoirement ici à cause du handler()
	//Location nextLocation;
	char nextLocationName[64];
	
	int retCode;

//	WorkorderRouting::WORKORDERROUTING_TYPE myType;

	char bufRequestIhm[256];
	//Todo migrer ces fonctions ailleurs (OutputHandler?
	/*bool isStateFailedToGetTo(std::string);
	bool isStateFailedToGetTo(char*);
	bool isStateArrivedAt(char * nextLocationName);
	bool isStateArrivedAt(std::string);*/
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