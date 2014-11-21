#include "TraiterWorkorderTask.h"
//#include "Srma.h"
//#include "Pilotage.h"


TraiterWorkorderTask::TraiterWorkorderTask(ArServerModeGoto *modeGoto, ArServerModeDock *modeDock, WorkOrder *pWorkorder, IhmCommunicationThread *pIhm):
workOrder(NULL),
workorderRouting(NULL),
myNewState(true),
myMutex(),
fin(false),
myState(STATE_START),
//server(NULL),
//serverEvt(NULL),
myModeGoto(modeGoto),
myModeDock(modeDock),
myIhm(pIhm)
{}


void TraiterWorkorderTask::setState(State state)
{
	retCode=0;
	myState=state;
	myNewState=true;
	myStateStartTime.setToNow();
}


/*void TraiterWorkorderTask::Start(WorkOrder *pWorkOrder, pilotage::Pilotage *pPilotage, A_ArNetServer *pServer,A_ArNetServer *pServerEvt)
{
	fin=false;
	setState(STATE_START);
	workOrder=pWorkOrder;
	worList=workOrder->getWorkOrderRoutingList();
}*/

void TraiterWorkorderTask::Stop()
{
	ArASyncTask::stopRunning();
	while(this->getRunningWithLock())
		ArUtil::sleep(50);

	printf("Thread TraiterWorkorderTask stopped\n");
}


void* TraiterWorkorderTask::runThread(void*) 
  {

	printf("Thread TraiterWorkorderTask %s running\n",workOrder->getWorkOrderNo());
	while(this->getRunningWithLock())
	{
		if(fin == true)
			break;
		myMutex.lock();
		handler();
		myMutex.unlock();
		ArUtil::sleep(100);
	}
	printf("Thread TraiterWorkorderTask %s ended\n",workOrder->getWorkOrderNo());
	ArASyncTask::stopRunning();
	return NULL;
}

void TraiterWorkorderTask::handler()
{	
	bool found;
	int nbWorkOrderRoutings(0);

	switch(myState)
	{
		case TraiterWorkorderTask::STATE_START:
			if(myNewState)
			{
				printf("Entering TraiterWorkorderTask::STATE_START\n");
				myNewState=false;
				workOrder->start();
				
				/* Todo : 
				  envoyer Autodock Disable
				  update workorkorder
				  */

				//DAL::updateWorkOrder(*workOrder);
				//DALRest::updateWorkOrder(*workOrder);
				//robot->SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE"));
				
				//robot->SetAutodock(false);
				setState(STATE_LECTURE_WORS);
			}
			break;

		case TraiterWorkorderTask::STATE_LECTURE_WORS:
			if(myNewState)
			{
				found = false;
				myNewState=false;
			}
			//nbWorkOrderRoutings=int(worList.size());
			printf("Entering TraiterWorkorderTask::STATE_LECTURE_WORS\n");

			for(std::list<WorkOrderRouting*>::const_iterator it=worList.begin();it!=worList.end();++it)
			{
				if((*it)->getStatusId()==WorkOrderRouting::NEW)
				{
					workorderRouting=(*it);
					//setState(STATE_TRAITEMENT_WOR);
					found=true;
					break;
				}
			}
			if(found == true)
				setState(STATE_TRAITEMENT_WOR);
			else
				setState(STATE_FIN_WO);
			break;

		case TraiterWorkorderTask::STATE_TRAITEMENT_WOR:
			if(myNewState)
			{
				printf("Entering TraiterWorkorderTask::STATE_TRAITEMENT_WOR\n");
				
				myNewState = false;
				printf("Debut traitement WorkOrderRoutingNo %s\n",workorderRouting->getWorkOrderRoutingNo());
				/*Todo : demarrer worTask*/

				//worTask.Start(workorderRouting,srma,server,serverEvt);
				//myPilotage->setCurrentWorkOrderRouting(workorderRouting);
			}
			//Todo : Attente
			/*if(worTask.getRunningWithLock())
				break;*/

			//Fin attente
			//Annulé
			if(workorderRouting->isCancelled())
			{
				workOrder->cancel();
				/*Todo : mettre à jour BD */
				//DAL::updateWorkOrder(*workOrder);
				//Prevenir la tabletteSM
				sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",workOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);

				//Todo : Mise à jour du state du SRMA
				
				/*if(srma->IsLoaded())
					srma->setStateId(SRMA::FAULT);
				else
					srma->setStateId(SRMA::READY);*/
				setState(STATE_CANCELLED);
			}
			//Closed
			else if(workorderRouting->isClosed())
			{
				setState(STATE_LECTURE_WORS);
			}
			//En cours
			else					
				break;
			break;
			
			

		case TraiterWorkorderTask::STATE_CANCELLED:
			if(myNewState)
			{
				printf("Entering TraiterWorkorderTask::STATE_FIN_WOR\n");
				myNewState=false;
			}
			
				//Notification de la fin de tache
			fin = true;
			//evtTaskEnded.notify(0);

			break;
		//Fin normale
		case TraiterWorkorderTask::STATE_FIN_WO:
			if(myNewState)
			{
				printf("Entering TraiterWorkorderTask::STATE_FIN_WO\n");
				myNewState=false;
			}
			//Tous les WorkOrderRoutings ont été réalisés

			//RAZ workorderRouting et currentWorkOrder
			workOrder->close();
			/*Todo mettre à jour BD */
			//DAL::updateWorkOrder(*workOrder);
			sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",workOrder->getWorkOrderNo(),(int)WorkOrder::CLOSE);	
			
			/*Todo : ihm.send */
			//this->myI
			
			//serverEvt->sendToAllClients(bufETCPServerEvt);

			//On remet l autodock
			/*Todo : envpyer autodock */
			myModeDock->setAutoDock(true);
			//robot->SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"ENABLE"));
			//robot->SetAutodock(true);
			//Mise à jour du state du SRMA

			/*Todo mettre à jour */
			//srma->setStateId(SRMA::READY);
			
			//On reset l attribut currentWorkOrder de SRMA
			//pilotage->setCurrentWorkOrder(NULL);
			//currentWorkOrder=NULL;
			//pilotage->setCurrentWorkOrderRouting(NULL);
			//workorderRouting=NULL;

			//Notification de la fin de tache
			//evtTaskEnded.notify(0);
			fin=true;
			break;
		}
	}


