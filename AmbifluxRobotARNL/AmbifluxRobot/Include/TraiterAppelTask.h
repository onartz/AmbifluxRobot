#ifndef DEF_TRAITERAPPELTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_TRAITERAPPELTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
#include "WorkOrder.h"
#include "DAL.h"
#include "WorkOrderRoutingTask.h"
#include "RobotSM.h"
#include "Tablette.h"
#include "CppEvent.h"
//#include "Pilotage.h"




class SRMA;
namespace pilotage{
struct Pilotage;
};


class TraiterAppelTask : public ArASyncTask
{
public:
	//Constructeurs
	TraiterAppelTask();
	//TraiterAppelTask(SRMA*, WorkOrder*);
	TraiterAppelTask(pilotage::Pilotage*);
	void Start();
	void Start(WorkOrder*);
	void Stop();

	CppEvent1<bool,int> evtTaskEnded;
	//void demanderStop();
	//Accesseurs
protected:
	//Workorder en cours de traitement
	WorkOrder *workorder;
	//WorkOrderRouting en cours de traitement
	WorkOrderRouting *workorderRouting;

	pilotage::Pilotage *pilotage;
	//pointeur vers SRMA
	/*SRMA* srma;
	TabletteSM *tablette;
	robotStateMachine::RobotSM *robot;*/

	//Entr�es
	//bool demandeStop;
	ArCondition condition;
	ArMutex mutex;

	//Tache asynchrone
	virtual void* runThread(void*); 
	
};
#endif