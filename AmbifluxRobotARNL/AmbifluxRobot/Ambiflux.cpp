/*Version client ARNL
La communication avec la tablette se fait via IhmCommunicationThread
A chaque réception d'un nouveau message, la listOfIncomingMessage est mise à jour
*/

#include "Aria.h"
#include "ArNetworking.h"
#include "Outputhandler.h"
#include "MainLoop.h"
#include "ServerLoop.h"
#include "TCPRequestsLoop.h"
#include "ArSoundsQueue.h"
#include "ArSoundPlayer.h"
#include "Globals.h"
#include "Srma.h"

IhmCommunicationThread ihm;

/* Key handler for the escape key: shutdown all of Aria. */
void escape(void)
{
  printf("esc pressed, shutting down aria\n");
  Aria::shutdown();
}

void queueNowEmpty() {
  //printf("The sound queue is now empty.\n");
}

void queueNowNonempty() {
  //printf("The sound queue is now non-empty.\n");
}

bool no() {
  // just a false tautology
  return false;
}

bool done;
char status[256];


/* Fin de déclararation des variables globales */

/* Callback appelé à chaque réception de nouveau message venant de la tablette */
void CallbackIhmReceived(Frame f)
{
	ArLog::log(ArLog::Verbose, "IHM Message Received : %s",f.msg[0]);
	//listOfIncomingMessage.push_back(f);
}


void logOptions(const char *progname)
{
  ArLog::log(ArLog::Verbose, "Usage: %s [options]\n", progname);
  ArLog::log(ArLog::Verbose, "[options] are any program options listed below, or any ARNL configuration");
  ArLog::log(ArLog::Verbose, "parameters as -name <value>, see params/arnl.p for list.");
  ArLog::log(ArLog::Verbose, "For example, -map <map file>.");
  Aria::logOptions();
}

bool gyroErrored = false;
const char* getGyroStatusString(ArRobot* robot)
{
  if(!robot || !robot->getOrigRobotConfig() || robot->getOrigRobotConfig()->getGyroType() < 2) return "N/A";
  if(robot->getFaultFlags() & ArUtil::BIT4)
  {
    gyroErrored = true;
    return "ERROR/OFF";
  }
  if(gyroErrored)
  {
    return "OK but error before";
  }
  return "OK";
}


/*
Executer Ambiflux -GUI si tablette
*/
int main(int argc, char **argv)
{
  // Initialize Aria and Arnl global information
 
 /* Aria initialization: */
  Aria::init();
  ArLog::init(ArLog::File, ArLog::Verbose,"c:\\temp\\AmbifluxRobot.log",true);
  ArLog::log(ArLog::Verbose, "Ambiflux Starting");

  // Create the sound queue.
  ArSoundsQueue soundQueue;

  // Set WAV file callbacks 
  soundQueue.setPlayWavFileCallback(ArSoundPlayer::getPlayWavFileCallback());
  soundQueue.setInterruptWavFileCallback(ArSoundPlayer::getStopPlayingCallback());

  // Notifications when the queue goes empty or non-empty.
  soundQueue.addQueueEmptyCallback(new ArGlobalFunctor(&queueNowEmpty));
  soundQueue.addQueueNonemptyCallback(new ArGlobalFunctor(&queueNowNonempty));

  // Run the sound queue in a new thread
  soundQueue.runAsync();
  

  /* Create our client object. This is the object which connects with a remote
   * server over the network, and which manages all of our communication with it
   * once connected by sending data "requests".  Requests may be sent once, or
   * may be repeated at any frequency. Requests and replies to requsets contain 
   * payload "packets", into which various data types may be packed (when making a 
   * request), and from which they may also be extracted (when handling a reply). 
   * See the InputHandler and OutputHandler classes above for
   * examples of making requests and reading/writing the data in packets.
   */
  ArClientBase client;
	
  
   /* Aria components use this to get options off the command line: */
  ArArgumentParser parser(&argc, argv);

  //La tablette doit elle être utilisee?
  g_Tablette = parser.checkArgument("-GUI");

  /* Pool de messages en provenance d'un client TCP
  Issu de l'implementation d'un modèle producteur/consommateur
  pour les messages entrants. Plusieurs thread y accèdent
  Tread-safe (mutex)*/
  Pool<TCPReceivedRequest> tcpReceivedPool;

  if(g_Tablette == true)
  {
	  /*Create our thread to communicate with iPad
	   Server start on port 7171 to receive requests from ipad
	   A client is created on port 7474 to request iPad
	   */

		IhmCommunicationThread ihm(7171, &tcpReceivedPool);
	   //On s'abonne à la réception de message par la classe IhmCommunicationThread
	  //Todo : A supprimer ?
	  //ArGlobalFunctor1<Frame> functMessageReceived(&CallbackIhmReceived);
	  //ihm.setCallback(&functMessageReceived); 
	  
	 /* while(!ihm.connect() == true)
	  {
	  //if(ihm.connect()!=0)
		soundQueue.play("c:\\temp\\ShortCircuit.wav");
		ArUtil::sleep(2000);
	  }*/
	  ihm.runAsync();
  }

  /* This will be used to connect our client to the server, including
   * various bits of handshaking (e.g. sending a password, retrieving a list
   * of data requests and commands...)
   * It will get the hostname from the -host command line argument: */
  ArClientSimpleConnector clientConnector(&parser);

  parser.loadDefaultArguments();

  /* Check for -help, and unhandled arguments: */
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    exit(0);
  }
  
  /* Connect our client object to the remote server: */
  if (!clientConnector.connectClient(&client))
  {
    if (client.wasRejected())
      printf("Server '%s' rejected connection, exiting\n", client.getHost());
    else
      printf("Could not connect to server '%s', exiting\n", client.getHost());
    exit(1);
  } 

  printf("Connected to server.\n");

	client.setRobotName(client.getHost()); // include server name in log messages
	client.logDataList();

	///* Create a key handler and also tell Aria about it */
	ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);

  /* Global escape-key handler to shut everythnig down */
  ArGlobalFunctor escapeCB(&escape);
  keyHandler.addKeyHandler(ArKeyHandler::ESCAPE, &escapeCB);

  /* Now that we're connected, we can run the client in a background thread, 
   * sending requests and receiving replies. When a reply to a request arrives,
   * or the server makes a request of the client, a handler functor is invoked. 
   * The handlers for this program are registered with the client by the 
   * InputHandler and OutputHandler classes (in their constructors, above) */
  client.runAsync();

  ///* Create the InputHandler object and request safe-drive mode */
  //InputHandler inputHandler(&client);
  //inputHandler.gotoGoal("215");
  ////inputHandler.safeDrive();
  

// Mode goto
if(!client.dataExists("gotoGoal") )
      printf("Warning: Pas de mode goto!\n");
  else
    printf("Mode goto accepte");


//ArFunctor1<ArNetPacket*>
//client.addHandler("pathPlannerStatus",);



  /* Create the OutputHandler object. It will begin printing out data from the
   * server. */
  OutputHandler outputHandler(&client);

   //On s'abonne à la réception de message par la classe IhmCommunicationThread
  //Todo : A supprimer ?
  //ArGlobalFunctor1<Frame> functMessageReceived(&CallbackIhmReceived);
  //ihm.setCallback(&functMessageReceived);
  //ihm.runAsync();

  //pour tester IHM
 // ArUtil::sleep(1000);
//  ihm.testCommunication();
 


	//SRMA object
	string strSRMA = DALRest::getResourceById("9");
	SRMA srma(strSRMA,client, outputHandler, ihm, &soundQueue);

	MainLoop myLoop(srma);

	if(g_Tablette == true)
	{
		myLoop.setTCPReceivedPool(&tcpReceivedPool);
	}
	myLoop.runAsync();

	//MainLoop myLoop(srma, &tcpReceivedPool);
	//myLoop.runAsync();

	//Loop du mode Ambiant
	/*if(g_Tablette == true)
	{
		
		myLoop(srma, &tcpReceivedPool);
		myLoop.runAsync();
	}*/
	/*else
	{
		MainLoop myLoop(srma);
		myLoop.runAsync();
	{*/
	
	
	//Thread loop : TCP commands
	//Produces messages in tcpMessagePool
	ServerLoop myServerLoop(srma, &tcpReceivedPool);
	myServerLoop.runAsync();
 
	//Traitement des requetes TCP
	//Consumes messages in tcpMessagePool
	TCPRequestsLoop myTCPRequestsLoop(srma, &tcpReceivedPool);
	myTCPRequestsLoop.runAsync();

 

  /* While the client is still running (getRunningWithLock locks the "running"
   * flag until it returns), check keys on the key handler (which will call
   * our callbacks), then tell the input handler to send drive commands. 
   * Sleep a fraction of a second as well to avoid using
   * too much CPU time, and give other threads time to work.
   */
  while (client.getRunningWithLock())
  {
    //keyHandler.checkKeys();
    //inputHandler.sendInput();
    ArUtil::sleep(100);
  }

  /* The client stopped running, due to disconnection from the server, general
   * Aria shutdown, or some other reason. */
  client.disconnect();
  Aria::shutdown();
  return 0;
}
