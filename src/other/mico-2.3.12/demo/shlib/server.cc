/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <iostream.h>
#include "hello.h"

class HelloWorld_impl : virtual public HelloWorld_skel {
public:
    void hello (const char *s)
    {
	cout << s << endl;
    }
};

static HelloWorld_ptr server = HelloWorld::_nil();

extern "C" CORBA::Boolean
mico_module_init (const char *version)
{
    if (strcmp (version, MICO_VERSION))
	return FALSE;
    server = new HelloWorld_impl;
    return TRUE;
}

extern "C" void
mico_module_exit ()
{
    CORBA::release (server);
}
