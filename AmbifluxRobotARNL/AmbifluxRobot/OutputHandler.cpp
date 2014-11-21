#include "outputHandler.h"

OutputHandler::OutputHandler(ArClientBase *client) :
  myClient(client),
  myHandleOutputCB(this, &OutputHandler::handleOutput),
  myHandleOutputNumbersCB(this, &OutputHandler::handleOutputNumbers),
  myHandleOutputStringsCB(this, &OutputHandler::handleOutputStrings),
  myHandleBatteryInfoCB(this, &OutputHandler::handleBatteryInfo),
  myHandlePhysicalInfoCB(this, &OutputHandler::handlePhysicalInfo),
  myHandleTemperatureInfoCB(this, &OutputHandler::handleTemperatureInfo),
  myHandlePathPlanningInfoCB(this,&OutputHandler::handlePathPlanningInfo),
  myHandleGotoGoalInfoCB(this,&OutputHandler::handleGotoGoalInfo),
  
  myNeedToPrintHeader(false),
  myGotBatteryInfo(false)
{
  /* Add a handler for battery info, and make a single request for it  */
  myClient->addHandler("physicalInfo", &myHandlePhysicalInfoCB);
  myClient->requestOnce("physicalInfo");

 

  /* Add a handler for battery info, and make a single request for it  */
  myClient->addHandler("batteryInfo", &myHandleBatteryInfoCB);
  myClient->requestOnce("batteryInfo");

  /* If it exists add a handler for temperature info, and make a
   * single request for it  */
  if (myClient->dataExists("temperatureInfo"))
  {
    myClient->addHandler("temperatureInfo", &myHandleTemperatureInfoCB);
    myClient->requestOnce("temperatureInfo");
  }

  // if we have the new way of broadcasting that only pushes strings
  // when they change then use that
  if (myClient->dataExists("updateNumbers") && 
      myClient->dataExists("updateStrings"))
  {
    printf("Using new updates\n");
    // get the numbers every 100 ms
    myClient->addHandler("updateNumbers", &myHandleOutputNumbersCB);
    myClient->request("updateNumbers", 100);
    // and the strings whenever they change (and are broadcast)
    myClient->addHandler("updateStrings", &myHandleOutputStringsCB);
    myClient->request("updateStrings", -1);
  }
  else
  {
    printf("Using old updates\n");
    // For the old way, just Add a handler for general info, and
    // request it to be called every 100 ms
    myClient->addHandler("update", &myHandleOutputCB);
    myClient->request("update", 100);
  }

  //myClient->addHandler("gotoGoal",&myHandleGotoGoalInfoCB);
 // myClient->request("gotoGoal");

  if(myClient->dataExists("pathPlannerStatus"))
  {
	  printf("pathPlannerStatus exists\n");
	myClient->addHandler("pathPlannerStatus", &myHandlePathPlanningInfoCB);
	myClient->request("pathPlannerStatus",-1);
  }	
}

OutputHandler::~OutputHandler(void)
{
  /* Halt the request for data updates */
  myClient->requestStop("update");
}

void OutputHandler::handleOutput(ArNetPacket *packet)
{
  /* Extract the data from the update packet. Its format is status and
   * mode (null-terminated strings), then 6 doubles for battery voltage, 
   * x position, y position and orientation (theta) (from odometry), current
   * translational velocity, and current rotational velocity. Translation is
   * always milimeters, rotation in degrees.
   */
  memset(myStatus, 0, sizeof(myStatus));
  memset(myMode, 0, sizeof(myMode));
  packet->bufToStr(myStatus, sizeof(myStatus));
  packet->bufToStr(myMode, sizeof(myMode));
  myVoltage = ( (double) packet->bufToByte2() )/10.0;
  myX = (double) packet->bufToByte4();
  myY = (double) packet->bufToByte4();
  myTh = (double) packet->bufToByte2();
  myVel = (double) packet->bufToByte2();
  myRotVel = (double) packet->bufToByte2();
  myLatVel = (double) packet->bufToByte2();
  myTemperature = (double) packet->bufToByte();

  if(myNeedToPrintHeader) 
  {
    /*printf("\n%6s|%6s|%6s|%6s|%6s|%6s|%4s|%6s|%15s|%20s|\n",
	   "x","y","theta", "vel", "rotVel", "latVel", "temp", myVoltageIsStateOfCharge ? "charge" : "volts", "mode","status");
    fflush(stdout);*/
    myNeedToPrintHeader = false;
  }
  if (myGotBatteryInfo)
    /*printf("%6.0f|%6.0f|%6.1f|%6.0f|%6.0f|%6.0f|%4.0d|%6.1f|%15s|%20s|\r",
	   myX, myY, myTh, myVel, myRotVel, myLatVel, myTemperature, myVoltage, myMode, myStatus);
  */
  fflush(stdout);
}

void OutputHandler::handleOutputNumbers(ArNetPacket *packet)
{
  /* Extract the data from the updateNumbers packet. Its format is 6
   * doubles for battery voltage, x position, y position and
   * orientation (theta) (from odometry), current translational
   * velocity, and current rotational velocity. Translation is always
   * milimeters, rotation in degrees.
   */
  myVoltage = ( (double) packet->bufToByte2() )/10.0;
  myX = (double) packet->bufToByte4();
  myY = (double) packet->bufToByte4();
  myTh = (double) packet->bufToByte2();
  myVel = (double) packet->bufToByte2();
  myRotVel = (double) packet->bufToByte2();
  myLatVel = (double) packet->bufToByte2();
  myTemperature = (double) packet->bufToByte();

  if(myNeedToPrintHeader) 
  {
  /*  printf("\n%6s|%6s|%6s|%6s|%6s|%6s|%4s|%6s|%15s|%20s|\n",
	   "x","y","theta", "vel", "rotVel", "latVel", "temp", myVoltageIsStateOfCharge ? "charge" : "volts", "mode","status");
    fflush(stdout);*/
    myNeedToPrintHeader = false;
  }
  /*if (myGotBatteryInfo)
    printf("%6.0f|%6.0f|%6.1f|%6.0f|%6.0f|%6.0f|%4.0d|%6.1f|%15s|%20s|\r",
	   myX, myY, myTh, myVel, myRotVel, myLatVel, myTemperature, myVoltage, myMode, myStatus);
  
  fflush(stdout);*/
}

void OutputHandler::handleOutputStrings(ArNetPacket *packet)
{
  /* Extract the data from the updateStrings packet. Its format is
   * status and mode (null-terminated strings).
   */
  memset(myStatus, 0, sizeof(myStatus));
  memset(myMode, 0, sizeof(myMode));
  packet->bufToStr(myStatus, sizeof(myStatus));
  packet->bufToStr(myMode, sizeof(myMode));
  
  //done = true;
}

void OutputHandler::handleBatteryInfo(ArNetPacket *packet)
{
  /* Get battery configuration parameters: when the robot will begin beeping and 
   * warning about low battery, and when it will automatically disconnect and
   * shutdown. */
  double lowBattery = packet->bufToDouble();
  double shutdown = packet->bufToDouble();
  printf("Low battery voltage: %6g       Shutdown battery voltage: %6g\n", lowBattery, shutdown);
  fflush(stdout);
  myNeedToPrintHeader = true;
  myGotBatteryInfo = true;

  if (packet->getDataReadLength() == packet->getDataLength())
  {
    printf("Packet is too small so its an old server, though you could just get to the bufToUByte anyways, since it'd be 0 anyhow\n");
    myVoltageIsStateOfCharge = false;
  }
  else
    myVoltageIsStateOfCharge = (packet->bufToUByte() == 1);

}


void OutputHandler::handlePhysicalInfo(ArNetPacket *packet)
{
  /* Get phyiscal configuration parameters: */
  char robotType[512];
  char robotSubtype[512];
  int width;
  int lengthFront;
  int lengthRear;

  packet->bufToStr(robotType, sizeof(robotType));
  packet->bufToStr(robotSubtype, sizeof(robotSubtype));
  width = packet->bufToByte2();
  lengthFront = packet->bufToByte2();
  lengthRear = packet->bufToByte2();

  printf("Type: %s Subtype: %s Width %d: LengthFront: %d LengthRear: %d\n",
	 robotType, robotSubtype, width, lengthFront, lengthRear);
  fflush(stdout);
}

void OutputHandler::handleTemperatureInfo(ArNetPacket *packet)
{
  char warning = packet->bufToByte();
  char shutdown = packet->bufToByte();
  printf("High temperature warning: %4d       High temperature shutdown: %4d\n", warning, shutdown);
  fflush(stdout);
  myNeedToPrintHeader = true;
}


 /// This callback is called when an update on pathplanning changes
  void OutputHandler::handlePathPlanningInfo(ArNetPacket *packet)
  {
	  //printf("ok");
  }

  /// This callback is called when an update on pathplanning changes
  void OutputHandler::handleGotoGoalInfo(ArNetPacket *packet)
  {
	 // printf("ok");
  }

    char* OutputHandler::getStatus(void){return myStatus;}
	char* OutputHandler::getMode(void){return myMode;}