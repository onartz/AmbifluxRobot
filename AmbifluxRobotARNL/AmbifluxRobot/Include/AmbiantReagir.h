#ifndef DEF_AMBIANTREAGIR_H //Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_AMBIANTREAGIR_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArAsyncTask.h"
#include "ArCondition.h"
#include "ArMutex.h"

#include "AmbiantCommander.h"

class SRMA;

class AmbiantReagir : public ArASyncTask
{
public:
	//Constructeurs
	AmbiantReagir(SRMA*, AmbiantCommander*);
	void Start();
	void Stop();
	//Accesseurs
protected:
	
	ArCondition condition;
	ArMutex mutex;
	SRMA *srma;
	AmbiantCommander *ambiantCommander;

	//Méthodes privées
	void tabletteConnexionLost();

	void stop();

	//Tache asynchrone
	virtual void* runThread(void*) ; 
	
};
#endif