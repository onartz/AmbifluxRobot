#ifndef DEF_AMBIANTCOMMANDER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_AMBIANTCOMMANDER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
#include "WorkOrder.h"
#include "Tablette.h"
#include "AriaUtil.h"
#include "CppEvent.h"

#include "DAL.h"
#include "Robot.h"
#include "AmbifluxTask.h"


class SRMA;
//class AmbifluxTask;

class AmbiantCommander : public ArASyncTask
{
public:
	//Codes des etapes en cours
enum COMMANDER_ETAPES_MODE
{
	INIT_GENERALE=1,
	ATTENTE_MODE,
	DEMARRAGE_MODE_AMBIANT,
	MODE_AMBIANT,
	ARRET_MODE_AMBIANT,
	DEMARRAGE_MODE_TP,
	MODE_TP
};

	//Constructeurs
	AmbiantCommander(SRMA*);
	void Start();
	void Stop();
	void demanderStop();
	//Accesseurs
	SRMA* getSRMA();

	//Handle des évènements en provenance de la tablette
	bool handleTabletteRequestMode(int);
	bool handleTabletteRequestQuitMode(int);


	//void toto();
	void LockMutex();
	void UnlockMutex();
//Accesseurs
	//Set
	void setDemandeModeAmbiant(bool);
	void setDemandeStopModeAmbiant(bool);
	//Demande d'activation du mode Ambiant
	int ActiverModeAmbiant(void);
	//Demande d'activation du mode nonoe
	int ActiverModeNone(void);
	//Demande d'activation du mode TP
	int ActiverModeTP(void);
	//Demande de quitter le mode Ambiant
	int QuitterModeAmbiant(void);
	//Demande de quitter le mode TP
	int QuitterModeTP(void);
protected:	
	//Arret propre du thread
	void stop();
	//Tache d'execution du mode Ambiant
	AmbifluxTask ambifluxTask;

	ArCondition condition;
	ArMutex mutex;
	SRMA *srma;
	TabletteSM *tablette;
	int etapeMode;

	//Entrées
	//Demande mode ambiant (de la tablette)
	bool demandeModeAmbiant;
	bool demandeModeTP;
	//Demande arret mode  ambiant
	bool demandeStopModeAmbiant;
	bool demandeStopModeTP;

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif