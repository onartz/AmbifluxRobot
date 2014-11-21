#ifndef DEF_PUBLISHSRMATASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_PUBLISHSRMATASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

//#include "Aria.h"
//#include "DAL.h"
//#include "tcpServerTask.h"
#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

class SRMA;

#define BASE_DE_TEMPS_ECRITURE_BD 60

class PublishSRMATask : public ArASyncTask
{
public:
	//Constructeur
	PublishSRMATask(SRMA *);
	void Start();
	void Stop();

protected:
	ArCondition myCondition;
	ArMutex myMutex;
	SRMA *mySrma;
	
	//Tache asynchrone
	virtual void* runThread(void*) ; 
};

#endif