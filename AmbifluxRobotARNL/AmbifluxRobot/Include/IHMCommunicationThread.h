/*
Classe qui gere la communication avec la tablette
Les trames recues sont placées dans une liste de frames

Emission de requetes vers la tablette par sock.write

Types de requetes : Open/Close, GetActiveForm
OpenForm FormName\r\n\r\n
CloseForm FormName\r\n\r\n
GetActiveForm\r\n\r\n

Réponses attendues
OK\r\nOpenForm FirstVC\r\n\r\n
FAILED\r\nOpenForm FirstVC\r\n\r\n
OK\r\nCloseForm FirstVC\r\n\r\n
FAILED\r\nCloseForm FirstVC\r\n\r\n
OK\r\nGetActiveForm FormName\r\n\r\n
FAILED\r\nGetActiveForm\r\n\r\n


*/
#ifndef DEF_IhmCommunicationThread_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_IhmCommunicationThread_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <map>
#include <list>
#include <vector>
#include <iostream>
#include "ArNetServer.h"
#include "ArAsyncTask.h"
#include "ArMutex.h"
#include "MessagePool.h"
#include "TCPReceivedRequest.h"
#include "ArSocket.h"

//Port de com avec la tablette
//Todo : modifier pour port reel tablette
#define PORT	7474
//IPaddress of Tablette
//Todo : modifier pour adresse reelle tablette
//iMAc
//#define HOSTIPADDRESS	"10.10.68.127"
//ipad
#define HOSTIPADDRESS	"10.10.68.2"
//Local
//#define HOSTIPADDRESS	"127.0.0.1"
//Portable
//#define HOSTIPADDRESS	"10.10.68.5"

#define TIMEOUT	1000


using namespace std;

//namespace Ambiflux
//{
	
class IhmCommunicationThread : public ArASyncTask
{
public :
	// Définit un type de structure
	//struct Frame
	//{
	//public : 
	//	Frame();
	//	Frame(char**, int);
	//	std::vector<std::string> msg;
	//	//char* msg[32];
	//	//int nbArgs;
	//};
	
	/*IhmCommunicationThread(list<Frame> *, ArMutex *mutex);*/
	IhmCommunicationThread(int port, Pool<TCPReceivedRequest> *);
	IhmCommunicationThread(int port);
	virtual ~IhmCommunicationThread();
	virtual void * runThread(void *arg);
	//list<Frame> * getIncomingCmdList();
	void lockMutex();
	void unlockMutex();
	void setCallback(ArGlobalFunctor1<Frame> *);
	//int sendRequest(Frame);
	int sendRequest(char*);
	char* frameToChar(Frame);
	
	bool isIhmConnected();
	//list<Frame>& getIncomingCmdList();
	
	void testCommunication();


private :
/* Liste des messages recus de la tablette */
	
	//Frame myFrame;
	Pool<TCPReceivedRequest> * myMessagePool;
	size_t num_items;

	//list<Frame> myIncomingCmdList;
	ArMutex myMutex;
	ArNetServer myServer;
	bool myRunning;

	
protected :
	// functor a declencher
	//Global functor
	ArFunctor1<Frame>* functMessageReceived;
	bool myIhmCommunicationStatus;
	
	//Socket closed by the server Callback
	ArFunctorC<IhmCommunicationThread> myHandleSocketClosedCB;
	void handleSocketClosed();

	ArFunctor3C<IhmCommunicationThread,char **, int, ArSocket *> myHandleMsgReceivedCB;
	//ArFunctor3C<IhmCommunicationThread,char **, int, ArSocket *> myHandleFormReceivedCB;
	//ArFunctor3C<IhmCommunicationThread,char **, int, ArSocket *> myHandleConnexionReceivedCB;
 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);
	//Socket pour com tablette
	ArSocket mySocket;

	int connect();

	
	
	
	// This callback is called when a "Form" message arrives
	//void handleFormReceived(char **, int, ArSocket *);
	// This callback is called when a "Connexion" message arrives
	//void handleConnexionReceived(char **, int, ArSocket *);
	//void addToIncomingCmd(Frame);

};
//}
#endif