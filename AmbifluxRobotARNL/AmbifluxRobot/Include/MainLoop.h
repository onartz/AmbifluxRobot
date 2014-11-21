#ifndef DEF_MAINLOOP_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_MAINLOOP_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "AriaUtil.h"
#include "SRMA.h"
#include "Workorder.h"
#include "DALRest.h"
#include "TraiterWorkorderRouting.h"
#include "TCPReceivedRequest.h"

#define LOOP_PERIOD 5000


class MainLoop : public ArASyncTask
{
public:
	//Constructeur
	//MainLoop(SRMA &);
	MainLoop(SRMA &, Pool<TCPReceivedRequest> *);
	//~MainLoop();

protected:
	/*Queue des messages arrivant de la tablette*/
	Pool<TCPReceivedRequest> * myMessagePool;
	/*Thread*/
	virtual void* runThread(void*) ; 
	/* Method to treat one workorder */
	void TraiterWorkorder(Workorder &);
	bool myRunning;
	/* Srma treats workorder */
	SRMA &mySrma;

};
#endif