#ifndef DEF_TABLETTEMONITORTASK_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TABLETTEMONITORTASK_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "CppEvent.h"

class Tablette;

class TabletteMonitorTask : public ArASyncTask
{
	
public:
	
  // constructor
	/*tcpServerTask(unsigned int portNumber, char* name);*/
	TabletteMonitorTask();
	TabletteMonitorTask(Tablette*);
	//Evt généré lors du chgt de connexionStatus
	//CppEvent1<bool,int> evtConnexionChanged;

	// empty destructor
	//~TabletteMonitorTask(void) {}
	//Demarrage
	void Start();
	//Arret
	void Stop();
	void LockMutex();
	void UnlockMutex();
	virtual void * runThread(void *arg);

protected:
	ArMutex myMutex;
	Tablette *tablette;
	//bool connexionStatus;
	
};
#endif