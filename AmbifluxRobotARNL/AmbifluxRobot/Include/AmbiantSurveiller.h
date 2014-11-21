#ifndef DEF_AMBIANTSURVEILLER_H //Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_AMBIANTSURVEILLER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
//#include "SRMA.h"

//#include "AmbiantReagir.h"

class SRMA;
class AmbiantReagir;

class AmbiantSurveiller : public ArASyncTask
{
public:
	//Constructeurs
	AmbiantSurveiller();
	AmbiantSurveiller(SRMA*,AmbiantReagir*);
	//AmbiantSurveiller(SRMA*, AmbiantCommander*);
	void Start();
	void Stop();
	void LockMutex();
	void UnlockMutex();

	//Accesseurs
protected:
	
	ArCondition condition;
	ArMutex myMutex;
	SRMA *srma;
	AmbiantReagir* ambiantReagir;
	//Erreur
	bool error;
	//Code erreur
	int errorCode;
	//Erreur tablette
	bool tabletteError;
	//Code erreur
	int tabletteErrorCode;
	//Erreur lecteur
	bool lecteurError;
	//Code erreur
	int lecteurErrorCode;
	//Erreur robot
	bool robotError;
	//Code erreur
	int robotErrorCode;

	//AmbiantCommander *ambiantCommander;

	//Méthodes privées
	//void tabletteConnexionLost();

	void stop();

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif