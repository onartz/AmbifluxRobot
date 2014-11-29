#ifndef DEF_SRMA_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_SRMA_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

//Ne pas enlever!!!!
#include "Winsock2.h"
///////////////////
#include "ArClientBase.h"
#include "Resource.h"
#include "Person.h"
#include "ArSoundPlayer.h"
#include "OutputHandler.h"
#include "ArSoundsQueue.h"
#include "ArSoundPlayer.h"

#include "IhmCommunicationThread.h"
//#include "Workorder.h"
#include "StrUtil.h"
#include "CommandeRobot.h"
#include "DALRest.h"
//#include "Identification.h"
//#include "TraiterWorkorderRouting.h"

#define FILE_BUTTON_PRESSED "c:\\temp\\buttonPressed.wav"
#define FILE_MSG_ARRIVED "c:\\temp\\Electrical_Sweep.wav"
#define FILE_BLIP "c:\\temp\\blip.wav"
#define FILE_BELL "c:\\temp\\indianbell.wav"

//SRMA hérite de Resource
class SRMA : public Resource
{
public:
	enum GOTOGOAL_STATUS 
	{
		RET_INTERRUPTED = -1,
		RET_ARRIVED_AT = 0,
		RET_FAILED_TO_GET_TO = 1
	};

	enum SOUNDS
	{
		MSG_ARRIVED,
		BUTTON_PRESSED,
		BLIP,
		BELL
	};

	//Avec JSON
	//Constructeur
	//SRMA(string, ArClientBase &, OutputHandler &, IhmCommunicationThread & );
	SRMA(string, ArClientBase &, OutputHandler &, IhmCommunicationThread &, ArSoundsQueue * );

	void gotoGoal(const char * goal);
	void play(char*);
	void play(SOUNDS);
	//GOTOGOAL_STATUS gotoGoal(string goal);

	//static int gotoGoal(ArClientBase &, OutputHandler &, char *);
	
	void SendCommand(CommandeRobot);

	//GOTOGOAL_STATUS gotoGoal(char *);

	//void TraiterWorkorder(Workorder &);

	IhmCommunicationThread & getIhmCommunicationThread(void);
	OutputHandler & getOutputHandler(void);
	//Méthodes publiques

//Codes des etapes en cours
/*enum ETAPES_MODE
{
	INIT_GENERALE=1,
	ATTENTE_MODE,
	DEMARRAGE_MODE_AMBIANT,
	MODE_AMBIANT,
	ARRET_MODE_AMBIANT,
	DEMARRAGE_MODE_TP,
	MODE_TP
};*/


/*enum SRMA_TASK
    {
        STEP_UNKNOWN=-1,
        STEP_INIT,
        STEP_IDENTIFICATION_PERSONNEL,
		STEP_CHARGEMENT_MAGASIN,
		STEP_CHARGEMENT_CLIENT,
		STEP_DECHARGEMENT_MAGASIN,
		STEP_LIVRAISON_CLIENT,
		STEP_DEPLACEMENT_MAGASIN,
		STEP_DEPLACEMENT_LIVRAISON,
		STEP_DEPLACEMENT,
		STEP_ATTENTE_ORDRE,
		STEP_INACTIF
    };*/

//Load Status
enum LOAD
{
	LOADED,
	UNLOADED
};

	void setLoad(LOAD);
	SRMA::LOAD getLoad();
	bool isStateGoingTo(std::string);
	bool isStateGoingTo(const char*);
	bool isStateFailedToGetTo(std::string);
	bool isStateFailedToGetTo(const char *);
	bool isStateArrivedAt(const char *);
	ArSoundsQueue* getSoundsQueue();

	void setCallbackEndGotoGoal(ArFunctor1<char*>*);

protected:
	//Load status
	LOAD myLoad;
	//Client robot
	ArClientBase& myClient;
	//Ihm
	IhmCommunicationThread &myIhm;
	//SoundQueue
	ArSoundsQueue *mySoundsQueue;
	//Callback du robot
	OutputHandler &myOutputHandler;
	//Callback fin de déplacement
	ArFunctor1<char*>* myEndGotoGoalFunctor;


};
#endif