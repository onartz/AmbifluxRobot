#ifndef DEF_ATTENTEDEMANDETASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_ATTENTEDEMANDETASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "WorkOrder.h"
#include "ArAsyncTask.h"
#include "AriaUtil.h"
#include "DAL.h"
#include "CPPEvent.h"


//class Srma;

class AttenteDemandeTask : public ArASyncTask
{
public:	
	AttenteDemandeTask(void);
	//AttenteDemandeTask(SRMA *);
	~AttenteDemandeTask();

	void Start();
	void Stop();	
	virtual void* runThread(void*);

	void LockMutex();
	void UnLockMutex();

	//Evenements g�n�r�s
	//Lorsuq'un nouveau workorder a �t� trouv�
	CppEvent1<bool,WorkOrder*> evtNewWorkOrder;
	CppEvent1<bool,int> evtTest;

protected:
	//Poiteur sur Srma
	//SRMA *p_Srma;
	//ArCondition condition;
	ArMutex mutex;
	//Demande de stop mode Ambiant (venant de Tablette)
	//bool demandeStopModeAmbiant;
	int retCode;
	//WorkOrder *p_CurrentWorkOrder;
	//WorkOrderRouting currentWorkOrderRouting;
	std::vector<WorkOrder*> *p_ListeWorkOrdersATraiter;

	//AmbiantCommander ambiantCommander;
	//AmbiantReagir ambiantReagir;

	void stop(void);
	//std::vector<WorkOrder*> getListeWorkOrders();
	//Strategie de choix du WorkOrder � traiter
	//WorkOrder* choixWorkOrder(std::vector<WorkOrder*> listeWorkOrders);

};
#endif