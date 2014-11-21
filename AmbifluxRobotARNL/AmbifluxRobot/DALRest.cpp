#include "DALRest.h"

using namespace std;

/*bool DALRest::testDatabase(){
	//Requete envoyée
	char request[128];
	//Buffer de réception
	char buff[1023];
	char *ptr=NULL;
	 // The size of the string the server sent
	size_t strSize;
	ArSocket sock;
	if(!(sock.connect(HOSTIPADDRESS,PORT,ArSocket::TCP,0)))
	{
		return false;
	}
	//Création de la requête
	sprintf(request,"GET %s%s HTTP/1.1\r\nHost: %s\r\n\r\n",SERVICE,REQUEST,HOSTWEBADDRESS);
	//Envoi de la requete
	sock.write(request,sizeof(request));
	//Lecture réponse
	strSize=sock.read(buff, sizeof(buff));

	// If the amount read is 0 or less, its an error condition.
	if (strSize > 0)
	{		
		buff[strSize]='\0'; // Terminate the string with a NULL character:
		//On regarde si réponse Ok du serveur
		ptr=strstr(buff,"HTTP/1.1 200 OK");
		if(!ptr)
		{
			sock.close();
			ArLog::log(ArLog::Verbose, "socketClientExample: Server error: \r\n%s", buff);
			return false;
		}
	}
	else
	{
		ArLog::log(ArLog::Terse, "socketClientExample: Error in waiting/reading from the server.");
		sock.close();
		return(false);
	}
	//Si réponse vide
	ptr=strstr(buff,"\r\n\r\n");
	if(ptr==NULL)
	{
		return false;
	}
		
	std::string s(ptr+sizeof("\r\n\r\n")-1);
		
	//Fermeture socket
	sock.close();

	return(JSONParser::parseRobotName(s));

}

*/
bool DALRest::logSRMA()
{
	return true;
}


string DALRest::sendRequest(string requestType, int param)
{
	//Conversion int to string
	string s = static_cast<ostringstream*>( &(ostringstream() << param) )->str();
	return(sendRequest(requestType, s));
}


string DALRest::sendRequest(string requestType, string param)
{
	//Requete envoyée
	char request[128];
	string response="";
	//Buffer de réception
	char buff[512];
	const char *ptr=NULL;
	 // The size of the string the server sent
	size_t strSize;
	ArSocket sock;
	
	if(!(sock.connect(WCFHOSTIPADDRESS,WCFSERVICEPORT,ArSocket::TCP,0)))
	{
		return false;
	}
	//Création de la requête
	sprintf(request,"GET %s%s/%s HTTP/1.1\r\nHost: %s\r\n\r\n",SERVICE,requestType.c_str(),param.c_str(),HOSTWEBADDRESS);
	//Envoi de la requete
	sock.write(request,sizeof(request));

	//lecture de la réponse
	if(strcmp(sock.readString(),"HTTP/1.1 200 OK")!=0)
	{
			sock.close();
			ArLog::log(ArLog::Verbose, "socketClientExample: Server error: \r\n%s", buff);
			return(string(""));
	}

	//Réponse OK, on lit ce qui arrive 
	while((strSize=sock.read(buff,sizeof(buff)-1))>0)
	{
		buff[strSize]='\0';		
		response+=string(buff);
	}

	//Si réponse vide
	ptr=strstr(response.c_str(),"\r\n\r\n");
	
	if(ptr==NULL)
	{
		sock.close();
		return "";
	}

	char *ptrFin=strstr(&response[0],"}HTTP");
	if(ptrFin!=NULL)
		*(ptrFin+sizeof(char))='\0';

	std::string s(ptr+sizeof("\r\n\r\n")-1);
	sock.close();
	return(s);
}

string DALRest::getWorkorders(Workorder::WORKORDER_STATUS status)
{
	string str = sendRequest("workorders",int(status));
	return(str);
}


string DALRest::getEmployeeByCardId(string cardId)
{
	return(sendRequest("employeeByCardId",cardId));
}

string DALRest::getResourceById(string id)
{
	return(sendRequest("resource",id));
}

void DALRest::updateWorkorder(Workorder* w){
	char buf[256];
	sprintf(buf, "%d/%d",w->getWorkorderId(), w->getStatusId());
	string str = sendRequest("updateWorkorder", string(buf) );

}

void DALRest::updateWorkorderRouting(WorkorderRouting * wor){
	char buf[256];
	sprintf(buf, "%s/%d/%d",wor->getWorkorderRoutingNo(), wor->getStatusId(), wor->getStateId());
	string str = sendRequest("updateWorkorderRouting", string(buf) );
	
}

