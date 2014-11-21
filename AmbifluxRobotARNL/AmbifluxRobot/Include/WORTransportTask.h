#ifndef DEF_WORTRANSPORTTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_WORTRANSPORTTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

#include "WorkOrderRouting.h"
#include "Location.h"
#include "DAL.h"
#include "Robot.h"

class SRMA;


class WORTransportTask : public ArASyncTask
{
public:
	//Constructeurs
	//WORTransportTask();
	WORTransportTask(SRMA*, WorkOrderRouting*);
	void Start();
	void Stop();
	//Accesseurs
protected:
	Robot* robot;
	WorkOrderRouting* workOrderRouting;
	SRMA* srma;

	ArCondition condition;
	ArMutex mutex;

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif