#ifndef DEF_OBS_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_OBS_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <algorithm>

typedef int Info;
class Observable;

class IListener
{
public:
	//virtual void HandleNotification()=0;
	virtual void HandleNotification(bool value)=0;
};

class Observable
{
	std::list<IListener*> m_List;
	typedef std::list<IListener*>::iterator iterator;
	typedef std::list<IListener*>::const_iterator const_iterator;
public:
	virtual Info Statut(void) const=0;
	void AddListener(IListener*);
	void RemoveListener(IListener*);
	// Destructor, its just empty
	virtual ~Observable();

protected:
	virtual void Notify();
};




class Observateur
{
protected:
	std::list<IListener*> m_List;
	typedef std::list<IListener*>::iterator iterator;
	typedef std::list<IListener*>::const_iterator const_iterator;
	virtual ~Observateur()=0;
public:
	void AddListener(IListener*);
	void RemoveListener(IListener*);
	
};




#endif