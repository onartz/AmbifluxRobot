#ifndef DEF_TRAITERREQUETESRMATASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TRAITERREQUETESRMATASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

#include "WorkOrder.h"
#include "DAL.h"
#include "WorkOrderRoutingTask.h"
#include "Robot.h"
#include "tablette2.h"


class SRMA;


class TraiterRequeteSRMATask : public ArASyncTask
{
public:
	//Constructeurs
	//TraiterWorkOrderTask();
	TraiterRequeteSRMATask(SRMA*, WorkOrder*);
	void Start();
	void Stop();
	void demanderStop();
	//Accesseurs
protected:
	//Workorder en cours de traitement
	WorkOrder* workOrder;
	//WorkOrderRouting en cours de traitement
	WorkOrderRouting* workOrderRouting;
	//pointeur vers SRMA
	SRMA* srma;
	TabletteSM *tablette;
	Robot *robot;

	//Entrées
	bool demandeStop;
	ArCondition condition;
	ArMutex mutex;

	//Tache asynchrone
	virtual void* runThread(void*); 
	void traiterRequeteSRMA(void);
	int traiterDemandeAide(void);
	int traiterDemandeAidePersonnel(void);
	
};
#endif