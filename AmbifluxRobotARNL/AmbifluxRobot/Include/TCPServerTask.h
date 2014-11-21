#ifndef DEF_tcpServerTask_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_tcpServerTask_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "aria.h"
#include "A_ArnetServer.h"
#include "CPPEvent.h"

class tcpServerTask : public ArASyncTask
{
public:
	enum errorCodes
	{
		ERR_NOERROR, ERR_CONNEXIONUNAVAILABLE
	};
	enum warningCodes
	{
		WARN_NOWARNING
	};
	enum statusCodes
	{
		STATUS_IDLE, STATUS_RUNNING
	};

  // constructor
	tcpServerTask();
	tcpServerTask(char*);
	tcpServerTask(A_ArNetServer *, char*);
	tcpServerTask(int, char*, bool);

	// empty destructor
	~tcpServerTask(void) {};
	//Demarrage
	void Start();
	void Start(A_ArNetServer* pArNetServer);
	//bool Open();
	void LockMutex();
	void UnlockMutex();
	//Arret
	void stop();
	//Envoi message
	void SendCommand(char *);
 

  //Accesseurs get
	A_ArNetServer *getArNetServer();
	char* getName();
	int getErrorCode();
	WORD getWarningCode();
	int getStatusCode();

	//Evenements générés
	//Lorsuq'un nouveau client se connecte
	CppEvent1<bool,char*> evtClientConnected;
	//Lorsqu'un client se deconnecte
	CppEvent1<bool,char*> evtClientDisconnected;

  virtual void * runThread(void *arg);

protected:
	A_ArNetServer *myArNetServer;  
	ArMutex myMutex;
	int portNumber;
	bool multipleConnexion;
	char myName[256];
	int myStatusCode;
	//Code Erreur
	int myErrorCode;
	//Code Warning
	WORD myWarningCode;
};
#endif