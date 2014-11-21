#ifndef DEF_OBSERVABLE_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_OBSERVABLE_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <list>
#include <iterator>

#include "Observateur.h"

typedef int Info;
//Location class
class Observable
{
	std::list<Observateur*> m_List;
	typedef list<Observateur*>::iterator iterator;
	typedef list<Observateur*>::const_iterator const_iterator;
public:
	virtual Info Statut(void) const=0;
	void AddObs(Observateur*);
	void DelObs(Observateur*);
  // Destructor, its just empty
  ~Observable(void){};

protected:
	void Notify(void);
};
#endif