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


#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include "Documents_impl.h"
#include <coss/RegisterHelper.h>

//#define DEBUG 1


AbstractDocument_impl::AbstractDocument_impl () 
    : POA_Documents::AbstractDocument(),
      POA_CosStream::Streamable(),
      IdentifiableObject_impl()
{
    doc_name = CORBA::string_dup ("");
    file_name = CORBA::string_dup ("");
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::Documents::AbstractDocument");
    external_from_id_[1].id = CORBA::string_dup("::CosStream::Streamable");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

AbstractDocument_impl::AbstractDocument_impl (const char* name)
    : POA_Documents::AbstractDocument(),
      POA_CosStream::Streamable(),
      IdentifiableObject_impl()
{
    doc_name = CORBA::string_dup (name);
    file_name = CORBA::string_dup ("");
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::Documents::AbstractDocument");
    external_from_id_[1].id = CORBA::string_dup("::CosStream::Streamable");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

AbstractDocument_impl::AbstractDocument_impl (const char* value, 
					      const char* file)
    : POA_Documents::AbstractDocument(),
      POA_CosStream::Streamable(),
      IdentifiableObject_impl()
{
    doc_name = CORBA::string_dup (value);
    file_name = CORBA::string_dup (file);
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::Documents::AbstractDocument");
    external_from_id_[1].id = CORBA::string_dup("::CosStream::Streamable");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

char*
AbstractDocument_impl::name ()
{
    return CORBA::string_dup (doc_name);
}

void
AbstractDocument_impl::name (const char* value)
{
    CORBA::string_free(doc_name);
    doc_name = CORBA::string_dup (value);
}

char*
AbstractDocument_impl::file ()
{
    return CORBA::string_dup (file_name);
}

void
AbstractDocument_impl::file (const char* value)
{
    CORBA::string_free(file_name);
    file_name = CORBA::string_dup (value);
}

void
AbstractDocument_impl::destroy ()
{
    CORBA::string_free(doc_name);
    CORBA::string_free(file_name);
#if USE_VAR
    PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
#else
    PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
#endif
    _default_POA ()->deactivate_object (*oid);

#ifndef USE_VAR
    delete oid;
#endif
    delete this;
}

CosLifeCycle::Key*
AbstractDocument_impl::external_form_id()
{
    return new CosLifeCycle::Key(external_from_id_);
}

void
AbstractDocument_impl::externalize_to_stream( CosStream::StreamIO_ptr targetStreamIO )
{
//  Write out it's non-object data using the primitive StreamIO::write_...(data) functions
    targetStreamIO -> write_unsigned_long(this->constant_random_id());
    targetStreamIO -> write_string(this->doc_name);
    targetStreamIO -> write_string(this->file_name);
}

void
AbstractDocument_impl::internalize_from_stream( CosStream::StreamIO_ptr sourceStreamIO, CosLifeCycle::FactoryFinder_ptr there )
{
    random_id = sourceStreamIO -> read_unsigned_long();
    doc_name = sourceStreamIO -> read_string();
    file_name = sourceStreamIO -> read_string();
}

Text_impl::Text_impl ()
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Text");
    external_from_id_[3].id = CORBA::string_dup("TextFactory");
}


Text_impl::Text_impl (const char* name)
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Text");
    external_from_id_[3].id = CORBA::string_dup("TextFactory");
    doc_name = CORBA::string_dup (name);
}


Text_impl::Text_impl (const char* name, const char* file)
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Text");
    external_from_id_[3].id = CORBA::string_dup("TextFactory");
    doc_name = CORBA::string_dup (name);
    file_name = CORBA::string_dup (file);
}

DocumentFactory_impl::DocumentFactory_impl(CORBA::ORB_ptr _orb)
    :POA_CosStream::StreamableFactory()
{
	IdentifiableObject_impl* tmp = new IdentifiableObject_impl (_orb);
}

CosStream::Streamable_ptr
DocumentFactory_impl::create_uninitialized ()
{
#if DEBUG
    cout << "  <Documents_impl> create uninitialized AbstractDocument\n";
#endif
    AbstractDocument_impl* doc = new AbstractDocument_impl ();
    return doc->_this ();
}  

FigureFactory_impl::FigureFactory_impl(CORBA::ORB_ptr _orb)
    : DocumentFactory_impl(_orb),
      POA_Documents::FigureFactory()
{
}

Documents::Figure_ptr
FigureFactory_impl::create (const char* name)
{
#if DEBUG
    cout << "  <Documents_impl> create figure\n";
#endif
    Figure_impl* fig = new Figure_impl (name);
    return fig->_this ();
}

Documents::Figure_ptr
FigureFactory_impl::create_with_file (const char* name, const char* file)
{
#if DEBUG
    cout << "  <Documents_impl> create figure\n";
#endif
    Figure_impl* fig = new Figure_impl (name, file);
    return fig->_this ();
}

TextFactory_impl::TextFactory_impl(CORBA::ORB_ptr _orb)
    : DocumentFactory_impl(_orb),
      POA_Documents::TextFactory()
{
}

Documents::Text_ptr
TextFactory_impl::create (const char* name)
{
#if DEBUG
    cout << "  <Documents_impl> create Text\n";
#endif
    Text_impl* text = new Text_impl (name);
    return text->_this ();
}

Documents::Text_ptr
TextFactory_impl::create_with_file (const char* name, const char* file)
{
#if DEBUG
    cout << "  <Documents_impl> create Text\n";
#endif
    Text_impl* text = new Text_impl (name, file);
    return text->_this ();
}

Figure_impl::Figure_impl ()
  : AbstractDocument_impl (),
    POA_Documents::Figure()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Figure");
    external_from_id_[3].id = CORBA::string_dup("FigureFactory");
}


Figure_impl::Figure_impl (const char* name)
  : AbstractDocument_impl (),
    POA_Documents::Figure()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Figure");
    external_from_id_[3].id = CORBA::string_dup("FigureFactory");
    doc_name = CORBA::string_dup (name);
}


Figure_impl::Figure_impl (const char* name, const char* file)
  : AbstractDocument_impl (),
    POA_Documents::Figure()
{
    external_from_id_[2].id = CORBA::string_dup("::Documents::Figure");
    external_from_id_[3].id = CORBA::string_dup("FigureFactory");
    doc_name = CORBA::string_dup (name);
    file_name = CORBA::string_dup (file);
}

