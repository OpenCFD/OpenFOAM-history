//
//  MICO --- an Open Source CORBA implementation
//  Copyright (C) 2003, 2005 ObjectSecurity Ltd.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  Send comments and/or bug reports to:
//                 micosec@objectsecurity.com
//

//  Written by Sorin Mustaca <msorin@objectsecurity.com>

#ifdef _WIN32

//compile with /EHsc
/**
 *   This class prevents abnormal termination of a program because of 
 *	 an uncaught exception
 *   or a CTRL+C or CTRL+BREAK.This will allow a normal exit of the program 
 *	 by calling exit(-1). 
 *   It will not catch Acces Violation errors or other errors like it.
 */

#include<exception>
#include<iostream>
#include <memory>

#include "setterm.h"

using namespace std;

void unexpectedfunction()
{
   cerr << "Unexpected Exception caught. Program will exit" << endl;
   exit(-1);
}

void termfunction( ) //never call terminate() or throw an exception here
{
   cerr << "Unhandled Exception caught. Program will exit" << endl;
   exit(-1);
}

SetHandlers::SetHandlers()
{
	oldTHand = set_terminate(termfunction);
	
	oldXHand = set_unexpected(unexpectedfunction);
}

int SetHandlers::Instance()
{
	if(s.get() == NULL)
	{
		s.reset(new SetHandlers);
		return 1;
	}
	
	return 0;
}

 SetHandlers::~SetHandlers()
{
	set_terminate(oldTHand);
	set_unexpected(oldXHand);
}

auto_ptr<SetHandlers> SetHandlers::s;

static int __x__=SetHandlers::Instance();

#endif
