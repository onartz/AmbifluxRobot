#ifndef DEF_PILOTAGE_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_PILOTAGE_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "SRMA.h"

#include "SRMASurveyTask.h"
#include "TraiterWorkorderTask.h"
#include "HelpTask.h"

#include "BDTask.h"


#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/deferral.hpp>

#include <boost/mpl/list.hpp>
#include <boost/config.hpp>

#ifdef BOOST_INTEL
#  pragma warning( disable: 304 ) // access control not specified
#endif

namespace sc = boost::statechart;
namespace mpl = boost::mpl;


namespace pilotage
{
	struct EvInitOK : sc::event< EvInitOK > {};
	struct EvDemModeAmbiant : sc::event< EvDemModeAmbiant > {};
	//struct EvDemManuel : sc::event< EvDemModeManuel > {};
	struct EvDemQuitterModeAmbiant : sc::event< EvDemQuitterModeAmbiant > {};
	struct EvNouvelleDemande : sc::event< EvNouvelleDemande > {};
	struct EvDebutHorairesCircuit: sc::event< EvDebutHorairesCircuit > {};
	struct EvFinHorairesCircuit: sc::event< EvFinHorairesCircuit > {};
	struct EvFinTraitementDemande : sc::event< EvFinTraitementDemande > {};
	struct EvFinTraitementHelp : sc::event< EvFinTraitementHelp > {};
	struct EvFinTraitementProd : sc::event< EvFinTraitementProd > {};
	//Modes
	enum MODEFN
    {
        NONE=0,
        AMBIANT,
        MANUEL
    };

//Les �tats
struct Active;
struct Initialization;
struct ModeAmbiant;
struct ModeManuel;
struct AttenteDemande;
struct Circuit;
struct Repos;
struct TraitementDemande;
struct TraitementAide;

//La StateMachine
//Premier etat actif : Active
struct Pilotage : sc::state_machine< Pilotage, Active >
{
	public:	
		enum ERRORCODES
		{
			NOERR=0
		};

		//Codes des etapes en cours
	enum ETAPES_MODE
	{
		INIT_GENERALE=1,
		ATTENTE_MODE,
		DEMARRAGE_MODE_AMBIANT,
		MODE_AMBIANT,
		ARRET_MODE_AMBIANT,
		DEMARRAGE_MODE_TP,
		MODE_TP
	};


	enum TASK
    {
        STEP_UNKNOWN=-1,
        STEP_INIT,
        STEP_IDENTIFICATION_PERSONNEL,
		STEP_CHARGEMENT_MAGASIN,
		STEP_CHARGEMENT_CLIENT,
		STEP_DECHARGEMENT_MAGASIN,
		STEP_LIVRAISON_CLIENT,
		STEP_DEPLACEMENT_MAGASIN,
		STEP_DEPLACEMENT_LIVRAISON,
		STEP_DEPLACEMENT,
		STEP_ATTENTE_ORDRE,
		STEP_INACTIF
    };


	//Constructeur 2 de la StateMachine
	Pilotage(SRMA *);

	SRMA* getSrma();

	//Accesseurs get
	WorkorderRouting* getCurrentWorkorderRouting(void);
	Workorder* getCurrentWorkorder(void);
	bool getDefaut_robot();
	bool getDefaut_tablette();
	bool getDefaut_lecteur();
	bool getDefaut_BD();
	TabletteSM* getTabletteSM();
	Robot* getRobot();
	ArSocket* getClientSocket();
	A_ArNetServer* getServer();
	A_ArNetServer* getServerEvt();

	//Accesseurs set
	void setCurrentWorkorder(Workorder*);
	void setCurrentWorkorderRouting(WorkorderRouting*);

	void setDefaut_robot(bool);
	void setDefaut_tablette(bool);
	void setDefaut_lecteur(bool);
	void setDefaut_BD(bool);

	void setEtapeMode(int);
	void setMode(MODEFN);
	void setTask(TASK);

	void setStateInitialization(bool);

	//Accesseurs Get
	bool IsStateInitialization();
	/*void SendRequest(char*);*/


	//Destructeur
	//~Pilotage();

	protected:
		ArFunctorC<Robot> myHandleDisconnectedOnErrorCB;
		 /// This callback is called when error with ARNLServer
		void handleDisconnectedOnErrorCB();

		//Workorder en cours de traitement
		Workorder* currentWorkorder;
		//WorkorderRouting en cours de traitement
		WorkorderRouting* currentWorkorderRouting;

		bool defaut_general;
		bool defaut_robot;
		bool defaut_tablette;
		bool defaut_lecteur;
		bool defaut_BD;

		// This callback is called when a new message arrives
		bool handleTabletteRequestMode(int, ArSocket *);
		bool handleTabletteRequestQuitMode(int, ArSocket *);
		bool handleTabletteMsgReceived(char **, int, ArSocket *);
		//La tablette a chang� d etat (de form)
		bool handleTabletteStateChanged(int);
		bool handleTabletteConnexionChanged(int);
		bool changeMode(MODEFN);

		bool isStateInitialization;

		SRMA *srma;
		Robot *robot;
		TabletteSM *tabletteSM;
		LecteurCarte *lecteurCarte;
		ArSocket *clientSocket;
		A_ArNetServer* server;
		A_ArNetServer* serverEvt;

		TASK taskId;
		MODEFN modeId;

		int tabletteConnexionStatus;
		int etapeMode;
};




//struct Surveillance;
//State : Active
//Le premier etat actif est Initialization pour la r�gion ortho 0
//Le premier etat actif est Surveillance pour la r�gion ortho 1

struct Active : sc::state< Active, Pilotage, Initialization >
{
  public:
	  //EntryAction
		Active(my_context ctx);
		
  private:
	SRMASurveyTask srmaSurveyTask;
	SRMA *srma;
	
};

//-------------------------------------------------------------------------
// State : ModeAmbiant
// ModeAmbiant est un �tat composite de Active
// Etat par d�faut : AttenteDemande
//-------------------------------------------------------------------------
struct ModeAmbiant : sc::state< ModeAmbiant, Active, AttenteDemande >
{
	public:
		// La transition ModeAmbiant -> ModeManuel
		typedef sc::transition< EvDemQuitterModeAmbiant, ModeManuel > reactions;
		
		ModeAmbiant(my_context ctx);
		~ModeAmbiant();

		void StartTraitementDemande();
		void StartTraitementHelp();

		//Renvoie une reference modifiable sur myCurrentWorkorder
		Workorder & getCurrentWorkorder(){return myCurrentWorkorder;};
		
	private:
		
		BDTask bdTask;
		TraiterWorkorderTask traiterWorkorderTask;
		HelpTask helpTask;
		
		//TraiterAppelTask traiterAppelTask;
		ArSocket *clientSocket;


		Robot *robot;
		SRMA *srma;
		TabletteSM *tabletteSM;
		LecteurCarte *lecteurCarte;
		
		//Workorder *currentWorkorder;
		//
		Workorder myCurrentWorkorder;
		double elapsedTime_;
		
		//Handler d'evt nouveau workorder dans la base
		CppEventHandler handlerNewWorkorderFound;
		//Handler de l'evt nouveau Workorder
		bool handleNewWorkorderFound(Workorder*);
		//Handler d'evt traitementdemande termin�
		CppEventHandler handlerTraitementDemandeEnded;
		//Handler d'evt traitement aide termin�
		CppEventHandler handlerHelpEnded;
		//Handler de l'evt fin traitement demande
		bool handleTraitementDemandeEnded(int);
		//Handler de l'evt fin traitement appel
		bool handleHelpEnded(int);
  };


//-------------------------------------------------------------------------
// State : Initialization
// Etat d'attente que les conditions soient r�unies pour passer en mode Ambiant
//-------------------------------------------------------------------------
struct Initialization : sc::state< Initialization, Active >
{
	public:
		// La transition EvInitOK permet de passer � ModeAmbiant
		//typedef sc::transition< EvInitOK, AttenteMode> reactions;
		typedef sc::transition< EvInitOK, ModeAmbiant> reactions;
		// ActionEntry
		Initialization(my_context ctx);
		~Initialization();
  };


//-------------------------------------------------------------------------
// State : AttenteDemande
// Le robot est en attente de demandes
// Son etat d'entr�e est "Repos"
// des circuits ne sont r�alis�s que pendant certaines plages horaires
//-------------------------------------------------------------------------
struct AttenteDemande : sc::state< AttenteDemande, ModeAmbiant, Repos >
{
	public:
		//typedef sc::transition< EvNouvelleDemande, TraitementDemande > reactions;
		typedef sc::custom_reaction<EvNouvelleDemande> reactions;
		sc::result react(const EvNouvelleDemande &);

		//ActionEntry
		AttenteDemande(my_context ctx);
		//ActionExit
		~AttenteDemande();
		//void StartCircuit();
	protected:
		SRMA *srma;
		//CircuitTask circuitTask;
		A_ArNetServer* server;
		A_ArNetServer* serverEvt;
  };

//-------------------------------------------------------------------------
// State : Repos
// Le robot ne fait rien
//-------------------------------------------------------------------------
struct Repos : sc::state< Repos, AttenteDemande >
{
	public:
		typedef sc::transition< EvDebutHorairesCircuit, Circuit> reactions;

		//ActionEntry
		Repos(my_context ctx);
		//ActionExit
		~Repos();
		//void StartCircuit();
	protected:
		SRMA *srma;
		//CircuitTask circuitTask;
		A_ArNetServer* server;
		A_ArNetServer* serverEvt;
  };

//-------------------------------------------------------------------------
// State : Circuit
// Le robot execute des circuits dans l'AIP
//-------------------------------------------------------------------------
struct Circuit : sc::state< Circuit, AttenteDemande >
{
	public:
		typedef sc::transition< EvFinHorairesCircuit, Repos> reactions;

		//ActionEntry
		Circuit(my_context ctx);
		//ActionExit
		~Circuit();
		//void StartCircuit();
	protected:
		SRMA *srma;
		//CircuitTask circuitTask;
		A_ArNetServer* server;
		A_ArNetServer* serverEvt;
  };




//-------------------------------------------------------------------------
// State : TraitementDemande
// TraitementDemande est un �tat de ModeAmbiant
//-------------------------------------------------------------------------
struct TraitementDemande : sc::state< TraitementDemande, ModeAmbiant >
{
	public:
		// La transition EvDisconnected permet de passer � Disconnected	
		// L'�v�nemement EvDemQuitterModeAmbiant est diff�r�
		// Il est plac� dans la Queue principale lorsque l'�tat est quitt�
		typedef mpl::list<sc::transition< EvFinTraitementDemande, AttenteDemande >,
			sc::deferral<EvDemQuitterModeAmbiant>
		> reactions;
		//ActionEntry
		TraitementDemande(my_context ctx);
		//ActionExit
		~TraitementDemande();

	protected:
		SRMA *srma;
		//Workorder *workorder;
		Workorder &myWorkorder;
		//TraiterWorkorderTask traiterWorkorderTask;
  };

//-------------------------------------------------------------------------
// State : TraitementHelp
// TraitementHelp est un �tat de ModeAmbiant
//-------------------------------------------------------------------------
struct TraitementHelp : sc::state< TraitementHelp, ModeAmbiant >
{
	public:
		// La transition EvDisconnected permet de passer � Disconnected	
		// L'�v�nemement EvDemQuitterModeAmbiant est diff�r�
		// Il est plac� dans la Queue principale lorsque l'�tat est quitt�
		typedef mpl::list<sc::transition< EvFinTraitementHelp, AttenteDemande >,
			sc::deferral<EvDemQuitterModeAmbiant>
		> reactions;
		//ActionEntry
		TraitementHelp(my_context ctx);
		//ActionExit
		~TraitementHelp();

	protected:
		SRMA *srma;
		//Workorder *workorder;
		Workorder &myWorkorder;
  };

//-------------------------------------------------------------------------
// State : ModeManuel
//-------------------------------------------------------------------------
struct ModeManuel : sc::state< ModeManuel, Active >
{
	public:
		typedef sc::transition< EvDemModeAmbiant, ModeAmbiant> reactions;

		//ActionEntry
		ModeManuel(my_context ctx);
		//ActionExit
		~ModeManuel();
		//void StartCircuit();
	protected:
		SRMA *srma;
		//CircuitTask circuitTask;
		A_ArNetServer* server;
		A_ArNetServer* serverEvt;
  };


}
#endif
