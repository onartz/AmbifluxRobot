#ifndef DEF_ROBOTSM_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ROBOTSM_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "ArNetworking.h"
#include "strutil.h"
#include "resource.h"
#include "CppEvent.h"
#include "ArArgumentBuilder.h"
#include "ArArgumentParser.h"
#include "ArNetServer.h"
#include "MovingTask.h"
#include "RobotValues.h"

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


namespace robotStateMachine
{

#define ERRORCODE_INIT_ARNL 1

//Les states
struct Active;

//States de Active
struct ModeStop;
struct ModeGotoGoal;
struct ModeDock;

struct BatteryOK;
struct BatteryLow;

/*struct Disconnected;
struct Connected;*/

//States de ModeGotoGoal
struct GoingTo;
struct ArrivedAt;
struct FailedToGetTo;

//States de ModeDock
struct GoingToDock;
struct DrivingIntoDock;
struct Docked;
struct Undocking;

//Les transitions internes à la machine

struct EvModeStop: sc::event< EvModeStop > {};
struct EvModeGotoGoal: sc::event< EvModeGotoGoal > {};
struct EvGoingTo: sc::event< EvGoingTo > {};
struct EvArrivedAt: sc::event< EvArrivedAt > {};
struct EvFailedToGetTo: sc::event< EvFailedToGetTo > {};

struct EvModeDock: sc::event< EvModeDock > {};
struct EvGoingToDock: sc::event< EvGoingToDock > {};
struct EvDrivingIntoDock: sc::event< EvDrivingIntoDock > {};
struct EvDocked: sc::event< EvDocked > {};
struct EvUndocking: sc::event< EvUndocking > {};

struct EvBatteryOK: sc::event< EvBatteryOK > {};
struct EvBatteryLow: sc::event< EvBatteryLow > {};

/*struct EvDisconnected: sc::event< Disconnected > {};
struct EvConnected: sc::event< Connected > {};*/

//The stateMachine
struct Robot : sc::state_machine< Robot, Active >
{
public:	
	//Constructeur
	Robot();
	Robot(ArClientBase *);
	//Destructeur
	~Robot();

	//Deplacement vers une position
	int GotoGoal(const char *);
	int GotoGoal(char *);
	void Stop(void);
	void Connect(void);
	void SetAutodock(bool);
	
  /// This callback is called when an update on general robot state arrives
  void handleOutputNumbers(ArNetPacket *packet);
  /// This callback is called when an update on general robot state arrives
  void handleOutputStrings(ArNetPacket *packet);
  /// This callback is called when an update on the battery configuration changes
  void handleBatteryInfo(ArNetPacket *packet);
  /// This is called when the physical robot information comes back
  void handlePhysicalInfo(ArNetPacket *packet);
  /// This callback is called when an update on the temperature information changes
  void handleTemperatureInfo(ArNetPacket *packet);


	//Les evenements générés pas la StateMachine
	//Perte de connexion
	CppEvent1<bool,int> evtConnexionChanged;
	//Changement de Mode
	CppEvent1<bool,char* const> evtModeChanged;
	//Changement de Status
	CppEvent1<bool,char* const> evtStatusChanged;


	//Accesseurs Set
	void setBatteryInfos(double);
	void setVel(double);
	void setX(double);
	void setY(double);
	void setTh(double);
	void setRotVel(double);
	void setLatVel(double);
	void setVoltage(double);
	void setTemperature(double);
	void setStatus(char * );
	void setMode(char * );
	void setVoltageIsStateOfCharge(bool);

	void SetStateActive(bool);
	void SetStateModeStop(bool);
	void SetStateGotoGoal(bool);
	void SetStateModeGoingTo(bool);
	void SetStateModeDock(bool);
	void SetStateBatteryOK(bool);
	void SetStateBatteryLow(bool);

	//States de ModeGotoGoal
	void SetStateGoingTo(bool);
	void SetStateArrivedAt(bool);
	void SetStateFailedToGetTo(bool);

	//States de ModeDock
	void SetStateGoingToDock(bool);
	void SetStateDrivingIntoDock(bool);
	void SetStateDocked(bool);
	void SetStateUndocking(bool);
	

	//Accesseurs get
	RobotValues* getRobotValues();

	double getBatteryInfos();
	int getErrorCode();
	double getVel();
	double getX();
	double getY();
	double getTh();
	double getRotVel();
	double getLatVel();
	double getTemperature();
	bool getVoltageIsStateOfCharge();
	double getVoltage();
	char * getStatus();
	char * getMode();
	bool IsConnectedToARNLServer();
	ArClientBase getRobotClient();
	ArNetServer *getArNetServer();

	bool IsStateActive();
	bool IsStateModeStop();
	bool IsStateModeGoingTo();
	bool IsStateModeDock();
	bool IsStateBatteryOK();
	bool IsStateBatteryLow();

	//States de ModeGotoGoal
	bool IsStateGoingTo();
	bool IsStateArrivedAt();
	bool IsStateArrivedAt(const char *);
	bool IsStateFailedToGetTo(const char *);
	bool IsStateFailedToGetTo();

	//States de ModeDock
	bool IsStateGoingToDock();
	bool IsStateDrivingIntoDock();
	bool IsStateDocked();
	bool IsStateUndocking();

protected:

	RobotValues robotValues;
	//Bools des états pour synchro
	bool stateActive;
	bool stateModeStop;
	bool stateModeGoingTo;
	bool stateModeDock;
	bool stateBatteryOK;
	bool stateBatteryLow;

	//States de ModeGotoGoal
	bool stateGotoGoal;
	bool stateGoingTo;
	bool stateArrivedAt;
	bool stateFailedToGetTo;

	//States de ModeDock
	bool stateGoingToDock;
	bool stateDrivingIntoDock;
	bool stateDocked;
	bool stateUndocking;

	//void connexion();
	void disableAutodock(void);
	void enableAutodock(void);


	// Functor ARIA récupérant les messages du serveur TCP
	ArFunctor1C<Robot, ArNetPacket *> myHandleOutputCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleOutputNumbersCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleOutputStringsCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleBatteryInfoCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandlePhysicalInfoCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleTemperatureInfoCB;
	 // This callback is called when a new message arrives
	void handleMsgReceived(char **, int, ArSocket *);
	// This callback is called when a "Form" message arrives
	void handleFormReceived(char **, int, ArSocket *);
	// This callback is called when a "Connexion" message arrives
	void handleConnexionReceived(char **, int, ArSocket *);



	//PublishRobotValueTask publishRobotValueTask;
//Renvoie si la connexion avec ARNL Server est OK ou non
	bool isConnectedToARNLServer;
  //CodeErreur
	int errorCode;
	double myBatteryInfos;
	  double myX;
	  double myY;
	  double myZ;
	  double myTh;
	  double myVel;
	  double myRotVel;
	  double myLatVel;
	  bool myVoltageIsStateOfCharge;
	  char myTemperature;
	  double myVoltage;
	  char myStatus[256];
	  char myMode[256];
	  //Client de ArnlServer
	  ArClientBase *myClient;
	  //Serveur de diffusion des valeurs du robot
	  ArNetServer myArNetServer;
	  //OutputHandler *myOutputHandler;
	 
	  /// A header for the columns in the data printout is sometimes printed
	  bool myNeedToPrintHeader;
	  /// Don't print any information until we get the battery info
	  bool myGotBatteryInfo;
	


};


//------------------------------------------------------------------------
//State : Active
//Le premier etat actif est Init
//------------------------------------------------------------------------

struct Active : sc::state< Active, Robot, mpl::list< ModeStop, BatteryOK  >>
{
  public:
	  //EntryAction
		Active(my_context ctx);
		//ExitAction
		~Active();
  private:
	  MovingTask movingTask;
};

//------------------------------------------------------------------------
//State : ModeStop
//Le premier etat actif est ModeStop
//------------------------------------------------------------------------
struct ModeStop : sc::state< ModeStop, Active::orthogonal< 0 > >
{
	public:
		// La transition EvModeDock permet de passer à ModeDock
		// La transition EvModeGotoGoal permet de passer à ModeGotoGoal
		typedef mpl::list<sc::transition< EvModeDock, ModeDock >,sc::transition< EvModeGotoGoal, ModeGotoGoal >>reactions;
		
		//ActionEntry
		ModeStop(my_context ctx);
		~ModeStop();
  };

//------------------------------------------------------------------------
//State : ModeGotoGoal
// State composite dont GoingTo est le premier state actif
//------------------------------------------------------------------------
struct ModeGotoGoal : sc::state< ModeGotoGoal, Active::orthogonal< 0 >, GoingTo >
{
	public:
		// La transition EvArrivedAt permet de passer à ArrivedAt
		// La transition EvFailedToGetTo permet de passer à FailedToGetTo
		typedef mpl::list<sc::transition< EvModeStop, ModeStop >,sc::transition< EvModeDock, ModeDock >>reactions;
		
		//ActionEntry
		ModeGotoGoal(my_context ctx);
		~ModeGotoGoal();
  };

//------------------------------------------------------------------------
//State : GoingTo
// State de ModeGotoGoal
//------------------------------------------------------------------------
struct GoingTo : sc::state< GoingTo, ModeGotoGoal>
{
  public:
	  //typedef sc::transition< EvArrivedAt, ArrivedAt >reactions;
	  typedef mpl::list<sc::transition< EvArrivedAt, ArrivedAt >,sc::transition< EvFailedToGetTo, FailedToGetTo >>reactions;
		
		
	  //EntryAction
		GoingTo(my_context ctx);
		//ExitAction
		~GoingTo();
  private:
		//DoAction
		//MovingTask movingTask;
};

//------------------------------------------------------------------------
//State : ArrivedAt
// State de ModeGotoGoal
//------------------------------------------------------------------------
struct ArrivedAt : sc::state< ArrivedAt, ModeGotoGoal>
{
  public:
	   typedef sc::transition< EvGoingTo, GoingTo >reactions;
	  //EntryAction
		ArrivedAt(my_context ctx);
		~ArrivedAt();
  private:
};

//------------------------------------------------------------------------
//State : FailedToGetTo
// State de ModeGotoGoal
//------------------------------------------------------------------------
struct FailedToGetTo : sc::state< FailedToGetTo, ModeGotoGoal>
{
  public:
	   typedef sc::transition< EvGoingTo, GoingTo >reactions;
	  //EntryAction
		FailedToGetTo(my_context ctx);
		~FailedToGetTo();
  private:
};

//------------------------------------------------------------------------
//State : ModeDock
// State composite dont GoingToDock est le premier state actif
//------------------------------------------------------------------------
struct ModeDock : sc::state< ModeDock, Active::orthogonal< 0 >, GoingToDock >
{
	public:
		// La transition EvArrivedAt permet de passer à ArrivedAt
		// La transition EvFailedToGetTo permet de passer à FailedToGetTo
		typedef mpl::list<sc::transition< EvModeGotoGoal, ModeGotoGoal >>reactions;
		
		//ActionEntry
		ModeDock(my_context ctx);
		~ModeDock();
  };


//------------------------------------------------------------------------
//State : GoingToDock
// State de ModeDock
//------------------------------------------------------------------------
struct GoingToDock : sc::state< GoingToDock, ModeDock >
{
	public:
		// La transition EvArrivedAt permet de passer à ArrivedAt
		// La transition EvFailedToGetTo permet de passer à FailedToGetTo
		typedef sc::transition< EvDrivingIntoDock, DrivingIntoDock >reactions;
		
		//ActionEntry
		GoingToDock(my_context ctx);
		~GoingToDock();
	private:
		//DoAction
		//MovingTask movingTask;
  };

//------------------------------------------------------------------------
//State : DrivingIntoDock
// State de ModeDock
//------------------------------------------------------------------------
struct DrivingIntoDock : sc::state< DrivingIntoDock, ModeDock >
{
	public:
		
		typedef sc::transition< EvDocked, Docked >reactions;
		
		//ActionEntry
		DrivingIntoDock(my_context ctx);
		~DrivingIntoDock();
  };

//------------------------------------------------------------------------
//State : Docked
// State de ModeDock
//------------------------------------------------------------------------
struct Docked : sc::state< Docked, ModeDock >
{
	public:	
		typedef sc::transition< EvUndocking, Undocking >reactions;
		
		//ActionEntry
		Docked(my_context ctx);
		~Docked();
  };
//------------------------------------------------------------------------
//State : Undocking
// State de ModeDock
//------------------------------------------------------------------------
struct Undocking : sc::state< Undocking, ModeDock >
{
	public:	
		//typedef sc::transition< EvUndocking, Undocking >reactions;
		
		//ActionEntry
		Undocking(my_context ctx);
		~Undocking();
  };

//------------------------------------------------------------------------
//State : BatteryOK
//------------------------------------------------------------------------
struct BatteryOK : sc::state< BatteryOK, Active::orthogonal< 1 >>
{
	public:
		
		typedef sc::transition< EvBatteryLow, BatteryLow >reactions;
		
		//ActionEntry
		BatteryOK(my_context ctx);
		~BatteryOK();

  };

//------------------------------------------------------------------------
//State : BatteryLow
//------------------------------------------------------------------------
struct BatteryLow : sc::state< BatteryLow, Active::orthogonal< 1 >>
{
	public:
		
		typedef sc::transition< EvBatteryOK, BatteryOK >reactions;
		
		//ActionEntry
		BatteryLow(my_context ctx);
		~BatteryLow();

  };
}

#endif
