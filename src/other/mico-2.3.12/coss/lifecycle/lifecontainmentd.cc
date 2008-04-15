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
#include <coss/CosLifeCycleContainment.h>
#include <coss/LifeContainment_impl.h>
#include <coss/RegisterHelper.h>


static void
usage(const char* progName)
{
    	std::cerr << "Usage:\n";
	std::cerr << progName << " [options]\n"
	"\n"
	"Options:\n"
	"-h, --help                Show this message.\n"
	"-v, --version             Show CosLifeCycleContainment Service Factories version.\n"
	"-n, --with-naming         Try to register CosLifeCycleContainment Service with Naming Service.\n"
	"-t, --with-trading        Try to register CosLifeCycleContainment Service with Trading Service.\n"
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
	    std::cerr << "Version of CosLifeCycleContainment Service Factories is " << MICO_VERSION << std::endl;
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

    InterfaceDefsHelper* idefs_contains = new InterfaceDefsHelper (orb);
    idefs_contains->add ("IDL:omg.org/CosCompoundLifeCycle/Node:1.0");

    InterfaceDefsHelper* idefs_containedin = new InterfaceDefsHelper (orb);
    idefs_containedin->add ("IDL:omg.org/CosCompoundLifeCycle/Node:1.0"); 

    RoleFactory_impl<LifeCycleContainsRole_impl>* contains_role_factory 
	= new RoleFactory_impl<LifeCycleContainsRole_impl>;

    contains_role_factory->set_max_cardinality (4294967295ul);
    contains_role_factory->set_related_object_types(idefs_contains->get_interfaces ());
    contains_role_factory->set_orb (orb);

    RoleFactory_impl<LifeCycleContainedInRole_impl>* containedin_role_factory 
	= new RoleFactory_impl<LifeCycleContainedInRole_impl>;

    containedin_role_factory->set_max_cardinality (1);
    containedin_role_factory->set_related_object_types(idefs_containedin->get_interfaces ());
    containedin_role_factory->set_orb (orb);

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    PortableServer::ObjectId_var contains_id 
	= PortableServer::string_to_ObjectId ("LifeCycleContainsRole_impl");
    PortableServer::ObjectId_var containedin_id 
	= PortableServer::string_to_ObjectId ("LifeCycleContainedInRole_impl");



    NamedRoleTypesHelper*
	nrth_containment = new NamedRoleTypesHelper (orb);
    nrth_containment->add ("IDL:omg.org/CosLifeCycleContainment/ContainsRole:1.0", 
			   "LifeCycleContainsRole");
    nrth_containment->add ("IDL:omg.org/CosLifeCycleContainment/ContainedInRole:1.0", 
			   "LifeCycleContainedInRole");


    RelationshipFactory_impl<LifeCycleContainment_impl>* containment_factory 
	= new RelationshipFactory_impl<LifeCycleContainment_impl> (orb);
  
    containment_factory->set_orb (orb);
    containment_factory->set_named_role_types (nrth_containment->get_named_role_types ());
    containment_factory->set_degree (2);

    PortableServer::ObjectId_var containment_id 
	= PortableServer::string_to_ObjectId ("LifeCycleContainment_impl");

    poa->activate_object_with_id (*contains_id, contains_role_factory);
    poa->activate_object_with_id (*containedin_id, containedin_role_factory);
    poa->activate_object_with_id (*containment_id, containment_factory);

    mgr->activate ();

	switch(type)
	{
		case RegisterHelper::none: break;
		case RegisterHelper::naming:
		case RegisterHelper::trading:
    	{
			RegisterHelper reg_help(orb,type);
			CosLifeCycle::Key contains_id(4),contained_id(4),containment_id(4);
			contains_id.length(4);
			contained_id.length(4);
			containment_id.length(4);

			contains_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			contains_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Role" );
			contains_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			contains_id[1].id = CORBA::string_dup("");
			contains_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			contains_id[2].id = CORBA::string_dup( "::CosLifeCycleContainment::ContainsRole" );
			contains_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			contains_id[3].id = CORBA::string_dup( "RoleFactory" );

			reg_help.register_factory( contains_role_factory->_this(),contains_id);

			contained_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			contained_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Role" );
			contained_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			contained_id[1].id = CORBA::string_dup("");
			contained_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			contained_id[2].id = CORBA::string_dup( "::CosLifeCycleContainment::ContainedInRole" );
			contained_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			contained_id[3].id = CORBA::string_dup( "RoleFactory" );

			reg_help.register_factory( containedin_role_factory->_this(),contained_id);

			containment_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			containment_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Relationship" );
			containment_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			containment_id[1].id = CORBA::string_dup("");
			containment_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			containment_id[2].id = CORBA::string_dup( "::CosLifeCycleContainment::Relationship" );
			containment_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			containment_id[3].id = CORBA::string_dup( "RelationshipFactory" );

			reg_help.register_factory( containment_factory->_this(),containment_id);
    	}
	}

    orb->run ();

    poa->destroy (TRUE, TRUE);

    return 0;
}

