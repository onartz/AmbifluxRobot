#ifndef DEF_DAL_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_DAL_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include "Workorder.h"
#include "WorkorderRouting.h"
#include "OrderHeader.h"
#include "Location.h"
#include "Resource.h"
//#include "SRMA.h"
#include "sqlutil.h"
//#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class SRMA;

#define DAL_SUCCESS 1
#define WORKORDERNO_LEN 12
#define WORKORDERTYPE_LEN 4

#define WORKORDERROUTINGNO_LEN 23
#define WORKORDERROUTINGTYPE_LEN 2

#define LOCATIONNAME_LEN 14
#define LOCATIONTYPE_LEN 33

#define RESOURCETYPE_LEN 2
#define RESOURCECARDID_LEN 51
#define RESOURCENAME_LEN 33

#define ORDERHEADERORDERNO_LEN 12
#define ORDERHEADERCOMMENT_LEN 257
#define ORDERHEADEROBJETDEMANDEEXPRESS_LEN 257
#define ORDERHEADERTYPE_LEN 2

class DAL
{
public:
	// constructor
	//DAL();
	//Destructor
	//~DAL();
	static int connect(void);
	static int connect( _TCHAR *, _TCHAR *, _TCHAR *);
	static int Disconnect(void);
	static bool IsConnected(void);

	//int test(std::vector<Workorder*> & ,int);

	static int updateWorkorder(Workorder);
	static int updateResource(SRMA*);
	static int updateWorkorderRouting(WorkorderRouting *);
	static int insertWorkorderRouting(WorkorderRouting);
	static int getLocationById(Location &, int);
	static int getLocations(std::vector<Location*> &);
	static int getWorkorders(std::vector<Workorder*> &);
	static int getWorkorders(std::vector<Workorder*> & ,int);
	static std::vector<Workorder*>* DAL::getWorkorders(int status);
	//Obtient la liste des workorderRoutings d
	static int getWorkorderRoutings(std::vector<WorkorderRouting*> &, Workorder*);
	
	static int getResourceById(Resource &,int);
	static int getResourceByName(Resource &,char*);
	static int getResourceByCardId(Resource &,char*);
	static int getResourceByCardId(Resource *,char*);

	static int getOrderHeaderById(OrderHeader &, int);

protected:
	//int connect( _TCHAR *, _TCHAR *, _TCHAR *);
	BOOL connexionStatus;
};
#endif