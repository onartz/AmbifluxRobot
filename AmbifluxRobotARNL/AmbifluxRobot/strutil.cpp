/********************************************************************
 * OAjabber library
 * Contains objects to aid in Jabber client development and extend the
 * basic functionality of jabber.
 *
 * Copyright (C) 2001 Justin Kirby
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @author Justin Kirby <justink@rochester.rr.com>
 *
 *******************************************************************/
/********************************************************************************************

	File:			strutil.cpp
	Created On:		07/19/2000
	Created By:		Justin Kirby
	Modified On:	10/31/2000


	Description:
		To provide frequently used code fragments dealing specifically with strings and 
		encapsulate them into functions. possibly move into class eventually?
		

	Functions:
		strcpyalloc(char **dest, const char *src);
		strcatalloc(char **dest, const char *src);
		stralloc(char **dest, insigned int sz);
		strcnt(const char *src, char chr, int &cnt);
        strnistr(const char *src, const char *sub);
        strrealloc(char **dest, unsigned int sz);
        ctow(const char* src);
        wtoc(const wchar_t* src);
		

	Dependancies
		strutil.h

	Modifications:

	07/31/2000
		added functions:
			strcatalloc()
			stralloc()

	08/20/2000
		added functions
			strcnt()
		cleaned up memory leak in strcatalloc(). the temp storage was not being deleted before return

    10/31/2000
        added function
            strrealloc()
        cleaned up left overs from removing dependancy on redefs.h
    07/30/2001
        added functions
            ctow()
            wtoc()
********************************************************************************************/

//#include "first.h"
#include "strutil.h"
#include <string>

/*
	Verifies:
		src is pointing to something, i.e. its has data
		dest is a pointer to a char* initialized to zero

	Allocates enough memory to copy src to dest.
	Adds one byte for '\0' 

	returns the number of bytes copied to *dest
*/
void aipl::DelSpace(char* str)
{
char c, *p, *q;

	p = q = str;
	do
		while ((c = *p++) == ' ') ;
	while (*q++ = c);
	return;
}
