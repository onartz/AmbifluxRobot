#include <iostream>
#include <set>
#include <algorithm>

class IListener
{
public:
       virtual void HandleNotification()=0;
};

class Notifier
{
protected:
       virtual void Notify();    
public:
       virtual void RegisterListener(IListener *);     
       virtual void UnregisterListener(IListener *);     

private:
       std::set<IListener*> m_listeners;

};