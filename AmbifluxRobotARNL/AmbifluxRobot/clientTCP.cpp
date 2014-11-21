#include "clientTCP.h"

//Par défaut
ClientTCP::ClientTCP():host(DEFAULT_HOST),port(DEFAULT_PORT){}

ClientTCP::ClientTCP(const char *pHost, int pPort):host(pHost),port(pPort){
if(!sock.connect(pHost,pPort,ArSocket::TCP))
		ArLog::log(ArLog::Normal,"Connexion impossible\n");
	else
		ArLog::log(ArLog::Normal,"Connexion OK\n");
}

bool ClientTCP::connect()
{
	if(port>0 && strcmp(host,"")!=0)
	{
		if(!sock.connect(host,port,ArSocket::TCP))
			ArLog::log(ArLog::Normal,"Connexion impossible\n");
		else
		{
			ArLog::log(ArLog::Normal,"Connexion OK\n");
			return true;
		}
	}
	else

			ArLog::log(ArLog::Normal,"Parametres manquant\n");
	return false;
}


bool ClientTCP::connect(const char *pHost, int pPort){
	
	if(!sock.connect(pHost,pPort,ArSocket::TCP))
	{
		ArLog::log(ArLog::Normal,"Connexion impossible\n");
		return false;
	}
	else
		ArLog::log(ArLog::Normal,"Connexion OK\n");
	return true;
}

bool ClientTCP::close()
{
	return(sock.close());
}
int ClientTCP::write(const char *strToSend)
{
  // return the same number of bytes that we told it to write. Otherwise,
  // its an error condition.
  if (sock.write(strToSend, strlen(strToSend)) == strlen(strToSend))
    ArLog::log(ArLog::Normal, "socketClientExample: Open Form FirstVC.");
  else
  {
      ArLog::log(ArLog::Terse, "socketClientExample: Error sending Open Form FirstVC string to the server.");
    return(-1);
  }
  return 0;
}

int ClientTCP::read()
{
	// Read data from the socket. read() will block until
  // data is received. 
  strSize=sock.read(bufr, sizeof(bufr));

  // If the amount read is 0 or less, its an error condition.
  if (strSize > 0)
  {
    bufr[strSize]='\0'; // Terminate the string with a NULL character:
    ArLog::log(ArLog::Normal, "socketClientExample: Server said: \"%s\"", bufr);
	
  }
  else
  {
      ArLog::log(ArLog::Terse, "socketClientExample: Error in waiting/reading from the server.");
  }
  return strSize;
}

int ClientTCP::read(unsigned int timeout)
{
	 // Read data from the socket. read() will block until
  // data is received. 
  strSize=sock.read(bufr, sizeof(bufr),timeout);

  // If the amount read is 0 or less, its an error condition.
  if (strSize > 0)
  {
    bufr[strSize]='\0'; // Terminate the string with a NULL character:
    ArLog::log(ArLog::Normal, "socketClientExample: Server said: \"%s\"", bufr);	
  }
  else
  {
      ArLog::log(ArLog::Terse, "socketClientExample: Error in waiting/reading from the server.");
  }
  return strSize;
}


int ClientTCP::read(char *bufr, unsigned int timeout)
{	
	//char *ptr;
	//Délimiteur
	const char delim[2]=" ";
	//char *token;
	int nbArgs=0;
	//char tab[8][256];

  // Read data from the socket. read() will block until
  // data is received. 
  strSize=sock.read(bufr,sizeof(bufr),timeout);

  // If the amount read is 0 or less, its an error condition.
  if (strSize > 0)
  {
    bufr[strSize]='\0'; // Terminate the string with a NULL character:
    decode();
	ArLog::log(ArLog::Normal, "socketClientExample: Server said: \"%s\"", bufr);
  }
  else
  {
      ArLog::log(ArLog::Terse, "socketClientExample: Error in waiting/reading from the server.");
  }
  return strSize;
}

void ClientTCP::decode()
{
	char delim[2]=" ";
	char tab[8][64];
	char* token;
	int nbArgs=0;

	token=strtok(bufr,delim);
	while(token!=NULL)
	{
		token=strtok(NULL,delim);
		strcpy(tab[nbArgs],token);
		nbArgs++;
	}

	printf("Nb args : %d\n",nbArgs);
	printf("arg0 : %s\n",tab[0]);

	//msg.nbArgs=nbArgs;
	//msg.socket=&sock;
	//msg.args=&tab;

	//printf("Nb args : %d\n",msg.nbArgs);
	//printf("arg0 : %s\n",msg.args[0]);
}
