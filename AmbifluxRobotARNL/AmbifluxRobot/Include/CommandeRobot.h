#ifndef DEF_COMMANDEROBOT_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_COMMANDEROBOT_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <string>
#include <vector>
#include "boost/algorithm/string.hpp"

using namespace std;

struct CommandeRobot
{
public:
	enum COMMANDE_TYPE
	{
		GOTOGOAL,
		WAIT,
		AUTODOCK,
		DOCK,
		UNDOCK,
		CIRCUIT,
		STOP
	};

	//CommandeRobot(void);
	CommandeRobot(COMMANDE_TYPE);
	CommandeRobot(COMMANDE_TYPE,string);

	COMMANDE_TYPE Type;
	string CommandeString;
	string Args;
	vector<string> VArgs;

	/*COMMANDE_TYPE getType();
	string getArgs();
	vector<string> getVArgs();
	string toString();	*/

/*protected:
	COMMANDE_TYPE myType;
	string commandeString;
	string args;
	vector<string> vArgs;*/
};

#endif