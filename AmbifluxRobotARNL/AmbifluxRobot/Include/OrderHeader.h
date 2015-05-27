#ifndef DEF_ORDERHEADER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ORDERHEADER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#define _WINSOCKAPI_ 
#include "Location.h"
#include "Person.h"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;

#define ORDERHEADER_STATUS_NEW 1
#define ORDERHEADER_STATUS_STARTED 2
#define ORDERHEADER_STATUS_CLOSED 3
#define ORDERHEADER_STATUS_CANCELLED 4

typedef Location * ptr_Location ;
typedef Person * ptr_Person;

//Commande
class OrderHeader
{
public:
// constructor
  OrderHeader();
  OrderHeader(ptree);
  //on fournit l'Id sous differentes formes
  OrderHeader(char*);
  OrderHeader(int);
    // Destructor, its just empty
  ~OrderHeader(void){};

 //Accesseurs get
 int getOrderHeaderId(void);
 char* getOrderHeaderNo(void);
 char* getOrderHeaderType(void);
 char* getObjetDemandeExpress(void);
 ptr_Location getLocation(void);
 int getLocationId(void);
 

 //Accesseurs Set
	void setOrderHeaderId(int);
	void setOrderHeaderNo(char *);
	void setOrderHeaderType(char *);
	void setObjetDemandeExpress(char *);
	void setLocation(ptr_Location);
	void setLocationId(int);
	void setPerson(ptr_Person);

	ptr_Person getDemandeur(void);
 

protected:
	int myOrderHeaderId;
	char myOrderHeaderNo[12];	
	char myOrderHeaderType[2];	
	char myObjetDemandeExpress[256];
	ptr_Person myDemandeur;
	ptr_Location myLocation;
	int myLocationId;

};
#endif