#ifndef DEF_CLIENTTCP_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_CLIENTTCP_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "aria.h"
#define DEFAULT_PORT	7474
#define DEFAULT_HOST	"192.168.122.2"

/*struct TCPMsg
{
	char** args;
	int nbArgs;
	ArSocket* socket;
};*/

class ClientTCP
{	
public:
	ClientTCP();
	//IP, Port
	ClientTCP(const char*, int); 
	bool connect(void);
	bool connect(const char*, int);
	bool close(void);
	int write(const char*);
	int read(void);
	//Avec Timeout
	int read(unsigned int);
	int read(char*, unsigned int);
	
	
protected:
	ArSocket sock;
	const char* host;
	size_t strSize;
	int port;
	char bufr[256];
	char bufe[256];
	//Réponse
	//TCPMsg msg;
	void decode();


};
#endif