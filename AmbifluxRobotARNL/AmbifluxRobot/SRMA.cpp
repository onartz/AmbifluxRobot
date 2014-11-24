#include <string>
#include "srma.h"

//class TraiterWorkorderRouting;

//Accesseurs Get

bool SRMA::isStateGoingTo(string locationName)
{
	return(isStateGoingTo(locationName.c_str()));
}

bool SRMA::isStateFailedToGetTo(string locationName)
{
	return(isStateFailedToGetTo(locationName.c_str()));
}

bool SRMA::isStateFailedToGetTo(const char * locationName)
{
	string resAttendu = string("Failed to get to ");
	resAttendu.append(string(locationName));
	string s = string(myOutputHandler.getStatus());
	return(resAttendu.compare(s)==0);
}

bool SRMA::isStateArrivedAt(const char * nextLocationName)
{
	string resAttendu = string("Arrived at ");
	resAttendu.append(string(nextLocationName));
	string s = string(myOutputHandler.getStatus());
	return(resAttendu.compare(s)==0);
}

bool SRMA::isStateGoingTo(const char * nextLocationName)
{
	string resAttendu = string("Going to ");
	resAttendu.append(string(nextLocationName));
	string s = string(myOutputHandler.getStatus());
	return(resAttendu.compare(s)==0);
}

ArSoundsQueue* SRMA::getSoundsQueue(){return mySoundsQueue;};


//Constructeur

/*SRMA::SRMA(string JSONString, ArClientBase &client, OutputHandler &outputHandler, IhmCommunicationThread &ihm ):
Resource(JSONString),
myLoad(UNLOADED),
myClient(client),
myOutputHandler(outputHandler),
myIhm(ihm),
myEndGotoGoalFunctor(0)
{
}*/
SRMA::SRMA(string JSONString, ArClientBase &client, OutputHandler &outputHandler, IhmCommunicationThread &ihm, ArSoundsQueue *soundsQueue ):
Resource(JSONString),
myLoad(UNLOADED),
myClient(client),
myOutputHandler(outputHandler),
myIhm(ihm),
myEndGotoGoalFunctor(0),
mySoundsQueue(soundsQueue)
{
}

IhmCommunicationThread & SRMA::getIhmCommunicationThread(void){return myIhm;};
void SRMA::setLoad(SRMA::LOAD load){myLoad = load;};
SRMA::LOAD SRMA::getLoad(){return myLoad;};

//Demande abonnement CB
void SRMA::setCallbackEndGotoGoal(ArFunctor1<char *>* endGotoGoalFunctor){myEndGotoGoalFunctor = endGotoGoalFunctor;};

/*************************************************/
/* SendCommand : Envoi d'une commande vers ARNL */
/*************************************************/	
void SRMA::SendCommand(CommandeRobot command)
{
	//Envoi d'une commande
	ArNetPacket packet;
	
	switch(command.Type)
	{
	//Demande de déplacement vers une position
	case CommandeRobot::GOTOGOAL:
		ArLog::log(ArLog::Verbose,command.VArgs[0].c_str());
		packet.strToBuf(command.VArgs[0].c_str());
		myClient.requestOnce("gotoGoal",&packet);
		break;

	case CommandeRobot::WAIT:
		break;
	case CommandeRobot::STOP:
		myClient.requestOnce("stop",&packet);
		break;

	case CommandeRobot::AUTODOCK:
		if(command.VArgs[0].compare("ENABLE")==0)
			myClient.requestOnce("autoDockEnable",&packet);
		else
			myClient.requestOnce("autoDockDisable",&packet);
	
	}
}


OutputHandler & SRMA::getOutputHandler(){return myOutputHandler;}

void SRMA::gotoGoal(const char * goal)
{
	ArTime startTime;
	
	char msgArrivedAt[256];
	char msgFailedToGetTo[256];
	char msgGoingTo[256];
	char tmpStatus[256];

	//Initialisation des messages attendus du robot
	sprintf(msgGoingTo,"Going to %s\0",goal);
	sprintf(msgArrivedAt,"Arrived at %s\0",goal);
	sprintf(msgFailedToGetTo,"Failed to get to %s\0",goal);
	strcpy_s(tmpStatus, myOutputHandler.getStatus());

	SendCommand(CommandeRobot::CommandeRobot(CommandeRobot::GOTOGOAL, goal));

	while(true)
	{
		if(strcmp(tmpStatus, myOutputHandler.getStatus()) !=0)
			myEndGotoGoalFunctor->invoke(myOutputHandler.getStatus());

		strcpy_s(tmpStatus, myOutputHandler.getStatus());
		if(strcmp(tmpStatus, msgArrivedAt) == 0 || strcmp(tmpStatus, msgFailedToGetTo) == 0)
			break;
		ArUtil::sleep(50);
	}
	return;


	//Si le robot déjà à la position demandée, on ne fait rien
	if(isStateArrivedAt(goal)){
		myEndGotoGoalFunctor->invoke(msgArrivedAt);
	//if(strcmp(msgArrivedAt,myOutputHandler.getStatus())==0)
		//return(RET_ARRIVED_AT);
		return;
	}

	//Sinon
	/*******************************************/
	/* 1- Envoyer ordre deplacement            */
	/*******************************************/
	//Envoi ordre de deplacement
	SendCommand(CommandeRobot::CommandeRobot(CommandeRobot::GOTOGOAL, goal));	
		
	//Le robot peut être docké
	//Attente que le robot renvoie goingTo
	startTime.setToNow();
	//Déclenche le CB
	
	while(!isStateGoingTo(goal))
	{
		if(startTime.mSecSince() > 5000)
			break;
	}
	/********************************************/
	/* 1-1 Ordre non pris en compte				*/
	/* On ne doit jamais passer par là!!		*/
	/********************************************/
	if(startTime.mSecSince() > 8000)
	{
		ArLog::log(ArLog::Verbose,"Timeout ordre robot : t=%d/5000",startTime.mSecSince());
		//ArLog::log(ArLog::Verbose,"Timeout ordre robot\n");
		myEndGotoGoalFunctor->invoke("Erreur");
		return;
	}

	//int cptTimeOutDeplacement(TIMEOUT_DEPLACEMENT);

	/*************************************************/
	/* 2- Attente Arrivée robot à la position ou non */
	/*************************************************/
	myEndGotoGoalFunctor->invoke(msgGoingTo);
	while(isStateGoingTo(goal))
	{
		ArUtil::sleep(100);
	}

	/************************************************/
	/* 2-1 Robot  arrivé							*/
	/************************************************/
	if(isStateArrivedAt(goal))
	{
		myEndGotoGoalFunctor->invoke(msgArrivedAt);
		return;
	}	
	/*************************************************/
	/* 2-3 Autre message							 */
	/*************************************************/
	myEndGotoGoalFunctor->invoke(msgFailedToGetTo);
	return;
}




void SRMA::play(char* fileName)
{
	try{
		mySoundsQueue->play(fileName);
	}
	catch(std::exception ex){
		ArLog::log(ArLog::Verbose,ex.what());
	}
}

//TODO : faire un tableuau de fileNames
void SRMA::play(SOUNDS sound)
{
	try
	{
		switch(sound)
		{
			case MSG_ARRIVED:
				play(FILE_MSG_ARRIVED);
				break;
			case BUTTON_PRESSED:
				play(FILE_BUTTON_PRESSED);
				break;
			case BLIP:
				play(FILE_BLIP);
				break;
			default:
				break;
		}
	}
	catch(std::exception ex)
	{
		ArLog::log(ArLog::Verbose, ex.what());
	}
}

