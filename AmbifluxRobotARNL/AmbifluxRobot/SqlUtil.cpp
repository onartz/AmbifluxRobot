#include "sqlutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
int  aipllib::convertSQL_TIMESTAMP_STRUCTToString(char *dest, const SQL_TIMESTAMP_STRUCT *timeStamp)
{	
sprintf(dest,"%d-%02d-%02d %02d:%02d:%02d",timeStamp->year,timeStamp->month,timeStamp->day,timeStamp->hour,timeStamp->minute,timeStamp->second);
return(0);
}

int aipllib::convertStringToSQL_TIMESTAMP_STRUCT(SQL_TIMESTAMP_STRUCT *dest, char *str)
{
	std::string s,strYear, strMonth, strDay, strHour, strMinute, strSecond;
	/*char* strYear;*/
	int year(0), month(0), day(0), hour(0), minute(0), second(0);
	s=str;

	if(s.length()<19)
		return(-1);
	strYear=s.substr(0,4);
	strMonth=s.substr(5,2);
	strDay=s.substr(8,2);
	strHour=s.substr(11,2);
	strMinute=s.substr(14,2);
	strSecond=s.substr(17,2);
	
	if(EOF == sscanf(strYear.c_str(), "%d", &year))
	{
	  return(-1);
	}
	if(year==0) year=1900;
	if(EOF == sscanf(strMonth.c_str(), "%d", &month))
	{
	  return(-1);
	}
	if(month==0) month=1;
	if(EOF == sscanf(strDay.c_str(), "%d", &day))
	{
	  return(-1);
	}
	if(day==0) day=1;
	if(EOF == sscanf(strHour.c_str(), "%d", &hour))
	{
	  return(-1);
	}
	if(EOF == sscanf(strMinute.c_str(), "%d", &minute))
	{
	  return(-1);
	}
	if(EOF == sscanf(strSecond.c_str(), "%d", &second))
	{
	  return(-1);
	}

	dest->year=year;
	dest->month=month;
	dest->day=day;
	dest->hour=hour;
	dest->minute=minute;
	dest->second=second;
	dest->fraction=0;
	
return 0;
}
