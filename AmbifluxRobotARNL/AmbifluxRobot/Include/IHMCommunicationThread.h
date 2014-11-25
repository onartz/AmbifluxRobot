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
//#define HOSTIPADDRESS	"10.10.68.2"
//Local
#define HOSTIPADDRESS	"127.0.0.1"
//Portable
//#define HOSTIPADDRESS	"10.10.68.5"

#define TIMEOUT	1000
using namespace std;
	
class IhmCommunicationThread : public ArASyncTask
{
public :
	/*enum Error { 
	NoErr, NetFail, ConBadHost, ConNoRoute, 
	ConRefused, NameLookup };*/
	//Satus of response
	enum ResponseStatus{
		Unknown, OK, FAILED, BadResponse
	}; 

	//Constructor
	IhmCommunicationThread(int port, Pool<TCPReceivedRequest> *);
	virtual ~IhmCommunicationThread();
	virtual void * runThread(void *arg);
	void lockMutex();
	void unlockMutex();
	void sendRequest(char*);
	void sendRequest(char* response, char* request);
	std::string sendRequest(const char* request, bool b);
	char* frameToChar(Frame);
	//test response en fonction du protocole
	ResponseStatus testResponse(std::string response, std::string request);

	//void testCommunication();

	//Connexion à l'Ipad
	bool connect();

	//Setters
	void setMsgReceivedCallback(ArGlobalFunctor1<Frame> *);

	//Getters
	ArSocket::Error getError(void);
	bool isIhmConnected(void);
	int getConnexionStatus(void);
	
private :
	/* Liste des messages recus de la tablette */
	Pool<TCPReceivedRequest> * myMessagePool;
	size_t num_items;
	ArMutex myMutex;
	//Server for iPad as a client
	ArNetServer myServer;
	bool myRunning;
	ArSocket::Error myError;

protected :
	// functor a declencher
	//Global functor
	ArFunctor1<Frame>* functMessageReceived;
	bool myIhmCommunicationStatus;
	
	//Functors
	ArFunctorC<IhmCommunicationThread> myHandleSocketClosedCB;
	ArFunctor3C<IhmCommunicationThread,char **, int, ArSocket *> myHandleMsgReceivedCB;
 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);
	//This callback is called when the socket has been closed by server
	void handleSocketClosed();
	//Socket pour com tablette
	ArSocket mySocket;
	int myConnexionStatus;

};
#endif