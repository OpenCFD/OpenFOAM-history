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
#include <coss/CosLifeCycleReference.h>
#include <coss/LifeReference_impl.h>
#include <coss/RegisterHelper.h>


static void
usage(const char* progName)
{
    	std::cerr << "Usage:\n";
	std::cerr << progName << " [options]\n"
	"\n"
	"Options:\n"
	"-h, --help                Show this message.\n"
	"-v, --version             Show CosLifeCycleReference Service Factories version.\n"
	"-n, --with-naming         Try to register CosLifeCycleReference Service with Naming Service.\n"
	"-t, --with-trading        Try to register CosLifeCycleReference Service with Trading Service.\n"
	;
}


int main( int argc, char *argv[] )
{
    const char* progName = argv[0];
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

    RegisterHelper::FinderType type(RegisterHelper::none);

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
	    std::cerr << "Version of CosLifeCycleReference Service Factories is " << MICO_VERSION << std::endl;
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

    InterfaceDefsHelper* idefs_references = new InterfaceDefsHelper (orb);
    idefs_references->add ("IDL:omg.org/CosCompoundLifeCycle/Node:1.0");

    InterfaceDefsHelper* idefs_referencedby = new InterfaceDefsHelper (orb);
    idefs_referencedby->add ("IDL:omg.org/CosCompoundLifeCycle/Node:1.0");


    RoleFactory_impl<LifeCycleReferencesRole_impl>* references_role_factory 
	= new RoleFactory_impl<LifeCycleReferencesRole_impl>;

    references_role_factory->set_max_cardinality (4294967295ul);
    references_role_factory->set_related_object_types(idefs_references->get_interfaces ());
    references_role_factory->set_orb (orb);

    RoleFactory_impl<LifeCycleReferencedByRole_impl>* referencedby_role_factory 
	= new RoleFactory_impl<LifeCycleReferencedByRole_impl>;

    referencedby_role_factory->set_max_cardinality (4294967295ul);
    referencedby_role_factory->set_related_object_types(idefs_referencedby->get_interfaces ());
    referencedby_role_factory->set_orb (orb);
  

    PortableServer::ObjectId_var references_id 
	= PortableServer::string_to_ObjectId ("LifeCycleReferencesRole_impl");
    PortableServer::ObjectId_var referencedby_id 
	= PortableServer::string_to_ObjectId ("LifeCycleReferencedByRole_impl");

    NamedRoleTypesHelper*
	nrth_reference = new NamedRoleTypesHelper (orb);
    nrth_reference->add ("IDL:omg.org/CosLifeCycleReference/ReferencesRole:1.0", 
			 "LifeCycleReferencesRole");
    nrth_reference->add ("IDL:omg.org/CosLifeCycleReference/ReferencedByRole:1.0", 
			 "LifeCycleReferencedByRole");
  

    RelationshipFactory_impl<LifeCycleReference_impl>* reference_factory 
	= new RelationshipFactory_impl<LifeCycleReference_impl> (orb);
  
    reference_factory->set_orb (orb);
    reference_factory->set_named_role_types (nrth_reference->get_named_role_types ());
    reference_factory->set_degree (2);

    PortableServer::ObjectId_var reference_id 
	= PortableServer::string_to_ObjectId ("LifeCycleReference_impl");

    poa->activate_object_with_id (*references_id, references_role_factory);
    poa->activate_object_with_id (*referencedby_id, referencedby_role_factory);
    poa->activate_object_with_id (*reference_id, reference_factory);

    mgr->activate ();

	switch (type)
	{
		case RegisterHelper::none: break;
		case RegisterHelper::naming:
		case RegisterHelper::trading:
		{
			RegisterHelper reg_help(orb,type);
			CosLifeCycle::Key references_id(4),referencedby_id(4),reference_id(4);
			references_id.length(4);
			referencedby_id.length(4);
			reference_id.length(4);

			references_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			references_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Role" );
			references_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			references_id[1].id = CORBA::string_dup("");
			references_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			references_id[2].id = CORBA::string_dup( "::CosLifeCycleReference::ReferencesRole" );
			references_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			references_id[3].id = CORBA::string_dup( "RoleFactory" );

			reg_help.register_factory( references_role_factory->_this(),references_id);

			referencedby_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			referencedby_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Role" );
			referencedby_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			referencedby_id[1].id = CORBA::string_dup("");
			referencedby_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			referencedby_id[2].id = CORBA::string_dup( "::CosLifeCycleReference::ReferencedByRole" );
			referencedby_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			referencedby_id[3].id = CORBA::string_dup( "RoleFactory" );

			reg_help.register_factory( referencedby_role_factory->_this(),referencedby_id);

			reference_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
			reference_id[0].id = CORBA::string_dup( "::CosCompoundLifeCycle::Relationship" );
			reference_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
			reference_id[1].id = CORBA::string_dup("");
			reference_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
			reference_id[2].id = CORBA::string_dup( "::CosLifeCycleReference::Relationship" );
			reference_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
			reference_id[3].id = CORBA::string_dup( "RelationshipFactory" );

			reg_help.register_factory( reference_factory->_this(),reference_id);
		}
	}

    orb->run ();

    poa->destroy (TRUE, TRUE);

    return 0;
}

