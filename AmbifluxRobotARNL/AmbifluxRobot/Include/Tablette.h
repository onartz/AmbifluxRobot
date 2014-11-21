#ifndef DEF_TabletteSM_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TabletteSM_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus


#include "CppEvent.h"
#include "A_ArNetServer.h"
#include "tcpServerTask.h"
#include "ArSocket.h"
#include "ClientTCP.h"

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/transition.hpp>

#include <boost/mpl/list.hpp>
#include <boost/config.hpp>

#ifdef BOOST_INTEL
#  pragma warning( disable: 304 ) // access control not specified
#endif

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

//Interface permettant de récupérer l'état hors de la Classe
struct IConnexionStatus
{
  virtual bool ConnexionStatus() const = false;
};


//Etat principal

struct Active;
//La StateMachine
struct TabletteSM : sc::state_machine< TabletteSM, Active >
{
	public:	
		//Fonction membre qui retourne le connexionStatus (public)
		bool ConnexionStatus() const;
  
		enum ERRORCODES
		{
			NOERR=0,
			ERROR_INIT_TCPSERVER=1,
			ERROR_INIT_TCPSERVEREVT=2,
			ERROR_CONNECTION_LOST=4
		};
	
	//Constructeur
	TabletteSM();
	//Destructeur
	//~TabletteSM();

	//envoi d'un message à la tablette
	//void SendMessage(char*);
	//Envoi d'une requete au serveur tablette
	//Utilisé pour l'ouverture de forms
	//int SendRequest(char*);

	//Accesseurs Get
	bool getConnexionStatus();
	bool getError();
	int getErrorCode();
	bool IsFirstVCOpened();
	bool IsIdentificationVCOpened();
	bool IsIdentificationFailedVCOpened();
	bool IsChargementVCOpened();
	bool IsDechargementVCOpened();
	bool IsAmbiantVCOpened();
	bool IsLivraisonVCOpened();
	bool IsTransfertAVCOpened();
	bool IsTransfertBVCOpened();
	A_ArNetServer *getServer();
	A_ArNetServer *getServerEvt();
	//ClientTCP* getClientTCP();

	//Les evenements générés pas la tablette

	CppEvent1<bool,int> evtConnexionChanged;
	CppEvent1<bool,int> evtError;
	CppEvent1<bool,int> evtNoError;
	//Evt envoyé lorsqu'une demande changement de mode arrive de la tablette
	//CppEvent1<bool,int> evtRequestMode;
	CppEvent2<bool,int,ArSocket*> evtRequestMode;
	CppEvent2<bool,int,ArSocket*> evtRequestQuitMode;
	CppEvent1<bool,int> evtFirstVCOpened;
	CppEvent1<bool,int> evtAmbiantVCOpened;
	CppEvent1<bool,int> evtChargementVCOpened;
	CppEvent1<bool,int> evtLivraisonVCOpened;
	CppEvent1<bool,int> evtTransfertAVCOpened;
	CppEvent1<bool,int> evtTransfertBVCOpened;

	CppEvent1<bool,int> evtStateChanged;


	CppEvent3<bool,char**,int,ArSocket*>evtMsgReceived;
	//CppEvent3<bool,char**,int,ArSocket*>evtConnexionReceived;


	CppEvent1<bool,int> evtDechargementVCOpened;
	CppEvent1<bool,int> evtIdentificationVCOpened;
	CppEvent1<bool,int> evtIdentificationFailedVCOpened;
	CppEvent1<bool,int> evtAmbiantVCClosed;
	CppEvent1<bool,int> evtChargementVCClosed;
	CppEvent1<bool,int> evtFirstVCClosed;
	CppEvent1<bool,int> evtLivraisonVCClosed;
	CppEvent1<bool,int> evtDechargementVCClosed;
	CppEvent1<bool,int> evtIdentificationVCClosed;
	CppEvent1<bool,int> evtTransfertAVCClosed;
	CppEvent1<bool,int> evtTransfertBVCClosed;

	//Handlers des évenements venant du serveur TCP
	bool handleTabletteDisconnected(char*);
	bool handleTabletteConnected(char*);

	//void SetConnexionStatus(bool);

	protected:
	
	/*void setIsFirstVCOpened(bool);
	void setIsIdentificationVCOpened(bool);
	void setIsChargementVCOpened(bool);
	void setIsLivraisonVCOpened(bool);
	void setIsDechargementVCOpened(bool);
	void setIsAmbiantVCOpened(bool);
	void setIsTransfertAVCOpened(bool);
	void setIsTransfertBVCOpened(bool);*/
	

	// Functor ARIA récupérant les messages du serveur TCP
	ArFunctor3C<TabletteSM, char **, int, ArSocket *> myHandleMsgReceivedCB;
	ArFunctor3C<TabletteSM, char **, int, ArSocket *> myHandleFormReceivedCB;
	ArFunctor3C<TabletteSM, char **, int, ArSocket *> myHandleConnexionReceivedCB;
	

	 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);
	// This callback is called when a "Form" message arrives
	void handleFormReceived(char **, int, ArSocket *);
	// This callback is called when a "Connexion" message arrives
	void handleConnexionReceived(char **, int, ArSocket *);
	

	//Code erreur
	//bool connectionError;
	bool error;
	int errorCode;
	char bufETablette[256];
	bool connexionStatus;
	bool isFirstVCOpened;
	bool isIdentificationVCOpened;
	bool isIdentificationFailedVCOpened;
	bool isChargementVCOpened;
	bool isDechargementVCOpened;
	bool isAmbiantVCOpened;
	bool isLivraisonVCOpened;
	bool isTransfertAVCOpened;
	bool isTransfertBVCOpened;
	//Socket TCP permettant la com avec le serveur de la tablette
	//ArSocket sock;
	//ClientTCP clientTCP;

	//Server A_ArNetServer pour la tablette
	A_ArNetServer arNetServerTablette;
	//Server A_ArNetServer pour la diffusion
	A_ArNetServer arNetServerEvt;
	//Tache de communication avec la tablette
	tcpServerTask tcpServerTablette;
	//Communication vers le serveur d evenements
	tcpServerTask tcpServerEvt;

};


//Les states
struct Init;
struct AffChoixMode;
struct AffAmbiant;
struct AffIdent;

struct TabletteDisconnected;
struct TabletteConnected;

//Les transitions internes à la machine
struct EvConnected : sc::event< EvConnected > {};
struct EvDisconnected : sc::event< EvDisconnected > {};
struct EvAffChoixMode : sc::event< EvAffChoixMode > {};
struct EvAffAmbiant : sc::event< EvAffAmbiant > {};
struct EvAffIdent : sc::event< EvAffIdent > {};
struct EvAffIdentificationFailed : sc::event< EvAffIdentificationFailed > {};
struct EvAffChargement : sc::event< EvAffChargement > {};
struct EvAffDechargement : sc::event< EvAffDechargement > {};
struct EvAffLivraison : sc::event< EvAffLivraison > {};
struct EvAffTransfertA : sc::event< EvAffTransfertA > {};
struct EvAffTransfertB : sc::event< EvAffTransfertB > {};

//-----------------------------------------------------
//Etat Active.TabletteDisconnected
//Le premier etat actif est Disconnected
//-----------------------------------------------------
struct Active : sc::state< Active, TabletteSM, TabletteDisconnected >
{
  public:
	  //EntryAction
		Active(my_context ctx);
  private:
};

//-----------------------------------------------------
//Etat Active.TabletteDisconnected
//-----------------------------------------------------
struct TabletteDisconnected : IConnexionStatus, sc::state< TabletteDisconnected, Active >
{
	public:
		// La transition EvConnected permet de passer à Connected
		typedef sc::transition< EvConnected, TabletteConnected > reactions;
		//ActionEntry
		TabletteDisconnected(my_context ctx);
		virtual bool ConnexionStatus() const;	
};

//-----------------------------------------------------
//Etat Active.TabletteConnected
//-----------------------------------------------------
struct TabletteConnected : IConnexionStatus, sc::state< TabletteConnected, Active, Init >
{
	public:
		// La transition EvDisconnected permet de passer à Disconnected
		typedef sc::transition< EvDisconnected, TabletteDisconnected > reactions;
		//ActionEntry
		TabletteConnected(my_context ctx);
		virtual bool ConnexionStatus() const;
};



//-----------------------------------------------------
//Etat Active.TabletteConnected.Init
//-----------------------------------------------------
struct Init : sc::state< Init, TabletteConnected >
{
	public:
		// La transition EvAffChoixMode permet de passer à AffChoixMode
		typedef sc::transition< EvAffChoixMode, AffChoixMode > reactions;
		//ActionEntry
		Init(my_context ctx);
		//Init();
  };

//State : ChoixMode
struct AffChoixMode : sc::state< AffChoixMode, TabletteConnected >
{
	public:
		// La transition EvAffAmbiant permet de passer à AffAmbiant
		typedef sc::transition< EvAffAmbiant, AffAmbiant > reactions;
		//ActionEntry
		AffChoixMode(my_context ctx);
  };

//State : AffAmbiant

struct AffAmbiant : sc::state< AffAmbiant, TabletteConnected >
{
	public:
		// La transition EvAffIdent permet de passer à AffIdent
		// La transition EvAffChoixMode permet de passer à AffChoixMode
		typedef mpl::list<sc::transition< EvAffIdent, AffIdent >,sc::transition< EvAffChoixMode, AffChoixMode >>reactions;
		
		//ActionEntry
		AffAmbiant(my_context ctx);
  };


//State : Affiche Identification
struct AffIdent : sc::state< AffIdent, TabletteConnected >
{
	public:
		// La transition EvAffAmbiant permet de passer à AffAmbiant
		typedef sc::transition< EvAffAmbiant, AffAmbiant > reactions;
		//ActionEntry
		AffIdent(my_context ctx);
  };



//State : Connected
// Connected est un état de Active
/*struct Connected : IConnexionStatus, sc::state< Connected, Active::orthogonal< 1 > >
{
	public:
		// La transition EvDisconnected permet de passer à Disconnected
		typedef sc::transition< EvDisconnected, Disconnected > reactions;
		//ActionEntry
		Connected(my_context ctx);
		//Methode permettant de fournir le Status
		virtual bool ConnexionStatus() const;
  };*/

//State : Disconnected
// Connected est un état de Active
/*struct Disconnected : IConnexionStatus, sc::state< Disconnected, Active::orthogonal< 1 > >
{
	public:
		typedef sc::transition< EvConnected, Connected > reactions;
		Disconnected(my_context ctx);
		virtual bool ConnexionStatus() const;   
};*/









#endif
