#include "TCPRequestsLoop.h"

using namespace std;


TCPRequestsLoop::TCPRequestsLoop(SRMA &srma, Pool<TCPReceivedRequest> *messagePool):myMessagePool(messagePool),
mySrma(srma), currentSock(0)
{
	//initialize map
	s_mapStringValues["GotoGoal"] = Cmd0;
	s_mapStringValues["Talk"] = Cmd1;
	s_mapStringValues["Dock"] = Cmd2;

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
	/*this->lock();
	ArLog::log(ArLog::Normal,"Recu : %s", f.frame.msg[1]);
	this->unlock();*/
	//Pour suivre l'avancement
	mySrma.setCallbackEndGotoGoal(&funct);
	currentSock = f.socket;
	//What type of request is it?
	switch(s_mapStringValues[f.frame.msg[1]])
	{
		//Gotogoal command
		case Cmd0:
			//ArLog::log(ArLog::Normal,"GotoGoal");
			//mySrma.SendCommand(CommandeRobot::CommandeRobot(CommandeRobot::GOTOGOAL, f.frame.msg[2]));	
			treatGotoGoal(f);
			//while(!mySrma.isStateArrivedAt(f.frame.msg[2].c_str()) || 
			
			break;
		case Cmd1:
			break;
		//Recu : Request Dock
		case Cmd2:
			//ArLog::log(ArLog::Normal,"Dock");
			mySrma.SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"ENABLE"));
			mySrma.SendCommand(CommandeRobot(CommandeRobot::DOCK));
			break;
		default:
			break;
	}
	return;
}

//Recu : Request GotoGoal xxx
void TCPRequestsLoop::treatGotoGoal(TCPReceivedRequest req)
{
	mySrma.SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE"));
	myMutex.lock();
	mySrma.gotoGoal(req.frame.msg[2].c_str());
	//req.socket->writeString("c parti");
	myMutex.unlock();

}

void TCPRequestsLoop::lockMutex(){myMutex.lock();};
void TCPRequestsLoop::unlockMutex(){myMutex.unlock();};

void TCPRequestsLoop::handleEndGotoGoal(char* msg)
{
	
	//SRMA::GOTOGOAL_STATUS
	try
	{
		myMutex.lock();
		//currentSock->writeString(msg);
		currentSock->writeString(msg);
		myMutex.unlock();
		if(strstr(msg,"Arrived")!=0 || strstr(msg,"Failed")!=0 )
			mySrma.play(SRMA::BELL);
	}		
	
	catch(std::exception const&  ex)
	{
		printf("Socket exception. %s", ex.what());
	}

	
	//printf("recu %d\n", p);

}


