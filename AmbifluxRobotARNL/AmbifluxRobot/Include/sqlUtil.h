#ifndef DEF_SQLUTIL_H
#define DEF_SQLUTIL_H

#define _WINSOCKAPI_
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

namespace aipllib
{
int convertSQL_TIMESTAMP_STRUCTToString(char *dest, const SQL_TIMESTAMP_STRUCT *);
int convertStringToSQL_TIMESTAMP_STRUCT(SQL_TIMESTAMP_STRUCT *, char *);

};
#endif