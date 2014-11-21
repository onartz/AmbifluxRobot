#include "TraiterWorkorder.h"

//Constructeurs

TraiterWorkorder::TraiterWorkorder(Workorder &workorder, SRMA &srma):
myWorkorder(workorder),
myWorkorderRouting(NULL),
mySrma(srma)
{
	myNewState = true;
	myState = STATE_START;
	worList = workorder.getWorkorderRoutingList();
	//Start loop
	start();
}



//TraiterWorkorder::TraiterWorkorder(ArServerModeGoto &modeGoto, ArServerModeDock* modeDock, Workorder &workorder, IhmCommunicationThread &ihm, SRMA &srma):
//myModeGoto(modeGoto),
//myIhm(ihm),
//myModeDock(modeDock),
//myWorkorder(workorder),
//myWorkorderRouting(NULL),
//mySrma(srma)
//{
//	myNewState = true;
//	myState = STATE_START;
//	worList = workorder.getWorkorderRoutingList();
//	//Start loop
//	start();
//}
//
/*void TraiterWorkorder::Start(ArServerModeGoto *modeGoto, ArServerModeDock *modeDock, Workorder *workorder, IhmCommunicationThread *ihm)
{	
	myNewState = true;
	myFin = false;
	myState = STATE_START;
	myModeGoto = modeGoto;
	myModeDock = modeDock;
	myIhm = ihm;
	myWorkorder = workorder;

	printf("Workorder %s started\n",myWorkorder.getWorkorderNo());
	while(true)
	{
		if(myFin == true)
			break;
		handler();
		ArUtil::sleep(100);
	}
	printf("Workorder %s ended\n",myWorkorder.getWorkorderNo());
}*/

void TraiterWorkorder::start()
{
	myRunning = true;
	setState(STATE_START);
	ArLog::log(ArLog::Normal,"Workorder %s started\n",myWorkorder.getWorkorderNo());
	while(myRunning == true)
	{
		handler();		
		ArUtil::sleep(100);
	}
	ArLog::log(ArLog::Normal,"Workorder %s ended\n",myWorkorder.getWorkorderNo());

}

void TraiterWorkorder::setState(State state)
{
	retCode=0;
	myState=state;
	myNewState=true;
	myStateStartTime.setToNow();
}

void TraiterWorkorder::handler()
{	
	bool found;
	int nbWorkorderRoutings(0);

	switch(myState)
	{
		case TraiterWorkorder::STATE_START:
			if(myNewState)
			{
				ArLog::log(ArLog::Normal,"Entering TraiterWorkorder::STATE_START\n");
				//printf("Entering TraiterWorkorder::STATE_START\n");
				myNewState=false;
				myWorkorder.start();
				//Todo autodock
				//myModeDock->setAutoDock(false);		
				/* Todo : 
				DALRest::Update ou Request workorkorder
				  update workorkorder
				  */
				setState(STATE_LECTURE_WORS);
			}
			break;

		case TraiterWorkorder::STATE_LECTURE_WORS:
			if(myNewState)
			{
				found = false;
				myNewState=false;
			}
			ArLog::log(ArLog::Normal,"Entering TraiterWorkorder::STATE_LECTURE_WORS\n");
			
			//printf("Entering TraiterWorkorder::STATE_LECTURE_WORS\n");
			
			
			for(std::list<WorkorderRouting>::iterator it = worList.begin();it != worList.end(); ++it)
			{
				if((*it).getStatusId()==WorkorderRouting::NEW)
				{
					//Set current workorderRouting
					//myWorkorderRouting = &WorkorderRouting(*it);
					myWorkorderRouting = &*it;
					setState(STATE_TRAITEMENT_WOR);
					found=true;
					break;
				}
			}
			if(found == true)
				setState(STATE_TRAITEMENT_WOR);
			else
				setState(STATE_FIN_WO);
			break;

		case TraiterWorkorder::STATE_TRAITEMENT_WOR:
			if(myNewState)
			{
				ArLog::log(ArLog::Normal,"Entering TraiterWorkorder::STATE_TRAITEMENT_WOR\n");
		
				//printf("Entering TraiterWorkorder::STATE_TRAITEMENT_WOR\n");				
				myNewState = false;
				ArLog::log(ArLog::Normal,"Debut traitement WorkorderRoutingNo %s\n",myWorkorderRouting->getWorkorderRoutingNo());
		
				//printf("Debut traitement WorkorderRoutingNo %s\n",myWorkorderRouting->getWorkorderRoutingNo());
				/*Todo : demarrer worTask*/
				//TraiterWorkorderRouting traiterWorkorderRouting = TraiterWorkorderRouting(myModeGoto, myModeDock, myWorkorderRouting, myIhm, mySrma);
				//traiterWorkorderRouting.~TraiterWorkorderRouting();
				ArUtil::sleep(100);
				//~traiterWorkorderRouting();
			}
			//Todo : Attente
			/*if(worTask.getRunningWithLock())
				break;*/

			//Fin attente
			//Annulé
			if(myWorkorderRouting->isCancelled())
			{
				myWorkorder.cancel();
				/*Todo : mettre à jour BD */
				//DAL::updateWorkorder(*workorder);
				//Prevenir la tabletteSM
				sprintf(bufRequestIhm,"Object Workorder WorkorderNo %s StatusId %d\n",myWorkorder.getWorkorderNo(),(int)Workorder::CANCELLED);

				//Todo : Mise à jour du state du SRMA
				
				/*if(srma->IsLoaded())
					srma->setStateId(SRMA::FAULT);
				else
					srma->setStateId(SRMA::READY);*/
				setState(STATE_CANCELLED);
			}
			//Closed
			else if(myWorkorderRouting->isClosed())
			{
				setState(STATE_LECTURE_WORS);
			}
			//En cours
			else					
				break;
			break;
			
			

		case TraiterWorkorder::STATE_CANCELLED:
			if(myNewState)
			{
				printf("Entering TraiterWorkorder::STATE_FIN_WOR\n");
				myNewState=false;
			}
			
				//Notification de la fin de tache
			//myFin = true;
			myRunning = false;
			//evtTaskEnded.notify(0);

			break;
		//Fin normale
		case TraiterWorkorder::STATE_FIN_WO:
			if(myNewState)
			{
				printf("Entering TraiterWorkorder::STATE_FIN_WO\n");
				myNewState=false;
			}
			//Tous les WorkorderRoutings ont été réalisés

			//RAZ myWorkorderRouting et currentWorkorder
			myWorkorder.close();
			/*Todo mettre à jour BD */
			//DAL::updateWorkorder(*workorder);
			sprintf(bufRequestIhm,"Object Workorder WorkorderNo %s StatusId %d\n",myWorkorder.getWorkorderNo(),(int)Workorder::CLOSE);	
			
			/*Todo : ihm.send */
			//this->myI
			
			//serverEvt->sendToAllClients(bufRequestIhm);

			//On remet l autodock
			/*Todo : envpyer autodock */
			//myModeDock->setAutoDock(true);
			//robot->SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"ENABLE"));
			//robot->SetAutodock(true);
			//Mise à jour du state du SRMA

			/*Todo mettre à jour */
			//srma->setStateId(SRMA::READY);
			
			//On reset l attribut currentWorkorder de SRMA
			//pilotage->setCurrentWorkorder(NULL);
			//currentWorkorder=NULL;
			//pilotage->setCurrentWorkorderRouting(NULL);
			//myWorkorderRouting=NULL;

			//Notification de la fin de tache
			//evtTaskEnded.notify(0);
			myRunning = false;
			//myFin=true;
			break;
		}
	}


