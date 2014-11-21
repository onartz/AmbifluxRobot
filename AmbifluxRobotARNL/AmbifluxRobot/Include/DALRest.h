#ifndef DEF_DALREST_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_DALREST_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#define _WINSOCKAPI_ 

//#include "SRMA.h"
#include "ArSocket.h"
#include "JSONParser.h"
#include "Workorder.h"

using namespace std;

//#define HOST	193.55.104.245	//aip-sqlaipl
#define WCFSERVICEPORT	80
#define WCFHOSTIPADDRESS	"193.55.104.238"
#define HOSTWEBADDRESS	"193.55.104.238"
#define HOSTURL		"http://193.55.104.238"
#define SERVICE	"/AmbifluxWcfService/Ambiflux.svc/json/"
#define REQUEST	"srma"
#define REQ_LOGSRMA	"logSRMA"
#define REQ_WORKORDERS	"workorders"

#define TIMEOUT	1000

class DALRest
{
public:
	
	//static int updateResource(SRMA*);
	//static bool testDatabase();
	//Ecrit présence SRMA dans la base
	static bool logSRMA();
	//Update incoming workorderList with those whose status is specified
	//static int getWorkorders(list<Workorder> &, Workorder::WORKORDER_STATUS);
	static string getWorkorders(Workorder::WORKORDER_STATUS);
	static string getEmployeeByCardId(string);
	static string getResourceById(string);
	static void updateWorkorderRouting(WorkorderRouting*);
	static void updateWorkorder(Workorder*);


private :
	//Send request to the server
	static string sendRequest(string req, int param);
	static string sendRequest(string req, string param);

};

#endif