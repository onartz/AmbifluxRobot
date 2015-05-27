/*
Ambiant mode with database requests to treat Workorders
*/
#include "MainLoop.h"

/*void print(boost::property_tree::ptree const& pt)
{
    using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
        print(it->second);
    }
}*/

//Get workorders from DB
			/*
			{"GetWorkordersResult":[{"ModifiedDate":null,"OrderHeader":{"CustomerFirstName":"Olivier",
			"CustomerLastName":"NARTZ","ObjetDemandeExpress":"Je veux 2 feutres noirs",
			"OrderDate":"","OrderId":99,"OrderNo":"ORD00000099"},
			"StartDate":null,"WorkOrderID":86,"WorkorderNo":"WOR00000086","WorkorderRoutings":
			[{"Location":{"LocationID":0,"LocationName":"S-104-1"},"Type":"T","WorkOrderRoutingNo":"WOR00000086-10"}
			,{"Location":{"LocationID":0,"LocationName":"S-104-1"},"Type":"C","WorkOrderRoutingNo":"WOR00000086-20"},
			{"Location":{"LocationID":0,"LocationName":"S-105"},"Type":"T","WorkOrderRoutingNo":"WOR00000086-30"},
			{"Location":{"LocationID":0,"LocationName":"S-105"},"Type":"L","WorkOrderRoutingNo":"WOR00000086-40"}],
			"type":"LIV"},{"ModifiedDate":null,"OrderHeader":{"CustomerFirstName":"Jean-Yves","CustomerLastName":"Bron",
			"ObjetDemandeExpress":"Besoin de feutres 4 couleurs","OrderDate":"","OrderId":110,"OrderNo":"ORD00000110"},
			"StartDate":null,"WorkOrderID":97,"WorkorderNo":"WOR00000097","WorkorderRoutings":[{"Location":
			{"LocationID":0,"LocationName":"Accueil"},"Type":"T","WorkOrderRoutingNo":"WOR00000097-10"},
			{"Location":{"LocationID":0,"LocationName":"Accueil"},"Type":"C","WorkOrderRoutingNo":"WOR00000097-20"},
			{"Location":{"LocationID":0,"LocationName":"S-112-1"},"Type":"T","WorkOrderRoutingNo":"WOR00000097-30"},
			{"Location":{"LocationID":0,"LocationName":"S-112-1"},"Type":"L","WorkOrderRoutingNo":"WOR00000097-40"}]
			,"type":"LIV"}]}

			*/

using namespace std;
/* Liste des Workorders en attente de traitement */
std::list<Workorder> workorderList;

MainLoop::MainLoop(SRMA &srma):mySrma(srma){
	myMessagePool = 0;
	myRunning = true;
}

MainLoop::MainLoop(SRMA &srma, Pool<TCPReceivedRequest> *messagePool):myMessagePool(messagePool),
mySrma(srma)
{
	myRunning = true;
}

void MainLoop::setTCPReceivedPool(Pool<TCPReceivedRequest>* messagePool){
	myMessagePool = messagePool;
}
//Thread du serveurc
void *MainLoop::runThread(void *arg)
	{
		while(myRunning == true)
		{
			//Read new workorders
			string s = DALRest::getWorkorders(Workorder::NEW);
			if(s!="")
			{
				//Store Workorders
				ptree pt = JSONParser::parse(s);
				BOOST_FOREACH(ptree::value_type &workorder,pt.get_child("GetWorkordersResult"))
					workorderList.push_back(Workorder(workorder.second));
				//Treat every workorder in the list
				//TODO : rendre interruptible à chaque wo

				//print(pt);

				while(workorderList.size()>0)
				{
					TraiterWorkorder(workorderList.front());
					ArLog::log(ArLog::Verbose, "Workorder : %s", workorderList.front().getWorkorderNo());
					workorderList.pop_front();
				}
			}
			ArUtil::sleep(LOOP_PERIOD);
		}		
	  // return out here, means the thread is done
	  return NULL;
	}


void MainLoop::TraiterWorkorder(Workorder &workorder)
{
	//Liste des WorkorderRouting du workorder
	std::list<WorkorderRouting> worList = workorder.getWorkorderRoutingList();
	WorkorderRouting * currentWorkorderRouting(0);
	ArLog::log(ArLog::Verbose,"Entering TraiterWorkorder");	
	workorder.start();
	DALRest::updateWorkorder(&workorder);
	/* Send command to disable autodock */
	mySrma.SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"DISABLE"));
	
	//ArLog::log(ArLog::Verbose,"Entering TraiterWorkorder::STATE_LECTURE_WORS\n");
					
	/* Treat every WorkorderRouting in WorkorderRoutingList */
	for(std::list<WorkorderRouting>::iterator it = worList.begin();it != worList.end(); ++it)
	{
		if((*it).getStatusId()==WorkorderRouting::NEW)
		{		
			currentWorkorderRouting = &*it;
			TraiterWorkorderRouting t(mySrma,*currentWorkorderRouting, myMessagePool);
			//Si le workorderrouting a été annulé, on annule le workorder
			if(currentWorkorderRouting->isCancelled())
			{
				workorder.cancel();
				DALRest::updateWorkorder(&workorder);
				return;
			}
		}
	}
	workorder.close();
	DALRest::updateWorkorder(&workorder);
	mySrma.SendCommand(CommandeRobot(CommandeRobot::AUTODOCK,"ENABLE"));
}


