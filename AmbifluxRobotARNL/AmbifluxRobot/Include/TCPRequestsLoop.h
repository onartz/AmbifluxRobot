/*Treat demands from TCP requests on port 7172 
Request pattern : Request xxx xxx xx
*/

#ifndef DEF_TCPREQUESTSLOOP_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TCPREQUESTSLOOP_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "AriaUtil.h"
#include "SRMA.h"
#include "ArMutex.h"
//#include "Frame.h"
#include "TCPReceivedRequest.h"
#include <map>

#define READING_PERIOD 200


class TCPRequestsLoop : public ArASyncTask
{
public:
	//Constructeur
	//MainLoop(SRMA &);
	TCPRequestsLoop(SRMA &, Pool<TCPReceivedRequest> *);
	//Traitement réception mesg from srma
	void handleEndGotoGoal(char*);
	//~MainLoop();
	void lockMutex();
	void unlockMutex();

protected:
	//Possible values for Param1
	enum Param1Value { Cmd0,
	Cmd1,
	Cmd2
	};

	ArMutex myMutex;

	std::map<std::string, Param1Value> s_mapStringValues;

	//Callback to know when SRMA is arrived or not
	//void EndGotoGoalCB(int, ArSocket *);

	ArFunctor1C<TCPReceivedRequest, int> myHandleEndGotoGoalCB;

	/*Queue des messages arrivant de la tablette*/
	Pool<TCPReceivedRequest> * myMessagePool;
	/*Thread*/
	virtual void* runThread(void*) ; 
	/* Method to treat one workorder */
	//void TraiterRequest();
	void treatRequest(TCPReceivedRequest);
	void treatGotoGoal(TCPReceivedRequest);

	bool myRunning;
	/* Srma treats workorder */
	SRMA &mySrma;
	ArSocket * currentSock;

};
#endif