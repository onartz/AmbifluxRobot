#ifndef DEF_OBSERVATEUR_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_OBSERVATEUR_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <list>
#include <iterator>

class Observable;

typedef int Info;
//Location class
class Observateur
{
protected:
	std::list<Observable*> m_List;
	typedef list<Observable*>::iterator iterator;
	typedef list<Observable*>::const_iterator const_iterator;
public:
	virtual Info Statut(void) const=0;
	void AddObs(Observable*);
	void DelObs(Observable*);
  // Destructor, its just empty
  ~Observateur(void){};
  void Update(const Observable* observable) const;

	
};
#endif