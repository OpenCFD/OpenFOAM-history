#include <iostream.h>
// sander include <sys/time.h>
#include <CORBA.h>
#include "hello.h"
#include <coss/CosExternalization.h>


int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");
    cout << "Binding to HelloFactory ..." << flush;
    assert (argc == 2);
    CORBA::Object_var 
	objhf = orb->bind ("IDL:omg.org/CosStream/StreamableFactory:1.0",argv[1]);
    assert (!CORBA::is_nil (objhf));
    CosStream::StreamableFactory_var hello_factory; 
    if (hello_factory = CosStream::StreamableFactory::_narrow (objhf))
	cout << "done.\n" << flush;
    else
	cout << "ERROR!\n" << flush;

    cout << "Binding to FactoryFinder ..." << flush;
    CORBA::ORB::ObjectTag_var 
	tagff = CORBA::ORB::string_to_tag ("FactoryFinder_impl");
    CORBA::Object_var 
	objff = orb->bind ("IDL:omg.org/CosLifeCycle/FactoryFinder:1.0", tagff.in());
    assert (!CORBA::is_nil (objff));
    CosLifeCycle::FactoryFinder_var factory_finder; 
    if (factory_finder = CosLifeCycle::FactoryFinder::_narrow (objff))
	cout << "done.\n" << flush;
    else
	cout << "ERROR!\n" << flush;

    cout << "Binding to FileStream ..." << flush;
    CORBA::ORB::ObjectTag_var 
	taggf = CORBA::ORB::string_to_tag ("FileStream_impl");
    CORBA::Object_var 
	objsf = orb->bind ("IDL:omg.org/CosExternalization/FileStreamFactory:1.0", taggf.in());
    assert (!CORBA::is_nil (objsf));
    CosExternalization::FileStreamFactory_var fstream_factory;
    if (fstream_factory = CosExternalization::FileStreamFactory::_narrow (objsf))
	cout << "done.\n" << flush;
    else
	cout << "ERROR!\n" << flush;

    cout << "Check CosStream::StreamableFactory::create_uninitialized() ...";
    CORBA::Boolean good=TRUE;
    Hello_var hello;
    try {
	hello = Hello::_narrow(hello_factory->create_uninitialized());
    } catch(...) {
	good=FALSE;
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    hello->hello_string("Hello");
    CORBA::String_var s = hello->hello ("MICO");

    cout << "Check CosExternalization::FileStreamFactory::create() ...";
    good=TRUE;
    CosExternalization::Stream_var stream;
    try {
	stream = fstream_factory->create("test.ext");
    } catch(CosExternalization::InvalidFileNameError& ex) {
	good=FALSE;
	cout << "Can not create file!";
    } catch(...) {
	good=FALSE;
	cout << "System exception";
    }

    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    cout << "Check CosExternalization::Stream::externalize() ...";
    good=TRUE;
    try {
	stream->externalize(hello.in());
    } catch(...) {
	good=FALSE;
	cout << "System exception";
    }

    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    Hello_var hello1;
    good=TRUE;
    cout << "Check CosExternalization::Stream::internalize() ...";
    try {
	hello1 = Hello::_narrow(stream->internalize(factory_finder.in()));
    } catch(CosLifeCycle::NoFactory& ex) {
	good=FALSE;
	cout << "Factory not found!";
    } catch(CosStream::StreamDataFormatError& ex) {
	good=FALSE;
	cout << "Stream data format error!";
    } catch(...) {
	good=FALSE;
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    s = hello1->hello ("- I am internalize!");

    cout << "Create stream with bad file ...";
    good=TRUE;
    try {
	stream = fstream_factory->create("bad.ext");
    } catch(CosExternalization::InvalidFileNameError& ex) {
	good=FALSE;
	cout << "Can not create file!";
    } catch(...) {
	good=FALSE;
	cout << "System exception";
    }

    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    cout << "Check CosStream::StreamDataFormatError ...";
    good=FALSE;
    try {
	hello1 = Hello::_narrow(stream->internalize(factory_finder.in()));
    } catch(CosLifeCycle::NoFactory& ex) {
	cout << "Factory not found!";
    } catch(CosStream::StreamDataFormatError& ex) {
	good=TRUE;
    } catch(...) {
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    return 0;
}
