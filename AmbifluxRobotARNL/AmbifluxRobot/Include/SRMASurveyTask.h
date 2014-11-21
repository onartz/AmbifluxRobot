#ifndef DEF_SRMASURVEYTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_SRMASURVEYTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"
#include "DALRest.h"
//#include "Pilotage.h"


class SRMA;


namespace pilotage
{
struct Pilotage;

#define PERIODE_TEST_DATABASE 30
#define PERIODE_LOGSRMA	60

class SRMASurveyTask : public ArASyncTask
{
public:
	//Constructeur
	SRMASurveyTask();
	SRMASurveyTask(SRMA *);
	void Start();
	void Start(Pilotage *);
	void Stop();

protected:
	ArCondition myCondition;
	ArMutex myMutex;
	ArTime testDatabaseStartTime;
	ArTime logSRMAStartTime;
	SRMA *mySrma;
	Pilotage *pilotageSM;
	//TabletteSM *tablette;
	
	//Tache asynchrone
	virtual void* runThread(void*) ; 
};
}

#endif