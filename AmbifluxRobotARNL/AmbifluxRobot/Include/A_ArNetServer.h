#ifndef DEF_A_ARNETSERVER_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_A_ARNETSERVER_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArNetServer.h"

//A_ArNetServer h�rite de ArNetServer
class A_ArNetServer:public ArNetServer
{
public:
	//constructeur
	A_ArNetServer();
	//Accesseurs
	std::list<ArSocket *> getConns(void);
	std::list<ArSocket *> getConnectingConns(void);
	std::list<ArSocket *> getDeleteList(void);
	//Constructeur
	//SRMA(void);
	//Destructeur
	//virtual ~SRMA(void);
};
#endif