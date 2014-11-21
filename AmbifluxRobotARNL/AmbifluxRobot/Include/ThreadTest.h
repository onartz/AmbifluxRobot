#ifndef DEF_THREADTEST_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_THREADTEST_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

class SRMA;
//class AmbifluxTask;

class ThreadTest : public ArASyncTask
{
public:
	//Codes des etapes en cours

	//Constructeurs
	ThreadTest(SRMA*);
	void Start();
	void Stop();
	void demanderStop();

	//void toto();
	void LockMutex();
	void UnlockMutex();
//Accesseurs
	//Set

protected:	

	ArCondition condition;
	ArMutex mutex;
	SRMA *p_Srma;

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif