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

#include <string>
using std::string;

#ifndef _WIN32
    #include <stream.h>
#endif

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#include <CORBA.h>
#include <coss/Externalization_impl.h>
#include <coss/CosStream_impl.h>
#include <coss/RegisterHelper.h>

Stream_impl::Stream_impl(const char* filename)
    : POA_CosExternalization::Stream(),
      POA_CosLifeCycle::LifeCycleObject()
{
	filename_ = CORBA::string_dup(filename);
	factory_id_.length(4);
	factory_id_[0].id = CORBA::string_dup("::CosExternalization::Stream");
	factory_id_[1].id = CORBA::string_dup("");
	factory_id_[2].id = CORBA::string_dup("");
	if (filename == NULL)
		factory_id_[3].id = CORBA::string_dup("StreamFactory");
	else
		factory_id_[3].id = CORBA::string_dup("FileStreamFactory");
	factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
	factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
	factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
	factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);

    ifstream_ = NULL;
    ofstream_ = NULL;
    context = false;
}

Stream_impl::~Stream_impl()
{
    if(ifstream_)
    {
	delete ifstream_;
	ifstream_ = NULL;
    }
    if(ofstream_) {
	delete ofstream_;
	ofstream_ = NULL;
    }
}

CosLifeCycle::LifeCycleObject_ptr
Stream_impl::copy(CosLifeCycle::FactoryFinder_ptr there,
		  const CosLifeCycle::Criteria& the_criteria)
{

    CosLifeCycle::Factories* stream_factories_ptr = there -> find_factories(factory_id_);
    if (!(*stream_factories_ptr).length())
    {
	delete stream_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw (ex);
    }
    CORBA::ULong i;
    CosExternalization::Stream_ptr new_stream_ptr;
    if (filename_)
    {
	for (i = 0;i < (*stream_factories_ptr).length();++i)
	{
	    CosExternalization::FileStreamFactory_ptr streamfactory_ptr =
		CosExternalization::FileStreamFactory::_narrow((*stream_factories_ptr)[i]);
	    if (CORBA::is_nil(streamfactory_ptr))
		continue;
#ifdef HAVE_EXCEPTIONS
	    try {
#endif
		    new_stream_ptr = (*streamfactory_ptr).create(this->filename_);
#ifdef HAVE_EXCEPTIONS
	    } catch(...) {
		continue;
	    }
#endif
	    if (CORBA::is_nil(new_stream_ptr))
		continue;
	    break;
	}
	if (i == (*stream_factories_ptr).length())
	{
	    delete stream_factories_ptr;
	    CosLifeCycle::NoFactory ex;
	    ex.search_key = factory_id_;
	    mico_throw (ex);
	}
    }
    else {
	for (i = 0;i < (*stream_factories_ptr).length();++i)
	{
	    CosExternalization::StreamFactory_ptr streamfactory_ptr =
		CosExternalization::StreamFactory::_narrow((*stream_factories_ptr)[i]);
	    if (CORBA::is_nil(streamfactory_ptr))
		continue;
#ifdef HAVE_EXCEPTIONS
	    try {
#endif
		new_stream_ptr = (*streamfactory_ptr).create();
#ifdef HAVE_EXCEPTIONS
	    } catch(...) {
		continue;
	    }
#endif
	    if (CORBA::is_nil(new_stream_ptr))
		continue;
	    break;
	}
	if (i == (*stream_factories_ptr).length())
	{
	    delete stream_factories_ptr;
	    CosLifeCycle::NoFactory ex;
	    ex.search_key = factory_id_;
	    mico_throw (ex);
	}
    }

    delete stream_factories_ptr;
    return new_stream_ptr;
}

void
Stream_impl::move(CosLifeCycle::FactoryFinder_ptr there,
		  const CosLifeCycle::Criteria& the_criteria)
{
}

void
Stream_impl::remove()
{
    this->flush();
	if (ofstream_)
	    if (ofstream_!=&cout) (*(ofstream *)ofstream_).close();
	if (ifstream_)
	    if (ifstream_!=&cin) (*(ifstream *)ifstream_).close();
    PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
    _default_POA ()->deactivate_object (*oid);

    delete oid;
    delete this;
}


void
Stream_impl::externalize(CosStream::Streamable_ptr theObject)
{

    if (!(char *)filename_)
	ofstream_ = &cout;
    else {
	if (!this->context) {
	    ofstream* out = new ofstream;
	    (*out).open(filename_,ios::out);
	    if ((*out).fail())
	    {
		cerr << "Can't open file: " << filename_ << " output to stdout." << endl;
		ofstream_ = &cout;
		delete out;
		CORBA::string_free(filename_);
	    }
	    else
		ofstream_ = out;
	}
    }
    CosLifeCycle::Key_var object_key_var((*theObject).external_form_id());
    (*ofstream_) << tag_object << (char)object_key_var->length();
    for(CORBA::ULong i = 0;i < object_key_var->length();++i)
	(*ofstream_) << object_key_var[i].id << '\0';
    StreamIO_impl* sio_impl = new StreamIO_impl(ofstream_);
    CosStream::StreamIO_ptr this_sio = sio_impl->_this();
    (*theObject).externalize_to_stream(this_sio);
    CORBA::release(this_sio);
    if (!this->context)
    {
	if (ofstream_!=&cout) (*(ofstream *)ofstream_).close();
	delete ofstream_;
	ofstream_ = NULL;
    }
}

CosStream::Streamable_ptr
Stream_impl::internalize(CosLifeCycle::FactoryFinder_ptr there)
{

    if (!(char *)filename_)
	ifstream_ = &cin;
    else {
	if (!this->context) {
	    ifstream* in = new ifstream;
	    (*in).open(filename_,ios::in);
	    if ((*in).fail())
	    {
		cerr << "Can't open file: " << filename_ << " input from stdin." << endl;
		ifstream_ = &cin;
		delete in;
		CORBA::string_free(filename_);
	    }
	    else
		ifstream_ = in;
	}
    }
    char tag_;
    char key_count;
    CosLifeCycle::Key factory_key;
    tag_ = (char)(*ifstream_).peek();
    if (tag_ != tag_object)
    {
	CosStream::StreamDataFormatError ex;
	mico_throw(ex);
    }
    (*ifstream_) >> tag_;
    (*ifstream_) >> key_count;
    factory_key.length((CORBA::UShort)key_count);
    CORBA::ULong i;
    for(i = 0;i < (CORBA::UShort)key_count;++i)
    {
	string key;
	(*ifstream_) >> tag_;
	while(tag_ && !(*ifstream_).eof()) {
	    key += tag_;
	    (*ifstream_) >> tag_;
	}
	if ((*ifstream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	factory_key[i].id = CORBA::string_dup(key.c_str());
	switch(i) {
	    case 0:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_object_interface);
		break;
	    case 1:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
		break;
	    case 2:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_object_implementation);
		break;
	    case 3:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_factory_interface);
		break;
	    default:
		factory_key[i].kind = CORBA::string_dup("Custom kind");
	}
    }

    CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_key);
    if (!(*object_factories_ptr).length())
    {
	delete object_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_key;
	mico_throw (ex);
    }
//    CORBA::ULong i;
    CosStream::Streamable_ptr new_object_ptr;
    for (i = 0;i < (*object_factories_ptr).length();++i)
    {
	CosStream::StreamableFactory_ptr objectfactory_ptr =
	    CosStream::StreamableFactory::_narrow((*object_factories_ptr)[i]);
	if (CORBA::is_nil(objectfactory_ptr))
	    continue;
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    new_object_ptr = (*objectfactory_ptr).create_uninitialized();
#ifdef HAVE_EXCEPTIONS
	} catch (...) {
	    continue;
	}
#endif
	if (CORBA::is_nil(new_object_ptr))
	    continue;
	break;
    }
    if (i == (*object_factories_ptr).length())
    {
	delete object_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_key;
	mico_throw (ex);
    }
    delete object_factories_ptr;
    StreamIO_impl* sio_impl = new StreamIO_impl(ifstream_);
    CosStream::StreamIO_ptr this_sio = sio_impl->_this();
    (*new_object_ptr).internalize_from_stream(this_sio,there);
    CORBA::release(this_sio);
    if (!this->context)
    {
//	if (ifstream_ != &cin) (*(ifstream *)ifstream_).close();
	delete ifstream_;
	ifstream_ = NULL;
    }
    return new_object_ptr;
}

void
Stream_impl::begin_context()
{

    if (this->context)
    {
	CosExternalization::ContextAlreadyRegistered ex;
	mico_throw(ex);
    }
    ofstream* out = new ofstream;
    (*out).open(filename_,ios::out);
    if ((*out).fail())
    {
	cerr << "Can't open file: " << filename_ << " output to stdout." << endl;
	ofstream_ = &cout;
	delete out;
	CORBA::string_free(filename_);
    }
    else
	ofstream_ = out;
    this->context=true;
}

void
Stream_impl::end_context()
{

    if (this->context) {
	if (ofstream_) if (ofstream_ != &cout) (*(ofstream *)ofstream_).close();
	if (ifstream_) if (ifstream_ != &cin) (*(ifstream *)ifstream_).close();
	this->context=false;
    }
}

void
Stream_impl::flush()
{

    if (!(char *)filename_)
	cout.flush();
    else {
	if (ofstream_)
	    (*ofstream_).flush();
    }
}

StreamFactory_impl::StreamFactory_impl()
    : POA_CosExternalization::StreamFactory()
{
}

CosExternalization::Stream_ptr
StreamFactory_impl::create()
{
    Stream_impl* stream = new Stream_impl();
    CosExternalization::Stream_ptr retval = stream->_this ();

    return retval; 
}

FileStreamFactory_impl::FileStreamFactory_impl()
    : POA_CosExternalization::FileStreamFactory()
{
}

CosExternalization::Stream_ptr
FileStreamFactory_impl::create(const char* theFileName)
{
    Stream_impl* stream = new Stream_impl(theFileName);
    CosExternalization::Stream_ptr retval = stream->_this ();

    return retval; 
};
