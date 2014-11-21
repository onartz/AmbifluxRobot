#ifndef DEF_IHMHANDLER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_IHMHANDLER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "IhmCommunicationThread.h"

class IhmHandler
{
public:
	//constructeurs
	IhmHandler();
	//OutputHandler(ArClientBase *client);
	IhmHandler(IhmCommunicationThread *);

	//Accesseurs Get
	virtual ~OutputHandler(void); 

protected:
	 //ArFunctor1<int> *myFunc2;


 // Functor ARIA récupérant les messages du serveur TCP
	ArFunctor1C<IhmCommunicationThread, IhmCommunicationThread::Frame *> myHandleMsgReceivedCB;
		
	 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);

	
};

#endif