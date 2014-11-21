#ifndef DEF_ROBOT_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ROBOT_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "ArNetworking.h"
#include "ArCepstral.h"

#include "RobotValues.h"
#include "CommandeRobot.h"
#include "CircuitTask.h"

//class CircuitTask;

class Robot
{
public:
   //Constructeurs
	Robot(ArClientBase*);

	//Destructeur
	virtual ~Robot(void);

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
  /// This callback is called when error with ARNLServer
  //void handleDisconnectedOnErrorCB();


	void SendCommand(CommandeRobot);
	//Commandes de haut niveau
	//Realisation d'un circuit aleatoire parmi des goals
	//Goals prédéfinis
	void StartTourAlongGoals();
	//Liste de goals : goal1 goal2 goal3...
	void StartTourAlongGoals(string pStrGoals);
	//Pareil avec list<string>
	void StartTourAlongGoals(std::list<string>);

	void StopTourAlongGoals();

	bool IsConnectedToARNLServer();
	ArClientBase* getRobotClient();
	RobotValues getRobotValues();

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

	bool IsTouringAlongGoals();

	bool isVoiceOn();
	void setIsTouringAlongGoals(bool);

	void setVoltageIsStateOfCharge(bool);
	void setVoiceOn(bool);

	void saySomething(char*);
	
protected:

  ArFunctor1C<Robot, ArNetPacket *> myHandleOutputCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleOutputNumbersCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleOutputStringsCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleBatteryInfoCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandlePhysicalInfoCB;
  ArFunctor1C<Robot, ArNetPacket *> myHandleTemperatureInfoCB;
  

  CircuitTask circuitTask;
  ArCepstral cepstral;

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

	bool voltageIsStateOfCharge;

	bool voiceOn;

	bool touringAlongGoals;

	//Structure contenant les valeurs du robot
	RobotValues robotValues;

	  //Client de ArnlServer
	  ArClientBase *myClient;

	  //Taches
	  void preparerCircuit(vector<string>);
	  
	  void cloturerCircuit();

	  vector<string> circuitLocations;
	 
	  /// A header for the columns in the data printout is sometimes printed
	  bool myNeedToPrintHeader;
	  /// Don't print any information until we get the battery info
	  bool myGotBatteryInfo;
};
#endif
