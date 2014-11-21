#ifndef DEF_PUBLISHROBOTVALUETASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_PUBLISHROBOTVALUETASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

//#include "Robot.h"
#include "TcpServerTask.h"

class Robot;

class PublishRobotValueTask : public ArASyncTask
{
 
public:
	//Constructeurs
	//PublishRobotValueTask();
	PublishRobotValueTask(Robot *robot);
	//PublishRobotValueTask(Robot *, ArClientBase *, tcpServerTask *);	
	void Start(void);
	void Stop(void);

	//Accesseurs Get
	int getErrorCode(void);

	//Tache asynchrone
	virtual void* runThread(void*);

protected:
	ArCondition myCondition;
	ArMutex myMutex;
	Robot *robot;
	//ArNetServer
	ArNetServer myArNetServer;  
	int myErrorCode;
	//tcpServerTask *tcpServerRobot;
	//char bufETCPServerRobot[256];
	//ArClientBase *client;

};

#endif