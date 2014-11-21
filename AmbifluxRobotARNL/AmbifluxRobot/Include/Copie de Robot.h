#ifndef DEF_ROBOT_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ROBOT_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Aria.h"
#include "ArNetworking.h"
#include "strutil.h"
#include "resource.h"
#include "OutputHandler.h"
#include "ArArgumentBuilder.h"
#include "ArArgumentParser.h"
#include "PublishRobotValueTask.h"

class PublishRobotValueTask;

#define ERRORCODE_INIT_ARNL 1
#define TIMEOUT_CONNEXION_ROBOT 120
#define TIMEOUT_PRISE_EN_COMPTE_ORDRE 60
#define TIMEOUT_DEPLACEMENT 300

class Robot
{
public:
  enum retCodes
	{
		RET_NOERROR=0,
		RET_FAILED_TO_CONNECT =1	
	};


   enum retCodesDeplacement
	{
		RET_GOAL_REACHED=0,
		RET_FAILED_TO_GET_TO
	};

   //Constructeurs
	Robot(void);

	//Destructeur
	virtual ~Robot(void);

	//Deplacement vers une position
	int GotoGoal(char *);
	void Connect(void);
	/*void SetRobotClient(ArClientBase *);*/
	void DisableAutodock(void);
	void EnableAutodock(void);

	//Accesseurs Set
	void setBatteryInfos(double);
	void setVel(double);
	void setX(double);
	void setY(double);
	void setTh(double);
	void setRotVel(double);
	void setLatVel(double);
	void setVoltage(double);
	void setTemperature(double);
	void setStatus(char * );
	void setMode(char * );
	void setVoltageIsStateOfCharge(bool);
	

	//Accesseurs get
	double getBatteryInfos();
	int getErrorCode();
	double getVel();
	double getX();
	double getY();
	double getTh();
	double getRotVel();
	double getLatVel();
	double getTemperature();
	bool getVoltageIsStateOfCharge();
	double getVoltage();
	char * getStatus();
	char * getMode();
	bool IsConnectedToARNLServer();
	ArClientBase getRobotClient();
	
protected:
	void publishValues();

	PublishRobotValueTask publishRobotValueTask;
//Renvoie si la connexion avec ARNL Server est OK ou non
	bool isConnectedToARNLServer;
  //CodeErreur
	int errorCode;
	  double myX;
	  double myY;
	  double myZ;
	  double myTh;
	  double myVel;
	  double myRotVel;
	  double myLatVel;
	  bool myVoltageIsStateOfCharge;
	  char myTemperature;
	  double myVoltage;
	  char myStatus[256];
	  char myMode[256];
	  ArClientBase myClient;
	  OutputHandler *myOutputHandler;
	 
	  /// A header for the columns in the data printout is sometimes printed
	  bool myNeedToPrintHeader;
	  /// Don't print any information until we get the battery info
	  bool myGotBatteryInfo;
};
#endif
