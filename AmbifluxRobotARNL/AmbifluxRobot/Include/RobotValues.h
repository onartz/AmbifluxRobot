#ifndef DEF_ROBOTVALUES_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_ROBOTVALUES_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus

//RobotValues
typedef struct {

	double BatteryInfos;
	double LowBatteryVoltage;
	double ShutdownBatteryVoltage;
	double X;
	double Y;
	double Z;
	double Th;
	double Vel;
	double RotVel;
	double LatVel;
	bool VoltageIsStateOfCharge;
	char Temperature;
	double Voltage;
	char Status[256];
	char Mode[256];
}RobotValues;

#endif
