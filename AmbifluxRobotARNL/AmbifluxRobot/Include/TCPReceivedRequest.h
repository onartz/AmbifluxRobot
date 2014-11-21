#ifndef DEF_TCPRECEIVEDREQUEST_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_TCPRECEIVEDREQUEST_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include "ArSocket.h"
#include "Frame.h"
//Messages recus
struct TCPReceivedRequest
	{
	public : 
		//Constructeurs
		TCPReceivedRequest();
		TCPReceivedRequest(ArSocket*, Frame);
		//Membres
		ArSocket * socket;
		Frame frame;
		
	};
#endif