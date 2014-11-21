#include "AmbifluxTask.h"
//Forward declaration
#include "Srma.h"

//Constructeur
AmbifluxTask::AmbifluxTask(SRMA *pSrma):p_Srma(pSrma)
{
	tablette=pSrma->getTablette();
}

void AmbifluxTask::setDemandeStopModeAmbiant(bool value){demandeStopModeAmbiant=value;}
void AmbifluxTask::LockMutex(){myMutex.lock();}
void AmbifluxTask::UnLockMutex(){myMutex.unlock();}

/************************************************************************/
/* stop :tentative d arret du Thread									*/
/************************************************************************/
void AmbifluxTask::stop()
{
		this->stopRunning();
		int i(0);
		//Attente arrêt Thread
		do
		{
			ArUtil::sleep(250);
			printf("Tentative arret Mode Ambiant : %d\n",i++);
		} 
		while(this->getRunning());
}


void AmbifluxTask::Start()
{
	printf("Thread AmbifluxAsyncTask started\n");	
	ArUtil::sleep(200);
	this->runAsync();  
}

/************************************************************************/
/* Stop : demande d'arret du mode Ambiant								*/
/************************************************************************/
void AmbifluxTask::Stop()
{		
	stop();
	demandeStopModeAmbiant=false;
}

//Tache asynchrone
void* AmbifluxTask::runThread(void*) 
{
	char bufETCPServerEvt[256];
	
	printf("Thread AmbifluxTask running\n");
	int retCode(0);
	
	printf("Attente ordre\n");
	// Run until the thread is requested to end by another thread.
	while(this->getRunning())
	{
		//Attention acces partagé
		p_Srma->setTaskId(SRMA::STEP_ATTENTE_ORDRE);
		//On peuple la liste avec les nouveaux WorkOrders
		p_ListeWorkOrdersATraiter=DAL::getWorkOrders(WorkOrder::NEW);
		//S'il en existe, il faut choisir le premier et le mettre en courant
		while(p_ListeWorkOrdersATraiter->size()>0 && !demandeStopModeAmbiant)
		{
			//On prend le premier
			p_CurrentWorkOrder=(*p_ListeWorkOrdersATraiter)[0];

			//Traitement de requete de SRMA (CALL)
			if(strcmp(p_CurrentWorkOrder->getWorkOrderType(),"CAL")==0)
			{
				//ModifON
				//p_Srma->TraiterRequeteSRMA(*p_CurrentWorkOrder);
				TraiterRequeteSRMATask woTask(p_Srma,p_CurrentWorkOrder);
				woTask.Start();
				while(!(p_CurrentWorkOrder->isCancelled()||p_CurrentWorkOrder->isClosed()))
					ArUtil::sleep(100);
			}
			else if(strcmp(p_CurrentWorkOrder->getWorkOrderType(),"LIV")==0)
			{
				TraiterWorkOrderTask woTask(p_Srma,p_CurrentWorkOrder);
				woTask.Start();
				while(!(p_CurrentWorkOrder->isCancelled()||p_CurrentWorkOrder->isClosed()))
					ArUtil::sleep(100);
				//p_Srma->TraiterWorkOrder(*lCurrentWorkOrder);
			}
			
			//On supprime le workOrder de la liste
			p_ListeWorkOrdersATraiter->erase(p_ListeWorkOrdersATraiter->begin());
			//Si le SRMA est en erreur (chargé dans un coin)
			//Il faut faire qque chose!
			if(p_Srma->getStateId()==Resource::FAULT)
			{
				printf("SRMA chargé dans un coin!\n");
				retCode=p_Srma->TraiterDemandeAide();
			}
		}	
		if(demandeStopModeAmbiant)
			stop();
	}
	
	return NULL;
}
