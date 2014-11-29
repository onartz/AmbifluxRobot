#include "ServerLoop.h"

void ServerLoop::handleRequestReceived(char ** msg, int nbArgs, ArSocket *sock)
{
	//ThreadSafe
	myMessagePool->push(TCPReceivedRequest(sock, Frame(msg, nbArgs)));
}

ServerLoop::ServerLoop(SRMA & srma, Pool<TCPReceivedRequest>* messagePool):
mySrma(srma), myMessagePool(messagePool),
myHandleRequestReceivedCB(this, &ServerLoop::handleRequestReceived)
{
	myArNetServer.squelchNormal();
	myArNetServer.addCommand("Request", &myHandleRequestReceivedCB, "Request");
	myRunning = true;

}

void *ServerLoop::runThread(void *arg)
{
	if(!myArNetServer.open(NULL, TCPPORT, "patrolbot", true))
		return 0;
	
	while(myArNetServer.isOpen() && myRunning == true)
	{
		myArNetServer.runOnce();
		ArUtil::sleep(50);
	}
	return 0;
}

