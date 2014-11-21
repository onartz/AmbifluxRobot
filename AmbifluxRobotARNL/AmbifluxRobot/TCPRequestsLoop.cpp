#include "TCPRequestsLoop.h"

using namespace std;


TCPRequestsLoop::TCPRequestsLoop(SRMA &srma, Pool<TCPReceivedRequest> *messagePool):myMessagePool(messagePool),
mySrma(srma), currentSock(0)
{
	//initialize map
	s_mapStringValues["GotoGoal"] = Cmd0;
	s_mapStringValues["Talk"] = Cmd1;
	s_mapStringValues["XXX"] = Cmd2;

	myRunning = true;
}
//Thread du serveurc
void *TCPRequestsLoop::runThread(void *arg)
	{
		ArLog::log(ArLog::Verbose,"Thread TCPRequestsLoop running\n");	
		while(myRunning == true)
		{
			if(myMessagePool->size() != 0)
				treatRequest(myMessagePool->pop());
			
			ArUtil::sleep(READING_PERIOD);
		}		
	  // return out here, means the thread is done
	  return NULL;
	}

void TCPRequestsLoop::treatRequest(TCPReceivedRequest f)
{
	ArFunctor1C<TCPRequestsLoop, char*> funct(this, &TCPRequestsLoop::handleEndGotoGoal);
	//Pour suivre l'avancement
	mySrma.setCallbackEndGotoGoal(&funct);
	currentSock = f.socket;
	//What type of request is it?
	switch(s_mapStringValues[f.frame.msg[1]])
	{
		//Gotogoal command
		case Cmd0:
			//mySrma.SendCommand(CommandeRobot::CommandeRobot(CommandeRobot::GOTOGOAL, f.frame.msg[2]));	
			treatGotoGoal(f);
			//while(!mySrma.isStateArrivedAt(f.frame.msg[2].c_str()) || 
			
			break;
		case Cmd1:
			break;
		case Cmd2:
			break;
		default:
			break;
	}
	return;
}

void TCPRequestsLoop::treatGotoGoal(TCPReceivedRequest req)
{
	mySrma.gotoGoal(req.frame.msg[2].c_str());
	//req.socket->writeString("c parti");

}

void TCPRequestsLoop::handleEndGotoGoal(char* msg)
{
	//SRMA::GOTOGOAL_STATUS
	try
	{
		currentSock->writeString(msg);
	}
	catch(std::exception const&  ex)
	{
		printf("Socket exception. %s", ex.what());
	}
	//printf("recu %d\n", p);

}


