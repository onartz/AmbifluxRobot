/*
Loop permettant de recevoir des commandes depuis un client TCP
et de les traiter en dehors du mode Ambiant
*/

#ifndef DEF_SERVERLOOP_H
#define DEF_SERVERLOOP_H

#include "ArASyncTask.h"
#include "ArNetServer.h"
#include "MessagePool.h"
#include "TCPReceivedRequest.h"
#include "Srma.h"
#include <vector>

#define TCPPORT	7172

class ServerLoop : public ArASyncTask
{
public :
	//Constructeur
	ServerLoop(SRMA&, Pool<TCPReceivedRequest>*);
protected:
	/*Queue des messages arrivant de la tablette*/
	 // a callback for our  function
	ArFunctor3C<ServerLoop, char **, int, ArSocket *> myHandleRequestReceivedCB;
	
	void handleRequestReceived(char **, int, ArSocket *);
	
	Pool<TCPReceivedRequest> * myMessagePool;
	/*Thread*/
	virtual void* runThread(void*) ; 
	
	ArNetServer myArNetServer;
	SRMA &mySrma;	// Srma to drive
	bool myRunning; // LoopStatus

};


#endif