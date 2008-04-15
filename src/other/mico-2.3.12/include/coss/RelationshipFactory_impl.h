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


#ifndef __RelationshipFactory_impl_h__
#define __RelationshipFactory_impl_h__

#include <string.h>
#include <coss/CosRelationships.h>
#include <coss/IdentifiableObject_impl.h>
//#define DEBUG 1



template <class T>
class RelationshipFactory_impl 
  : public virtual POA_CosRelationships::RelationshipFactory
{
  CORBA::UShort _degree;
  CosRelationships::RelationshipFactory::NamedRoleTypes* _named_role_types;
  //CORBA::Boolean first;
  CORBA::InterfaceDef* _relationship_type;
  //char* name_of_relship_server;
  CORBA::ORB_ptr orb;
  CORBA::Repository* interface_repository;
public:
  RelationshipFactory_impl (CORBA::ORB_ptr _orb);

  virtual CORBA::InterfaceDef_ptr relationship_type ();
  virtual CORBA::UShort degree ();
  virtual CosRelationships::RelationshipFactory::NamedRoleTypes* 
    named_role_types ();
  virtual CosRelationships::Relationship_ptr 
    create (const CosRelationships::NamedRoles& named_roles);
  virtual void destroy ();

  //extension only to _impl !!!
  void set_orb (CORBA::ORB_ptr);
  void set_degree (CORBA::UShort);
  void set_named_role_types (CosRelationships::RelationshipFactory::
			     NamedRoleTypes*);
};


template <class T>
RelationshipFactory_impl<T>::RelationshipFactory_impl (CORBA::ORB_ptr _orb)
{
#if DEBUG
  cout << "<RelationshipFactory_impl> RelationshipFactory constructor..."
       << flush;
#endif
  _degree = 0;
  _named_role_types 
    = new CosRelationships::RelationshipFactory::NamedRoleTypes;
  _named_role_types->length (0);
  _relationship_type = CORBA::InterfaceDef::_nil ();
  interface_repository = CORBA::Repository::_nil ();
  // this is for binding to random server
  IdentifiableObject_impl* tmp = new IdentifiableObject_impl (_orb);
#if DEBUG
  cout << "done.\n" << flush;
#endif
}


template <class T>
CORBA::InterfaceDef_ptr
RelationshipFactory_impl<T>::relationship_type ()
{
#if DEBUG
  cout << "  <RelationshipFactory_impl> relationship_type():\n" << flush;
#endif

  if (CORBA::is_nil (_relationship_type)) {
#if DEBUG
    cout << "  <RelationshipFactory_impl> first call...\n" << flush;
#endif
    CosRelationships::NamedRoles tmp_named_roles;
    //tmp_named_roles = new CosRelationships::NamedRoles;
    T* tmp_relationship
      = new T (tmp_named_roles);
    CosRelationships::Relationship_ptr relationship = tmp_relationship
      ->_this ();

    const char* repoid = relationship->_repoid ();
#if DEBUG
    cout << "  <RelationshipFactory_impl> repoid is " << repoid << "\n" << flush;
    cout << "  <RelationshipFactory_impl> Binding to Interface Repository..."
         << flush;
#endif
    if (CORBA::is_nil (interface_repository)) {
      CORBA::Object_var ir_obj
	= orb->resolve_initial_references ("InterfaceRepository");
      interface_repository = CORBA::Repository::_narrow (ir_obj);
    }
    assert (!CORBA::is_nil (interface_repository));

    _relationship_type = CORBA::InterfaceDef::_narrow 
      (interface_repository->lookup_id (CORBA::string_dup (repoid)));

    relationship->destroy ();
    CORBA::release (relationship);
  }
#if DEBUG
  cout << "  <RelationshipFactory_impl> destroy relationship\n" << flush;
  cout << "  <RelationshipFactory_impl> describe interface of relationship:\n" << flush;
  CORBA::InterfaceDef::FullInterfaceDescription_var full_of_relationship
      = _relationship_type ->describe_interface ();
  cout << "  <RelationshipFactory_impl> name: " << full_of_relationship->name 
       << "\n" << flush;
  cout << "  <RelationshipFactory_impl> id: " << full_of_relationship->id 
       << "\n" << flush;
  cout << "  <RelationshipFactory_impl> defined_in: " 
       << full_of_relationship->defined_in << "\n" << flush;
  cout << "  <RelationshipFactory_impl> version: "
       << full_of_relationship->version << "\n" << flush;
#endif

  return CORBA::InterfaceDef::_duplicate (_relationship_type);
}


template <class T>
CORBA::UShort
RelationshipFactory_impl<T>::degree ()
{
  return _degree;
}


template <class T>
CosRelationships::RelationshipFactory::NamedRoleTypes*
RelationshipFactory_impl<T>::named_role_types ()
{
  CosRelationships::RelationshipFactory::NamedRoleTypes* retval;
  retval = new CosRelationships::RelationshipFactory::NamedRoleTypes;
  retval->length (_named_role_types->length ());
  for (CORBA::ULong i = 0; i<_named_role_types->length (); i++) {
    (*retval)[i].name = CORBA::string_dup ((*_named_role_types)[i].name);
    (*retval)[i].named_role_type = CORBA::InterfaceDef::_duplicate 
      ((*_named_role_types)[i].named_role_type);
  }
  return retval;
}

template <class T>
CosRelationships::Relationship_ptr
RelationshipFactory_impl<T>::create 
(const CosRelationships::NamedRoles& named_roles)
{
#if DEBUG
  cout << "  <RelationshipFactory_impl> create (...);\n" << flush;
#endif

  if (_degree != named_roles.length ()) {
#if DEBUG
    cout << "  <RelationshipFactory_impl> Degree ERROR!!!\n" << flush;
#endif
    CosRelationships::RelationshipFactory::DegreeError ex;
    ex.required_degree = _degree;
    mico_user_throw_macro (ex);
  }

  CosRelationships::NamedRoles tmp_nrs;
  tmp_nrs.length (0);

#if DEBUG
  cout << "  <RelationshipFactory_impl> exist duplicate role name?\n" << flush;
#endif
  // exist duplicate role name?
  CORBA::Boolean is_duplicate = FALSE;
  //CosRelationships::RoleName tmp_role_name;
  for (CORBA::ULong i=0; i<named_roles.length (); i++) {
    for (CORBA::ULong j=0; j<named_roles.length (); j++) {
      if (named_roles[i].name == named_roles[j].name)
	if (i != j) {
#if DEBUG
	  cout << "  <RelationshipFactory_impl> duplicate!\n" << flush;
#endif
	  tmp_nrs.length (tmp_nrs.length () + 1);
	  tmp_nrs[tmp_nrs.length () - 1] = named_roles[i];
	  is_duplicate = TRUE;
	  break;
	}
    }
    //if (is_duplicate)
    //  break;
  }  
  if (is_duplicate) {
#if DEBUG
    cout << "  <RelationshipFactory_impl> is duplicate!!!\n" << flush;
#endif
    CosRelationships::RelationshipFactory::DuplicateRoleName ex;
#if DEBUG
    cout << "  <RelationshipFactory_impl> ex.culprits = tmp_nrs;\n" << flush;
#endif
    ex.culprits = tmp_nrs;
#if DEBUG
    cout << "  <RelationshipFactory_impl> DuplicateRoleName Exception!!!\n" << flush;
#endif
    mico_user_throw_macro (ex);
  }
#if DEBUG
  cout << "  <RelationshipFactory_impl> continue...\n" << flush;
#endif

  CORBA::Boolean is_in = FALSE;
  CORBA::Boolean UnknownRoleNameError = FALSE;
  
  tmp_nrs.length (0);
#if DEBUG
  cout << "  <RelationshipFactory_impl> comparing roles names.\n" << flush;
#endif
  //comparing roles names
  {
    for (CORBA::ULong i=0; i<named_roles.length (); i++) {
      for (CORBA::ULong j=0; j<_named_role_types->length (); j++) {
        if (named_roles[i].name == (*_named_role_types)[j].name) {
          is_in = TRUE;
          break;
        }
      }
      if (!is_in) {
        tmp_nrs.length (tmp_nrs.length () + 1);
        tmp_nrs[tmp_nrs.length () - 1] = named_roles[i];
        UnknownRoleNameError = TRUE;
      }
      is_in = FALSE;
    }
  }
  if (UnknownRoleNameError) {
#if DEBUG
    cout << "  <RelationshipFactory_impl> UnknownRoleName ERROR!!!\n" << flush;
#endif
    CosRelationships::RelationshipFactory::UnknownRoleName ex;
    ex.culprits = tmp_nrs;
    mico_user_throw_macro (ex);
  }


  CORBA::Boolean RoleTypeError = FALSE;
  tmp_nrs.length (0);
  
  //comparing roles types
#if DEBUG
  cout << "  <RelationshipFactory_impl> comparing roles types.\n" << flush;
  cout << "  <RelationshipFactory_impl> _named_role_types->length (): "
       << _named_role_types->length () << "\n" << flush;
  cout << "  <RelationshipFactory_impl> named_roles.length (): " 
       << named_roles.length () << "\n" << flush;
#endif
  {
    for (CORBA::ULong i=0; i<_named_role_types->length (); i++) {
      for (CORBA::ULong j=0; j<named_roles.length (); j++) {
#if DEBUG
        cout << "  <RelationshipFactory_impl> i, j = " << i << ", " << j << "\n" << flush;
#endif
        if (strcmp((*_named_role_types)[i].name, named_roles[j].name) == 0) {
#if DEBUG
	  cout << "  <RelationshipFactory_impl> the same names...\n" << flush;
          if (CORBA::is_nil ((*_named_role_types)[i].named_role_type))
	    cout << "  <RelationshipFactory_impl> named_role_type is NIL!\n" << flush;
          else
	    cout << "  <RelationshipFactory_impl> named_role_type is value\n" << flush;
#endif
#if DEBUG
          cout << "  <RelationshipFactory_impl> "
               << (*_named_role_types)[i].named_role_type->describe_interface () -> name  << "\n" << flush;
          cout << "  <RelationshipFactory_impl> "
               << named_roles[j].aRole->_repoid () << "\n" << flush;
#endif
          if (!(*_named_role_types)[i].named_role_type->is_a 
              //(CORBA::string_dup (named_roles[j].aRole->_repoid ()))) {
              (named_roles[j].aRole->_repoid ())) {
#if DEBUG
            cout << "  <RelationshipFactory_impl> RoleTypeError!\n" << flush;
#endif
            RoleTypeError = TRUE;
            tmp_nrs.length (tmp_nrs.length () + 1);
            tmp_nrs[tmp_nrs.length () - 1] = named_roles[j];
          }
          else {
#if DEBUG
            cout << "  <RelationshipFactory_impl> RoleType OK!\n" << flush;
#endif
          }
	  
        }
      }
    }
  }
  if (RoleTypeError) {
#if DEBUG
    cout << "  <RelationshipFactory_impl> RoleTypeError !!!\n" << flush;
#endif
    CosRelationships::RelationshipFactory::RoleTypeError ex;
    ex.culprits = tmp_nrs;
    mico_user_throw_macro (ex);
  }

#if DEBUG
    cout << "  <RelationshipFactory_impl> create_relationship ...\n" << flush;
    for (CORBA::ULong i=0; i<named_roles.length (); i++)
      cout << i << ". " << named_roles[i].aRole->_repoid () << " - " 
	   << named_roles[i].name << "\n" << flush;
#endif

  T* rel_impl 
    = new T (named_roles);
#if DEBUG
  cout << "  <RelationshipFactory_impl> create ..." <<flush;
#endif

  CosRelationships::Relationship_ptr rel = rel_impl->_this ();
#if DEBUG
  cout << "ok.\n" << flush;
#endif
 
#if DEBUG
  cout << "  <RelationshipFactory_impl> repoid: " << rel->_repoid ()
       << "\n" << flush;
  cout << "  <RelationshipFactory_impl> prepare exceptions.\n" <<flush;
#endif
 
  CosRelationships::RelationshipFactory::MaxCardinalityExceeded ex;
  ex.culprits.length (0);
#if DEBUG
  cout << "  <RelationshipFactory_impl> 1...\n" << flush;
#endif
 
  CosRelationships::Role::RelationshipTypeError ex2;
  int ex_index = 0;
  CosRelationships::RelationshipHandle rh;
#if DEBUG
  cout << "  <RelationshipFactory_impl> 2...\n" << flush;
#endif
#if DEBUG
  cout << "  <RelationshipFactory_impl> rel->constant_random_id () : "
       << rel->constant_random_id () << "\n" << flush;
#endif

  rh.constant_random_id = rel->constant_random_id ();
  rh.the_relationship = CosRelationships::Relationship::_duplicate (rel);

  CosRelationships::NamedRoles tmp_nr;
  tmp_nr.length (0);

#if DEBUG
  cout << "  <RelationshipFactory_impl> link role with this rel...\n" << flush;
#endif

  {
    for (CORBA::ULong i=0; i<named_roles.length (); i++) { 
#ifdef HAVE_EXCEPTIONS
      try {
#endif
#if DEBUG
        cout << "  <RelationshipFactory_impl> " << i << ". " 
             << named_roles[i].name << "\n" << flush;
        cout << orb->object_to_string (named_roles[i].aRole) << "\n" << flush;
#endif
        /*
        CORBA::Object_var obj = orb->string_to_object 
	(orb->object_to_string (named_roles[i].aRole));
        CosRelationships::Role_ptr role = CosRelationships::Role::_narrow (obj);
        role->link (rh, named_roles);
        */
        named_roles[i].aRole
          ->link (rh, named_roles);
#ifdef HAVE_EXCEPTIONS
      }
      catch (CosRelationships::RelationshipFactory::MaxCardinalityExceeded_catch &e)
      {
#if DEBUG
	cout << "  <RelationshipFactory_impl> cought maxcard exceeded "
	     << "exception\n" << flush;
#endif
#if DEBUG
	cout << "  <RelationshipFactory_impl> name: "
	     << e->culprits[0].name << "\n" << flush;
#endif
	ex.culprits.length (ex.culprits.length () + 1);
	ex.culprits[ex_index].aRole 
	  = CosRelationships::Role::_duplicate (e->culprits[0].aRole);
	ex.culprits[ex_index].name = strdup (e->culprits[0].name);
	tmp_nr.length (tmp_nr.length () + 1);
	tmp_nr[tmp_nr.length () - 1].aRole 
	  = CosRelationships::Role::_duplicate (named_roles[i].aRole.in());
	tmp_nr[tmp_nr.length () - 1].name = strdup (named_roles[i].name);
	ex_index++;
      }
      catch (CosRelationships::Role::RelationshipTypeError_catch &e)
      {
	if (ex_index > 0) {
	  for (CORBA::ULong j=0; j<named_roles.length (); j++) {
	    is_in = FALSE;
	    for (CORBA::ULong k=0; k<ex.culprits.length (); k++) 
	      if (ex.culprits[k].aRole->_is_equivalent (named_roles[j].aRole.in())){
		is_in = TRUE;
		break;
	      }
	    if (!is_in)
	      named_roles[j].aRole->unlink (rh);
	  }
	}
	rel->destroy ();
	mico_user_throw_macro (ex2);
      }
#endif
  	  
      //delete rh;
    }
  }
  if (ex_index > 0) {
#if DEBUG
  cout << "  <RelationshipFactory_impl> unlinking role\n" << flush;
#endif
    for (CORBA::ULong i=0; i<named_roles.length (); i++) {
      is_in = FALSE;
      for (CORBA::ULong j=0; j<ex.culprits.length (); j++) 
	if (ex.culprits[j].aRole->_is_equivalent (named_roles[i].aRole.in())) {
	  is_in = TRUE;
	  break;
	}
      if (!is_in)
	named_roles[i].aRole->unlink (rh);
    }
    rel->destroy (); 
    mico_user_throw_macro (ex);
  }
  if (CORBA::is_nil (_relationship_type)) { 
    _relationship_type = rel->_get_interface (); 
  }
#if DEBUG
  cout << "  <RelationshipFactory_impl> create was successfull!!!" <<flush;
#endif

  //return CosRelationships::Relationship::_duplicate (rel);
  return rel;
}


template <class T>
void
RelationshipFactory_impl<T>::destroy ()
{
#if DEBUG
   cout << "  <RelationshipFactory_impl> END!\n" << flush;
#endif 
  _default_POA ()->deactivate_object (*(_default_POA ()->servant_to_id (this)));

}


template <class T>
void 
RelationshipFactory_impl<T>::set_orb (CORBA::ORB_ptr x)
{
  orb = x;
}


template <class T>
void 
RelationshipFactory_impl<T>::set_degree (CORBA::UShort x)
{
  _degree = x;
}


template <class T>
void 
RelationshipFactory_impl<T>::set_named_role_types 
(CosRelationships::RelationshipFactory::NamedRoleTypes* x)
{
  _named_role_types = x;
}


#endif

