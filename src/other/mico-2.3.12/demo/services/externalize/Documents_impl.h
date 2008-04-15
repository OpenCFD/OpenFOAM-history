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


#include "Documents.h"
#include <coss/CosExternalization.h>
#include <coss/IdentifiableObject_impl.h>

#ifndef __Documents_impl_h__
#define __Documents_impl_h__


class AbstractDocument_impl : virtual public POA_Documents::AbstractDocument,
			      virtual public POA_CosStream::Streamable,
			      virtual public IdentifiableObject_impl
{
protected:
    CosLifeCycle::Key external_from_id_;
    char* doc_name;
    char* file_name;
public:
    AbstractDocument_impl ();
    AbstractDocument_impl (const char*);
    AbstractDocument_impl (const char*, const char*);
    virtual char* name ();
    virtual void name (const char*);
    virtual char* file ();
    virtual void file (const char*);
    virtual void destroy ();
    virtual CosLifeCycle::Key* external_form_id();

    virtual void externalize_to_stream( CosStream::StreamIO_ptr targetStreamIO );
    virtual void internalize_from_stream( CosStream::StreamIO_ptr sourceStreamIO, CosLifeCycle::FactoryFinder_ptr there );
};

class Text_impl : virtual public AbstractDocument_impl,
		  virtual public POA_Documents::Text
{
public:
    Text_impl ();
    Text_impl (const char*);
    Text_impl (const char*, const char*);
};

class Figure_impl : virtual public AbstractDocument_impl,
		    virtual public POA_Documents::Figure
{
public:
    Figure_impl ();
    Figure_impl (const char*);
    Figure_impl (const char*, const char*);
};

class DocumentFactory_impl : virtual public POA_CosStream::StreamableFactory
{
public:
    DocumentFactory_impl(CORBA::ORB_ptr _orb);
    virtual CosStream::Streamable_ptr create_uninitialized();
};

class FigureFactory_impl : virtual public DocumentFactory_impl,
			   virtual public POA_Documents::FigureFactory
{
public:
    FigureFactory_impl(CORBA::ORB_ptr _orb);
    Documents::Figure_ptr create( const char* name );
    Documents::Figure_ptr create_with_file( const char* name, const char* file );
};


class TextFactory_impl : virtual public DocumentFactory_impl,
			 virtual public POA_Documents::TextFactory
{
public:
    TextFactory_impl(CORBA::ORB_ptr _orb);
    virtual Documents::Text_ptr create( const char* name );
    virtual Documents::Text_ptr create_with_file( const char* name, const char* file );
};

#endif
