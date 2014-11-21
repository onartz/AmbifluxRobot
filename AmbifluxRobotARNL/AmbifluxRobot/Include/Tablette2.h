#ifndef DEF_Tablette_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_Tablette_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "CppEvent.h"
#include "A_ArNetServer.h"
#include "tcpServerTask.h"
#include "ArSocket.h"

class Tablette : public ArASyncTask
{
public:	
	enum ERRORCODES
	{
		NOERR=0,
		ERROR_INIT_TCPSERVER=1,
		ERROR_INIT_TCPSERVEREVT=2,
		ERROR_CONNECTION_LOST=4
	};
	
	//Constructeur
	Tablette();
	//Destructeur
	virtual ~Tablette();

	void LockMutex();
	void UnlockMutex();
	//Tache asynchrone
	virtual void* runThread(void*) ; 

	//Initialisation de la com avec la tablette
	void initialize();
	//envoi d'un message à la tablette
	void SendMessage(char*);
	//Connecter la tablette
	/*bool Connecter(void);*/
	//Accesseurs Set
	//void setConnexionStatus(bool connexionStatus);
	

	//Accesseurs Get
	bool getConnexionStatus();
	bool getError();
	int getErrorCode();
;
	bool IsFirstVCOpened();
	bool IsIdentificationVCOpened();
	bool IsChargementVCOpened();
	bool IsDechargementVCOpened();
	bool IsAmbiantVCOpened();
	bool IsLivraisonVCOpened();
	bool IsTransfertAVCOpened();
	bool IsTransfertBVCOpened();

	//Les evenements

	CppEvent1<bool,int> evtConnexionChanged;
	CppEvent1<bool,int> evtError;
	CppEvent1<bool,int> evtNoError;
	//Evt envoyé lorsqu'une demande changement de mode arrive de la tablette
	CppEvent1<bool,int> evtRequestMode;
	CppEvent1<bool,int> evtRequestQuitMode;
	CppEvent1<bool,int> evtFirstVCOpened;
	CppEvent1<bool,int> evtAmbiantVCOpened;
	CppEvent1<bool,int> evtChargementVCOpened;
	CppEvent1<bool,int> evtLivraisonVCOpened;
	CppEvent1<bool,int> evtTransfertAVCOpened;
	CppEvent1<bool,int> evtTransfertBVCOpened;

	CppEvent3<bool,char**,int,ArSocket*>evtMsgReceived;

	CppEvent1<bool,int> evtDechargementVCOpened;
	CppEvent1<bool,int> evtIdentificationVCOpened;
	CppEvent1<bool,int> evtAmbiantVCClosed;
	CppEvent1<bool,int> evtChargementVCClosed;
	CppEvent1<bool,int> evtFirstVCClosed;
	CppEvent1<bool,int> evtLivraisonVCClosed;
	CppEvent1<bool,int> evtDechargementVCClosed;
	CppEvent1<bool,int> evtIdentificationVCClosed;
	CppEvent1<bool,int> evtTransfertAVCClosed;
	CppEvent1<bool,int> evtTransfertBVCClosed;



protected:
	enum ETAPES
	{
		INIT=0,
		CONNEXION,
		CONNECTEE
	};
	void setIsFirstVCOpened(bool);
	void setIsIdentificationVCOpened(bool);
	void setIsChargementVCOpened(bool);
	void setIsLivraisonVCOpened(bool);
	void setIsDechargementVCOpened(bool);
	void setIsAmbiantVCOpened(bool);
	void setIsTransfertAVCOpened(bool);
	void setIsTransfertBVCOpened(bool);
	
	ETAPES etape;
	ArMutex myMutex;
	void start();
	void stop();
	// the functor callbacks
	ArFunctor3C<Tablette, char **, int, ArSocket *> myHandleMsgReceivedCB;

	 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);

	//Code erreur
	//bool connectionError;
	bool error;
	int errorCode;
	char bufETablette[256];
	bool connexionStatus;
	//bool isConnected;
	bool isFirstVCOpened;
	bool isIdentificationVCOpened;
	bool isChargementVCOpened;
	bool isDechargementVCOpened;
	bool isAmbiantVCOpened;
	bool isLivraisonVCOpened;
	bool isTransfertAVCOpened;
	bool isTransfertBVCOpened;

	//Tache de communication avec la tablette
	tcpServerTask tcpServerTablette;
	//Server A_ArNetServer pour la tablette
	A_ArNetServer *arNetServerTablette;
	//Server A_ArNetServer pour la diffusion
	A_ArNetServer *arNetServerEvt;
	//Communication vers le serveur d evenements
	tcpServerTask tcpServerEvt;


};
#endif