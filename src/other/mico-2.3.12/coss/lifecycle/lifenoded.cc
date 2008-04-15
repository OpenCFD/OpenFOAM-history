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


#include <coss/InterfaceDefsHelper.h>
#include <coss/NamedRoleTypesHelper.h>
#include <coss/CosCompoundLifeCycle.h>
#include <coss/RoleFactory_impl.h>
#include <coss/RelationshipFactory_impl.h>
#include <coss/NodeFactory_impl.h>
#include <coss/LifeNode_impl.h>
#include <coss/RegisterHelper.h>


static void
usage(const char* progName)
{
        std::cerr << "Usage:\n";
	std::cerr << progName << " [options]\n"
	"\n"
	"Options:\n"
	"-h, --help                Show this message.\n"
	"-v, --version             Show CosCompoundLifeCycle::Node Service Factories version.\n"
	"-n, --with-naming         Try to register CosCompoundLifeCycle::Node Service with Naming Service.\n"
	"-t, --with-trading        Try to register CosCompoundLifeCycle::Node Service with Trading Service.\n"
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
	    std::cerr << "Version of CosCompoundLifeCycle::Node Service Factories is " << MICO_VERSION << std::endl;
	    return 0;
	}
	else if(strcmp(argv[i], "--with-naming") == 0 ||
		strcmp(argv[i], "-n") == 0)
	{
	    type = RegisterHelper::naming;
	}
	else if(strcmp(argv[i], "--with-trading") == 0 ||
		strcmp(argv[i], "-t") == 0)
	{
	    type = RegisterHelper::trading;
	}
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


    NodeFactory_impl<CosCompoundLifeCycle_impl::Node_impl>* node_factory 
	= new NodeFactory_impl<CosCompoundLifeCycle_impl::Node_impl> (orb);

    PortableServer::ObjectId_var node_factory_id 
	= PortableServer::string_to_ObjectId ("CosCompoundLifeCycle_Node_impl");

    poa->activate_object_with_id (*node_factory_id, node_factory);

    mgr->activate ();

	switch (type)
	{
		case RegisterHelper::none: break;
		case RegisterHelper::naming:
		case RegisterHelper::trading:
		{
			RegisterHelper reg_help(orb,type);
			CosLifeCycle::Key node_id(4);
			node_id.length(4);
			node_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			node_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Node" );
			node_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			node_id[1].id = CORBA::string_dup("");
			node_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			node_id[2].id = CORBA::string_dup( "" );
			node_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			node_id[3].id = CORBA::string_dup( "NodeFactory" );

			reg_help.register_factory( node_factory->_this(),node_id);
		}
	}

    orb->run ();

    poa->destroy (TRUE, TRUE);

    return 0;
}

