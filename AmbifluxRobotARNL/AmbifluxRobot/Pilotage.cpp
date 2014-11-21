#include "Pilotage.h"

//using namespace pilotage;

struct TabletteSM;

//Constructeur par défaut
namespace pilotage
{
Pilotage::Pilotage(SRMA* pSrma):
		srma(pSrma),
		robot(pSrma->getRobot()),
		currentWorkOrder(NULL),
		currentWorkOrderRouting(NULL),
		defaut_tablette(true),
		server(pSrma->getTabletteSM()->getServer()),
		serverEvt(pSrma->getTabletteSM()->getServerEvt())
		//myHandleDisconnectedOnErrorCB(*robot,&Pilotage::handleDisconnectedOnErrorCB)
{
	CppEventHandler h1=srma->getTabletteSM()->evtRequestMode.attach(this,&Pilotage::handleTabletteRequestMode);
	CppEventHandler h2=srma->getTabletteSM()->evtRequestQuitMode.attach(this,&Pilotage::handleTabletteRequestQuitMode);
	CppEventHandler h3=srma->getTabletteSM()->evtMsgReceived.attach(this,&Pilotage::handleTabletteMsgReceived);
	CppEventHandler h4=srma->getTabletteSM()->evtConnexionChanged.attach(this,&Pilotage::handleTabletteConnexionChanged);	
	//robot->getRobotClient()->addDisconnectOnErrorCB(&myHandleDisconnectedOnErrorCB);
		}

		void Pilotage::handleDisconnectedOnErrorCB()
		{
			printf("Disconencted\r\n\r\n");
		}

	//Accesseurs get
	WorkOrderRouting* Pilotage::getCurrentWorkOrderRouting(void){return(currentWorkOrderRouting);}
	WorkOrder* Pilotage::getCurrentWorkOrder(void){return(currentWorkOrder);}
	
	bool Pilotage::getDefaut_robot(){return(defaut_robot);}
	bool Pilotage::getDefaut_tablette(){return(defaut_tablette);}
	bool Pilotage::getDefaut_lecteur(){return(defaut_lecteur);}
	bool Pilotage::getDefaut_BD(){return(defaut_BD);}

	bool Pilotage::IsStateInitialization(){return isStateInitialization;};

	TabletteSM* Pilotage::getTabletteSM(){return tabletteSM;};
	Robot* Pilotage::getRobot(){return robot;};
	SRMA* Pilotage::getSrma(){return srma;};
	A_ArNetServer* Pilotage::getServer(){return server;};
	A_ArNetServer* Pilotage::getServerEvt(){return serverEvt;};
	ArSocket* Pilotage::getClientSocket(){return clientSocket;};
	//ArSocket* Pilotage::getServerSocket(){return serverSocket;};
	
	//Accesseurs set
	void Pilotage::setCurrentWorkOrder(WorkOrder* value){currentWorkOrder=value;}
	void Pilotage::setCurrentWorkOrderRouting(WorkOrderRouting* value){currentWorkOrderRouting=value;}

	void Pilotage::setDefaut_robot(bool value){defaut_robot=value;}
	void Pilotage::setDefaut_tablette(bool value){defaut_tablette=value;}
	void Pilotage::setDefaut_lecteur(bool value){defaut_lecteur=value;}
	void Pilotage::setDefaut_BD(bool value){defaut_BD=value;}

	void Pilotage::setEtapeMode(int value)
	{
		//srma->setEtapeMode(value);
		etapeMode=value;
	}

	void Pilotage::setMode(MODEFN value){modeId = value;}
	void Pilotage::setTask(TASK value){taskId = value;}

	void Pilotage::setStateInitialization(bool value){isStateInitialization=value;}


//Handle des messages venant de la tablette sour forme d'évts
bool Pilotage::handleTabletteMsgReceived(char** msg, int nbArgs, ArSocket *clientSocket)
{
	//Attention au pointeur pourri!!
	//memcpy(this->clientSocket,clientSocket,sizeof(clientSocket));
	//this->clientSocket=clientSocket;
	//SRMA *srma=outermost_context().getSrma();
	
	printf("Client (%s) send : ",clientSocket->getIPString());
	for(int i=0;i<nbArgs;i++)
		printf("%s ",msg[i]);
	printf("\n");

	char bufETablette[256];
		//Reçu : SetObject WorkOrderRouting WorkOrderRoutingNo xxx StateId xxx StatusId xxx
		if(strcmp("SetObject",msg[0])==0)
		{
			if(nbArgs>1)
			{
				if (strcmp(msg[1],"WorkOrderRouting")==0)
					//Si c 'est le WorkorderRouting courant			
					if (strcmp(msg[3],currentWorkOrderRouting->getWorkOrderRoutingNo())==0)
						//if (strcmp(msg[3],gCurrentWorkOrderRouting->getWorkOrderRoutingNo())==0)
					{
						currentWorkOrderRouting->setStateId((WorkOrderRouting::WORKORDERROUTING_STATE)atoi(msg[5]));
						currentWorkOrderRouting->setStatusId((WorkOrderRouting::WORKORDERROUTING_STATUS)atoi(msg[7]));
						//WorkOrderRouting type Chargement + Status close
						if(atoi(msg[7])==WorkOrderRouting::CLOSE)						
							currentWorkOrderRouting->close();					
					}
			}
		}

		
		//Reçu : RequeteSRMA
		if(strcmp("RequeteSRMA",msg[0])==0)
		{
			if(nbArgs>1)
			{
				if (strcmp(msg[1],"Location")==0)
				{
					//Reçu : RequeteSRMA Location xxx
					//Attention, on passe la locationdans le currentwor!! Pas terrible a corriger
					Location* location=new Location();
					DAL::getLocationById(*location, atoi(msg[2]));
					currentWorkOrderRouting->setLocation(location);
					currentWorkOrderRouting->close();
				}
				else if (strcmp(msg[1],"Stop")==0)
				{
					currentWorkOrderRouting->close();
					//finTraiterRequeteSRMA=true;
				}
				else if (strcmp(msg[1],"Continue")==0)
				{
					currentWorkOrderRouting->close();
					//finTraiterRequeteSRMA=false;
					//reqFinTraiterRequeteSRMA=false;
				}
			}
		}

		//Reçu : GetObject Robot
		if(strcmp("GetObject",msg[0])==0)
		{
			if(nbArgs>1)
			{
				if (strcmp(msg[1],"Robot")==0)
				{
					//char buf1[256];
					printf("Recu : %s %s\n",msg[0], msg[1]);
					sprintf(bufETablette,"Object Robot IsConnectedToARNLServer %s\0",srma->getRobot()->IsConnectedToARNLServer() ? "YES" : "NO");				
					printf("Renvoye : %s\n", bufETablette);
					clientSocket->writeString(bufETablette);
					strcpy(bufETablette,"");

					//ArUtil::sleep(100);
				}
				else if (strcmp(msg[1],"SRMA")==0)
				{
					char *status=srma->getRobot()->getRobotValues().Status;

					printf("Recu : %s %s\n",msg[0], msg[1]);
					sprintf(bufETablette,"Object SRMA EtapeMode %d TaskId %d ModeId %d Status '%s'",srma->getEtapeMode(),taskId,modeId,status);				
					printf("Renvoye : %s\n", bufETablette);
					clientSocket->writeString(bufETablette);
					strcpy(bufETablette,"");
				}
			}
		}
return true;
}

//Handler de requetes mode tablette
//La tablette a envoye : SetMode xxx
bool Pilotage::handleTabletteRequestMode(int intMode, ArSocket* socket)
{
	//Attention au pointeur pourri!!
	//memcpy(clientSocket,socket,sizeof(*socket));
	clientSocket=socket;
	//Requete demande Mode Ambiant
	//Demande de changement de mode à pilotage
	changeMode((MODEFN)intMode);
	/*if(changeMode((MODEFN)intMode))
	{*/
		switch(modeId){
			case AMBIANT:
				clientSocket->writeString("Mode Ambiflux Started\n");
				
				break;
			//case XXX:
				//break;
		}
	/*}*/
	return true;
}

	//Requete demande quitter mode courant
	bool Pilotage::handleTabletteRequestQuitMode(int modeToQuit, ArSocket *sock)
	{
		switch((MODEFN)modeToQuit){
			case AMBIANT:
				changeMode(MODEFN::NONE);
				break;
		}
		return true;
	}

	bool Pilotage::handleTabletteConnexionChanged(int intConnexionStatus)
	{
		//SRMA *srma=outermost_context().getSrma();
		//On set/reset le défaut tablette de SRMA suivant l'état de la connexion
		defaut_tablette=(intConnexionStatus==0);

		/*if(intConnexionStatus==1)
			//On envoie l'evt EvInitOK
			outermost_context().process_event(EvInitOK());*/
			
		return true;
	}




	//Changement d'état de la tablette
	bool Pilotage::handleTabletteStateChanged(int state)
	{
		//process_event(EvDemQuitterModeAmbiant());
		/*if(state==0)
		else if(state==1);

		else if(state==2);*/

		//printf("Tablette State = %d\n",state);
		return true;
	}

	//Changement de mode
	bool Pilotage::changeMode(MODEFN newMode)
	{
		//Switch mode demandé
		switch(newMode){
			case AMBIANT:
				//if(modeId==NONE)
					process_event(EvDemModeAmbiant());
				break;
			case NONE:
				if(modeId==AMBIANT)
					process_event(EvDemQuitterModeAmbiant());
				break;
		}
		return true;
		//return(modeId==newMode);
	}


	//---------------------------------------------------------------------
	// State : Pilotage.Active
	// EntryAction
	//---------------------------------------------------------------------
	Active::Active(my_context ctx ) : my_base( ctx ),srma(outermost_context().getSrma()),
	srmaSurveyTask()
	{
		printf("Entering Pilotage.Active\n");		
		srmaSurveyTask.Start(&outermost_context());
	}


	//---------------------------------------------------------------------
	// State : Pilotage.Active.Initialization
	// Attente que les sous-systèmes soient OK (pas de défaut)
	// EntryAction
	//---------------------------------------------------------------------
	Initialization::Initialization(my_context ctx ) : my_base( ctx )
	{
		printf("Entering Pilotage.Active.Initialization\n");
		outermost_context().setStateInitialization(true);
		//pilotageSM->process_event(EvInitOK());
	}

	//Destructeur ModeAmbiant
	Initialization::~Initialization()
	{
		printf("Exiting Pilotage.Active.Initialization\n");
		outermost_context().setStateInitialization(false);
	}

	//---------------------------------------------------------------------
	// State : Pilotage.Active.ModeAmbiant
	// EntryAction
	//---------------------------------------------------------------------
	ModeAmbiant::ModeAmbiant(my_context ctx ) : my_base( ctx ),
	elapsedTime_(0)
	{	
		//Pilotage* pilotage=&(outermost_context());
		srma=outermost_context().getSrma();
		ArSocket *clientSocket=outermost_context().getClientSocket();
		
		printf("Entering Pilotage.Active.ModeAmbiant\n");
		//clientSocket->writeString("Mode Ambiflux Started\n");
		outermost_context().setMode(AMBIANT);
		
		//tabletteSM=srma->getTabletteSM();
		//robot=srma->getRobot();
		
		//EntryAction
		srma->setEtapeMode(3);
		//sprintf(msg,"Object SRMA EtapeMode 3 TaskId %d ModeId 1 Status '%s'\n",srma->getTaskId(), srma->getStatus());	
		//srma->EnvoyerMsg(msg);
		
		//Démarrage de la tache de scrutation de la BD
		
		handlerNewWorkorderFound = bdTask.evtNewWorkorderFound.attach(this,&ModeAmbiant::handleNewWorkorderFound);	
		handlerTraitementDemandeEnded = traiterWorkorderTask.evtTaskEnded.attach(this,&ModeAmbiant::handleTraitementDemandeEnded);
		handlerHelpEnded = helpTask.evtTaskEnded.attach(this,&ModeAmbiant::handleHelpEnded);
		
		bdTask.Start();
	}

	//Destructeur ModeAmbiant
	ModeAmbiant::~ModeAmbiant()
	{
		ArSocket *clientSocket=outermost_context().getClientSocket();
		
		//ExitAction
		//On se désabonne des évts
		bdTask.evtNewWorkorderFound.detach(handlerNewWorkorderFound);
		traiterWorkorderTask.evtTaskEnded.detach(handlerTraitementDemandeEnded);
		helpTask.evtTaskEnded.detach(handlerHelpEnded);
	

		bdTask.Stop();
		//Attente fin de la tache
		do{
			ArUtil::sleep(100);
		} while(bdTask.getRunningWithLock());
		printf("Exiting Pilotage.Active.ModeAmbiant\n");
		clientSocket->writeString("Mode Ambiflux Stopped\r\n");
		/*if(!outermost_context().getTabletteSM()->getClientTCP()->write("Mode Ambiflux Stopped\r\n"))
			printf("Error\n");*/
		ArUtil::sleep(100);
	}



	////---------------------------------------------------------------------
	//// State : Pilotage.Active.AttenteMode
	//// Attente du choix du mode par l'opérateur
	//// EntryAction
	////---------------------------------------------------------------------
	//AttenteMode::AttenteMode(my_context ctx ) : my_base( ctx )
	//{
	//	printf("Entering Pilotage.Active.AttenteMode\n");
	//	//EntryAction
	//	outermost_context().setEtapeMode(Pilotage::ATTENTE_MODE);
	//	//Démarrage du circuit predefini en attente d'une commande
	//	//outermost_context().getRobot()->StartTourAlongGoals();
	//}

	////Destructeur
	//AttenteMode::~AttenteMode()
	//{	
	//	//ExitAction
	//	printf("Exiting Pilotage.Active.ModeAmbiant.AttenteDemande\n");
	//	//Arret du circuit
	//	//outermost_context().getRobot()->StopTourAlongGoals();
	//}


	AttenteDemande::AttenteDemande(my_context ctx ) : my_base( ctx ),srma(outermost_context().getSrma())
	{
		//EntryAction
		//char msg[256];
		printf("Entering Pilotage.Active.ModeAmbiant.AttenteDemande\n");
		//SRMA *srma=outermost_context().getSrma();
		srma->setTaskId(Pilotage::STEP_ATTENTE_ORDRE);

		A_ArNetServer* server=outermost_context().getServer();
		A_ArNetServer* serverEvt=outermost_context().getServerEvt();
		Pilotage* pilotage=&(outermost_context());
		//Location* l=myCurrentWorkOrder.getOrderHeader()->getLocation();
		//circuitTask.Start(srma,server,serverEvt);	

		//Do action
		//handlerNewWorkOrderFound = attenteDemandeTask.evtNewWorkOrder.attach(this,&AttenteDemande::handleNewWorkorderFound);
		//handlerEvtTest = attenteDemandeTask.evtTest.attach(this,&AttenteDemande::handleEvtTest);
		
		//attenteDemandeTask.Start();
	}

	//Destructeur
	AttenteDemande::~AttenteDemande()
	{	
		//ExitAction
		printf("Exiting Pilotage.Active.ModeAmbiant.AttenteDemande\n");
	}

	//Reaction à l'evt EvNouvelleDemande
	// en fonction de la condition Guard
	sc::result AttenteDemande::react(const EvNouvelleDemande &)
	{
		char* type=context<ModeAmbiant>().getCurrentWorkOrder().getWorkOrderType();
		if(strcmp(type,"LIV")==0)
			return transit<TraitementDemande>();
		else if(strcmp(type,"CAL")==0)
			return transit<TraitementHelp>();
		else
			return transit<TraitementDemande>();
	}
		
	Repos::Repos(my_context ctx ) : my_base( ctx )
	{
		printf("Entering Pilotage.Active.ModeAmbiant.AttenteDemande.Repos\n");
	}
	
	//Destructeur
	Repos::~Repos()
	{	
		//ExitAction
		printf("Exiting Pilotage.Active.ModeAmbiant.AttenteDemande.Repos\n");
	}

	Circuit::Circuit(my_context ctx ) : my_base( ctx ),srma(outermost_context().getSrma())
	{
		printf("Entering Pilotage.Active.ModeAmbiant.AttenteDemande.Circuit\n");
		srma->getRobot()->StartTourAlongGoals();
	}
	
	//Destructeur
	Circuit::~Circuit()
	{	
		//ExitAction
		printf("Exiting Pilotage.Active.ModeAmbiant.AttenteDemande.Circuit\n");
		srma->getRobot()->StopTourAlongGoals();
	}

	ModeManuel::ModeManuel(my_context ctx ) : my_base( ctx ),srma(outermost_context().getSrma())
	{

		printf("Entering Pilotage.Active.ModeAmbiant.AttenteDemande.ModeManuel\n");

	}
	
	//Destructeur
	ModeManuel::~ModeManuel()
	{	
		//ExitAction
		printf("Exiting Pilotage.Active.ModeAmbiant.AttenteDemande.ModeManuel\n");
	}




	
	//Etat Surveillance version thread
	/*Surveillance::Surveillance(my_context ctx ) : my_base( ctx ),
	srma(outermost_context().getSrma()),
	srmaSurveyTask(outermost_context().getSrma())
	{
		CppEventHandler h1=srma->getTablette()->evtConnexionChanged.attach(this,&Surveillance::handleTabletteConnexionChanged);
		//CppEventHandler h1=srma->evtConnexionChanged.attach(this,&Pilotage::handleTabletteConnexionChanged);
		printf("Entering Pilotage.Active.Surveillance");
		
		srmaSurveyTask.Start(&outermost_context());
	}*/



	bool ModeAmbiant::handleNewWorkorderFound(WorkOrder *pWorkOrder)
	{
		//BD task a detecté,un nouveau WorkOrder
		myCurrentWorkOrder=WorkOrder(*pWorkOrder);		
		outermost_context().process_event(EvNouvelleDemande());	
		return true;
	}

	bool ModeAmbiant::handleTraitementDemandeEnded(int i)
	{
		outermost_context().process_event(EvFinTraitementDemande());	
		return true;
	}

	bool ModeAmbiant::handleHelpEnded(int i)
	{
		outermost_context().process_event(EvFinTraitementHelp());	
		return true;
	}

	//---------------------------------------------------------------------
	// State : Pilotage.Active.ModeAmbiant.TraitementDemande
	// EntryAction
	//---------------------------------------------------------------------
	TraitementDemande::TraitementDemande(my_context ctx ) : my_base( ctx ),
	srma(outermost_context().getSrma()),
	//workorder(context<ModeAmbiant>().getCurrentWorkOrder()),
	myWorkOrder(context<ModeAmbiant>().getCurrentWorkOrder())
	{
		//EntryAction
		printf("Entering Pilotage.Active.ModeAmbiant.TraitementDemande\n");
		//Do Action
		context<ModeAmbiant>().StartTraitementDemande();
	}

	//---------------------------------------------------------------------
	// State : Pilotage.Active.ModeAmbiant.TraitementDemande
	// ExitAction
	//---------------------------------------------------------------------
	TraitementDemande::~TraitementDemande()
	{	
		//ExitAction
		//traiterWorkorderTask.Stop();
		printf("Exiting Pilotage.Active.ModeAmbiant.TraitementDemande\n");
	}

	//---------------------------------------------------------------------
	// State : Pilotage.Active.ModeAmbiant.TraitementHelp
	// EntryAction
	//---------------------------------------------------------------------
	TraitementHelp::TraitementHelp(my_context ctx ) : my_base( ctx ),
	srma(outermost_context().getSrma()),
	myWorkOrder(context<ModeAmbiant>().getCurrentWorkOrder())
	{
		//EntryAction
		printf("Entering Pilotage.Active.ModeAmbiant.TraitementHelp\n");
		//Do Action
		context<ModeAmbiant>().StartTraitementHelp();
	}

	//---------------------------------------------------------------------
	// State : Pilotage.Active.ModeAmbiant.TraitementHelp
	// ExitAction
	//---------------------------------------------------------------------
	TraitementHelp::~TraitementHelp()
	{	
		//ExitAction
		printf("Exiting Pilotage.Active.ModeAmbiant.TraitementHelp\n");
	}


	void ModeAmbiant::StartTraitementDemande()
	{
		A_ArNetServer* server=outermost_context().getServer();
		A_ArNetServer* serverEvt=outermost_context().getServerEvt();
		Pilotage* pilotage=&(outermost_context());
		traiterWorkorderTask.Start(&myCurrentWorkOrder,pilotage,server,serverEvt);
	}

	void ModeAmbiant::StartTraitementHelp()
	{
		A_ArNetServer* server=outermost_context().getServer();
		A_ArNetServer* serverEvt=outermost_context().getServerEvt();
		Pilotage* pilotage=&(outermost_context());
		Location* l=myCurrentWorkOrder.getOrderHeader()->getLocation();
		helpTask.Start(&myCurrentWorkOrder,srma,server,serverEvt);	
		//traiterWorkorderTask.Start(&myCurrentWorkOrder,pilotage,server,serverEvt);	
	}

	/*void AttenteDemande::StartCircuit()
	{
		A_ArNetServer* server=outermost_context().getServer();
		A_ArNetServer* serverEvt=outermost_context().getServerEvt();
		Pilotage* pilotage=&(outermost_context());
		//Location* l=myCurrentWorkOrder.getOrderHeader()->getLocation();
		circuitTask.Start(srma,server,serverEvt);	
		//traiterWorkorderTask.Start(&myCurrentWorkOrder,pilotage,server,serverEvt);	
	}*/
}

