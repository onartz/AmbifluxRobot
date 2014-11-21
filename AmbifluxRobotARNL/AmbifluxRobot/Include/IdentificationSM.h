#ifndef DEF_IDENTIFICATIONSM_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_IDENTIFICATIONSM_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "LecteurCarte.h"
#include "Resource.h"
#include "Srma.h"
#include "DALRest.h"
#include "IhmCommunicationThread.h"

class IdentificationSM
{
public:
	enum State {			
		 STATE_DEM_AFFICHAGE_IDENTIFICATION,			//Affichage Ecran Ident
		 STATE_DEM_FERMETURE_IDENTIFICATION,
		 STATE_ATTENTE_CARTE,					//Attente carte
		 STATE_IDENTIFICATION,					//Identification BD
		 STATE_DEM_AFFICHAGE_IDENTIFICATIONFAILED,
		 STATE_DEM_FERMETURE_IDENTIFICATIONFAILED,
		 STATE_ATTENTE_ACQUITTEMENT,						//Identification OK
		 STATE_FIN
		};

	enum Resultat
	{
		RES_UNKNOWN,
		RES_IDENTIFICATION_FAILED,
		RES_IDENTIFICATION_OK,
		RES_TIMEOUT,
		RES_ERROR
	};

	//Constructeur
	//IdentificationSM();
	IdentificationSM(SRMA &, IhmCommunicationThread &);
	~IdentificationSM(void);
	
	
	 //Accesseurs
	 void setState(State);
	 void start();
	
	 State getState(void);
	 Resultat getResultat(void);
	 Resource *getOperateur(void);

private:
	void handler(void);
	IhmCommunicationThread &myIhm;
	State myState;
	Resultat myResult;
	//Permet de gérer les fm sur états
	bool myNewState;
	int cptTentativeIdentification;
	int cptEchec;
	bool myRunning;
	char bufRequestIhm[256];
	BYTE bufCardId[5];
	char strCardId[64];
	//Operateur identifié
	Resource operateur;
	//Pour gérer les Timeouts sur états
	ArTime myStateStartTime;

	SRMA &mySrma;
	LecteurCarte lecteurCarte;

	
};

#endif