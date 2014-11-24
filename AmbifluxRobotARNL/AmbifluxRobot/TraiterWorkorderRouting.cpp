#include "TraiterWorkorderRouting.h"


//Constructeur utilisé
TraiterWorkorderRouting::TraiterWorkorderRouting(SRMA &srma, WorkorderRouting &workorderRouting, Pool<TCPReceivedRequest> * messagePool ):
myResult(RES_UNKNOWN),
mySrma(srma),
myMessagePool(messagePool),
myWorkorderRouting(workorderRouting),
myOutputHandler(mySrma.getOutputHandler()),
myIhm(mySrma.getIhmCommunicationThread()),
myOperateur()
{
	setup();
	start();	
}


void TraiterWorkorderRouting::setState(State state)
{
	retCode=0;
	myState=state;
	myNewState=true;
	myStateStartTime.setToNow();
}

void TraiterWorkorderRouting::handler()
{
	//char bufr[256];
	TCPReceivedRequest req;
	myCycleStartTime.setToNow();
	//Frame f;
	switch(myState)
	{
		/*-------------------------------------------------------------
		Branche WorkorderRouting myType TRANSPORT
		-------------------------------------------------------------*/

		case STATE_TRANSPORT_START:
			ArLog::log(ArLog::Verbose, "Entering STATE_TRANSPORT_START\n");
			//Initialisation de la position cible
			strcpy(nextLocationName, myWorkorderRouting.getLocation().getLocationName());
			
			//nextLocationName = myWorkorderRouting.getLocation().getLocationName();
			
			//Initialisation du WorkorderRouting
			myWorkorderRouting.setResource(&mySrma);			
			myWorkorderRouting.start();

			ArLog::log(ArLog::Verbose, "WorkorderRouting %s demarre\n",myWorkorderRouting.getWorkorderRoutingNo());
			DALRest::updateWorkorderRouting(&myWorkorderRouting);		
			mySrma.SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE"));
			//If Robot still at position
			if(mySrma.isStateArrivedAt(nextLocationName))
			{
				setState(STATE_TRANSPORT_FIN);
				break;
			}
			//Robot not at position
			//nextLocationName = myWorkorderRouting.getLocation().getLocationName();
				//ArLog::log(ArLog::Verbose, "Entering STATE_TRANSPORT_DEMANDE_DEPLACEMENT");				
			mySrma.SendCommand(CommandeRobot(CommandeRobot::GOTOGOAL,nextLocationName ));
			setState(STATE_TRANSPORT_PREPARATION);
			break;

			case STATE_TRANSPORT_PREPARATION:
				if(myNewState)
				{
					myNewState = false;	
					//nextLocationName = myWorkorderRouting.getLocation().getLocationName();
					ArLog::log(ArLog::Verbose, "Entering STATE_TRANSPORT_DEMANDE_DEPLACEMENT");				
					mySrma.SendCommand(CommandeRobot(CommandeRobot::GOTOGOAL,nextLocationName ));
					//setState(STATE_TRANSPORT_P);
					break;
				}
				//Robot must return GoingTo after a while
				if(mySrma.isStateGoingTo(nextLocationName))
				{
					setState(STATE_TRANSPORT_DEPLACEMENT);
					break;
				}
				//Timeout left
				if(myStateStartTime.mSecSince() > 8000 )
				{
					setState(STATE_TRANSPORT_FIN);
					break;
				}
				break;
				//Attente fin déplacement
				/*if(mySrma.isStateFailedToGetTo(nextLocationName) || mySrma.isStateArrivedAt(nextLocationName))		
					setState(STATE_TRANSPORT_FIN);
				break;*/

				//Robot must return GoingTo after a while
				/*if((myStateStartTime.mSecSince() > 8000 ) && !mySrma.isStateGoingTo(nextLocationName))
				{
					setState(STATE_TRANSPORT_FIN);
					break;
				}

				//Robot not going to (Arrived, Failed or else)
				if(!mySrma.isStateGoingTo(nextLocationName))
				{
					setState(STATE_TRANSPORT_FIN);
				}
				break;*/

				
				//Attente fin déplacement
				/*if(mySrma.isStateFailedToGetTo(nextLocationName) || mySrma.isStateArrivedAt(nextLocationName))		
					setState(STATE_TRANSPORT_FIN);
				break;*/
					

		case STATE_TRANSPORT_DEPLACEMENT:
			if(myNewState)
			{
				myNewState = false;	
				//nextLocationName = myWorkorderRouting.getLocation().getLocationName();
				ArLog::log(ArLog::Verbose, "Entering STATE_TRANSPORT_DEPLACEMENT");				
				break;
			}
			if(!mySrma.isStateGoingTo(nextLocationName))
			{
				setState(STATE_TRANSPORT_FIN);
				break;
			}
			break;
			

		case STATE_TRANSPORT_FIN:
			ArLog::log(ArLog::Verbose, "Entering STATE_TRANSPORT_FIN\n");
			
			if(mySrma.isStateFailedToGetTo(nextLocationName))
			{
				/*************************************************/
				/* 1.1.2 Position non atteinte, fin			      */
				/*************************************************/	
				myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
				myWorkorderRouting.cancel();
				ArLog::log(ArLog::Verbose, "WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());		
				
			}
			else if(mySrma.isStateArrivedAt(nextLocationName))
			{
				/*************************************************/
				/* 1.1.2 Position atteinte, fin normale          */
				/*************************************************/	
				myWorkorderRouting.setStateId(WorkorderRouting::COMPLETE);
				myWorkorderRouting.close();
				ArLog::log(ArLog::Verbose, "WorkorderRouting %s cloture\n",myWorkorderRouting.getWorkorderRoutingNo());
			}
			//TODO : we have a serious pb
				else
				{
					/*************************************************/
					/* 1.1.2 Position non atteinte, fin			      */
					/*************************************************/	
					myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
					myWorkorderRouting.cancel();
					ArLog::log(ArLog::Verbose, "WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());		
					
					break;
				}

			//Todo : Decommenter
			DALRest::updateWorkorderRouting(&myWorkorderRouting);

			myRunning = false;
			break;


		/*-------------------------------------------------------------
		Branche WorkorderRouting myType CHARGEMENT
		-------------------------------------------------------------*/
		case STATE_CHARGEMENT_START:
			ArLog::log(ArLog::Verbose, "Entering STATE_CHARGEMENT_START\n");		
			myWorkorderRouting.start();
			ArLog::log(ArLog::Verbose, "WorkorderRouting %s demarre\n",myWorkorderRouting.getWorkorderRoutingNo());
			//TODO : Décommenter
			//DALRest::updateWorkorderRouting(&myWorkorderRouting);
			
			setState(STATE_CHARGEMENT_IDENTIFICATION);			
			break;

		case STATE_CHARGEMENT_IDENTIFICATION:
			//Gérer timeout
			if(myNewState)
			{
				mySrma.play(SRMA::MSG_ARRIVED);		
				myNewState = false;
				ArLog::log(ArLog::Verbose, "Entering STATE_CHARGEMENT_IDENTIFICATION\n");
				sprintf(bufRequestIhm, "OpenForm IdentificationVC\n");
				//TODO : si ca ne se passe pas bien
				if(myIhm.sendRequest(bufRequestIhm) != 0){
					setState(STATE_CHARGEMENT_IDENTIFICATION);
					break;
				}
				myOperateur = &Person();
				mySrma.play(SRMA::BUTTON_PRESSED);
				
				Identification::Identifier(myOperateur);
				
			}
				sprintf(bufRequestIhm, "CloseForm IdentificationVC\n");
				//TODO : si ca ne se passe pas bien
				if(myIhm.sendRequest(bufRequestIhm) != 0){
					setState(STATE_CHARGEMENT_IDENTIFICATION);
					break;
				}
			if(myOperateur->getCardId() == "")
			{
				setState(STATE_CHARGEMENT_FIN);
				break;
			}
			setState(STATE_DEM_AFFICHAGE_CHARGEMENT);
			myWorkorderRouting.setResource(myOperateur);

			break;


		/*case STATE_CHARGEMENT_CANCEL:
			printf("Entering STATE_CHARGEMENT_CANCEL\n");
			//Si 3 echecs, on annule le WorkorderRouting

			switch (myIdentificationSM.getResultat())
			{
				case IdentificationSM::RES_IDENTIFICATION_FAILED:
					myWorkorderRouting.setErrorCodeId(WorkorderRouting::ERR_IDENTIFICATION_FAILED);
					break;
				case IdentificationSM::RES_TIMEOUT:
					myWorkorderRouting.setErrorCodeId(WorkorderRouting::ERR_TIMEOUT_ARRIVEE_OPERATEUR);
					break;
			}
			//myIdentificationSM();
			myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
			printf("WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());
			myWorkorderRouting.cancel();
			DAL::updateWorkorderRouting(myWorkorderRouting);
			//On sort de la boucle
			myRunning = true;
			//STate.......
			break;

*/

		case STATE_DEM_AFFICHAGE_CHARGEMENT:
			if(myNewState)
			{
				myNewState = false;
				ArLog::log(ArLog::Verbose, "Entering STATE_DEM_AFFICHAGE_CHARGEMENT\n");	
				//Envoi requete d'ouverture et attente retour OK
				//Todo : create frame and send
				sprintf(bufRequestIhm, "OpenForm ChargementVC %d %s 9\n",myWorkorderRouting.getWorkorder()->getOrderHeader().getOrderHeaderId(),myWorkorderRouting.getWorkorderRoutingNo());
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_ATTENTE_CHARGEMENT_TERMINE);
					break;
				}
				//TODO : faire qque chose si myIhm.sendRequest ne se passe pas bien
				/*if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_LIVRAISON_FIN);
				}*/
				setState(STATE_DEM_AFFICHAGE_CHARGEMENT);
				break;
			}

			//5sec pour que la tablette ait ouvert le form
			/*if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_CHARGEMENT_FIN);
				}
				else
					setState(STATE_DEM_AFFICHAGE_CHARGEMENT);
			}*/
			break;


			case STATE_ATTENTE_CHARGEMENT_TERMINE:
				if(myNewState)
				{
					ArLog::log(ArLog::Verbose, "Entering STATE_ATTENTE_CHARGEMENT_TERMINE\n");
					myNewState = false;
					break;
				}
				if(myStateStartTime.mSecSince() > TIMEOUT_DUREE_CHARGEMENT*1000)
				{
					myResult=RES_TIMEOUT;
					//Faire qque chose
					setState(STATE_DEM_FERMETURE_CHARGEMENT);
					break;
				}
				//en attente des messages
				//Info ChargementTermine WorkorderRoutingNo 0
				//Info ChargementTermine WorkorderRoutingNo 1
				
				if(myMessagePool->size()==0)
					break;
				
				//Todo : Attention on n'est pas sur que c'est le bon message
				//A corriger
				req = myMessagePool->pop();
				//On vérifie que c'est bien un peformative Info
				if(req.frame.msg[0] != "Info")
				{
					//Si pas "Info"
					//On remet le message dans la queue
					myMessagePool->push(req);
					break;
				}
				//Message Info
				//On vérifie la taille
				//Faire qque chose au bout d'un moment
				if(req.frame.msg.size()<4)
					break;
				if(req.frame.msg[1] != "ChargementTermine")
					break;
				if(req.frame.msg[2] != myWorkorderRouting.getWorkorderRoutingNo())
					break;
				myWorkorderRouting.setStateId((WorkorderRouting::WORKORDERROUTING_STATE)atoi(req.frame.msg[3].c_str()));
				myResult=RES_OK;
				mySrma.play(SRMA::BUTTON_PRESSED);
				setState(STATE_CHARGEMENT_FIN);
					break;

			case STATE_DEM_FERMETURE_CHARGEMENT:
				if(myNewState)
				{
					myNewState = false;
					ArLog::log(ArLog::Verbose, "Entering STATE_DEM_FERMETURE_CHARGEMENT\n");	
					sprintf(bufRequestIhm,"CloseForm ChargementVC\n");
					//Envoi requete d'ouverture et attente retour OK
					if(myIhm.sendRequest(bufRequestIhm) == 0){
						setState(STATE_CHARGEMENT_FIN);
						break;
					}
					setState(STATE_CHARGEMENT_FIN);
					break;
				}

			//5sec pour que la tablette ait ouvert le form
			/*if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_CHARGEMENT_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_CHARGEMENT);
			}*/
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
				ArLog::log(ArLog::Verbose, "Entering STATE_CHARGEMENT_FIN\n");
				switch(myResult)
				{
					case RES_OK:
						mySrma.setLoad(SRMA::LOADED);
						myWorkorderRouting.close();
					
						ArLog::log(ArLog::Verbose, "WorkorderRouting %s cloture\n",myWorkorderRouting.getWorkorderRoutingNo());
						//Mise à jour du myWorkorderRouting
						//Todo : DALRest
						
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						//On arrete la boucle
						myRunning = false;
						break;
					case RES_FAILED:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_CHARGEMENT_FAILED\n");
						myWorkorderRouting.setErrorCodeId(WorkorderRouting::ERR_TIMEOUT_OPERATION);
				
						myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
						ArLog::log(ArLog::Verbose, "WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						
						//On arrete la boucle
						myRunning = false;
						break;
					case RES_ERROR:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_ERROR\n");
						//TODO : à vérifier
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						myRunning = false;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_TIMEOUT\n");
						//TODO : à vérifier
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						myRunning = false;
						//Faire qque chose
						break;

				}

				break;

			/*-------------------------------------------------------------
			Branche WorkorderRouting myType LIVRAISON
			-------------------------------------------------------------*/
			case STATE_LIVRAISON_START:
				ArLog::log(ArLog::Verbose, "Entering STATE_LIVRAISON_START\n");		
				myWorkorderRouting.start();
				ArLog::log(ArLog::Verbose, "WorkorderRouting %s demarre\n",myWorkorderRouting.getWorkorderRoutingNo());
				//Todo : DalRest
				//DAL::updateWorkorderRouting(myWorkorderRouting);
				DALRest::updateWorkorderRouting(&myWorkorderRouting);
				setState(STATE_DEM_AFFICHAGE_LIVRAISON);			
			break;

			case STATE_DEM_AFFICHAGE_LIVRAISON:
			if(myNewState)
			{
				mySrma.play(SRMA::MSG_ARRIVED);
				ArLog::log(ArLog::Verbose, "Entering STATE_DEM_AFFICHAGE_LIVRAISON\n");
				myNewState = false;				
				sprintf(bufRequestIhm,"OpenForm LivraisonVC %d %s\n",myWorkorderRouting.getWorkorder()->getOrderHeader().getOrderHeaderId(), myWorkorderRouting.getWorkorderRoutingNo());
				//Envoi requete d'ouverture et attente retour OK
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_ATTENTE_LIVRAISON_TERMINE);
					break;
				}
				//TODO : On peut boucler à l'infini. Faire en sorte de pouvoir en sortir
				//myResult=RES_OK;
				setState(STATE_DEM_AFFICHAGE_LIVRAISON);
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
				if(myNewState)
				{
					ArLog::log(ArLog::Verbose, "Entering STATE_ATTENTE_LIVRAISON_TERMINE\n");
					myNewState = false;
				}
				//Timeout chargement trop long
				if(myStateStartTime.mSecSince() > TIMEOUT_DUREE_LIVRAISON*1000)
				{
					myResult=RES_TIMEOUT;
					//Faire qque chose
					setState(STATE_DEM_FERMETURE_LIVRAISON);
					break;
				}
				
				//en attente des messages
				//LivraisonTerminee WorkorderRoutingNo Complete
				//LivraisonTerminee WorkorderRoutingNo Refusee
				if(myMessagePool->size()==0)
					break;
				//Todo : Attention on n'est pas sur que c'est le bon message
				//A corriger
				req = myMessagePool->pop();
				//On vérifie que c'est bien un peformative Info
				if(req.frame.msg[0] != "Info")
				{
					//Si pas "Info"
					//On remet le message dans la queue
					myMessagePool->push(req);
					break;
				}
				//Message Info
				//On vérifie la taille
				//Faire qque chose au bout d'un moment
				if(req.frame.msg.size()<4)
					break;
				if(req.frame.msg[1] != "LivraisonTerminee")
					break;
				if(req.frame.msg[2] != myWorkorderRouting.getWorkorderRoutingNo())
					break;
				myWorkorderRouting.setStateId((WorkorderRouting::WORKORDERROUTING_STATE)atoi(req.frame.msg[3].c_str()));
				myResult=RES_OK;
				setState(STATE_LIVRAISON_FIN);
					break;
		


			case STATE_DEM_FERMETURE_LIVRAISON:
				if(myNewState)
				{
					ArLog::log(ArLog::Verbose, "Entering STATE_DEM_FERMETURE_LIVRAISON\n");
					myNewState = false;
					sprintf(bufRequestIhm,"CloseForm LivraisonVC\n");
					if(myIhm.sendRequest(bufRequestIhm) == 0){
						setState(STATE_LIVRAISON_FIN);
						break;
					}
					//TODO : On peut boucler à l'infini. Faire en sorte de pouvoir en sortir
					setState(STATE_DEM_FERMETURE_LIVRAISON);
					break;
				}


			//5sec pour que la tablette ait ouvert le form
			/*if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_LIVRAISON_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_LIVRAISON);
			}*/
			break;

			case STATE_LIVRAISON_FIN:
				ArLog::log(ArLog::Verbose, "Entering STATE_LIVRAISON_FIN\n");
				switch(myResult)
				{
					case RES_OK:
						mySrma.setLoad(SRMA::UNLOADED);
						mySrma.play(SRMA::BUTTON_PRESSED);
										
						ArLog::log(ArLog::Verbose, "WorkorderRouting %s cloture\n",myWorkorderRouting.getWorkorderRoutingNo());
						myWorkorderRouting.close();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						//Mise à jour du myWorkorderRouting
						//Todo : DALRest
						//DAL::updateWorkorderRouting(myWorkorderRouting);
						//On arrete la boucle
						myRunning = false;
						break;
					case RES_FAILED:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_FAILED\n");
						myWorkorderRouting.setErrorCodeId(WorkorderRouting::ERR_TIMEOUT_OPERATION);
				
						myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
						ArLog::log(ArLog::Verbose, "WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						//Todo :: DALRest
						//DAL::updateWorkorderRouting(myWorkorderRouting);
						//On arrete la boucle
						myRunning = false;
						break;
					case RES_ERROR:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_ERROR\n");
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						myRunning = false;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						ArLog::log(ArLog::Verbose, "TraiterWorkorderRouting en RES_TIMEOUT\n");
						myWorkorderRouting.cancel();
						DALRest::updateWorkorderRouting(&myWorkorderRouting);
						myRunning = false;
						//Faire qque chose
						break;

				}
				break;

			/*-------------------------------------------------------------
			Branche WorkorderRouting myType STATE_TRANSFERTA_START
			-------------------------------------------------------------*/
			
	/*		case STATE_TRANSFERTA_START:
				printf("Entering STATE_TRANSFERTA_START\n");		
				myWorkorderRouting.start();
				printf("WorkorderRouting %s demarre\n",myWorkorderRouting.getWorkorderRoutingNo());
				DAL::updateWorkorderRouting(myWorkorderRouting);
				setState(STATE_DEM_AFFICHAGE_TRANSFERTA);			
			break;

			case STATE_DEM_AFFICHAGE_TRANSFERTA:
			if(myNewState)
			{
				printf("Entering STATE_DEM_AFFICHAGE_TRANSFERTA\n");	
				sprintf(bufRequestIhm,"Open TransfertAVC\n");
				//Envoi requete d'ouverture
				serverEvt->sendToAllClients(bufRequestIhm);
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
					if(myWorkorderRouting.isClosed())
					{
						myResult=RES_OK;
						setState(STATE_TRANSFERTA_FIN);
						break;
					}
					if(myWorkorderRouting.isCancelled())
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
					sprintf(bufRequestIhm,"Close TransfertAVC\n");
					serverEvt->sendToAllClients(bufRequestIhm);
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
					
						printf("WorkorderRouting %s cloture\n",myWorkorderRouting.getWorkorderRoutingNo());
						//Mise à jour du myWorkorderRouting
						DAL::updateWorkorderRouting(myWorkorderRouting);
						//On arrete la boucle
						myRunning = true;
						break;
					case RES_FAILED:
						printf("TraiterWorkorderRouting en RES_CHARGEMENT_FAILED\n");
						myWorkorderRouting.setErrorCodeId(WorkorderRouting::ERR_TIMEOUT_OPERATION);
				
						myWorkorderRouting.setStateId(WorkorderRouting::FAILURE);
						printf("WorkorderRouting %s annule\n",myWorkorderRouting.getWorkorderRoutingNo());
						myWorkorderRouting.cancel();
						DAL::updateWorkorderRouting(myWorkorderRouting);
						//On arrete la boucle
						myRunning = true;
						break;
					case RES_ERROR:
						printf("TraiterWorkorderRouting en RES_ERROR\n");
						myRunning = true;
						//Faire qque chose
						break;
					case RES_TIMEOUT:
						printf("TraiterWorkorderRouting en RES_TIMEOUT\n");
						myRunning = true;
						//Faire qque chose
						break;

				}

				break;*/
	}
		
}


/*void TraiterWorkorderRouting::Start(WorkorderRouting *pWorkorderRouting, SRMA *pSrma, A_ArNetServer *pServer,A_ArNetServer *pServerEvt)
{
	//serverSocket=pClientSocket;
	server=pServer;
	serverEvt=pServerEvt;
	myWorkorderRouting=pWorkorderRouting;
	srma=pSrma;
	tabletteSM=pSrma->getTabletteSM();
	lecteurCarte=pSrma->getLecteurCarte();
	robot=pSrma->getRobot();
	myIdentificationSM=IdentificationSM(pSrma,pServer,pServerEvt);
	//start();

}*/



void TraiterWorkorderRouting::stop()
{
	myRunning = false;
}

void TraiterWorkorderRouting::setup()
{
	myRunning = false;
	switch(myWorkorderRouting.getTypeLong())
	{
		case WorkorderRouting::TRANSPORT :
			setState(STATE_TRANSPORT_START);
			break;
		case WorkorderRouting::CHARGEMENT:
			setState(STATE_CHARGEMENT_START);
			break;
		case WorkorderRouting::LIVRAISON :
			setState(STATE_LIVRAISON_START);
			break;
		case WorkorderRouting::DECHARGEMENT:
			setState(STATE_DECHARGEMENT_START);
			break;
		case WorkorderRouting::TRANSFERTA:
			setState(STATE_TRANSFERTA_START);
			break;
		case WorkorderRouting::TRANSFERTB:
			setState(STATE_TRANSFERTB_START);
			break;
	}
}

void TraiterWorkorderRouting::start()
{
	myRunning = true;
	printf("TraiterWorkorderRouting loop started\n");
	while(myRunning == true)
	{
		/*if(myRunning == false)
			break;*/
		handler();		
		ArUtil::sleep(100);
	}
	printf("TraiterWorkorderRouting loop stopped\n");
}

//Accesseurs Get
TraiterWorkorderRouting::Resultat TraiterWorkorderRouting::getResultat(){return myResult;}

/*bool TraiterWorkorderRouting::mySrma.isStateFailedToGetTo(std::string nextLocationName)
{
	string resAttendu = string("Failed to get to ");
	resAttendu.append(nextLocationName);
	
	string s = string(mySrma.getOutputHandler().getStatus());
	return(resAttendu.compare(s)==0);
}

bool TraiterWorkorderRouting::mySrma.isStateFailedToGetTo(char * nextLocationName)
{
	string resAttendu = string("Failed to get to ");
	resAttendu.append(string(nextLocationName));
	string s = string(mySrma.getOutputHandler().getStatus());
	return(resAttendu.compare(s)==0);
}

bool TraiterWorkorderRouting::mySrma.isStateArrivedAt(char * nextLocationName)
{
	string resAttendu = string("Arrived at ");
	resAttendu.append(string(nextLocationName));
	string s = string(mySrma.getOutputHandler().getStatus());
	return(resAttendu.compare(s)==0);
}*/




