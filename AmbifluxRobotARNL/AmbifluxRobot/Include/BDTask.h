#ifndef DEF_BDTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_BDTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArMutex.h"
#include "CppEvent.h"
#include "DAL.h"
#include "DALRest.h"
#include "Workorder.h"
#include "AriaUtil.h"

#define BASE_DE_TEMPS_ECRITURE_BD 60

class BDTask : public ArASyncTask
{
public:
	//Constructeur
	BDTask();
	~BDTask();
	void Start();
	void Stop();

	//Lorsuq'un nouveau workorder a �t� trouv�
	CppEvent1<bool,Workorder*> evtNewWorkorderFound;
	//Lorsuq'un workorder a �t� cloture ou annule
	//CppEvent1<bool,Workorder*> evtWorkorderClosedOrCancelled;

	void LockMutex();
	void UnlockMutex();

protected:
	//ArCondition myCondition;
	ArMutex myMutex;
	virtual void* runThread(void*) ; 
};

#endif