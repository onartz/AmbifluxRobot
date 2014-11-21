#include "WorkOrderRoutingTask.h"

WorkOrderRoutingTask::WorkOrderRoutingTask():
//srma(NULL),
//tabletteSM(NULL),
//clientTCP(NULL),
//robot(NULL),
workorderRouting(NULL),
myLecteurCarte(NULL),
nextLocation(NULL),
nextLocationName(NULL),
fin(false),
myState(STATE_TRANSPORT_START),
myNewState(true),
myResult(RES_UNKNOWN)
//server(NULL),
//serverEvt(NULL)
{}

WorkOrderRoutingTask::WorkOrderRoutingTask(ArServerModeGoto *modeGoto, ArServerModeDock *modeDock, WorkOrderRouting *wor, LecteurCarte *lecteurCarte, IhmCommunicationThread *ihm):
workorderRouting(wor),
myLecteurCarte(lecteurCarte),
nextLocation(NULL),
nextLocationName(NULL),
fin(false),
myState(STATE_TRANSPORT_START),
myNewState(true),
myResult(RES_UNKNOWN),
myModeGoto(modeGoto),
myModeDock(modeDock)
{}


void WorkOrderRoutingTask::setState(State state)
{
	retCode=0;
	myState=state;
	myNewState=true;
	myStateStartTime.setToNow();
}

void WorkOrderRoutingTask::handler()
{
	char bufr[256];
	myCycleStartTime.setToNow();
	switch(myState)
	{
		/*-------------------------------------------------------------
		Branche WorkOrderRouting type TRANSPORT
		-------------------------------------------------------------*/

		case STATE_TRANSPORT_START:
			printf("Entering STATE_TRANSPORT_START\n");
			//Initialisation de la position cible
			nextLocation=workorderRouting->getLocation();
			//Suppression des espaces en trop
			nextLocationName=nextLocation->getLocationName();
			aipl::DelSpace(nextLocationName);

			//Initialisation du WorkOrderRouting
			/* Todo : workorderRouting->setResource(srma);*/
			
			workorderRouting->start();
			printf("WorkOrderRouting %s demarre\n",workorderRouting->getWorkOrderRoutingNo());
			//Todo : DAL::updateWorkOrderRouting(workorderRouting);
			
			//On enleve l autodock
			//CommandeRobot c=CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE");
			
			myModeDock->setAutoDock(true);
			//robot->SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE"));
			//robot->SetAutodock(false);

			setState(STATE_TRANSPORT_DEPLACEMENT);
			break;


		case STATE_TRANSPORT_DEPLACEMENT:
			//Envoi demande de déplacement
			if(myNewState)
			{
				myNewState = false;		
				printf("Entering STATE_TRANSPORT_DEMANDE_DEPLACEMENT\n");
				myModeGoto->gotoGoal(nextLocationName);
				//robot->SendCommand(CommandeRobot(CommandeRobot::GOTOGOAL,nextLocationName));
				//robot->GotoGoal(nextLocationName);
				break;
			}
			//Attente fin déplacement
			// Todo : modifier robot en myModeGoto
			if(isStateFailedToGetTo(nextLocationName) || isStateArrivedAt(nextLocationName));
			/*if(robot->IsStateFailedToGetTo(nextLocationName) || robot->IsStateArrivedAt(nextLocationName) )
			{	
				setState(STATE_TRANSPORT_FIN);
				break;
			}*/

			break;
				

		case STATE_TRANSPORT_FIN:
			printf("Entering STATE_TRANSPORT_FIN\n");
			//Todo : remplacer robot par myModeGoto->
			
			if(isStateFailedToGetTo(nextLocationName))
			{
				/*************************************************/
				/* 1.1.2 Position non atteinte, fin			      */
				/*************************************************/	
				workorderRouting->setStateId(WorkOrderRouting::FAILURE);
				workorderRouting->cancel();
				printf("WorkOrderRouting %s annule\n",workorderRouting->getWorkOrderRoutingNo());		
				
			}
			else if(isStateArrivedAt(nextLocationName))
			{
				/*************************************************/
				/* 1.1.2 Position atteinte, fin normale          */
				/*************************************************/	
				workorderRouting->setStateId(WorkOrderRouting::COMPLETE);
				workorderRouting->close();
				printf("WorkOrderRouting %s cloture\n",workorderRouting->getWorkOrderRoutingNo());
			}
			//Todo : DALREST DAL::updateWorkOrderRouting(workorderRouting);
			//Faut il??
			fin=true;
			break;


		/*-------------------------------------------------------------
		Branche WorkOrderRouting type CHARGEMENT
		-------------------------------------------------------------*/
		case STATE_CHARGEMENT_START:
			printf("Entering STATE_CHARGEMENT_START\n");		
			workorderRouting->start();
			printf("WorkOrderRouting %s demarre\n",workorderRouting->getWorkOrderRoutingNo());
			//Todo DALRest::updateWorkOrderRouting(workorderRouting);
			setState(STATE_CHARGEMENT_IDENTIFICATION);			
			break;

		case STATE_CHARGEMENT_IDENTIFICATION:
			//Gérer timeout
			if(myNewState)
			{
				printf("Entering STATE_CHARGEMENT_IDENTIFICATION\n");
				identificationSM=IdentificationSM(srma,server,serverEvt);
					//Attention bloquant!!
				identificationSM.Start();
				myNewState = false;
			}
			//Test du résultat de l'identification
			//identificationSM.stop???
			if(identificationSM.getState()== IdentificationSM::STATE_FIN)
			{
				switch(identificationSM.getResultat())
				{
					case IdentificationSM::RES_IDENTIFICATION_OK:
						setState(STATE_DEM_AFFICHAGE_CHARGEMENT);
						workorderRouting->setResource(identificationSM.getOperateur());
						break;
					case IdentificationSM::RES_IDENTIFICATION_FAILED:
						setState(STATE_CHARGEMENT_FIN);
						break;
					case IdentificationSM::RES_TIMEOUT:
						setState(STATE_CHARGEMENT_FIN);
						break;
					default:
						
						//setState(STATE_CHARGEMENT_CANCEL);
						break;
				}
			}
			break;

		/*case STATE_CHARGEMENT_CANCEL:
			printf("Entering STATE_CHARGEMENT_CANCEL\n");
			//Si 3 echecs, on annule le WorkOrderRouting

			switch (identificationSM.getResultat())
			{
				case IdentificationSM::RES_IDENTIFICATION_FAILED:
					workorderRouting->setErrorCodeId(WorkOrderRouting::ERR_IDENTIFICATION_FAILED);
					break;
				case IdentificationSM::RES_TIMEOUT:
					workorderRouting->setErrorCodeId(WorkOrderRouting::ERR_TIMEOUT_ARRIVEE_OPERATEUR);
					break;
			}
			//identificationSM();
			workorderRouting->setStateId(WorkOrderRouting::FAILURE);
			printf("WorkOrderRouting %s annule\n",workorderRouting->getWorkOrderRoutingNo());
			workorderRouting->cancel();
			DAL::updateWorkOrderRouting(workorderRouting);
			//On sort de la boucle
			fin=true;
			//STate.......
			break;

*/

		case STATE_DEM_AFFICHAGE_CHARGEMENT:
			if(myNewState)
			{
				printf("Entering STATE_DEM_AFFICHAGE_CHARGEMENT\n");	
				sprintf(bufETCPServerEvt,"Open ChargementVC %d %s %d\n",workorderRouting->getWorkOrder()->getOrderHeader()->getOrderHeaderId(), workorderRouting->getWorkOrderRoutingNo(),workorderRouting->getResource()->getResourceId());
				//Envoi requete d'ouverture
				serverEvt->sendToAllClients(bufETCPServerEvt);
				myNewState = false;

				break;
			}

			//Attente cr ouverture form
			if(tabletteSM->IsChargementVCOpened())
			{
				setState(STATE_ATTENTE_CHARGEMENT_TERMINE);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_CHARGEMENT_FIN);
				}
				else
					setState(STATE_DEM_AFFICHAGE_CHARGEMENT);
			}
			break;


			case STATE_ATTENTE_CHARGEMENT_TERMINE:
				printf("Entering STATE_ATTENTE_CHARGEMENT_TERMINE\n");

				//Fin normale
				if(!tabletteSM->IsChargementVCOpened())
				{
					if(workorderRouting->isClosed())
					{
						myResult=RES_OK;
						setState(STATE_CHARGEMENT_FIN);
						break;
					}
					if(workorderRouting->isCancelled())
					{
						myResult=RES_FAILED;
						setState(STATE_CHARGEMENT_FIN);
						break;
					}

					break;
				}
				//Timeout chargement trop long
				if(myStateStartTime.mSecSince() > TIMEOUT_DUREE_CHARGEMENT*1000)
				{
					myResult=RES_TIMEOUT;
					//Faire qque chose
					setState(STATE_DEM_FERMETURE_CHARGEMENT);
					break;
				}
		
				break;

			case STATE_DEM_FERMETURE_CHARGEMENT:
				if(myNewState)
				{
					printf("Entering STATE_DEM_FERMETURE_CHARGEMENT\n");
					myNewState = false;
					sprintf(bufETCPServerEvt,"Close ChargementVC\n");
					serverEvt->sendToAllClients(bufETCPServerEvt);
					//tabletteSM->SendMessage(bufETCPServerEvt);
					break;
				}

			//Attente cr fermeture form
			if(!(tabletteSM->IsChargementVCOpened()))
			{
				setState(STATE_CHARGEMENT_FIN);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_CHARGEMENT_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_CHARGEMENT);
			}
			break;


			//case STATE_ATTENTE_FERMETURE_CHARGEMENT:
			//	printf("Entering STATE_ATTENTE_FERMETURE_CHARGEMENT\n");
			//	if(tabletteSM->IsChargementVCOpened() && myStateStartTime.mSecSince() > 5000)
			//	{
			//		break;
			//	}
			//	// Le formulaire s'est bien refermé
			//	else if(!tabletteSM->IsChargementVCOpened())
			//	{
			//		setState(STATE_CHARGEMENT_OK);
			//	}
			//	//Timeout fermeture formulaire
			//	else {
			//		setState(STATE_CHARGEMENT_INTERNAL_ERROR);
			//	}
			//	

			//	break;

			case STATE_CHARGEMENT_FIN:
				printf("Entering STATE_CHARGEMENT_FIN\n");
				switch(myResult)
				{
					case RES_OK:
						srma->setIsLoaded(true);
					
						printf("WorkOrderRouting %s cloture\n",workorderRouting->getWorkOrderRoutingNo());
						//Mise à jour du workorderRouting
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_FAILED:
						printf("WorkOrderRoutingTask en RES_CHARGEMENT_FAILED\n");
						workorderRouting->setErrorCodeId(WorkOrderRouting::ERR_TIMEOUT_OPERATION);
				
						workorderRouting->setStateId(WorkOrderRouting::FAILURE);
						printf("WorkOrderRouting %s annule\n",workorderRouting->getWorkOrderRoutingNo());
						workorderRouting->cancel();
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_ERROR:
						printf("WorkOrderRoutingTask en RES_ERROR\n");
						fin=true;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						printf("WorkOrderRoutingTask en RES_TIMEOUT\n");
						fin=true;
						//Faire qque chose
						break;

				}

				break;

			/*-------------------------------------------------------------
			Branche WorkOrderRouting type LIVRAISON
			-------------------------------------------------------------*/
			case STATE_LIVRAISON_START:
				printf("Entering STATE_LIVRAISON_START\n");		
				workorderRouting->start();
				printf("WorkOrderRouting %s demarre\n",workorderRouting->getWorkOrderRoutingNo());
				DAL::updateWorkOrderRouting(workorderRouting);
				setState(STATE_DEM_AFFICHAGE_LIVRAISON);			
			break;

			case STATE_DEM_AFFICHAGE_LIVRAISON:
			if(myNewState)
			{
				printf("Entering STATE_DEM_AFFICHAGE_LIVRAISON\n");
				myNewState = false;				
				sprintf(bufETCPServerEvt,"Open LivraisonVC %d %s\n",workorderRouting->getWorkOrder()->getOrderHeaderId(), workorderRouting->getWorkOrderRoutingNo());
				serverEvt->sendToAllClients(bufETCPServerEvt);
				//tabletteSM->SendMessage(bufETCPServerEvt);
				break;
			}

			//Attente cr ouverture form
			if(tabletteSM->IsLivraisonVCOpened())
			{
				setState(STATE_ATTENTE_LIVRAISON_TERMINE);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_LIVRAISON_FIN);
				}
				else
					setState(STATE_DEM_AFFICHAGE_LIVRAISON);
			}
			break;

			case STATE_ATTENTE_LIVRAISON_TERMINE:
				printf("Entering STATE_ATTENTE_LIVRAISON_TERMINE\n");

				//Fin normale
				if(!tabletteSM->IsLivraisonVCOpened())
				{
					if(workorderRouting->isClosed())
					{
						myResult=RES_OK;
						setState(STATE_LIVRAISON_FIN);
						break;
					}
					if(workorderRouting->isCancelled())
					{
						myResult=RES_FAILED;
						setState(STATE_LIVRAISON_FIN);
						break;
					}

					break;
				}
				//Timeout chargement trop long
				if(myStateStartTime.mSecSince() > TIMEOUT_DUREE_LIVRAISON*1000)
				{
					myResult=RES_TIMEOUT;
					//Faire qque chose
					setState(STATE_DEM_FERMETURE_LIVRAISON);
					break;
				}
		
				break;

			case STATE_DEM_FERMETURE_LIVRAISON:
				if(myNewState)
				{
					printf("Entering STATE_DEM_FERMETURE_LIVRAISON\n");
					myNewState = false;
					sprintf(bufETCPServerEvt,"Close LivraisonVC\n");
					serverEvt->sendToAllClients(bufETCPServerEvt);
					//tabletteSM->SendMessage(bufETCPServerEvt);
					break;
				}

			//Attente cr fermeture form
			if(!(tabletteSM->IsLivraisonVCOpened()))
			{
				setState(STATE_LIVRAISON_FIN);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_LIVRAISON_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_LIVRAISON);
			}
			break;

			case STATE_LIVRAISON_FIN:
				printf("Entering STATE_LIVRAISON_FIN\n");
				switch(myResult)
				{
					case RES_OK:
						srma->setIsLoaded(false);
					
						printf("WorkOrderRouting %s cloture\n",workorderRouting->getWorkOrderRoutingNo());
						//Mise à jour du workorderRouting
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_FAILED:
						printf("WorkOrderRoutingTask en RES_FAILED\n");
						workorderRouting->setErrorCodeId(WorkOrderRouting::ERR_TIMEOUT_OPERATION);
				
						workorderRouting->setStateId(WorkOrderRouting::FAILURE);
						printf("WorkOrderRouting %s annule\n",workorderRouting->getWorkOrderRoutingNo());
						workorderRouting->cancel();
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_ERROR:
						printf("WorkOrderRoutingTask en RES_ERROR\n");
						fin=true;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						printf("WorkOrderRoutingTask en RES_TIMEOUT\n");
						fin=true;
						//Faire qque chose
						break;

				}
				break;

			/*-------------------------------------------------------------
			Branche WorkOrderRouting type STATE_TRANSFERTA_START
			-------------------------------------------------------------*/
			
	/*		case STATE_TRANSFERTA_START:
				printf("Entering STATE_TRANSFERTA_START\n");		
				workorderRouting->start();
				printf("WorkOrderRouting %s demarre\n",workorderRouting->getWorkOrderRoutingNo());
				DAL::updateWorkOrderRouting(workorderRouting);
				setState(STATE_DEM_AFFICHAGE_TRANSFERTA);			
			break;

			case STATE_DEM_AFFICHAGE_TRANSFERTA:
			if(myNewState)
			{
				printf("Entering STATE_DEM_AFFICHAGE_TRANSFERTA\n");	
				sprintf(bufETCPServerEvt,"Open TransfertAVC\n");
				//Envoi requete d'ouverture
				serverEvt->sendToAllClients(bufETCPServerEvt);
				myNewState = false;
				break;
			}

			//Attente cr ouverture form
			if(tabletteSM->IsTransfertAVCOpened())
			{
				setState(STATE_ATTENTE_TRANSFERTA_TERMINE);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_TRANSFERTA_FIN);
				}
				else
					setState(STATE_DEM_AFFICHAGE_TRANSFERTA);
			}
			break;


			case STATE_ATTENTE_TRANSFERTA_TERMINE:
				printf("Entering STATE_ATTENTE_TRANSFERTA_TERMINE\n");

				//Fin normale
				if(!tabletteSM->IsTransfertAVCOpened())
				{
					if(workorderRouting->isClosed())
					{
						myResult=RES_OK;
						setState(STATE_TRANSFERTA_FIN);
						break;
					}
					if(workorderRouting->isCancelled())
					{
						myResult=RES_FAILED;
						setState(STATE_TRANSFERTA_FIN);
						break;
					}

					break;
				}
				//Timeout chargement trop long
				if(myStateStartTime.mSecSince() > TIMEOUT_DUREE_CHARGEMENT*1000)
				{
					myResult=RES_TIMEOUT;
					//Faire qque chose
					setState(STATE_DEM_FERMETURE_TRANSFERTA);
					break;
				}
		
				break;

			case STATE_DEM_FERMETURE_TRANSFERTA:
				if(myNewState)
				{
					printf("Entering STATE_DEM_FERMETURE_TRANSFERTA\n");
					myNewState = false;
					sprintf(bufETCPServerEvt,"Close TransfertAVC\n");
					serverEvt->sendToAllClients(bufETCPServerEvt);
					break;
				}

			//Attente cr fermeture form
			if(!(tabletteSM->IsTransfertAVCOpened()))
			{
				setState(STATE_TRANSFERTA_FIN);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_TRANSFERTA_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_TRANSFERTA);
			}
			break;



			case STATE_TRANSFERTA_FIN:
				printf("Entering STATE_TRANSFERTA_FIN\n");
				switch(myResult)
				{
					case RES_OK:
						srma->setIsLoaded(true);
					
						printf("WorkOrderRouting %s cloture\n",workorderRouting->getWorkOrderRoutingNo());
						//Mise à jour du workorderRouting
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_FAILED:
						printf("WorkOrderRoutingTask en RES_CHARGEMENT_FAILED\n");
						workorderRouting->setErrorCodeId(WorkOrderRouting::ERR_TIMEOUT_OPERATION);
				
						workorderRouting->setStateId(WorkOrderRouting::FAILURE);
						printf("WorkOrderRouting %s annule\n",workorderRouting->getWorkOrderRoutingNo());
						workorderRouting->cancel();
						DAL::updateWorkOrderRouting(workorderRouting);
						//On arrete la boucle
						fin=true;
						break;
					case RES_ERROR:
						printf("WorkOrderRoutingTask en RES_ERROR\n");
						fin=true;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						printf("WorkOrderRoutingTask en RES_TIMEOUT\n");
						fin=true;
						//Faire qque chose
						break;

				}

				break;*/
	}
		
}


/*void WorkOrderRoutingTask::Start(WorkOrderRouting *pWorkOrderRouting, SRMA *pSrma, A_ArNetServer *pServer,A_ArNetServer *pServerEvt)
{
	//serverSocket=pClientSocket;
	server=pServer;
	serverEvt=pServerEvt;
	workorderRouting=pWorkOrderRouting;
	srma=pSrma;
	tabletteSM=pSrma->getTabletteSM();
	lecteurCarte=pSrma->getLecteurCarte();
	robot=pSrma->getRobot();
	identificationSM=IdentificationSM(pSrma,pServer,pServerEvt);
	//start();

}*/

void WorkOrderRoutingTask::start()
{
	fin=false;
	type=workorderRouting->getTypeLong();
	switch(type)
	{
		case WorkOrderRouting::TRANSPORT :
			setState(STATE_TRANSPORT_START);
			break;
		case WorkOrderRouting::CHARGEMENT:
			setState(STATE_CHARGEMENT_START);
			break;
		case WorkOrderRouting::LIVRAISON :
			setState(STATE_LIVRAISON_START);
			break;
		case WorkOrderRouting::DECHARGEMENT:
			setState(STATE_DECHARGEMENT_START);
			break;
		case WorkOrderRouting::TRANSFERTA:
			setState(STATE_TRANSFERTA_START);
			break;
		case WorkOrderRouting::TRANSFERTB:
			setState(STATE_TRANSFERTB_START);
			break;
	}
	ArASyncTask::runAsync();
	printf("Thread WorkOrderRoutingTask %s started\n",workorderRouting->getWorkOrderRoutingNo());
}

/*void WorkOrderRoutingTask::Start()
{
	start();
}*/



void WorkOrderRoutingTask::Stop()
{
	ArASyncTask::stopRunning();
	printf("Thread WorkOrderRoutingTask %s stopped\n",workorderRouting->getWorkOrderRoutingNo());
}


void* WorkOrderRoutingTask::runThread(void*) 
{
	printf("Thread WorkOrderRoutingTask %s running\n",workorderRouting->getWorkOrderRoutingNo());
	while(myRunning == true)
	{
		if(fin == true)
			break;
		myMutex.lock();
		handler();
		myMutex.unlock();
		ArUtil::sleep(100);
	}
	printf("Thread WorkOrderRoutingTask %s ended\n",workorderRouting->getWorkOrderRoutingNo());
	ArASyncTask::stopRunning();
	return NULL;
}

//Accesseurs Get
WorkOrderRoutingTask::Resultat WorkOrderRoutingTask::getResultat(){return myResult;}

bool WorkOrderRoutingTask::isStateFailedToGetTo(std::string nextLocationName)
{
	string resAttendu = string("Failed to get to ");
	resAttendu.append(nextLocationName);
	string s = string(myModeGoto->getStatus());
	return(resAttendu.compare(s));
}

bool WorkOrderRoutingTask::isStateFailedToGetTo(char * nextLocationName)
{
	string resAttendu = string("Failed to get to ");
	resAttendu.append(string(nextLocationName));
	string s = string(myModeGoto->getStatus());
	return(resAttendu.compare(s));
}

bool WorkOrderRoutingTask::isStateArrivedAt(char * nextLocationName)
{
	string resAttendu = string("Arrived at ");
	resAttendu.append(string(nextLocationName));
	string s = string(myModeGoto->getStatus());
	return(resAttendu.compare(s));
}




