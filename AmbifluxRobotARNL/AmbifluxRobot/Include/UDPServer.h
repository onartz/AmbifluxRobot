#ifndef DEF_UDPSERVER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_UDPSERVER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "aria.h"

class UDPServer : public ArASyncTask
{
public:

	UDPServer();
	// empty destructor
	~UDPServer(void) {};
	//Demarrage
	void Start();
	
	//Envoi message
	void SendCommand(char *);
 
  virtual void * runThread(void *arg);

protected:
	ArSocket serverSock;
	ArSocket clientSock;
	//char *strToSend="Hello Client";
  // The buffer in which to recieve the hello from the client
	char buff[100];
	// The size of the string the client sent
	 size_t strSize;

};
#endif