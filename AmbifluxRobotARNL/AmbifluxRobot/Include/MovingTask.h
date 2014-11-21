#ifndef DEF_MOVINGTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_MOVINGTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
#include "ArNetServer.h"
#include "RobotValues.h"

namespace robotStateMachine{

	//struct RobotValues;

class MovingTask : public ArASyncTask
{
public:
	//Constructeurs
	MovingTask();	
	// empty destructor
	~MovingTask(void);

	int Start(RobotValues*, ArNetServer*);
	void Stop(void);
	//Tache asynchrone
	virtual void* runThread(void*);

	

protected:
	ArCondition myCondition;
	ArMutex myMutex;

	ArNetServer *arNetServer;
	RobotValues *robotValues;

};
}

#endif