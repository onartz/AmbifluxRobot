#ifndef DEF_CIRCUITTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_CIRCUITTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

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

//Diff�rents �tats
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
	//Liste des commandes � envoyer au robot
	//vector<CommandeRobot> commandList;
	queue<CommandeRobot> commandQueue;
	int nbCycles;
	ArTime myStateStartTime;
	ArTime myCycleStartTime;

	Robot* robot;
	//d�marrage du thread
	virtual void* runThread(void*) ; 

	void start();
	//Machine d'�tat
	void cycle(void);


};
#endif