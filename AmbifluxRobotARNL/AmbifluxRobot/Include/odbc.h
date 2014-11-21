#ifndef DEF_ODBC_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ODBC_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <tchar.h>
//#include "Order.h"
//#include "Resource.h"
//#include "WorkOrder.h"
  
class odbc
{
public:
	 // constructor
  //odbc();
    // Destructor, its just empty
  //~odbc(void){}
 static int connect ( _TCHAR *, _TCHAR *, _TCHAR *);
 static int disconnect(void);

 // Attributes  
HENV envHandle;  
HDBC conHandle;  
HSTMT stmtHandle;  
RETCODE rc;  

 /*int updateResourceStatus(_TCHAR *, int);
 int updateResourceStatus(int, int);
 int horodateDebut(_TCHAR *);
 int horodateFin(_TCHAR *);*/
 //Obtient les commandes en fonction du status
 /*int getOrderHeadersNo(_TCHAR *,char **, int *);
 int getOrderHeadersId(_TCHAR *,int *, int *);*/

 ////Obtient les WorkOrders en fonction du status
 //int getWorkOrders(WorkOrder* , int, int *);
 //static int updateWorkOrderStartDate(int, char*);
 //static int updateWorkOrderStartDate(WorkOrder);


 /*int getWorkOrderHeadersNo(_TCHAR *,char **, int *);
 int getWorkOrderHeadersId(_TCHAR *,int *, int *);
 int logError(_TCHAR *,_TCHAR *);*/
 ////Obtient l'emplacement de chargement de la demande
 //int getEmplChargement(_TCHAR *,char *);
 ////Obtient l'emplacement de livraison de la demande
 //int getEmplLivraison(_TCHAR *,char *);
 //int getOrderById(Order& ,int);
 //int getOrderById(Order& ,char *);
 
 //int getResourceById(Resource& ,int);
 //int getResourceById(Resource& ,char *);
 //int getResourceByName(Resource& ,char *);
 //int getUserInfos(_TCHAR *, char *, char*);
 // //Recuperation d'une valeur dans une table à partir d'un ID
 //int getValue(_TCHAR *, _TCHAR *, _TCHAR *, _TCHAR *, char*);
 //
 ////Obtient le WorkOrder correspondant a l ID passe
 //int getWorkOrderById(WorkOrder& ,int);
 ////int getWorkOrderById(WorkOrder& ,char*);

protected :
	//int insert (_TCHAR *,  _TCHAR *);
	//int select (_TCHAR *,  _TCHAR *, _TCHAR *);
	////Verification si la carte appartien bien a un personnel
	//int checkCard(_TCHAR *);
	//static int updateWorkOrderDate(int, char*, char*);
};
#endif


