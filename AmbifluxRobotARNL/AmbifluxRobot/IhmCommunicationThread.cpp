#include "IhmCommunicationThread.h"
#include "AmbifluxRobot.h"
#include <vector>
#include <string>

using namespace std;

//Constructors
IhmCommunicationThread::IhmCommunicationThread(int port, Pool<TCPReceivedRequest>* messagePool ):
myMessagePool(messagePool),
myHandleMsgReceivedCB(this,&IhmCommunicationThread::handleMsgReceived),
myHandleSocketClosedCB(this, &IhmCommunicationThread::handleSocketClosed)
{
		myIhmCommunicationStatus = false;
		myRunning = true;
		myServer.open(NULL,port,"ipad",true);
		myServer.squelchNormal();
		myServer.addCommand("Close",&myHandleMsgReceivedCB,"Close\n");
		myServer.addCommand("Info",&myHandleMsgReceivedCB,"Info\n");
		myServer.addCommand("Form",&myHandleMsgReceivedCB,"Formulaire ouvert\n");
		//Todo : faire pour que ça marche
		
		mySocket.setCloseCallback(&myHandleSocketClosedCB);
		//mySocket.
		//&myHandleSocketClosedCB = mySocket.getCloseCallback();
}

//Pour pouvoir s'abonner
void IhmCommunicationThread::setMsgReceivedCallback(ArGlobalFunctor1<Frame> *functorMsgReceived)
{
	functMessageReceived = functorMsgReceived;
}

	IhmCommunicationThread::~IhmCommunicationThread(){}

//Callback called when mysocket is closed
void IhmCommunicationThread::handleSocketClosed()
{
	ArLog::log(ArLog::Verbose, "Socket closed by server");
	myIhmCommunicationStatus = false;
}

//Getters
int IhmCommunicationThread::getConnexionStatus(){return myConnexionStatus;};

//Getters
bool IhmCommunicationThread::isIhmConnected(){return myIhmCommunicationStatus;}


//Thread du serveur
void *IhmCommunicationThread::runThread(void *arg)
{
	std::cout << "Thread IhmCommunicationThread running\n";
	
	while (myRunning==true)
	{
		myServer.runOnce();
		ArUtil::sleep(100);
	}

  // return out here, means the thread is done
  return NULL;
}


// This callback is called when a new message arrives
void IhmCommunicationThread::handleMsgReceived(char ** msg, int nbArgs, ArSocket *sock)
{
	ArLog::log(ArLog::Verbose, "Received : %s\n",msg[1]);
	//ThreadSafe
	myMessagePool->push(TCPReceivedRequest(sock, Frame(msg, nbArgs)));
};

	
void IhmCommunicationThread::lockMutex(){myMutex.lock();};
void IhmCommunicationThread::unlockMutex(){myMutex.unlock();};



int IhmCommunicationThread::connect()
{
	char buffR[256];
	char buffE[256];
	// The size of the string the server sent
	size_t strSize;

	if(myIhmCommunicationStatus == false)
	{
		if(!(mySocket.connect(HOSTIPADDRESS,PORT,ArSocket::TCP,0)))
			return mySocket.getError();

		myIhmCommunicationStatus = true;		

		ArLog::log(ArLog::Verbose, "Connected to the server %s", mySocket.getIPString());

		// Read data from the socket. read() will block until
		strSize=mySocket.read(buffR, sizeof(buffR));

	  // If the amount read is 0 or less, its an error condition.
		if (strSize > 0)
		{
			buffR[strSize]='\0'; // Terminate the string with a NULL character:
			ArLog::log(ArLog::Verbose, "socketClientExample: Server said: \"%s\"", buffR);
		}
		else
		{
			  ArLog::log(ArLog::Terse, "socketClientExample: Error in waiting/reading from the server.");
			return(-1);
		}
		//Cas d'un serveur ArServerBase (utilisé pour les tests)
		//On envoie le mot de passe d'abord

		if(strcmp(buffR,"Enter password:\n\r")==0)
		{
			strcpy(buffE, "ipad\n");
			if (mySocket.write(buffE, strlen(buffE)) == strlen(buffE))
				ArLog::log(ArLog::Verbose, "Envoi mot de passe ok : %s",buffE);
			  else
			  {
				  ArLog::log(ArLog::Terse, "Failed to send : %s", buffE);
				return(-1);
			  }
		}
		
	}
	return 0;
}

int IhmCommunicationThread::sendRequest(char* request)
{
	//Buffer de réception
	char buffR[256];
	//Buffer d'émission
	char buffE[256];

	string response="";

	char *ptr=NULL;
	// The size of the string the server sent
	size_t strSize;
	
	//Connexion to the server if necessary
	//while(!connect() == 0)
	//if(myIhmCommunicationStatus == false)
		//if(connect() != 0)
			//return(-1);
		
	
	//Envoi de la requete
	strcpy(buffE,request);

	if (mySocket.write(buffE, strlen(buffE)) == strlen(buffE))
		ArLog::log(ArLog::Verbose, "Envoi ok : %s",buffE);
	else
	{
		ArLog::log(ArLog::Terse, "Failed to send : %s", buffE);
		return(-1);
	}

	//Une seule lecture devrait suffire
	lockMutex();
	strSize = mySocket.read(buffR,sizeof(buffR)-1);
	unlockMutex();
	//Status OK/Failed

	char* status = strtok (buffR,"\r\n");
	char* frame = strtok ( NULL,"\r\n");
	if (frame == NULL)
		return(2);
	//Obligatoire
	strcat(frame,"\n");

	if(strcmp(status,"OK") != 0)
	{
		return(2);
	}

	//OK	
	if(strcmp(frame, request) != 0)
	{
		return(2);
	}

	return 0;
	
 
};



char* IhmCommunicationThread::frameToChar(Frame frame)
{
	/*char* ch;
	for(int i = 0; i < frame.nbArgs; i++)
	{
		strcat(ch, frame.msg[i]);
		strcat(ch, " ");
	}
	ch[sizeof(ch)-1] = '/n';
	ArLog::log(ArLog::Verbose,"Frame to char = %s\n",ch);
	return ch;*/

	char* ch;
	for (std::vector<std::string>::iterator it = frame.msg.begin() ; it != frame.msg.end(); ++it)
	{
		strcat(ch, (*it).c_str());
		strcat(ch, " ");
	}
	
	
	ch[sizeof(ch)-2] = '/0';
	//ArLog::log(ArLog::Verbose,"Frame to char = %s\n",ch);
	return ch;

};
void IhmCommunicationThread::testCommunication()
{
	queue<const char*> q;
	char lmsg[256];
	q.push("OpenForm IdentificationVC\n");
	q.push("CloseForm IdentificationVC\n");
	q.push("OpenForm ChargementVC 110 WOR00000086-20 9\n");
	q.push("CloseForm ChargementVC 110 WOR00000086-20\n");
	q.push("OpenForm LivraisonVC 110 WOR00000086-20\n");
	q.push("CloseForm LivraisonVC 110 WOR00000086-20\n");
	q.push("OpenForm IdentificationVC\n");
	q.push("CloseForm IdentificationVC\n");
	q.push("OpenForm ChargementVC 110 WOR00000086-20 9\n");
	q.push("CloseForm ChargementVC 110 WOR00000086-20\n");
	q.push("OpenForm LivraisonVC 110 WOR00000086-20\n");
	q.push("CloseForm LivraisonVC 110 WOR00000086-20\n");

	while(q.size()>0)
	{
		strcpy(lmsg, q.front());
		//sprintf(msg, q.front());
		ArLog::log(ArLog::Verbose, "msg : %s", lmsg);
		if(sendRequest(lmsg) == 0)
		{
			ArLog::log(ArLog::Verbose, "OK");
			q.pop();
		}
		else
			ArLog::log(ArLog::Verbose, "Failed");
		ArUtil::sleep(3000);
	}
	ArLog::log(ArLog::Verbose, "Fin");

	


};
	
//int IhmCommunicationThread::sendRequest(Frame frame)
//{
//	//Requete envoyée
//	char* request;
//	string response="";
//	//Buffer de réception
//	char buff[512];
//	char *ptr=NULL;
//	 // The size of the string the server sent
//	size_t strSize;
//	//ArSocket sock;
//	//Si pas de connexion
//	if(myIhmCommunicationStatus == false)
//	{
//		if(!(mySocket.connect(HOSTIPADDRESS,PORT,ArSocket::TCP,0)))
//		{
//			return 1;
//		}
//		myIhmCommunicationStatus = true;
//	
//		//Todo : Enlever
//
//		mySocket.write("ipad",sizeof("ipad"));
//		ArUtil::sleep(200);
//	}
//
//	//Création de la requête
////	sprintf(request,"GET %s%s/%d HTTP/1.1\r\nHost: %s\r\n\r\n",SERVICE,(char*)requestType.c_str(),param,HOSTWEBADDRESS);
//	//request = frameToChar(frame);
//	//frame.msg.
//	
//	//Envoi de la requete
//	sock.write(request,sizeof(request));
//
//	//lecture de la réponse (OK\r\n)
//
//	if(strcmp(sock.readString(),"OK")!=0)
//	{
//			//sock.close();
//			ArLog::log(ArLog::Verbose, "socketClientExample: Server error: \r\n%s", buff);
//			return(2);
//	}
//
//	//strSize=sock.read(buff,sizeof(buff)-1);
//	//buff[strSize]='\0';
//
//	//Réponse OK, on lit ce qui arrive 
//	while((strSize=sock.read(buff,sizeof(buff)-1))>0)
//	{
//		buff[strSize]='\0';		
//		response+=string(buff);
//	}
//
//	//Si réponse vide
//	ptr=strstr(buff,"\r\n\r\n");
//	
//	if(ptr==NULL)
//	{
//		return 3;
//	}
//
//	char *ptrFin=strstr(buff,"\r\n\r\n");
//	if(ptrFin!=NULL)
//		*(ptrFin+sizeof(char))='\0';
//
//	std::string s(ptr+sizeof("\r\n\r\n")-1);
//
//	return 0;
//};
//
//
//


//list<IhmCommunicationThread::Frame> * IhmCommunicationThread::getIncomingCmdList(){return &myIncomingCmdList;};

//}
