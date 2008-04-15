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
      POA_CosLifeCycle::LifeCycleObject()
{
  doc_name = CORBA::string_dup ("");
  file_name = CORBA::string_dup ("");
}

AbstractDocument_impl::AbstractDocument_impl (const char* name)
    : POA_Documents::AbstractDocument(),
      POA_CosLifeCycle::LifeCycleObject()
{
  doc_name = CORBA::string_dup (name);
  file_name = CORBA::string_dup ("");
}

AbstractDocument_impl::AbstractDocument_impl (const char* value, 
					      const char* file)
    : POA_Documents::AbstractDocument(),
      POA_CosLifeCycle::LifeCycleObject()
{
  doc_name = CORBA::string_dup (value);
  file_name = CORBA::string_dup (file);
}

char*
AbstractDocument_impl::name ()
{
  return CORBA::string_dup (doc_name);
}

void
AbstractDocument_impl::name (const char* value)
{
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

Text_impl::Text_impl ()
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
}


Text_impl::Text_impl (const char* name)
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
  doc_name = CORBA::string_dup (name);
}


Text_impl::Text_impl (const char* name, const char* file)
  : AbstractDocument_impl (),
    POA_Documents::Text()
{
  doc_name = CORBA::string_dup (name);
  file_name = CORBA::string_dup (file);
}

CosLifeCycle::LifeCycleObject_ptr
Text_impl::copy( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
    CosLifeCycle::Key factory_id_;
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::Documents::Text");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("TextFactory");
    factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
    factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
    factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
    factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
    CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_id_);
    if (!(*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	throw *(new CosLifeCycle::NoFactory(factory_id_));
    }

    CORBA::ULong i;
    Documents::TextFactory_ptr textfactory_ptr;
    for (i = 0;i < (*object_factories_ptr).length();++i)
    {
	textfactory_ptr = Documents::TextFactory::_narrow((*object_factories_ptr)[i]);
	if (!CORBA::is_nil(textfactory_ptr))
	    break;
    }
    
    if (i == (*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	throw *(new CosLifeCycle::NoFactory(factory_id_));
    }
    delete object_factories_ptr;
    Documents::Text_ptr new_text;
    if (doc_name)
	if (file_name)
	    new_text = textfactory_ptr -> create_with_file(this->doc_name,this->file_name);
	else
	    new_text = textfactory_ptr -> create(this->doc_name);
    else
	    new_text = textfactory_ptr -> create("");
    if (CORBA::is_nil(new_text))
    {
	CORBA::release(textfactory_ptr);
	throw *(new CosLifeCycle::NotCopyable("Can`t create Text object because Documents::TextFactory::create... cann`t return Documents::Text object."));
    }
    return new_text;
}

void
Text_impl::move( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
}

void
Text_impl::remove()
{
    this->destroy();
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
}


Figure_impl::Figure_impl (const char* name)
  : AbstractDocument_impl (),
    POA_Documents::Figure()
{
  doc_name = CORBA::string_dup (name);
}


Figure_impl::Figure_impl (const char* name, const char* file)
  : AbstractDocument_impl (),
    POA_Documents::Figure()
{
  doc_name = CORBA::string_dup (name);
  file_name = CORBA::string_dup (file);
}

CosLifeCycle::LifeCycleObject_ptr
Figure_impl::copy( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
    CosLifeCycle::Key factory_id_;
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::Documents::Figure");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("FigureFactory");
    factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
    factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
    factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
    factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
    CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_id_);
    if (!(*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	throw *(new CosLifeCycle::NoFactory(factory_id_));
    }

    CORBA::ULong i;
    Documents::FigureFactory_ptr Figurefactory_ptr;
    for (i = 0;i < (*object_factories_ptr).length();++i)
    {
	Figurefactory_ptr = Documents::FigureFactory::_narrow((*object_factories_ptr)[i]);
	if (!CORBA::is_nil(Figurefactory_ptr))
	    break;
    }
    
    if (i == (*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	throw *(new CosLifeCycle::NoFactory(factory_id_));
    }
    delete object_factories_ptr;
    Documents::Figure_ptr new_Figure;
    if (doc_name)
	if (file_name)
	    new_Figure = Figurefactory_ptr -> create_with_file(this->doc_name,this->file_name);
	else
	    new_Figure = Figurefactory_ptr -> create(this->doc_name);
    else
	    new_Figure = Figurefactory_ptr -> create("");
    if (CORBA::is_nil(new_Figure))
    {
	CORBA::release(Figurefactory_ptr);
	throw *(new CosLifeCycle::NotCopyable("Can`t create Figure object because Documents::FigureFactory::create... cann`t return Documents::Figure object."));
    }
    return new_Figure;
}

void
Figure_impl::move( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
}

void
Figure_impl::remove()
{
    this->destroy();
}

