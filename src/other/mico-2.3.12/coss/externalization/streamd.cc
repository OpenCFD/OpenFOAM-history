/*
 *  Externalization Service for MICO
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


#include <coss/CosExternalization.h>
#include <coss/Externalization_impl.h>
#include <coss/CosTrading.h>
#include <coss/CosTradingRepos.h>

#ifdef USE_CORBA_2_1
#define CORBA_CXX_PREFIX(x) _##x
#else
#define CORBA_CXX_PREFIX(x) _cxx_##x
#endif


static void
usage(const char* progName)
{
	std::cerr << "Usage:\n";
	std::cerr << progName << " [options]\n"
	"\n"
	"Options:\n"
	"-h, --help                Show this message.\n"
	"-v, --version             Show CosExternalization::Stream Service Factories version.\n"
	"-t, --trading             Try to resolve Trading Service and export to it.\n"
	;
}


int main( int argc, char *argv[] )
{
    const char* progName = argv[0];
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

    CORBA::Boolean trader = false;
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
	    std::cerr << "Version of CosExternalizationContainment Service Factories is " << MICO_VERSION << std::endl;
	    return 0;
	}
	else if(strcmp(argv[i], "--trading") == 0 ||
		strcmp(argv[i], "-t") == 0)
	{
	    trader = true;
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


    StreamFactory_impl* stream_factory = new StreamFactory_impl ();

    PortableServer::ObjectId_var stream_factory_id = PortableServer::string_to_ObjectId ("Stream_impl");

    poa->activate_object_with_id (*stream_factory_id, stream_factory);

    FileStreamFactory_impl* fstream_factory = new FileStreamFactory_impl ();

    PortableServer::ObjectId_var fstream_factory_id = PortableServer::string_to_ObjectId ("FileStream_impl");

    poa->activate_object_with_id (*fstream_factory_id, fstream_factory);

    mgr->activate ();
    if (trader)
    {
	CORBA::Object_var tr_obj;
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    tr_obj = orb -> resolve_initial_references("TradingService");
#ifdef HAVE_EXCEPTIONS
	} catch(CORBA::ORB::InvalidName&) {
	    std::cerr << "Can`t resolve Trading Service. Abort" << std::endl;
	    exit(1);
	}
#endif
	if (CORBA::is_nil(tr_obj))
	{
	    std::cerr << "Can`t resolve Trading Service. Abort" << std::endl;
	    exit(1);
	}
	CosTrading::Lookup_var trader_ = CosTrading::Lookup::_narrow (tr_obj);
	CosTrading::Register_var reg = trader_->register_if();
	if (CORBA::is_nil(reg))
	{
	    std::cerr << "Invalid Trading Service. Abort" << std::endl;
	    exit(1);
	}
	CORBA::Object_var obj = trader_->type_repos();
	CosTradingRepos::ServiceTypeRepository_var repo =
		CosTradingRepos::ServiceTypeRepository::_narrow(obj);
	if (CORBA::is_nil(repo))
	{
	    std::cerr << "Invalid Trading Service. Abort" << std::endl;
	    exit(1);
	}

	CORBA::Boolean stream_type = false;
	CORBA::Boolean fstream_type = false;
	CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes all_;
	all_._d(CosTradingRepos::ServiceTypeRepository::all);
	CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* list_of_type = 
		repo -> list_types(all_);
	for( CORBA::ULong i = 0;i < list_of_type->length();++i)
	{
	    CosTradingRepos::ServiceTypeRepository::TypeStruct* tmp_type =
		    repo -> describe_type((*list_of_type)[i]);
	    if (!strcmp("StreamFactory",(*list_of_type)[i]) &&
		!strcmp("IDL:omg.org/CosExternalization/StreamFactory:1.0",tmp_type->if_name))
		stream_type = true;
	    if (!strcmp("FileStreamFactory",(*list_of_type)[i]) &&
		!strcmp("IDL:omg.org/CosExternalization/FileStreamFactory:1.0",tmp_type->if_name))
		fstream_type = true;
	    delete tmp_type;
	    if (stream_type && fstream_type)
		break;
	}
	delete list_of_type;
	if (!stream_type)
	{
	    CosTradingRepos::ServiceTypeRepository::IncarnationNumber i1;
	    {
		CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq super;
		super.length( 0 );
		CosTradingRepos::ServiceTypeRepository::PropStructSeq props;
		props.length( 3 );
		props[0].name = CORBA::string_dup( "ObjectInterface" );
		props[0].value_type = CORBA::_tc_string;
		props[0].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
		props[1].name = CORBA::string_dup( "ImplEquivClass" );
		props[1].value_type = CORBA::_tc_string;
		props[1].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
		props[2].name = CORBA::string_dup( "ObjectImpl" );
		props[2].value_type = CORBA::_tc_string;
		props[2].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;

		i1 = repo->add_type( "StreamFactory", "IDL:omg.org/CosExternalization/StreamFactory:1.0", props, super );
	    }
	}
	if (!fstream_type)
	{
	    CosTradingRepos::ServiceTypeRepository::IncarnationNumber i2;
	    {
		CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq super;
		super.length( 0 );
		CosTradingRepos::ServiceTypeRepository::PropStructSeq props;
		props.length( 3 );
		props[0].name = CORBA::string_dup( "ObjectInterface" );
		props[0].value_type = CORBA::_tc_string;
		props[0].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
		props[1].name = CORBA::string_dup( "ImplEquivClass" );
		props[1].value_type = CORBA::_tc_string;
		props[1].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
		props[2].name = CORBA::string_dup( "ObjectImpl" );
		props[2].value_type = CORBA::_tc_string;
		props[2].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
    
		i2 = repo->add_type( "FileStreamFactory", "IDL:omg.org/CosExternalization/FileStreamFactory:1.0", props, super );
    	    }
	}
	CosTrading::OfferId id1;
	CosTrading::OfferId id2;

	CosTrading::PropertySeq seq;
	seq.length(3);
	seq[0].name = CORBA::string_dup( "ObjectInterface" );
	seq[0].value <<= CORBA::Any::from_string( "::CosExternalization::Stream", 0 );
	seq[1].name = CORBA::string_dup( "ImplEquivClass" );
	seq[1].value <<= CORBA::Any::from_string("", 0);
	seq[2].name = CORBA::string_dup( "ObjectImpl" );
	seq[2].value <<= CORBA::Any::from_string("", 0);

	id1 = reg->CORBA_CXX_PREFIX(export)(stream_factory->_this(),
					    "StreamFactory", seq);

	seq.length(3);
	seq[0].name = CORBA::string_dup( "ObjectInterface" );
	seq[0].value <<= CORBA::Any::from_string( "::CosExternalization::Stream", 0 );
	seq[1].name = CORBA::string_dup( "ImplEquivClass" );
	seq[1].value <<= CORBA::Any::from_string("", 0);
	seq[2].name = CORBA::string_dup( "ObjectImpl" );
	seq[2].value <<= CORBA::Any::from_string("", 0);

        id2 = reg->CORBA_CXX_PREFIX(export)(fstream_factory->_this(),
					    "FileStreamFactory", seq);

    }
    orb->run ();

    poa->destroy (TRUE, TRUE);

    return 0;
}

