// -*- c++ -*-
/*
 *  Relationship Service for MICO
 *  Copyright (C) 1998-99 Karel Gardas 
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


#ifndef __RoleFactory_impl_h__
#define __RoleFactory_impl_h__

#include <coss/CosRelationships.h>
//#include <Role_impl.h>


//#define DEBUG 1
//#undef DEBUG


template<class T>
class RoleFactory_impl : virtual public POA_CosRelationships::RoleFactory
{
  CORBA::InterfaceDef_ptr _role_type;
  CosRelationships::InterfaceDefs* _related_object_types;
  CORBA::Repository* interface_repository;

  CORBA::Long max_card;
  CORBA::Long min_card;
  
  CORBA::ORB_var orb;
//  char* iterator_server;
  char* rel_ship_repoid;
//  char* name_of_role_server;
public:
  RoleFactory_impl ();
  virtual CORBA::InterfaceDef_ptr role_type ();
  virtual CORBA::ULong max_cardinality ();
  virtual void set_max_cardinality (CORBA::ULong);
  virtual CORBA::ULong min_cardinality ();
  virtual void set_min_cardinality (CORBA::ULong);
  virtual CosRelationships::InterfaceDefs* related_object_types ();
  virtual void set_related_object_types (CosRelationships::InterfaceDefs*);
  virtual CosRelationships::Role_ptr 
    create_role (CosRelationships::RelatedObject_ptr related_object);

  // extension
  virtual void set_orb (CORBA::ORB_var);
};



template <class T>
RoleFactory_impl<T>::RoleFactory_impl()
  : POA_CosRelationships::RoleFactory ()
{
#if DEBUG
  cout << "<RoleFactory_impl> RoleFactory constructor..." << flush;
#endif
  rel_ship_repoid = CORBA::string_dup ("");
  //InterfaceDefs_nedef = TRUE;
  /*
  _related_object_types = new CosRelationships::InterfaceDefs;
  _related_object_types->length(0);
  */
  _related_object_types = NULL;
  _role_type = CORBA::InterfaceDef::_nil ();
  interface_repository = CORBA::Repository::_nil ();
#if DEBUG
  cout << "done.\n" << flush;
  cout << flush;
#endif
}


template <class T>
CORBA::InterfaceDef_ptr
RoleFactory_impl<T>::role_type ()
{
#if DEBUG
  cout << "  <RoleFactory_impl> role_type():\n" << flush;
#endif

  if (CORBA::is_nil (_role_type)) {
    T* tmp_role 
      =  new T (CORBA::Object::_nil ());
    CosRelationships::Role_ptr role = tmp_role->_this (); 
    const char* repoid = role->_repoid (); 
#if DEBUG
    cout << "  <RoleFactory_impl> repoid is " << repoid << "\n" << flush;
    cout << "  <RoleFactory_impl> Binding to Interface Repository..."
	 << flush;
#endif
    if (CORBA::is_nil (interface_repository)) {
      CORBA::Object_var ir_obj
	= orb->resolve_initial_references ("InterfaceRepository");
      interface_repository = CORBA::Repository::_narrow (ir_obj);
      assert (!CORBA::is_nil (interface_repository));
    }
    _role_type = CORBA::InterfaceDef::_narrow 
      (interface_repository->lookup_id (CORBA::string_dup (repoid)));
#if DEBUG
    cout << "  <RoleFactory_impl> destroy role\n" << flush;
#endif
    //delete repoid;
    tmp_role->destroy ();
    CORBA::release (role);
    //delete tmp_role; ---- it'll crash server after second binding !!!! 
}
  
#if DEBUG
//   cout << "  <RoleFactory_impl> describe interface of role:\n" << flush;
//   cout << "  <RoleFactory_impl> interface is " << flush;
//   if (CORBA::is_nil (_role_type))
//     cout << "NULL!!!\n" << flush;
//   else
//     cout << "object.\n" << flush;
//    CORBA::InterfaceDef::FullInterfaceDescription_var full_of_role
//        = _role_type ->describe_interface ();
//    cout << "  <RoleFactory_impl> name: " << full_of_role->name << "\n" << flush;
//    cout << "  <RoleFactory_impl> id: " << full_of_role->id << "\n" << flush;
//    cout << "  <RoleFactory_impl> defined_in: " << full_of_role->defined_in
//         << "\n" << flush;
//    cout << "  <RoleFactory_impl> version: "<< full_of_role->version << "\n" << flush;
#endif

  return CORBA::InterfaceDef::_duplicate (_role_type);
}



template <class T>
CORBA::ULong
RoleFactory_impl<T>::max_cardinality ()
{
//  return 2000000000;   // max long - maybe :))
  return max_card;
}

template <class T>
void
RoleFactory_impl<T>::set_max_cardinality (CORBA::ULong x)
{
  max_card = x;
}

template <class T>
CORBA::ULong
RoleFactory_impl<T>::min_cardinality ()
{
  // return 0;
  return min_card;
}

template <class T>
void
RoleFactory_impl<T>::set_min_cardinality (CORBA::ULong x)
{
  min_card = x;
}

template <class T>
CosRelationships::InterfaceDefs*
RoleFactory_impl<T>::related_object_types()
{
  if (_related_object_types == NULL) {
    CosRelationships::InterfaceDefs*
      tmp = new CosRelationships::InterfaceDefs();
    tmp->length(0);
    return tmp;
  }
  return new CosRelationships::InterfaceDefs (*_related_object_types);
}


template <class T>
void
RoleFactory_impl<T>::set_related_object_types 
(CosRelationships::InterfaceDefs* x)
{
  if (_related_object_types == NULL)
    delete _related_object_types;
  _related_object_types = x;
}


template <class T>
CosRelationships::Role_ptr
RoleFactory_impl<T>::create_role 
(CosRelationships::RelatedObject_ptr related_object)
{

#if DEBUG
  cout << "  <RoleFactory_impl> create_role ()\n" << flush;
#endif

#if DEBUG
  if (CORBA::is_nil (related_object))
    cout << "  <RoleFactory_impl> related_object is NIL!\n" << flush;
#endif

  if (CORBA::is_nil (related_object)) {
    CosRelationships::RoleFactory::NilRelatedObject exception;
      mico_user_throw_macro (exception);
  }

  CORBA::Boolean is_in = FALSE;
  const char* repoid = related_object->_repoid ();
#if DEBUG
  cout << "  <RoleFactory_impl> related objects repoid: " << repoid << "\n" << flush;
#endif

  for (CORBA::ULong i=0; i<_related_object_types->length (); i++) {
#if DEBUG
    CORBA::InterfaceDef::FullInterfaceDescription_var full =
      (*_related_object_types)[i]->describe_interface ();
    cout << "  <RoleFactory_impl> " << i+1 << ". " << full->id << " == "
         << repoid << " : " << flush;
#endif
    if ((*_related_object_types)[i]->is_a (repoid)) {
      is_in = TRUE;
#if DEBUG
      cout << "YES!\n" << flush;
#endif
      break;
    }
#if DEBUG
    cout << "NO!\n" << flush;
#endif
  }

  if (!is_in) {
#if DEBUG
    cout << "RoleFactory::CosRelationships::RoleFactory::RelatedObjectTypeError!\n";
#endif
    CosRelationships::RoleFactory::RelatedObjectTypeError exception;
      mico_user_throw_macro (exception);
  }

#if DEBUG
  cout << "  <RoleFactory_impl> creating:\n" << flush;
#endif

  T* tmp_role = new T (CosRelationships::RelatedObject::_duplicate 
		       (related_object));
#if DEBUG
  cout << "  <RoleFactory_impl> setting created role\n" << flush;
#endif
  tmp_role->set_cardinality (0);
  tmp_role->set_min_cardinality (this->min_cardinality ());	 
  tmp_role->set_max_cardinality (this->max_cardinality ());	 
  tmp_role->set_relship_repoid (this->rel_ship_repoid);		 
  tmp_role->set_orb (orb);
  
  //T_tie* role = new T_tie (*tmp_role);
  CosRelationships::Role_ptr role = tmp_role->_this ();
#if DEBUG
  cout << "  <RoleFactory_impl> role->_repoid (): " << role->_repoid () 
       << "\n" << flush;
#endif
  
#if DEBUG
  cout << "  <RoleFactory_impl> create_role  done.\n" << flush;
  if (CORBA::is_nil (role->related_object ()))
    cout << "  <RoleFactory_impl> role->related_object () is NIL!\n" << flush;
#endif
  
  if (CORBA::is_nil (_role_type)) {
    const char* repoid = role->_repoid (); 
#if DEBUG
    cout << "  <RoleFactory_impl> repoid is " << repoid << "\n" << flush;
    cout << "  <RoleFactory_impl> Binding to Interface Repository..."
	 << flush;
#endif
    if (CORBA::is_nil (interface_repository)) {
      CORBA::Object_var ir_obj
	= orb->resolve_initial_references ("InterfaceRepository");
      interface_repository = CORBA::Repository::_narrow (ir_obj);
    }
    assert (!CORBA::is_nil (interface_repository));
    _role_type = CORBA::InterfaceDef::_narrow 
      (interface_repository->lookup_id (CORBA::string_dup (repoid)));

    //delete repoid;
  }
  
#if DEBUG
  cout << "  <RoleFactory_impl> create END! with return:\n" << flush;
#endif
  
  
  //return CosRelationships::Role::_duplicate (role);
  return role;
}


template <class T>
void
RoleFactory_impl<T>::set_orb (CORBA::ORB_var x)
{
  orb = x;
}


#endif
