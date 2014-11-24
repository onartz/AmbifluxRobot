#include "CommandeRobot.h"

using namespace std;


/*CommandeRobot::CommandeRobot():myType(GOTOGOAL)
{

}*/

CommandeRobot::CommandeRobot(COMMANDE_TYPE commandeType):Type(commandeType)
{
}

CommandeRobot::CommandeRobot(COMMANDE_TYPE commandeType, string pArgs):Type(commandeType),
Args(pArgs)
{
	boost::split(VArgs, Args, boost::is_any_of(" "), boost::token_compress_on);
	
}


