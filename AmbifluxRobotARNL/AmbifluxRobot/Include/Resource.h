#ifndef DEF_RESOURCE_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_RESOURCE_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include <string>
//#include <sstream>
/*#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>*/
#include "JSONParser.h"

using boost::property_tree::ptree;
using namespace std;
//Commande
class Resource
{
public:
// constructor
  Resource();
  //on fournit l'Id sous differentes formes
  //Resource(char*);
  //Resource(int);
  //A partir de JSON
	Resource(string);
	Resource(ptree);
    // Destructor, its just empty
	~Resource(void){};

//Etat général de la resource

enum STATES
    {
        READY=1,
        BUSY,
		FAULT
    };

 //Accesseurs get
string getName(void);
int getId(void);

 /*char* getResourceNo(void);
 char* getResourceType(void);
 char* getResourceName(void);
 string getCardId(void);
 int getResourceId(void);
 char* getResourceStatus(void);
 int getResourceStatusId(void);
 int getResourceStateId(void);*/

 //Accesseurs Set
 /*void setResourceNo(char *);
 void setResourceId(int);
 void setCardId(char *);
 void setResourceName(char *);
 void setResourceStatusId(int);
  void setResourceStateId(int);
 void setResourceStatus(char *);
 void setResourceType(char *);*/

protected:
	string myName;
	int myId;

	//int myResourceId;
	//char myResourceType[2];	
	////char myCardId[51];
	//char myResourceStatus[51];
	////char myResourceName[33];
	//int myResourceStatusId;
	//int myResourceStateId;
	//string myFirstName;
	//string myLastName;
	//string myCardId;
	//string myEmailAddress;

};
#endif