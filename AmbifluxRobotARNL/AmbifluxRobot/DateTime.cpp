#include "DateTime.h"
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss 
const char* aipllib::currentDateTime()
{    
	time_t now = time(0);     
	struct tm  tstruct;     
	char       buf[80];     
	//tstruct = *localtime_s(&now); 
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);     
	return buf; 
}