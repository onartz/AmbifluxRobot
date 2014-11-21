#ifndef DEF_CIRCUITTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_CIRCUITTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

//#include "Utils.h"
#include <queue>
#include <sstream>
#include "ArAsyncTask.h"
#include "ArMutex.h"
#include "AriaUtil.h"
#include "CommandeRobot.h"


//#include "Robot.h"

#include "boost/assign.hpp"
#include "boost/assign/list_of.hpp"
#include <map>
#include <string>

using namespace boost::assign;
using namespace std;

class Robot;

#define DIFF_VOLTAGE_BEFORE_DOCK	1
#define DIFF_VOLTAGE_BEFORE_UNDOCK	2


class CircuitTask: public ArASyncTask
{
public:

	//CircuitTask();
	CircuitTask(Robot*);

//Différents états
 enum State { 
	 STATE_INIT,
	 STATE_CREATE_CIRCUIT,
	 STATE_REQ_ROBOT,
	 STATE_GOTOGOAL_CLOTURE,
	 STATE_ATTENTE_FIN_GOTOGOAL,
	 STATE_WAIT,
	 STATE_WAIT_CLOTURE,
	 STATE_FIN,
	 STATE_FIN_CIRCUIT,
	 STATE_RECHARGE,
	 STATE_RECHARGE_CLOTURE,
	 STATE_CLOTURE
 };

 enum Resultat
	{
		RES_UNKNOWN,
		RES_FAILED,
		RES_OK,
		RES_TIMEOUT,
		RES_ERROR
	};

	void setState(State);
	void LockMutex();
	void UnlockMutex();

	void Start();
	void Start(Robot *);
	//void Start(Robot *, vector<string>);
	void Start(list<string>);
	void Start(vector<string>);
	void Stop();

protected:
	string nextLocationName;
	int attenteSec;
	int nbPosteParCircuit;
	ArMutex myMutex;
	int randomInt;
	State myState;
	Resultat myResult;
	bool myNewState;
	//Fin de la boucle du thread
	bool finThread;
	bool fin;

	//Dictionnaire des positions possibles
	map<int, string> locationDict;
	//Liste des commandes à envoyer au robot
	//vector<CommandeRobot> commandList;
	queue<CommandeRobot> commandQueue;
	int nbCycles;
	ArTime myStateStartTime;
	ArTime myCycleStartTime;

	Robot* robot;
	//démarrage du thread
	virtual void* runThread(void*) ; 

	void start();
	//Machine d'état
	void cycle(void);


};
#endif