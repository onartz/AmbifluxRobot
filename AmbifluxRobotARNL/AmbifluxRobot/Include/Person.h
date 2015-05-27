#ifndef DEF_PERSON_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_PERSON_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

//Ne pas enlever!!!!
#include "Winsock2.h"
///////////////////
#include "Resource.h"

//SRMA h�rite de Resource
class Person : public Resource
{
public:
	//Constructeur par defaut
	Person();
	//Avec JSON
	Person(string);
	//M�thodes publiques
	string getCardId();
	string getFirstName();
	string getLastName();

protected:
	string myFirstName;
	string myLastName;
	string myCardId;
	string myEmailAddress;
};
#endif