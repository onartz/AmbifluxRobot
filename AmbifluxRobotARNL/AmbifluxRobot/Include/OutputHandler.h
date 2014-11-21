#ifndef DEF_OUTPUTHANDLER_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_OUTPUTHANDLER_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "Arnetworking.h"

class OutputHandler
{
public:
  OutputHandler(ArClientBase *client);
  virtual ~OutputHandler(void);
  /// This callback is called when an update on general robot state arrives
  void handleOutput(ArNetPacket *packet);
  /// This callback is called when an update on general robot state arrives
  void handleOutputNumbers(ArNetPacket *packet);
  /// This callback is called when an update on general robot state arrives
  void handleOutputStrings(ArNetPacket *packet);

  /// This callback is called when an update on the battery configuration changes
  void handleBatteryInfo(ArNetPacket *packet);
  /// This is called when the physical robot information comes back
  void handlePhysicalInfo(ArNetPacket *packet);
  /// This callback is called when an update on the temperature information changes
  void handleTemperatureInfo(ArNetPacket *packet);

  /// This callback is called when an update on pathplanning changes
  void handlePathPlanningInfo(ArNetPacket *packet);
  /// This callback is called when an update on pathplanning changes
  void handleGotoGoalInfo(ArNetPacket *packet);

  char* getStatus(void);
  char* getMode(void);
protected:

  /// The results from the data update are stored in these variables
  //@{
  double myX;
  double myY;
  double myTh;
  double myVel;
  double myRotVel;
  double myLatVel;
  bool myVoltageIsStateOfCharge;
  char myTemperature;
  double myVoltage;
  char myStatus[256];
  char myMode[256];
  //@}
  ArClientBase *myClient;

  /** These functor objects are given to the client to receive updates when they
   * arrive from the server.
   */
  //@{
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleOutputCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleOutputNumbersCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleOutputStringsCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleBatteryInfoCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandlePhysicalInfoCB;
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleTemperatureInfoCB;
  //Modifs Oli
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandlePathPlanningInfoCB;
  //@}
  ArFunctor1C<OutputHandler, ArNetPacket *> myHandleGotoGoalInfoCB;
  
  /// A header for the columns in the data printout is sometimes printed
  bool myNeedToPrintHeader;
  /// Don't print any information until we get the battery info
  bool myGotBatteryInfo;
};

#endif