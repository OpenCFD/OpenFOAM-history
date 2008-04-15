/*
 *  LifeCycle Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 sedov@postman.simcb.ru
 */


#include <coss/CosCompoundLifeCycle.h>
#include <coss/GenericFactory_impl.h>

static void
usage(const char* progName)
{
    	std::cerr << "Usage:\n";
	std::cerr << progName << " [options]\n"
	"\n"
	"Options:\n"
	"-h, --help                Show this message.\n"
	"-v, --version             Show CosLifeCycle::GenericFactory Service version.\n"
	"-n, --with-naming         Find Factories with Naming Service.\n"
	"-t, --with-trading        Find Factories with Trading Service.\n"
	;
}


int main( int argc, char *argv[] )
{
    const char* progName = argv[0];
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
	RegisterHelper::FinderType type = RegisterHelper::none;
    CORBA::Long i;
    for(i = 1 ; i < argc && *argv[i] == '-' ; i++)
    {
	if(strcmp(argv[i], "--help") == 0 ||
	    strcmp(argv[i], "-h") == 0)
	{
	    usage(progName);
	    return 0;
	}
	else if(strcmp(argv[i], "--version") == 0 ||
		strcmp(argv[i], "-v") == 0)
	{
	    std::cerr << "Version of CosLifeCycle::GenericFactory Service is " << MICO_VERSION << std::endl;
	    return 0;
	}
	else if(strcmp(argv[i], "--with-naming") == 0 ||
			strcmp(argv[i], "-n") == 0)
			type = RegisterHelper::naming;
	else if(strcmp(argv[i], "--with-trading") == 0 ||
			strcmp(argv[i], "-t") == 0)
			type = RegisterHelper::trading;
	else
	{
	    std::cerr << progName << ": unknown option `"
		 << argv[i] << "'\n" << std::endl;
	    usage(progName);
	    return 1;
	}
    }

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    GenericFactory_impl* genericfactory =
	new GenericFactory_impl(orb,type);

    PortableServer::ObjectId_var genericfactory_id 
	= PortableServer::string_to_ObjectId ("GenericFactory_impl");

    poa->activate_object_with_id (*genericfactory_id, genericfactory);

    mgr->activate ();
    orb->run ();

    poa->destroy (TRUE, TRUE);

    return 0;
}

