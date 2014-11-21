#ifndef DEF_STRUTIL_H
#define DEF_STRUTIL_H

#include "stddef.h"

#define MAX_STR (255)//useful number ;)
#define BIG_STR (512)
#define HUGE_STR (1024)

#define BIG_A	(0x41)
#define BIG_Z	(0x5a)
#define SMALL_A	(0x61)
#define SMALL_Z (0x7a)
#define STOB	(0x20)	//substract this to make a lowercase letter upper case

namespace aipl
{
int convertDateTimeToString(char **dest, const char *src);
//allocate dest and copy src to dest
//returns number of bytes copied
unsigned int strcpyalloc(char **dest, const char *src);
//reallocates dest and appends contents of src
//returns total size of dest
unsigned int strcatalloc(char **dest, const char *src);
//allocates a string of size sz
unsigned int stralloc(char **dest, unsigned int sz);
//reallocates a string and maintains original data
unsigned int strrealloc(char **dest, unsigned int sz);
//counts number of characters in string
unsigned int strcnt(const char *src, char chr, int &cnt);
//same as strstr only case insensitive
char* strnistr(const char *src, const char *sub);
//encapsulate wchar_t to char converstions
wchar_t* ctow(const char* src);
//encapsulate char to wchar_t converstions
//NOTE: imp contains hack, need "real" fix
char* wtoc(const wchar_t* src);
//Supprime les espacs inutiles dune string
void DelSpace(char *);
};
#endif