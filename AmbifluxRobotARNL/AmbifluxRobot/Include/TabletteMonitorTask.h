#ifndef DEF_TABLETTEMONITORTASK_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_TABLETTEMONITORTASK_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

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
	//Evt g�n�r� lors du chgt de connexionStatus
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