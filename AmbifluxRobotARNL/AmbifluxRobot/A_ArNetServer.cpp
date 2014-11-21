#include "A_ArNetServer.h"

A_ArNetServer::A_ArNetServer():ArNetServer(true){}


std::list<ArSocket *> A_ArNetServer::getConns(void)
{
	return ArNetServer::myConns;
}