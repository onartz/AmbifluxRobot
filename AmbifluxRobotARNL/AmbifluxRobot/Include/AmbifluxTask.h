#ifndef DEF_AMBIFLUXTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_AMBIFLUXTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "WorkOrder.h"
#include "WorkOrderRouting.h"
//#include "aria.h"
//#include "AmbiantCommander.h"
//#include "AmbiantReagir.h"
#include "Tablette.h"
#include "TraiterWorkOrderTask.h"
#include "TraiterRequeteSRMATask.h"
#include "AriaUtil.h"


class Srma;

class AmbifluxTask : public ArASyncTask
{
public:	
	AmbifluxTask(void);
	AmbifluxTask(SRMA *);
	void Start();
	void Stop();	
	virtual void* runThread(void*);
	void setDemandeStopModeAmbiant(bool);
	void LockMutex();
	void UnLockMutex();
	TabletteSM *tablette;


	//WorkOrder *getCurrentWorkOrder();
	//WorkOrderRouting *getCurrentWorkOrderRouting();

protected:
	//Poiteur sur Srma
	SRMA *p_Srma;
	ArCondition condition;
	ArMutex mutex;
	//Demande de stop mode Ambiant (venant de Tablette)
	bool demandeStopModeAmbiant;
	int retCode;
	WorkOrder *p_CurrentWorkOrder;
	WorkOrderRouting currentWorkOrderRouting;
	std::vector<WorkOrder*> *p_ListeWorkOrdersATraiter;

	//AmbiantCommander ambiantCommander;
	//AmbiantReagir ambiantReagir;

	void stop(void);
	//std::vector<WorkOrder*> getListeWorkOrders();
	//Strategie de choix du WorkOrder à traiter
	//WorkOrder* choixWorkOrder(std::vector<WorkOrder*> listeWorkOrders);

};
#endif