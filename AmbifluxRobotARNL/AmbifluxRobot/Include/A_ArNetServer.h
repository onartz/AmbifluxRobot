#ifndef DEF_A_ARNETSERVER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_A_ARNETSERVER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArNetServer.h"

//A_ArNetServer hérite de ArNetServer
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