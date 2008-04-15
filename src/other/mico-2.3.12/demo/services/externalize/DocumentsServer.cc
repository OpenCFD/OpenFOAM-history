/*
 *  Demo of COSS Relationship Service
 *  Copyright (C) 1998 Karel Gardas
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
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */


#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include "Documents.h"
#include "Documents_impl.h"
#include <coss/RegisterHelper.h>

int main( int argc, char *argv[] )
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
	RegisterHelper::FinderType type(RegisterHelper::none);
	if (argc>0)
		if (!strcmp(argv[1],"-t"))
			type=RegisterHelper::trading;
		else
			type=RegisterHelper::naming;


    FigureFactory_impl* ff = new FigureFactory_impl(orb);

    TextFactory_impl* tf = new TextFactory_impl(orb);

    poa->activate_object (ff);
    poa->activate_object (tf);

    mgr->activate ();

	{
    RegisterHelper reg_help(orb,type);
	CosNaming::Name tf_id(4), ff_id(4);
	tf_id.length(4); ff_id.length(4);
	tf_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	tf_id[0].id = CORBA::string_dup( "::Documents::AbstractDocument" );
	tf_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	tf_id[1].id = CORBA::string_dup("::CosStream::Streamable");
	tf_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	tf_id[2].id = CORBA::string_dup("::Documents::Text");
	tf_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
	tf_id[3].id = CORBA::string_dup("TextFactory");

	reg_help.register_factory(tf->_this(),tf_id);

	ff_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	ff_id[0].id = CORBA::string_dup( "::Documents::AbstractDocument" );
	ff_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	ff_id[1].id = CORBA::string_dup("::CosStream::Streamable");
	ff_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	ff_id[2].id = CORBA::string_dup("::Documents::Figure");
	ff_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
	ff_id[3].id = CORBA::string_dup("FigureFactory");

	reg_help.register_factory(ff->_this(),ff_id);
	}

    orb->run ();
    return 0;
}

