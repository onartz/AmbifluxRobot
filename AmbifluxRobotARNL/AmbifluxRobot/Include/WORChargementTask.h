#ifndef DEF_WORCHARGEMENTTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_WORCHARGEMENTTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

#include "WorkOrderRouting.h"
#include "Location.h"
#include "DAL.h"
#include "Robot.h"
#include "tablette.h"

class SRMA;


class WORChargementTask : public ArASyncTask
{
public:
	//Constructeurs
	//WORChargementTask();
	WORChargementTask(SRMA*, WorkOrderRouting*);
	void Start();
	void Stop();
	//Accesseurs
protected:
	Robot* robot;
	WorkOrderRouting* workOrderRouting;
	SRMA* srma;
	Tablette* tablette;

	ArCondition condition;
	ArMutex mutex;

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif