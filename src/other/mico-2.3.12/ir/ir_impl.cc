/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "ir_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <mico/ir_creator.h>
#include <ctype.h>
#include "ir_impl.h"

#endif // FAST_PCH

#ifndef HAVE_DYNAMIC_CAST
#error "oops"
#endif


using namespace std;

/*
 * Helper: case insensitive string compare
 */

static int
strnocasecmp (const char * s1, const char * s2)
{
  int i;

  while (*s1 && *s2) {
    i = (int) tolower (*s1) - (int) tolower (*s2);
    if (i != 0) {
      return i;
    }
    s1++;
    s2++;
  }

  if (*s1 && !*s2) {
    return 1;
  }
  else if (!*s1 && *s2) {
    return -1;
  }

  return 0;
}

//-- IRObject -----------------------------------------------------------

PortableServer::POA_var IRObject_impl::_ifrpoa;

IRObject_impl::IRObject_impl(CORBA::DefinitionKind kind)
{
  _dk = kind;
  _already_destructed = 0;
}

CORBA::DefinitionKind
IRObject_impl::def_kind()
{
  return _dk;
}

void
IRObject_impl::destroy ()
{
  if (!_already_destructed) {
    _already_destructed = 1;
    deactivate ();
    PortableServer::POA_var poa = _default_POA();
    PortableServer::ObjectId_var oid = poa->servant_to_id (this);
    poa->deactivate_object (oid.in());
  }
}

PortableServer::POA_ptr
IRObject_impl::_default_POA ()
{
  return PortableServer::POA::_duplicate (_ifrpoa);
}

//-- Contained ----------------------------------------------------------

Contained_impl::Contained_impl (Container_impl * mycontainer,
				Repository_impl * myrepository,
				const char * id, const char * name,
				const char * version)
  : IRObject_impl (CORBA::dk_none)
{
  _id = id;
  _name = name;
  _version = version;
  _mycontainer = mycontainer;
  _myrepository = myrepository;
}

CORBA::RepositoryId
Contained_impl::id()
{
  return CORBA::string_dup( _id );
}

void
Contained_impl::id (const char* _value)
{
  if (strnocasecmp (_id.in(), _value) == 0) {
    return;
  }

  /*
   * We register the new value before unregistering the old. Therefore,
   * if registering throws an exception, we stay registered under the
   * old id.
   */

  if (_value[0]) {
    _myrepository->register_repoid (_value, this);
  }

  if (_id.in()[0]) {
    _myrepository->unregister_repoid (_id.in());
  }

  _id = _value;
}

char*
Contained_impl::name()
{
  return CORBA::string_dup( _name );
}

void
Contained_impl::name (const char* _value)
{
  if (strnocasecmp (_name.in(), _value) == 0) {
    return;
  }

  /*
   * We register the new value before unregistering the old. Therefore,
   * if registering throws an exception, we stay registered under the
   * old id.
   */

  if (_value[0]) {
    _mycontainer->register_name (_value, this);
  }

  if (_name.in()[0]) {
    _myrepository->unregister_name (_name.in());
  }

  _name = _value;
}

char*
Contained_impl::version()
{
  return CORBA::string_dup( _version );
}

void
Contained_impl::version( const char* _value )
{
  _version = _value;
}

CORBA::Container_ptr
Contained_impl::defined_in()
{
  return _mycontainer->_this();
}

char*
Contained_impl::absolute_name()
{
  string s;

  CORBA::Container_var def_in = _mycontainer->_this();
  CORBA::Contained_var con = CORBA::Contained::_narrow (def_in);

  if (CORBA::is_nil (con)) {
    s  = "::";
    s += _name.in();
  }
  else {
    CORBA::String_var conname = con->absolute_name ();
    s  = conname.in();
    s += "::";
    s += _name.in();
  }

  return CORBA::string_dup( s.c_str() );
}

CORBA::Repository_ptr
Contained_impl::containing_repository()
{
  return _myrepository->_this();
}

void
Contained_impl::deactivate ()
{
  _mycontainer->unregister_name (_name.in());
  _myrepository->unregister_repoid (_id.in());
}

void
Contained_impl::move (CORBA::Container_ptr new_container,
		      const char* new_name,
		      const char* new_version)
{
  PortableServer::ServantBase_var serv;

  try {
    serv = IRObject_impl::_ifrpoa->reference_to_servant (new_container);
  }
  catch (PortableServer::POA::WrongAdapter &) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  Container_impl * nc = dynamic_cast<Container_impl *> (serv.in());
  assert (nc);

  if (nc == _mycontainer) {
    return;
  }

  /*
   * XXX Todo: check that the target container supports me
   */

  nc->register_name (new_name, this);
  _mycontainer->unregister_name (_name.in());
  _mycontainer = nc;

  _name = new_name;
  version (new_version);
}


//-- Container ----------------------------------------------------------

Container_impl::Container_impl()
  : IRObject_impl (CORBA::dk_none)
{
  /*
   * I cannot initialize the repository yet. If we are the repository
   * ourselves, a dynamic_cast<Repository_impl*> would fail, because
   * its constructor hasn't run yet.
   *
   * Repository_impl will set _myrepository
   */

  assert (_dk == CORBA::dk_Repository);
  _mycontainer = 0;
  _myrepository = 0;
}

Container_impl::Container_impl (Container_impl * mycontainer,
				Repository_impl * myrepository)
  : IRObject_impl (CORBA::dk_none)
{
  _mycontainer = mycontainer;
  _myrepository = myrepository;
}

CORBA::Contained_ptr
Container_impl::lookup (const char* search_name)
{
  string scoped_name;

  /*
   * Absolute name? Ask repository
   */

  if (search_name[0] == ':' && search_name[1] == ':') {
    if (_dk == CORBA::dk_Repository) {
      /*
       * hey, that's me!
       */
      scoped_name = &search_name[2];
    }
    else {
      return _myrepository->lookup (search_name);
    }
  }
  else {
    scoped_name = search_name;
  }

  /*
   * A qualified name (one of the form <scoped-name>::<identifier>
   * is resolved by first resolving the qualifier <scoped-name> to
   * a scope S, and then locating the definition of <identifier>
   * within S. The identifier must be directly defined in S or (if
   * S is an interface) inherited into S. The <identifier> is not
   * searched for in enclosing scopes.
   */

  CORBA::ULong pos;

  if ((pos = scoped_name.rfind ("::")) != (CORBA::ULong) -1) {
    string identifier;
    identifier  = scoped_name.substr (pos+2);
    scoped_name = scoped_name.substr (0, pos);

    /*
     * resolve the qualifier into a scope by recursion
     */

    CORBA::Contained_var c1 = lookup (scoped_name.c_str());

    /*
     * Must be a container
     */

    CORBA::Container_var c2 = CORBA::Container::_narrow (c1);

    if (CORBA::is_nil (c2)) {
      return CORBA::Contained::_nil ();
    }

    /*
     * The identifier must be directly contained in this scope or
     * inherited into it. This is exactly what lookup_name does.
     *
     * However, types might be redefined in a derived interface,
     * resulting in multiple (non-ambiguous) matches if lookup_name
     * is asked to search inherited names.
     *
     * Therefore, we first search the container itself (excluding
     * inherited identifiers). If the identifier is not found
     * there, we extend the search to inherited identifiers.
     */

    CORBA::ContainedSeq_var cs = c2->lookup_name (identifier.c_str(), 1,
						  CORBA::dk_all, 1);

    if (cs->length() == 0) {
      cs = c2->lookup_name (identifier.c_str(), 1, CORBA::dk_all, 0);
    }

    /*
     * If cs->length() == 1, then we have found a unique name.
     * If cs->length() == 0, we know that the name does not exist.
     * If cs->length() >  1, then the name is ambiguous.
     *
     * Ambiguity arising through multiple inheritance is fine.
     * Therefore, we don't complain if all elements of cs are equivalent.
     */

    if (cs->length() == 0) {
      return CORBA::Contained::_nil ();
    }

    for (CORBA::ULong i=1; i<cs->length(); i++) {
      if (!cs[(CORBA::ULong)0]->_is_equivalent (cs[i])) {
	/*
	 * Ambiguity detected
	 */
	mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_AMBIGUOUS_NAME,
				       CORBA::COMPLETED_NO ) );
      }
    }

    return CORBA::Contained::_duplicate (cs[(CORBA::ULong)0]);
  }

  /*
   * We are dealing with a simple identifier. It will be resolved by
   * successively searching farther out in enclosing scopes, while
   * taking into consideration inheritance relationships among inter-
   * faces.
   */

  Container_impl * search_container = this;
  NameMap::iterator it;

  while (search_container) {
    /*
     * Search current container
     */

    for (it = search_container->_names.begin();
	 it != search_container->_names.end(); it++) {
      if (strnocasecmp ((*it).name.c_str(), scoped_name.c_str()) == 0) {
	break;
      }
    }

    /*
     * If found, use it. In case of an enum value, (*it).value is nil
     */

    if (it != search_container->_names.end()) {
      if (!(*it).value) {
	return CORBA::Contained::_nil ();
      }
      return (*it).value->_this ();
    }

    /*
     * The identifier is not defined in this scope. Search inherited
     * scopes with the help of lookup_name.
     */

    CORBA::ContainedSeq_var cs =
      search_container->lookup_name (scoped_name.c_str(), 1,
				     CORBA::dk_all, 0);

    /*
     * See above
     */

    if (cs->length() > 0) {
      for (CORBA::ULong i=1; i<cs->length(); i++) {
	if (!cs[(CORBA::ULong)0]->_is_equivalent (cs[i])) {
	  /*
	   * Ambiguity detected
	   */
	  mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_AMBIGUOUS_NAME,
					 CORBA::COMPLETED_NO ) );
	}
      }
      return CORBA::Contained::_duplicate (cs[(CORBA::ULong)0]);
    }  

    /*
     * Not found. Go up to parent container.
     */

    search_container = search_container->_mycontainer;
  }

  return CORBA::Contained::_nil ();
}

/*
 * Helper: merge CORBA::ContainedSeq lists
 */

void
Container_impl::merge (CORBA::ContainedSeq & l1,
		       const CORBA::ContainedSeq & l2)
{
  CORBA::ULong j=l1.length();
  l1.length (j + l2.length());
  for (CORBA::ULong i=0; i<l2.length(); i++, j++) {
    l1[j] = l2[i];
  }
}

void
Container_impl::merge_contents (CORBA::ContainedSeq & l1,
				CORBA::Container_ptr cont,
				CORBA::DefinitionKind limit_type,
				CORBA::Boolean exclude_inherited)
{
  if (!CORBA::is_nil (cont)) {
    CORBA::ContainedSeq_var inherited =
      cont->contents (limit_type, exclude_inherited);
    merge (l1, inherited.in());
  }
}

void
Container_impl::merge_lookup (CORBA::ContainedSeq & l1,
			      CORBA::Container_ptr cont,
			      const char * search_name,
			      CORBA::Long levels_to_search,
			      CORBA::DefinitionKind limit_type,
			      CORBA::Boolean exclude_inherited)
{
  if (!CORBA::is_nil (cont)) {
    CORBA::ContainedSeq_var inherited =
      cont->lookup_name (search_name, levels_to_search,
			 limit_type, exclude_inherited);
    merge (l1, inherited.in());
  }
}

CORBA::ContainedSeq*
Container_impl::contents( CORBA::DefinitionKind limit_type,
			  CORBA::Boolean exclude_inherited )
{
  CORBA::ContainedSeq *s = new CORBA::ContainedSeq;

  CORBA::ULong j = 0;
  NameMap::iterator it;

  for (it = _names.begin(); it != _names.end(); it++) {
    if (!(*it).value) {
      continue;
    }
    if (limit_type == CORBA::dk_all ||
	(*it).value->def_kind() == limit_type) {
      s->length( j + 1 );
      (*s)[ j++ ] = (*it).value->_this ();
    }
  }

  if( !exclude_inherited &&
      (_dk == CORBA::dk_Interface ||
       _dk == CORBA::dk_AbstractInterface ||
       _dk == CORBA::dk_LocalInterface)) {
    CORBA::Container_var me = _this ();
    CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( me );
    assert( !CORBA::is_nil( in ) );
    CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      merge_contents (*s, bases[i], limit_type, exclude_inherited);
    }
  }

  if( !exclude_inherited && _dk == CORBA::dk_Value ) {
    CORBA::Container_var me = _this ();
    CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( me );
    assert( !CORBA::is_nil( val ) );
    CORBA::ValueDefSeq_var bases = val->abstract_base_values();
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      merge_contents (*s, bases[i], limit_type, exclude_inherited);
    }
    CORBA::InterfaceDefSeq_var supported = val->supported_interfaces();
    for (CORBA::ULong i1 = 0; i1 < supported->length(); i1++) {
      merge_contents (*s, supported[i1], limit_type, exclude_inherited);
    }
    CORBA::ValueDef_var base = val->base_value();
    merge_contents (*s, base, limit_type, exclude_inherited);
  }

#ifdef USE_CCM
  if (!exclude_inherited && _dk == CORBA::dk_Component) {
    CORBA::Container_var me = _this ();
    CORBA::ComponentIR::ComponentDef_var in =
      CORBA::ComponentIR::ComponentDef::_narrow (me);
    CORBA::ComponentIR::ComponentDef_var base = in->base_component ();
    merge_contents (*s, base, limit_type, exclude_inherited);

    CORBA::InterfaceDefSeq_var supported = in->supported_interfaces ();
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      merge_contents (*s, supported[i], limit_type, exclude_inherited);
    }
  }

  if (!exclude_inherited && _dk == CORBA::dk_Home) {
    CORBA::Container_var me = _this ();
    CORBA::ComponentIR::HomeDef_var in =
      CORBA::ComponentIR::HomeDef::_narrow (me);
    CORBA::ComponentIR::HomeDef_var base = in->base_home ();
    merge_contents (*s, base, limit_type, exclude_inherited);
  }
#endif
  
  return s;
}

CORBA::ContainedSeq*
Container_impl::lookup_name( const char* search_name,
			     CORBA::Long levels_to_search,
			     CORBA::DefinitionKind limit_type,
			     CORBA::Boolean exclude_inherited )
{
  CORBA::ContainedSeq *s = new CORBA::ContainedSeq;

  if( levels_to_search == 0 )
    return s;
  
  CORBA::ULong j = 0;
  NameMap::iterator it;

  for (it = _names.begin (); it != _names.end(); it++) {
    if (strnocasecmp ((*it).name.c_str(), search_name) == 0) {
      break;
    }
  }

  /*
   * Search here
   */

  if (it != _names.end()) {
    if ((*it).value &&
	(limit_type == CORBA::dk_all ||
	 (*it).value->def_kind() == limit_type)) {
      s->length (j+1);
      (*s)[j++] = (*it).value->_this ();
    }
  }

  /*
   * Search contained containers
   */

  if (levels_to_search != 0) {
    for (it = _names.begin(); it != _names.end(); it++) {
      if (!(*it).value) {
	continue;
      }

      CORBA::Contained_var c1 = (*it).value->_this ();
      CORBA::Container_var c2 = CORBA::Container::_narrow (c1);

      merge_lookup (*s, c2,
		    search_name,
		    (levels_to_search == -1) ? -1 : levels_to_search-1,
		    limit_type,
		    exclude_inherited);
    }
  }

  /*
   * Search inherited stuff
   */

  if( !exclude_inherited &&
      (_dk == CORBA::dk_Interface ||
       _dk == CORBA::dk_AbstractInterface ||
       _dk == CORBA::dk_LocalInterface)) {
    CORBA::Container_var me = _this ();
    CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( me );
    assert( !CORBA::is_nil( in ) );
    CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      merge_lookup (*s, bases[i],
		    search_name, levels_to_search,
		    limit_type, exclude_inherited);
    }
  }

  if( !exclude_inherited && _dk == CORBA::dk_Value ) {
    CORBA::Container_var me = _this ();
    CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( me );
    assert( !CORBA::is_nil( val ) );
    CORBA::ValueDefSeq_var bases = val->abstract_base_values();
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      merge_lookup (*s, bases[i],
		    search_name, levels_to_search,
		    limit_type, exclude_inherited);
    }
    CORBA::InterfaceDefSeq_var supported = val->supported_interfaces();
    for( CORBA::ULong i1 = 0; i1 < supported->length(); i1++) {
      merge_lookup (*s, supported[i1],
		    search_name, levels_to_search,
		    limit_type, exclude_inherited);
    }
    CORBA::ValueDef_var base = val->base_value();
    merge_lookup (*s, base,
		  search_name, levels_to_search,
		  limit_type, exclude_inherited);
  }

#ifdef USE_CCM
  if (!exclude_inherited && _dk == CORBA::dk_Component) {
    CORBA::Container_var me = _this ();
    CORBA::ComponentIR::ComponentDef_var in =
      CORBA::ComponentIR::ComponentDef::_narrow (me);
    CORBA::ComponentIR::ComponentDef_var base = in->base_component ();
    merge_lookup (*s, base,
		  search_name, levels_to_search,
		  limit_type, exclude_inherited);

    CORBA::InterfaceDefSeq_var supported = in->supported_interfaces ();
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      merge_lookup (*s, supported[i],
		    search_name, levels_to_search,
		    limit_type, exclude_inherited);
    }
  }

  if (!exclude_inherited && _dk == CORBA::dk_Home) {
    CORBA::Container_var me = _this ();
    CORBA::ComponentIR::HomeDef_var in =
      CORBA::ComponentIR::HomeDef::_narrow (me);
    CORBA::ComponentIR::HomeDef_var base = in->base_home ();
    merge_lookup (*s, base,
		  search_name, levels_to_search,
		  limit_type, exclude_inherited);

    CORBA::InterfaceDefSeq_var supported = in->supported_interfaces ();
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      merge_lookup (*s, supported[i],
		    search_name, levels_to_search,
		    limit_type, exclude_inherited);
    }
  }
#endif

  return s;
}

CORBA::Container::DescriptionSeq*
Container_impl::describe_contents( CORBA::DefinitionKind limit_type,
				   CORBA::Boolean exclude_inherited,
				   CORBA::Long max_returned_objs )
{
  CORBA::ContainedSeq_var cont = contents(limit_type, exclude_inherited);
  
  CORBA::Long limit = cont->length();
  if( limit > max_returned_objs && max_returned_objs != -1 )
    limit = max_returned_objs;
  
  CORBA::Container::DescriptionSeq* s = new CORBA::Container::DescriptionSeq;
  s->length( limit );
  
  for( CORBA::ULong i = 0; (CORBA::Long)i < limit; i++ ) {
    CORBA::Contained::Description_var cont_desc = cont[ i ]->describe();
    (*s)[i].contained_object = cont[i];
    (*s)[i].kind = cont_desc->kind;
    (*s)[i].value = cont_desc->value;
  }
  return s;
}

CORBA::ModuleDef_ptr
Container_impl::create_module( const char* id,
			       const char* name,
			       const char* version )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create a module in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  ModuleDef_impl * mi = new ModuleDef_impl( this, _myrepository,
					    id, name, version );
  try {
    insert_contained (mi, id, name);
  }
  catch (...) {
    delete mi;
    throw;
  }

  CORBA::ModuleDef_ptr m = mi->_this ();
  mi->_remove_ref ();
  return m;
}

CORBA::ConstantDef_ptr
Container_impl::create_constant( const char* id,
				 const char* name,
				 const char* version,
				 CORBA::IDLType_ptr type,
				 const CORBA::Any& value )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module     &&
      _dk != CORBA::dk_Value      &&
#ifdef USE_CCM
      _dk != CORBA::dk_Component  &&
      _dk != CORBA::dk_Home       &&
#endif
      _dk != CORBA::dk_Interface  &&
      _dk != CORBA::dk_AbstractInterface &&
      _dk != CORBA::dk_LocalInterface ) {
    // Can only create a constant in the repository, in a module
    // or in an interface
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  ConstantDef_impl * ci = new ConstantDef_impl( this, _myrepository,
						id, name, version );

  try {
    ci->type_def (type);
    ci->value (value);
    insert_contained (ci, id, name);
  }
  catch (...) {
    delete ci;
    throw;
  }

  CORBA::ConstantDef_ptr c = ci->_this ();
  ci->_remove_ref ();
  return c;
}

CORBA::StructDef_ptr
Container_impl::create_struct( const char* id,
			       const char* name,
			       const char* version,
			       const CORBA::StructMemberSeq& members )
{
  StructDef_impl * si = new StructDef_impl( this, _myrepository,
					    id, name, version );

  try {
    si->members (members);
    insert_contained (si, id, name);
  }
  catch (...) {
    delete si;
    throw;
  }

  CORBA::StructDef_ptr s = si->_this ();
  si->_remove_ref ();
  return s;
}

CORBA::ExceptionDef_ptr
Container_impl::create_exception( const char* id,
				  const char* name,
				  const char* version,
				  const CORBA::StructMemberSeq& members )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module     &&
      _dk != CORBA::dk_Value      &&
#ifdef USE_CCM
      _dk != CORBA::dk_Component  &&
      _dk != CORBA::dk_Home       &&
#endif
      _dk != CORBA::dk_Interface  &&
      _dk != CORBA::dk_AbstractInterface &&
      _dk != CORBA::dk_LocalInterface ) {
    // Can only create an exception in the repository, in a module
    // or in an interface
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  ExceptionDef_impl * ei = new ExceptionDef_impl( this, _myrepository,
						  id, name, version );

  try {
    ei->members ( members );
    insert_contained (ei, id, name);
  }
  catch (...) {
    delete ei;
    throw;
  }

  CORBA::ExceptionDef_ptr e = ei->_this ();
  ei->_remove_ref ();
  return e;
}

CORBA::UnionDef_ptr
Container_impl::create_union( const char* id,
			      const char* name,
			      const char* version,
			      CORBA::IDLType_ptr discr,
			      const CORBA::UnionMemberSeq& members )
{
  UnionDef_impl * ui = new UnionDef_impl ( this, _myrepository,
					   id, name, version );

  try {
    ui->discriminator_type_def (discr);
    ui->members (members);
    insert_contained (ui, id, name);
  }
  catch (...) {
    delete ui;
    throw;
  }

  CORBA::UnionDef_ptr u = ui->_this ();
  ui->_remove_ref ();
  return u;
}

CORBA::EnumDef_ptr
Container_impl::create_enum( const char* id,
			     const char* name,
			     const char* version,
			     const CORBA::EnumMemberSeq& members )
{
  EnumDef_impl * ei = new EnumDef_impl( this, _myrepository,
					id, name, version );

  try {
    ei->members( members );
    insert_contained (ei, id, name);
  }
  catch (...) {
    delete ei;
    throw;
  }

  CORBA::EnumDef_ptr e = ei->_this ();
  return e;
}

CORBA::AliasDef_ptr
Container_impl::create_alias( const char* id,
			      const char* name,
			      const char* version,
			      CORBA::IDLType_ptr original_type )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module     &&
      _dk != CORBA::dk_Value      &&
#ifdef USE_CCM
      _dk != CORBA::dk_Component  &&
      _dk != CORBA::dk_Home       &&
#endif
      _dk != CORBA::dk_Interface  &&
      _dk != CORBA::dk_AbstractInterface &&
      _dk != CORBA::dk_LocalInterface ) {
    // Can only create an alias in the repository, in a module
    // or in an interface
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  AliasDef_impl * ai = new AliasDef_impl( this, _myrepository,
					  id, name, version );

  try {
    ai->original_type_def( original_type );
    insert_contained (ai, id, name);
  }
  catch (...) {
    delete ai;
    throw;
  }

  CORBA::AliasDef_ptr a = ai->_this ();
  return a;
}

CORBA::InterfaceDef_ptr
Container_impl::create_interface( const char* id,
				  const char* name,
				  const char* version,
				  const CORBA::InterfaceDefSeq& base_interfaces)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create an interface in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }
  
  InterfaceDef_impl * ii = new InterfaceDef_impl( this, _myrepository,
						  id, name, version );

  try {
    ii->base_interfaces (base_interfaces);
    insert_contained (ii, id, name);
  }
  catch (...) {
    delete ii;
    throw;
  }

  CORBA::InterfaceDef_ptr i = ii->_this ();
  ii->_remove_ref ();
  return i;
}

CORBA::AbstractInterfaceDef_ptr
Container_impl::create_abstract_interface( const char* id,
					   const char* name,
					   const char* version,
					   const CORBA::AbstractInterfaceDefSeq& bases)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create an interface in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }
  
  AbstractInterfaceDef_impl * ii =
    new AbstractInterfaceDef_impl( this, _myrepository,
				   id, name, version );

  CORBA::InterfaceDefSeq b;
  b.length (bases.length());
  for (CORBA::ULong bi=0; bi<bases.length(); bi++) {
    b[bi] = CORBA::InterfaceDef::_narrow (bases[bi].in());
  }

  try {
    ii->base_interfaces (b);
    insert_contained (ii, id, name);
  }
  catch (...) {
    delete ii;
    throw;
  }

  CORBA::AbstractInterfaceDef_ptr i = ii->_this ();
  ii->_remove_ref ();
  return i;
}

CORBA::LocalInterfaceDef_ptr
Container_impl::create_local_interface( const char* id,
					const char* name,
					const char* version,
					const CORBA::InterfaceDefSeq& bases)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create an interface in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }
  
  LocalInterfaceDef_impl * ii =
    new LocalInterfaceDef_impl( this, _myrepository,
				id, name, version );

  try {
    ii->base_interfaces (bases);
    insert_contained (ii, id, name);
  }
  catch (...) {
    delete ii;
    throw;
  }

  CORBA::LocalInterfaceDef_ptr i = ii->_this ();
  ii->_remove_ref ();
  return i;
}

CORBA::ValueDef_ptr
Container_impl::create_value( const char* id,
			      const char* name,
			      const char* version,
			      CORBA::Boolean is_custom,
			      CORBA::Boolean is_abstract,
			      CORBA::ValueDef_ptr base_value,
			      CORBA::Boolean is_truncatable,
			      const CORBA::ValueDefSeq& abstract_base_values,
			      const CORBA::InterfaceDefSeq& supported_interfaces,
			      const CORBA::InitializerSeq& initializers )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create a value in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }
  
  ValueDef_impl * val = new ValueDef_impl( this, _myrepository,
					   id, name, version );

  try {
    val->is_custom (is_custom);
    val->is_abstract (is_abstract);
    val->base_value (base_value);
    val->is_truncatable (is_truncatable);
    val->abstract_base_values (abstract_base_values);
    val->supported_interfaces (supported_interfaces);
    val->initializers (initializers);
    insert_contained (val, id, name);
  }
  catch (...) {
    delete val;
    throw;
  }

  CORBA::ValueDef_ptr v = val->_this ();
  val->_remove_ref ();
  return v;
}

CORBA::ExtValueDef_ptr
Container_impl::create_ext_value( const char* id,
				  const char* name,
				  const char* version,
				  CORBA::Boolean is_custom,
				  CORBA::Boolean is_abstract,
				  CORBA::ValueDef_ptr base_value,
				  CORBA::Boolean is_truncatable,
				  const CORBA::ValueDefSeq& abstract_base_values,
				  const CORBA::InterfaceDefSeq& supported_interfaces,
				  const CORBA::ExtInitializerSeq& initializers )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create a value in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }
  
  ValueDef_impl * val = new ValueDef_impl( this, _myrepository,
					   id, name, version );

  try {
    val->is_custom (is_custom);
    val->is_abstract (is_abstract);
    val->base_value (base_value);
    val->is_truncatable (is_truncatable);
    val->abstract_base_values (abstract_base_values);
    val->supported_interfaces (supported_interfaces);
    val->ext_initializers (initializers);
    insert_contained (val, id, name);
  }
  catch (...) {
    delete val;
    throw;
  }

  CORBA::ExtValueDef_ptr v = val->_this ();
  val->_remove_ref ();
  return v;
}

CORBA::ValueBoxDef_ptr
Container_impl::create_value_box( const char* id,
				  const char* name,
				  const char* version,
				  CORBA::IDLType_ptr original_type_def )
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    // Can only create a value box in the repository or in a module
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  ValueBoxDef_impl * val = new ValueBoxDef_impl( this, _myrepository,
						 id, name, version);

  try {
    val->original_type_def ( original_type_def );
    insert_contained (val, id, name);
  }
  catch (...) {
    delete val;
    throw;
  }

  CORBA::ValueBoxDef_ptr v = val->_this ();
  val->_remove_ref ();
  return v;
}

CORBA::NativeDef_ptr
Container_impl::create_native( const char* id,
			       const char* name,
			       const char* version )
{
  NativeDef_impl * ni = new NativeDef_impl( this, _myrepository,
					    id, name, version );

  try {
    insert_contained (ni, id, name);
  }
  catch (...) {
    delete ni;
    throw;
  }

  CORBA::NativeDef_ptr n = ni->_this ();
  ni->_remove_ref ();
  return n;
}

/*
 * local interface
 */

void
Container_impl::register_name (const char * name, Contained_impl * impl)
{
  /*
   * The name of an interface, value type, struct, union, exception or
   * a module may not be redefined within the immediate scope.
   */

  if (_dk == CORBA::dk_Interface ||
      _dk == CORBA::dk_AbstractInterface ||
      _dk == CORBA::dk_LocalInterface ||
#ifdef USE_CCM
      _dk == CORBA::dk_Component ||
      _dk == CORBA::dk_Home ||
#endif
      _dk == CORBA::dk_Struct ||
      _dk == CORBA::dk_Union ||
      _dk == CORBA::dk_Exception ||
      _dk == CORBA::dk_Module) {
    Contained_impl * me = dynamic_cast<Contained_impl *> (this);
    assert (me);

    if (strnocasecmp (me->_name.in(), name) == 0) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO));
    }
  }

  /*
   * Insert name
   */

  NameMap::iterator it;

  for (it = _names.begin(); it != _names.end(); it++) {
    if (strnocasecmp ((*it).name.c_str(), name) == 0) {
      break;
    }
  }

  if (it != _names.end()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO));
  }

  _name_entry ne;
  ne.name = name;
  ne.value = impl;
  _names.push_back (ne);
}

void
Container_impl::unregister_name (const char * name)
{
  NameMap::iterator it;

  for (it = _names.begin(); it != _names.end(); it++) {
    if (strnocasecmp ((*it).name.c_str(), name) == 0) {
      break;
    }
  }

  if (it != _names.end()) {
    _names.erase (it);
  }
}

void
Container_impl::insert_contained (Contained_impl * impl,
				  const char * id,
				  const char * name)
{
  if (name[0]) {
    try {
      register_name (name, impl);
    }
    catch (CORBA::BAD_PARAM &) {
      throw;
    }
  }

  if (id[0]) {
    try {
      _myrepository->register_repoid (id, impl);
    }
    catch (CORBA::BAD_PARAM &) {
      if (name[0]) {
	unregister_name (name);
      }
      throw;
    }
  }
}

void
Container_impl::deactivate ()
{
  /*
   * Upon deactivation, destroy all contents
   */

  NameMap::iterator it = _names.begin();

  while (it != _names.end()) {
    if (!(*it).value) {
      _names.erase (it);
    }
    else {
      (*it).value->destroy ();
    }
    // iterator invalidated
    it = _names.begin();
  }
}

#ifdef USE_CCM

CompContainer_impl::CompContainer_impl ()
  : IRObject_impl (CORBA::dk_none)
{
}

CompContainer_impl::CompContainer_impl (Container_impl * mycontainer,
					Repository_impl * myrepository)
  : IRObject_impl  (CORBA::dk_none),
    Container_impl (mycontainer, myrepository)
{
}

CORBA::ComponentIR::ComponentDef_ptr
CompContainer_impl::
create_component (const char * id,
		  const char * name,
		  const char * version,
		  CORBA::ComponentIR::ComponentDef_ptr base_component,
		  const CORBA::InterfaceDefSeq & supports_interfaces)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  ComponentDef_impl * comp =
    new ComponentDef_impl (this, _myrepository,
			   id, name, version);

  try {
    comp->base_component (base_component);
    comp->supported_interfaces (supports_interfaces);
    insert_contained (comp, id, name);
  }
  catch (...) {
    delete comp;
    throw;
  }

  CORBA::ComponentIR::ComponentDef_ptr c = comp->_this ();
  comp->_remove_ref ();
  return c;
}

CORBA::ComponentIR::HomeDef_ptr
CompContainer_impl::
create_home (const char * id,
	     const char * name,
	     const char * version,
	     CORBA::ComponentIR::HomeDef_ptr base_home,
	     CORBA::ComponentIR::ComponentDef_ptr managed_component,
	     const CORBA::InterfaceDefSeq & supports_interfaces,
	     CORBA::ValueDef_ptr primary_key)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  HomeDef_impl * home = new HomeDef_impl (this, _myrepository,
					  id, name, version);

  try {
    home->base_home (base_home);
    home->managed_component (managed_component);
    home->supported_interfaces (supports_interfaces);
    home->primary_key (primary_key);
    insert_contained (home, id, name);
  }
  catch (...) {
    delete home;
    throw;
  }

  CORBA::ComponentIR::HomeDef_ptr h = home->_this ();
  home->_remove_ref ();
  return h;
}

CORBA::ComponentIR::EventDef_ptr
CompContainer_impl::
create_event (const char * id,
	      const char * name,
	      const char * version,
	      CORBA::Boolean is_custom,
	      CORBA::Boolean is_abstract,
	      CORBA::ValueDef_ptr base_value,
	      CORBA::Boolean is_truncatable,
	      const CORBA::ValueDefSeq& abstract_base_values,
	      const CORBA::InterfaceDefSeq& supported_interfaces,
	      const CORBA::ExtInitializerSeq& initializers)
{
  if( _dk != CORBA::dk_Repository &&
      _dk != CORBA::dk_Module ) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO));
  }

  EventDef_impl * ev =
    new EventDef_impl (this, _myrepository,
		       id, name, version);

  try {
    ev->is_custom (is_custom);
    ev->is_abstract (is_abstract);
    ev->base_value (base_value);
    ev->is_truncatable (is_truncatable);
    ev->abstract_base_values (abstract_base_values);
    ev->supported_interfaces (supported_interfaces);
    ev->ext_initializers (initializers);
    insert_contained (ev, id, name);
  }
  catch (...) {
    delete ev;
    throw;
  }

  CORBA::ComponentIR::EventDef_ptr c = ev->_this ();
  ev->_remove_ref ();
  return c;
}

#endif

//-- IDLType ------------------------------------------------------------

IDLType_impl::IDLType_impl()
  : IRObject_impl (CORBA::dk_none)
{
  _type = CORBA::TypeCode::_nil();
}

IDLType_impl::IDLType_impl( CORBA::TypeCode_ptr type )
  : IRObject_impl (CORBA::dk_none)
{
  _type = CORBA::TypeCode::_duplicate( type );
}

CORBA::TypeCode_ptr
IDLType_impl::type()
{
  if (CORBA::is_nil (_type)) {
    mico_throw (CORBA::BAD_INV_ORDER ());
  }
  return CORBA::TypeCode::_duplicate( _type );
}

CORBA::TypeCode_ptr
IDLType_impl::recursive_type()
{
  return type ();
}

/*
 * A type must not directly contain itself, recursion is only possible
 * via sequences. Throws an exception if a case of illegal recursion is
 * detected, otherwise, it doesn't do anything.
 */

void
IDLType_impl::check_for_bad_recursion (CORBA::IDLType_ptr elm)
{
  PortableServer::ServantBase_var serv =
    IRObject_impl::_ifrpoa->reference_to_servant (elm);
  IDLType_impl * idi = dynamic_cast<IDLType_impl *> (serv.in());

  if (idi == this) {
    mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_RECURSION,
				   CORBA::COMPLETED_NO ) );
  }

  switch (elm->def_kind()) {
  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var e = CORBA::StructDef::_narrow (elm);
      CORBA::StructMemberSeq_var m = e->members();
      for (CORBA::ULong i=0; i<m->length(); i++) {
	check_for_bad_recursion (m[i].type_def.in());
      }
      break;
    }

  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var e = CORBA::UnionDef::_narrow (elm);
      CORBA::UnionMemberSeq_var m = e->members();
      for (CORBA::ULong i=0; i<m->length(); i++) {
	check_for_bad_recursion (m[i].type_def.in());
      }
      break;
    }
    
  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var e = CORBA::AliasDef::_narrow (elm);
      CORBA::IDLType_var m = e->original_type_def ();
      check_for_bad_recursion (m.in());
      break;
    }

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var e = CORBA::ArrayDef::_narrow (elm);
      CORBA::IDLType_var m = e->element_type_def ();
      check_for_bad_recursion (m.in());
      break;
    }
  default:
    break;
  }
}


//-- Repository ---------------------------------------------------------

Repository_impl::Repository_impl()
  : IRObject_impl (CORBA::dk_Repository)
{
  /*
   * Initialize Repository
   */

  _myrepository = this;

  // Make all possible primitive kinds

  PrimitiveDef_impl * pi;

  pi = new PrimitiveDef_impl( CORBA::pk_void );
  _pk_void = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_float );
  _pk_float = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_double );
  _pk_double = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_long );
  _pk_long = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_short );
  _pk_short = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_ulong );
  _pk_ulong = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_ushort );
  _pk_ushort = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_char );
  _pk_char = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_boolean );
  _pk_boolean = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_octet );
  _pk_octet = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_string );
  _pk_string = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_any );
  _pk_any = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_objref );
  _pk_objref = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_TypeCode );
  _pk_typecode = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_Principal );
  _pk_principal = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_longlong );
  _pk_longlong = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_ulonglong );
  _pk_ulonglong = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_longdouble );
  _pk_longdouble = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_wchar );
  _pk_wchar = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_wstring );
  _pk_wstring = pi->_this ();
  pi->_remove_ref ();

  pi = new PrimitiveDef_impl( CORBA::pk_value_base );
  _pk_valuebase = pi->_this ();
  pi->_remove_ref ();
}

CORBA::Contained_ptr
Repository_impl::lookup_id( const char* search_id )
{
  RepoIdMap::iterator it = _repoids.find (search_id);
  if (it != _repoids.end()) {
    return (*it).second->_this();
  }
  return CORBA::Contained::_nil ();
}

CORBA::PrimitiveDef_ptr
Repository_impl::get_primitive( CORBA::PrimitiveKind kind )
{
  switch( kind ) {
  case CORBA::pk_void:
    return CORBA::PrimitiveDef::_duplicate( _pk_void );
  case CORBA::pk_float:
    return CORBA::PrimitiveDef::_duplicate( _pk_float );
  case CORBA::pk_double:
    return CORBA::PrimitiveDef::_duplicate( _pk_double );
  case CORBA::pk_long:
    return CORBA::PrimitiveDef::_duplicate( _pk_long );
  case CORBA::pk_short:
    return CORBA::PrimitiveDef::_duplicate( _pk_short );
  case CORBA::pk_ulong:
    return CORBA::PrimitiveDef::_duplicate( _pk_ulong );
  case CORBA::pk_ushort:
    return CORBA::PrimitiveDef::_duplicate( _pk_ushort );
  case CORBA::pk_char:
    return CORBA::PrimitiveDef::_duplicate( _pk_char );
  case CORBA::pk_boolean:
    return CORBA::PrimitiveDef::_duplicate( _pk_boolean );
  case CORBA::pk_octet:
    return CORBA::PrimitiveDef::_duplicate( _pk_octet );
  case CORBA::pk_string:
    return CORBA::PrimitiveDef::_duplicate( _pk_string );
  case CORBA::pk_any:
    return CORBA::PrimitiveDef::_duplicate( _pk_any );
  case CORBA::pk_objref:
    return CORBA::PrimitiveDef::_duplicate( _pk_objref );
  case CORBA::pk_TypeCode:
    return CORBA::PrimitiveDef::_duplicate( _pk_typecode );
  case CORBA::pk_Principal:
    return CORBA::PrimitiveDef::_duplicate( _pk_principal );
  case CORBA::pk_longlong:
    return CORBA::PrimitiveDef::_duplicate( _pk_longlong );
  case CORBA::pk_ulonglong:
    return CORBA::PrimitiveDef::_duplicate( _pk_ulonglong );
  case CORBA::pk_longdouble:
    return CORBA::PrimitiveDef::_duplicate( _pk_longdouble );
  case CORBA::pk_wchar:
    return CORBA::PrimitiveDef::_duplicate( _pk_wchar );
  case CORBA::pk_wstring:
    return CORBA::PrimitiveDef::_duplicate( _pk_wstring );
  case CORBA::pk_value_base:
    return CORBA::PrimitiveDef::_duplicate( _pk_valuebase );
  default:
    assert( 0 );
  }
  return CORBA::PrimitiveDef::_nil();
}

void
Repository_impl::add_anonymous_type( CORBA::IDLType_ptr type )
{
  int i = _anonymous_types.length();
  _anonymous_types.length( i + 1 );
  _anonymous_types[ i ] = CORBA::IDLType::_duplicate( type );
}

CORBA::StringDef_ptr Repository_impl::create_string( CORBA::ULong bound )
{
  StringDef_impl * si = new StringDef_impl;
  si->bound( bound );
  CORBA::StringDef_ptr s = si->_this ();
  add_anonymous_type( s );
  return s;
}

CORBA::WstringDef_ptr Repository_impl::create_wstring( CORBA::ULong bound )
{
  WstringDef_impl * wi = new WstringDef_impl;
  wi->bound( bound );
  CORBA::WstringDef_ptr w = wi->_this ();
  add_anonymous_type( w );
  return w;
}

CORBA::SequenceDef_ptr
Repository_impl::create_sequence( CORBA::ULong bound,
				  CORBA::IDLType_ptr element_type )
{
  SequenceDef_impl * si = new SequenceDef_impl;
  si->element_type_def( element_type );
  si->bound( bound );
  CORBA::SequenceDef_ptr s = si->_this ();
  add_anonymous_type( s );
  return s;
}

CORBA::ArrayDef_ptr
Repository_impl::create_array( CORBA::ULong length,
			       CORBA::IDLType_ptr element_type )
{
  ArrayDef_impl * ai = new ArrayDef_impl;
  ai->element_type_def( element_type );
  ai->length( length );
  CORBA::ArrayDef_var a = ai->_this ();
  add_anonymous_type (a);
  return a;
}

CORBA::FixedDef_ptr
Repository_impl::create_fixed( CORBA::UShort digits,
			       CORBA::Short scale )
{
  FixedDef_impl * fi = new FixedDef_impl;
  fi->digits( digits );
  fi->scale( scale );
  CORBA::FixedDef_var f = fi->_this ();
  add_anonymous_type (f);
  return f;
}

void
Repository_impl::register_repoid (const char * id, Contained_impl * impl)
{
  RepoIdMap::iterator it = _repoids.find (id);

  if (it != _repoids.end()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO));
  }

  _repoids[id] = impl;
}

void
Repository_impl::unregister_repoid (const char * id)
{
  RepoIdMap::iterator it = _repoids.find (id);
  if (it != _repoids.end()) {
    _repoids.erase (it);
  }
}

void
Repository_impl::destroy ()
{
  mico_throw (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO));
}

void
Repository_impl::deactivate ()
{
  assert (0);
}

//-- ModuleDef ----------------------------------------------------------

ModuleDef_impl::ModuleDef_impl( Container_impl * mycontainer,
				Repository_impl * myrepository,
				const char* id,
				const char* name,
				const char* version )
  : IRObject_impl (CORBA::dk_Module),
    Contained_impl (mycontainer, myrepository, id, name, version),
#ifdef USE_CCM
    CompContainer_impl (mycontainer, myrepository),
#endif
    Container_impl (mycontainer, myrepository)
{
}

CORBA::Contained::Description *
ModuleDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ModuleDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

void
ModuleDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- ConstantDef --------------------------------------------------------

ConstantDef_impl::ConstantDef_impl( Container_impl * mycontainer,
				    Repository_impl * myrepository,
				    const char* id,
				    const char* name,
				    const char* version )
  : IRObject_impl (CORBA::dk_Constant),
    Contained_impl (mycontainer, myrepository, id, name, version)    
{
  _type_def = CORBA::IDLType::_nil ();
}

CORBA::TypeCode_ptr
ConstantDef_impl::type()
{
  if (CORBA::is_nil (_type_def)) {
    mico_throw (CORBA::BAD_INV_ORDER ());
  }
  return _type_def->type();
}

CORBA::IDLType_ptr
ConstantDef_impl::type_def()
{
  if (CORBA::is_nil (_type_def)) {
    mico_throw (CORBA::BAD_INV_ORDER ());
  }
  return CORBA::IDLType::_duplicate( _type_def );
}

void
ConstantDef_impl::type_def( CORBA::IDLType_ptr value )
{
  _type_def = CORBA::IDLType::_duplicate( value );
}


CORBA::Any*
ConstantDef_impl::value()
{
  CORBA::Any *a = new CORBA::Any;
  *a = _value;
  return a;
}

void ConstantDef_impl::value( const CORBA::Any& a )
{
  if (CORBA::is_nil (_type_def)) {
    mico_throw (CORBA::BAD_INV_ORDER (0, CORBA::COMPLETED_NO));
  }
  CORBA::TypeCode_var tc1 = _type_def->type();
  CORBA::TypeCode_var tc2 = ((CORBA::Any&) a).type();
  if( !tc1->equivalent( tc2 ) ) {
    mico_throw (CORBA::NO_PERMISSION());
  }
  _value = a;
}

CORBA::Contained::Description *
ConstantDef_impl::describe ()
{
  if (CORBA::is_nil (_type_def)) {
    mico_throw (CORBA::BAD_INV_ORDER (0, CORBA::COMPLETED_NO));
  }

  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ConstantDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.type        = _type_def->type ();
  d.value       = _value;

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

//-- TypedefDef ---------------------------------------------------------

TypedefDef_impl::TypedefDef_impl (Container_impl * mycontainer,
				  Repository_impl * myrepository,
				  const char * id, const char * name,
				  const char * version)
  : IRObject_impl (CORBA::dk_none),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
}

CORBA::Contained::Description *
TypedefDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::TypeDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.type        = type ();

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

//-- StructDef ----------------------------------------------------------

StructDef_impl::StructDef_impl( Container_impl * mycontainer,
				Repository_impl * myrepository,
				const char* id,
				const char* name,
				const char* version )
  : IRObject_impl (CORBA::dk_Struct),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version),
    Container_impl (mycontainer, myrepository)
{
  _typedirty = 1;
  _visited = 0;
}

CORBA::StructMemberSeq* StructDef_impl::members()
{
  /*
   * Make sure type code (and therefore member type codes) are up to date
   */

  CORBA::TypeCode_var mytype = type ();
  return new CORBA::StructMemberSeq (_members);
}

void StructDef_impl::members( const CORBA::StructMemberSeq& _value )
{
  for (CORBA::ULong i=0; i<_value.length(); i++) {
    if (!*_value[i].name.in()) {
      /*
       * empty name
       */
      mico_throw (CORBA::BAD_PARAM());
    }
    if (strnocasecmp (_value[i].name, _name.in()) == 0) {
      /*
       * member name must be different from scope name
       */
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO));
    }
    if (CORBA::is_nil (_value[i].type_def.in())) {
      /*
       * empty type
       */
      mico_throw (CORBA::BAD_PARAM());
    }
    check_for_bad_recursion (_value[i].type_def.in());
  }

  /*
   * member names are introduced into our scope. First,
   * unregister the old names, then register the new names.
   *
   * If a problem happens with registering the new names, then we
   * must roll back and re-register the old ones.
   */

  CORBA::ULong j=0;

  for (j=0; j<_members.length(); j++) {
    unregister_name (_members[j].name.in());
  }

  try {
    for (j=0; j<_value.length(); j++) {
      register_name (_value[j].name.in(), 0);
    }
  }
  catch (CORBA::BAD_PARAM &) {
    if (j > 0) {
      do {
	j--;
	unregister_name (_value[j].name.in());
      }
      while (j != 0);
    }
    // rollback
    for (j=0; j<_members.length(); j++) {
      register_name (_members[j].name.in(), 0);
    }
    throw;
  }

  _typedirty = 1;
  _members = _value;
}

CORBA::TypeCode_ptr
StructDef_impl::recursive_type ()
{
  if (_visited) {
    return CORBA::TypeCode::create_recursive_tc (_id);
  }

  _visited = 1;

  for (CORBA::ULong i = 0; i < _members.length(); ++i) {
    PortableServer::ServantBase_var serv =
      IRObject_impl::_ifrpoa->reference_to_servant (_members[i].type_def);
    IDLType_impl * idi = dynamic_cast<IDLType_impl *> (serv.in());
    assert (idi);
    _members[i].type = idi->recursive_type ();
  }

  _visited = 0;

  return CORBA::TypeCode::create_struct_tc (_id, _name, _members);
}

CORBA::TypeCode_ptr
StructDef_impl::type()
{
  if (_visited) {
    return recursive_type ();
  }

  if (_typedirty) {
    _type = recursive_type ();
    _typedirty = 0;
  }

  return CORBA::TypeCode::_duplicate (_type);
}

void
StructDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- UnionDef -----------------------------------------------------------

UnionDef_impl::UnionDef_impl( Container_impl * mycontainer,
			      Repository_impl * myrepository,
			      const char* id,
			      const char* name,
			      const char* version )
  : IRObject_impl(CORBA::dk_Union),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version),
    Container_impl (mycontainer, myrepository)
{
  _discr = CORBA::IDLType::_nil ();
  _typedirty = 1;
  _visited = 0;
}

CORBA::TypeCode_ptr UnionDef_impl::discriminator_type()
{
  return _discr->type();
}

CORBA::IDLType_ptr UnionDef_impl::discriminator_type_def()
{
  return CORBA::IDLType::_duplicate( _discr );
}

void UnionDef_impl::discriminator_type_def( CORBA::IDLType_ptr value )
{
  _typedirty = 1;
  _discr = CORBA::IDLType::_duplicate( value );
}

CORBA::UnionMemberSeq* UnionDef_impl::members()
{
  /*
   * Make sure type code (and therefore member type codes) are up to date
   */

  CORBA::TypeCode_var mytype = type ();
  return new CORBA::UnionMemberSeq (_members);
}

void UnionDef_impl::members( const CORBA::UnionMemberSeq& _value )
{
  check_explicit_default_case (_value);

  for (CORBA::ULong i=0; i<_value.length(); i++) {
    if (!*_value[i].name.in()) {
      /*
       * empty name
       */
      mico_throw (CORBA::BAD_PARAM());
    }
    if (strnocasecmp (_value[i].name, _name.in()) == 0) {
      /*
       * member name must be different from scope name
       */
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO));
    }
    if (CORBA::is_nil (_value[i].type_def.in())) {
      /*
       * empty type
       */
      mico_throw (CORBA::BAD_PARAM());
    }
    for (CORBA::ULong o=i+1; o<_value.length(); o++) {
      if (strnocasecmp (_value[i].name.in(), _value[o].name.in()) == 0) {
	/*
	 * duplicate name is ok if the type is the same
	 */
	CORBA::TypeCode_var tc1 = _value[i].type_def->type ();
	CORBA::TypeCode_var tc2 = _value[o].type_def->type ();
	if (!tc1->equivalent (tc2.in())) {
	  mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_DUPLICATE_MEMBER,
                                         CORBA::COMPLETED_NO ) );
	}
      }
      if (_value[i].label == _value[o].label) {
	/*
	 * duplicate label
	 */
	mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_DUPLICATE_LABEL,
                                       CORBA::COMPLETED_NO ) );
      }
    }
    check_for_bad_recursion (_value[i].type_def.in());
  }

  /*
   * member names are introduced into our scope. First,
   * unregister the old names, then register the new names.
   *
   * If a problem happens with registering the new names, then we
   * must roll back and re-register the old ones.
   *
   * Member names may be equal for "fall-through" cases, so we must
   * take care at each step.
   */


  CORBA::ULong j=0, k=0;

  for (j=0; j<_members.length(); j++) {
    unregister_name (_members[j].name.in());
  }

  try {
    for (j=0; j<_value.length(); j++) {
      for (k=0; k<j; k++) {
	if (strcmp (_value[k].name.in(), _value[j].name.in()) == 0) {
	  break;
	}
      }
      if (k == j) {
	register_name (_value[j].name.in(), 0);
      }
    }
  }
  catch (CORBA::BAD_PARAM &) {
    if (j > 0) {
      do {
	j--;
	unregister_name (_value[j].name.in());
      }
      while (j != 0);
    }
    // rollback
    for (j=0; j<_members.length(); j++) {
      for (k=0; k<j; k++) {
	if (strcmp (_members[k].name.in(), _members[j].name.in()) == 0) {
	  break;
	}
      }
      if (k == j) {
	register_name (_members[j].name.in(), 0);
      }
    }
    throw;
  }

  _members = _value;
  _typedirty = 1;
}



CORBA::TypeCode_ptr
UnionDef_impl::recursive_type()
{
  if (_visited) {
    return CORBA::TypeCode::create_recursive_tc (_id);
  }

  _visited = 1;

  CORBA::TypeCode_var discr_type = _discr->type ();

  for (CORBA::ULong i = 0; i < _members.length(); ++i) {
    PortableServer::ServantBase_var serv =
      IRObject_impl::_ifrpoa->reference_to_servant (_members[i].type_def);
    IDLType_impl * idi = dynamic_cast<IDLType_impl *> (serv.in());
    assert (idi);
    _members[i].type = idi->recursive_type ();
  }

  _visited = 0;

  return CORBA::TypeCode::create_union_tc(_id, _name, discr_type, _members);
}

CORBA::TypeCode_ptr
UnionDef_impl::type()
{
  if (_visited) {
    return recursive_type ();
  }

  if (_typedirty) {
    _type = recursive_type ();
    _typedirty = 0;
  }

  return CORBA::TypeCode::_duplicate (_type);
}


void
UnionDef_impl::check_explicit_default_case (const CORBA::UnionMemberSeq &mems)
{
  // If we have an explicit default case, make sure that we have
  // value of the discriminator type for the default value left
  CORBA::ULong i;
  
  for( i = 0; i < mems.length(); i++ ) {
    CORBA::TypeCode_var tc = mems[ i ].label.type();
    if( tc->kind() == CORBA::tk_octet )
      break;
  }
  if( i == mems.length() )
    // There was no explicit default case. So there is nothing to check
    return;

  // Determine the number of possible values for the discriminator type
  CORBA::ULongLong num_discr_vals;
  CORBA::TypeCode_var discr_tc = _discr->type();
  switch( discr_tc->kind() ) {
  case CORBA::tk_boolean:
    num_discr_vals = 2;
    break;
  case CORBA::tk_char:
    num_discr_vals = 256;
    break;
  case CORBA::tk_enum:
    num_discr_vals = discr_tc->member_count();
    break;
  default:
    // For all other discriminator types we just assume a
    // ridiciously high value. Its highly unlikely that
    // there exisits a IDL-union specification with >> 1000 cases.
    num_discr_vals = 0xffffffff;
    break;
  }
  
  if( num_discr_vals == mems.length() - 1 )
    mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_UNION,
                                   CORBA::COMPLETED_NO ) );
}

void
UnionDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- EnumDef ------------------------------------------------------------

EnumDef_impl::EnumDef_impl( Container_impl * mycontainer,
			    Repository_impl * myrepository,
			    const char* id,
			    const char* name,
			    const char* version )
  : IRObject_impl(CORBA::dk_Enum),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version)
{
}

CORBA::EnumMemberSeq* EnumDef_impl::members()
{
  CORBA::EnumMemberSeq *m = new CORBA::EnumMemberSeq;
  *m = _members;
  return m;
}

void EnumDef_impl::members( const CORBA::EnumMemberSeq& _value )
{
  /*
   * enum values are introduced into the surrounding scope. First,
   * unregister the old names, then register the new names.
   *
   * If a problem happens with registering the new names, then we
   * must roll back and re-register the old ones.
   */

  CORBA::ULong i=0;

  for (i=0; i<_members.length(); i++) {
    _mycontainer->unregister_name (_members[i]);
  }

  try {
    for (i=0; i<_value.length(); i++) {
      _mycontainer->register_name (_value[i], 0);
    }
  }
  catch (CORBA::BAD_PARAM &) {
    if (i > 0) {
      do {
	i--;
	_mycontainer->unregister_name (_value[i]);
      }
      while (i != 0);
    }
    // rollback
    for (i=0; i<_members.length(); i++) {
      _mycontainer->register_name (_members[i], 0);
    }
    throw;
  }

  _members = _value;
  _type = CORBA::TypeCode::create_enum_tc( _id, _name, _members );
}


//-- AliasDef -----------------------------------------------------------

AliasDef_impl::AliasDef_impl( Container_impl * mycontainer,
			      Repository_impl * myrepository,
			      const char* id,
			      const char* name,
			      const char* version )
  : IRObject_impl(CORBA::dk_Alias),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version)
{
  _original = CORBA::IDLType::_nil ();
}

CORBA::IDLType_ptr AliasDef_impl::original_type_def()
{
  return CORBA::IDLType::_duplicate( _original );
}

void AliasDef_impl::original_type_def( CORBA::IDLType_ptr _value )
{
  check_for_bad_recursion (_value);
  _original = CORBA::IDLType::_duplicate( _value );
}

CORBA::TypeCode_ptr
AliasDef_impl::type()
{
  CORBA::TypeCode_var original_type = _original->type ();
  return CORBA::TypeCode::create_alias_tc (_id, _name, original_type);
}

//-- NativeDef -----------------------------------------------------------

NativeDef_impl::NativeDef_impl ( Container_impl * mycontainer,
				 Repository_impl * myrepository,
				 const char* id,
				 const char* name,
				 const char* version )
  : IRObject_impl(CORBA::dk_Native),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version)
{
  _dk = CORBA::dk_Native;
  _id = id;
  _name = name;
  _version = version;
  _type = CORBA::TypeCode::create_native_tc( _id, _name );
}

//-- PrimitiveDef -------------------------------------------------------

PrimitiveDef_impl::PrimitiveDef_impl( CORBA::PrimitiveKind kind )
  : IRObject_impl (CORBA::dk_Primitive)
{
  _kind = kind;
  switch( kind ) {
  case CORBA::pk_void:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_void );
    break;
  case CORBA::pk_float:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_float );
    break;
  case CORBA::pk_double:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_double );
    break;
  case CORBA::pk_long:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_long );
    break;
  case CORBA::pk_short:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_short );
    break;
  case CORBA::pk_ulong:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_ulong );
    break;
  case CORBA::pk_ushort:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_ushort );
    break;
  case CORBA::pk_char:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_char );
    break;
  case CORBA::pk_boolean:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_boolean );
    break;
  case CORBA::pk_octet:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_octet );
    break;
  case CORBA::pk_string:
    _type = CORBA::TypeCode::create_string_tc( 0 );
    break;
  case CORBA::pk_any:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_any );
    break;
  case CORBA::pk_objref:
    _type = CORBA::TypeCode::create_interface_tc( 
                      (const char *) "IDL:omg.org/CORBA/Object:1.0",
		      "Object" );
    break;
  case CORBA::pk_TypeCode:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_TypeCode );
    break;
  case CORBA::pk_Principal:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_Principal );
    break;
  case CORBA::pk_longlong:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_longlong );
    break;
  case CORBA::pk_ulonglong:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_ulonglong );
    break;
  case CORBA::pk_longdouble:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_longdouble );
    break;
  case CORBA::pk_wchar:
    _type = CORBA::TypeCode::create_basic_tc( CORBA::tk_wchar );
    break;
  case CORBA::pk_wstring:
    _type = CORBA::TypeCode::create_wstring_tc( 0 );
    break;
  case CORBA::pk_value_base:
    _type = CORBA::TypeCode::create_value_tc(
                      "IDL:omg.org/CORBA/ValueBase:1.0",
		      "ValueBase",
		      CORBA::VM_NONE,
		      CORBA::TypeCode::_nil(),
		      CORBA::ValueMemberSeq());
    break;
  default:
    assert( 0 );
  }
}

CORBA::PrimitiveKind PrimitiveDef_impl::kind()
{
  return _kind;
}

void
PrimitiveDef_impl::destroy ()
{
  mico_throw (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO));
}

void
PrimitiveDef_impl::deactivate ()
{
  assert (0);
}

//-- StringDef ----------------------------------------------------------

StringDef_impl::StringDef_impl()
  : IRObject_impl (CORBA::dk_String)
{
  _type = CORBA::TypeCode::create_string_tc( 0 );
  _bound = 0;
}

CORBA::ULong StringDef_impl::bound()
{
  return _bound;
}

void StringDef_impl::bound( CORBA::ULong _value )
{
  if( _value == 0 )
    mico_throw (CORBA::BAD_PARAM());

  _bound = _value;
  _type = CORBA::TypeCode::create_string_tc( _value );
}

void
StringDef_impl::deactivate ()
{
}

//-- WstringDef ---------------------------------------------------------

WstringDef_impl::WstringDef_impl()
  : IRObject_impl (CORBA::dk_Wstring)
{
  _type = CORBA::TypeCode::create_wstring_tc( 0 );
  _bound = 0;
}

CORBA::ULong WstringDef_impl::bound()
{
  return _bound;
}

void WstringDef_impl::bound( CORBA::ULong _value )
{
  if( _value == 0 )
    mico_throw (CORBA::BAD_PARAM());

  _bound = _value;
  _type = CORBA::TypeCode::create_wstring_tc( _value );
}


void
WstringDef_impl::deactivate ()
{
}

//-- FixedDef -----------------------------------------------------------

FixedDef_impl::FixedDef_impl()
  : IRObject_impl (CORBA::dk_Fixed)
{
  _type = CORBA::TypeCode::create_fixed_tc( 0, 0 );
  _digits = 0;
  _scale = 0;
}

CORBA::UShort FixedDef_impl::digits()
{
  return _digits;
}

void FixedDef_impl::digits( CORBA::UShort _value )
{
  _digits = _value;
  _type = CORBA::TypeCode::create_fixed_tc( _digits, _scale );
}

CORBA::Short FixedDef_impl::scale()
{
  return _scale;
}

void FixedDef_impl::scale( CORBA::Short _value )
{
  _scale = _value;
  _type = CORBA::TypeCode::create_fixed_tc( _digits, _scale );
}

void
FixedDef_impl::deactivate ()
{
}

//-- SequenceDef --------------------------------------------------------

SequenceDef_impl::SequenceDef_impl()
  : IRObject_impl (CORBA::dk_Sequence)
{
  _bound = 0;
  _element_type_def = CORBA::IDLType::_nil();
}

CORBA::TypeCode_ptr
SequenceDef_impl::type()
{
  CORBA::TypeCode_var element_type = _element_type_def->type ();
  return CORBA::TypeCode::create_sequence_tc (_bound, element_type);
}

CORBA::ULong SequenceDef_impl::bound()
{
  return _bound;
}

void SequenceDef_impl::bound( CORBA::ULong _value )
{
  _bound = _value;
}

CORBA::TypeCode_ptr SequenceDef_impl::element_type()
{
  assert( !CORBA::is_nil( _element_type_def ) );
  return _element_type_def->type();
}

CORBA::IDLType_ptr SequenceDef_impl::element_type_def()
{
  return CORBA::IDLType::_duplicate( _element_type_def );
}

void SequenceDef_impl::element_type_def( CORBA::IDLType_ptr _value )
{
  _element_type_def = CORBA::IDLType::_duplicate( _value );
}

void
SequenceDef_impl::deactivate ()
{
}

//-- ArrayDef -----------------------------------------------------------

ArrayDef_impl::ArrayDef_impl()
  : IRObject_impl (CORBA::dk_Array)
{
  _length = 0;
  _element_type_def = CORBA::IDLType::_nil();
}

CORBA::TypeCode_ptr
ArrayDef_impl::type()
{
  CORBA::TypeCode_var element_type = _element_type_def->type ();
  return CORBA::TypeCode::create_array_tc (_length, element_type);
}

CORBA::ULong ArrayDef_impl::length()
{
  return _length;
}

void ArrayDef_impl::length( CORBA::ULong _value )
{
  _length = _value;
}

CORBA::TypeCode_ptr ArrayDef_impl::element_type()
{
  assert( !CORBA::is_nil( _element_type_def ) );
  return _element_type_def->type();
}

CORBA::IDLType_ptr ArrayDef_impl::element_type_def()
{
  return CORBA::IDLType::_duplicate( _element_type_def );
}

void ArrayDef_impl::element_type_def( CORBA::IDLType_ptr _value )
{
  check_for_bad_recursion (_value);
  _element_type_def = CORBA::IDLType::_duplicate( _value );
}

void
ArrayDef_impl::deactivate ()
{
}

//-- ExceptionDef -------------------------------------------------------

ExceptionDef_impl::ExceptionDef_impl( Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char* id,
				      const char* name,
				      const char* version )
  : IRObject_impl( CORBA::dk_Exception ),
    Contained_impl (mycontainer, myrepository, id, name, version),
    Container_impl (mycontainer, myrepository)
{
  _type = CORBA::TypeCode::_nil ();
}

CORBA::StructMemberSeq* ExceptionDef_impl::members()
{
  // update member types
  CORBA::TypeCode_var dummy = type ();
  return new CORBA::StructMemberSeq (_members);
}

void ExceptionDef_impl::members( const CORBA::StructMemberSeq& _value )
{
  for (CORBA::ULong i0=0; i0<_value.length(); i0++) {
    if (!*_value[i0].name.in()) {
      /*
       * empty name
       */
      mico_throw (CORBA::BAD_PARAM());
    }
    if (strnocasecmp (_value[i0].name, _name.in()) == 0) {
      /*
       * member name must be different from scope name
       */
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO));
    }
    if (CORBA::is_nil (_value[i0].type_def.in())) {
      /*
       * empty type
       */
      mico_throw (CORBA::BAD_PARAM());
    }
  }

  /*
   * member names are introduced into our scope. First,
   * unregister the old names, then register the new names.
   *
   * If a problem happens with registering the new names, then we
   * must roll back and re-register the old ones.
   */

  CORBA::ULong j=0;

  for (j=0; j<_members.length(); j++) {
    unregister_name (_members[j].name.in());
  }

  try {
    for (j=0; j<_value.length(); j++) {
      register_name (_value[j].name.in(), 0);
    }
  }
  catch (CORBA::BAD_PARAM &) {
    do {
      j--;
      unregister_name (_value[j].name.in());
    }
    while (j != 0);
    // rollback
    for (j=0; j<_members.length(); j++) {
      register_name (_members[j].name.in(), 0);
    }
    throw;
  }

  _members = _value;
}

CORBA::TypeCode_ptr
ExceptionDef_impl::type()
{
  for (CORBA::ULong i1 = 0; i1 < _members.length(); ++i1) {
    _members[i1].type = _members[i1].type_def->type();
  }

  _type = CORBA::TypeCode::create_exception_tc (_id, _name, _members);
  return CORBA::TypeCode::_duplicate (_type);
}

CORBA::Contained::Description *
ExceptionDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ExceptionDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.type        = type ();

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

void
ExceptionDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- AttributeDef -------------------------------------------------------

AttributeDef_impl::AttributeDef_impl (Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char* id,
				      const char* name,
				      const char* version)
  : IRObject_impl( CORBA::dk_Attribute ),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
  _type_def = CORBA::IDLType::_nil ();
}

CORBA::TypeCode_ptr AttributeDef_impl::type()
{
  return _type_def->type();
}

CORBA::IDLType_ptr AttributeDef_impl::type_def()
{
  return CORBA::IDLType::_duplicate( _type_def );
}

void AttributeDef_impl::type_def( CORBA::IDLType_ptr _value )
{
  _type_def = CORBA::IDLType::_duplicate( _value );
}

CORBA::AttributeMode AttributeDef_impl::mode()
{
  return _mode;
}

void AttributeDef_impl::mode( CORBA::AttributeMode _value )
{
  _mode = _value;
}

CORBA::ExceptionDefSeq *
AttributeDef_impl::get_exceptions ()
{
  return new CORBA::ExceptionDefSeq (_get_exceptions);
}

void
AttributeDef_impl::get_exceptions (const CORBA::ExceptionDefSeq & _value)
{
  _get_exceptions = _value;
}

CORBA::ExceptionDefSeq *
AttributeDef_impl::set_exceptions ()
{
  return new CORBA::ExceptionDefSeq (_set_exceptions);
}

void
AttributeDef_impl::set_exceptions (const CORBA::ExceptionDefSeq & _value)
{
  _set_exceptions = _value;
}

CORBA::Contained::Description *
AttributeDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::AttributeDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.type        = _type_def->type ();
  d.mode        = _mode;

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

CORBA::ExtAttributeDescription *
AttributeDef_impl::describe_attribute ()
{
  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ExtAttributeDescription * d = new CORBA::ExtAttributeDescription;

  d->name        = _name;
  d->id          = _id;
  d->defined_in  = def_in_id;
  d->version     = _version;
  d->type        = _type_def->type ();
  d->mode        = _mode;

  d->get_exceptions.length (_get_exceptions.length());

  for (CORBA::ULong i0=0; i0<_get_exceptions.length(); i0++) {
    CORBA::Contained::Description_var desc = _get_exceptions[i0]->describe ();
    assert (desc->kind == CORBA::dk_Exception);
    desc->value >>= d->get_exceptions[i0];
  }

  d->set_exceptions.length (_set_exceptions.length());

  for (CORBA::ULong i1=0; i1<_set_exceptions.length(); i1++) {
    CORBA::Contained::Description_var desc = _set_exceptions[i1]->describe ();
    assert (desc->kind == CORBA::dk_Exception);
    desc->value >>= d->set_exceptions[i1];
  }

  return d;
}

//-- OperationDef -------------------------------------------------------

OperationDef_impl::OperationDef_impl( Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char* id,
				      const char* name,
				      const char* version )
  : IRObject_impl( CORBA::dk_Operation ),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
  _result_def = CORBA::IDLType::_nil ();
}

CORBA::TypeCode_ptr OperationDef_impl::result()
{
  return _result_def->type();
}

CORBA::IDLType_ptr OperationDef_impl::result_def()
{
  return CORBA::IDLType::_duplicate( _result_def );
}

void OperationDef_impl::result_def( CORBA::IDLType_ptr _value )
{
  _result_def = CORBA::IDLType::_duplicate( _value );
}

CORBA::ParDescriptionSeq* OperationDef_impl::params()
{
  for (CORBA::ULong i=0; i<_params.length(); i++) {
    _params[i].type = _params[i].type_def->type ();
  }

  return new CORBA::ParDescriptionSeq (_params);
}

void OperationDef_impl::params( const CORBA::ParDescriptionSeq& _value )
{
  _params = _value;
}

CORBA::OperationMode OperationDef_impl::mode()
{
  return _mode;
}

void OperationDef_impl::mode( CORBA::OperationMode _value )
{
  _mode = _value;
}

CORBA::ContextIdSeq* OperationDef_impl::contexts()
{
  return new CORBA::ContextIdSeq (_contexts);
}

void OperationDef_impl::contexts( const CORBA::ContextIdSeq& _value )
{
  _contexts = _value;
}

CORBA::ExceptionDefSeq* OperationDef_impl::exceptions()
{
  return new CORBA::ExceptionDefSeq (_exceptions);
}

void OperationDef_impl::exceptions( const CORBA::ExceptionDefSeq& _value )
{
  _exceptions = _value;
}

CORBA::Contained::Description *
OperationDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ParDescriptionSeq_var myparams = params ();

  CORBA::OperationDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.result      = _result_def->type ();
  d.mode        = _mode;
  d.contexts    = _contexts;
  d.parameters  = myparams.in();

  d.exceptions.length (_exceptions.length());

  for (CORBA::ULong i=0; i<_exceptions.length(); i++) {
    CORBA::Contained::Description_var desc = _exceptions[i]->describe ();
    assert (desc->kind == CORBA::dk_Exception);
    desc->value >>= d.exceptions[i];
  }

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

//-- InterfaceDef -------------------------------------------------------

InterfaceDef_impl::InterfaceDef_impl()
  : IRObject_impl( CORBA::dk_none ),
    Contained_impl(0 , 0, "", "", "")
{
}

InterfaceDef_impl::InterfaceDef_impl( Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char* id,
				      const char* name,
				      const char* version )
  : IRObject_impl( CORBA::dk_Interface ),
    Container_impl (mycontainer, myrepository),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
}

CORBA::InterfaceDefSeq*
InterfaceDef_impl::base_interfaces()
{
  return new CORBA::InterfaceDefSeq (_base_interfaces);
}

void
InterfaceDef_impl::base_interfaces( const CORBA::InterfaceDefSeq& _value )
{
  for (CORBA::ULong i=0; i<_value.length(); i++) {
    if (_dk == CORBA::dk_AbstractInterface &&
	_value[i]->def_kind() != CORBA::dk_AbstractInterface) {
      /*
       * Attempt to derive abstract interface from non-abstract base interface
       */
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 11,
				    CORBA::COMPLETED_NO));
    }
    else if (_dk == CORBA::dk_LocalInterface &&
	     _value[i]->def_kind() == CORBA::dk_AbstractInterface) {
      /*
       * Local interface can not inherit from abstract interface
       */
      mico_throw (CORBA::BAD_PARAM ());
    }
    else if (_dk == CORBA::dk_Interface &&
	     _value[i]->def_kind() == CORBA::dk_LocalInterface) {
      /*
       * Normal interface cannot inherit from local interface
       */
      mico_throw (CORBA::BAD_PARAM ());
    }
  }

  _base_interfaces = _value;
}

CORBA::Boolean
InterfaceDef_impl::is_a( const char* interface_id )
{
  if( strcmp( _id, interface_id ) == 0 )
    return TRUE;

  if( strcmp( "IDL:omg.org/CORBA/Object:1.0", interface_id ) == 0 &&
      def_kind() == CORBA::dk_Interface )
    return TRUE;

  if( strcmp( "IDL:omg.org/CORBA/AbstractBase:1.0", interface_id ) == 0 &&
      def_kind() == CORBA::dk_AbstractInterface )
    return TRUE;

  if( strcmp( "IDL:omg.org/CORBA/LocalObject:1.0", interface_id ) == 0 &&
      def_kind() == CORBA::dk_LocalInterface )
    return TRUE;
   
  for( CORBA::ULong i = 0; i < _base_interfaces.length(); i++ ) {
    if( _base_interfaces[ i ]->is_a( interface_id ) )
      return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef::FullInterfaceDescription*
InterfaceDef_impl::describe_interface()
{
  CORBA::InterfaceDef::FullInterfaceDescription *desc =
    new CORBA::InterfaceDef::FullInterfaceDescription;
  
  desc->name = _name;
  desc->id = _id;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  if( !CORBA::is_nil( def_in ) )
    desc->defined_in = def_in->id();
  else
    desc->defined_in = (const char *) "";
  
  desc->version = _version;

  desc->base_interfaces.length( _base_interfaces.length() );
  for( CORBA::ULong j0 = 0; j0 < _base_interfaces.length(); j0++ )
	desc->base_interfaces[ j0 ] = _base_interfaces[ j0 ]->id();

  desc->type = type();
  
  CORBA::ContainedSeq_var csv = contents (CORBA::dk_Operation, FALSE);
  desc->operations.length (csv->length());
  for (CORBA::ULong i0=0; i0<csv->length(); i0++) {
    CORBA::Contained::Description_var d = csv[i0]->describe ();
    const CORBA::OperationDescription * od;
    assert (d->kind == CORBA::dk_Operation);
    CORBA::Boolean r = (d->value >>= od);
    assert (r);
    desc->operations[i0] = *od;
  }

  csv = contents (CORBA::dk_Attribute, FALSE);
  desc->attributes.length (csv->length());
  for (CORBA::ULong i1=0; i1<csv->length(); i1++) {
    CORBA::Contained::Description_var d = csv[i1]->describe ();
    const CORBA::AttributeDescription * ad;
    assert (d->kind == CORBA::dk_Attribute);
    CORBA::Boolean r = (d->value >>= ad);
    assert (r);
    desc->attributes[i1] = *ad;
  }

  return desc;
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription*
InterfaceDef_impl::describe_ext_interface()
{
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *desc =
    new CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription;
  
  desc->name = _name;
  desc->id = _id;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  if( !CORBA::is_nil( def_in ) )
    desc->defined_in = def_in->id();
  else
    desc->defined_in = (const char *) "";
  
  desc->version = _version;

  desc->base_interfaces.length( _base_interfaces.length() );
  for( CORBA::ULong j0 = 0; j0 < _base_interfaces.length(); j0++ )
	desc->base_interfaces[ j0 ] = _base_interfaces[ j0 ]->id();

  desc->type = type();
  
  CORBA::ContainedSeq_var csv = contents (CORBA::dk_Operation, FALSE);
  desc->operations.length (csv->length());
  for (CORBA::ULong i0=0; i0<csv->length(); i0++) {
    CORBA::Contained::Description_var d = csv[i0]->describe ();
    const CORBA::OperationDescription * od;
    assert (d->kind == CORBA::dk_Operation);
    CORBA::Boolean r = (d->value >>= od);
    assert (r);
    desc->operations[i0] = *od;
  }

  csv = contents (CORBA::dk_Attribute, FALSE);
  desc->attributes.length (csv->length());
  for (CORBA::ULong i1=0; i1<csv->length(); i1++) {
    CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (csv[i1]);
    CORBA::ExtAttributeDescription_var ad = ead->describe_attribute ();
    desc->attributes[i1] = ad.in();
  }

  return desc;
}

CORBA::AttributeDef_ptr
InterfaceDef_impl::create_attribute( const char* id,
				     const char* name,
				     const char* version,
				     CORBA::IDLType_ptr type,
				     CORBA::AttributeMode modeval )
{
  return create_ext_attribute (id, name, version, type, modeval,
			       CORBA::ExceptionDefSeq(0),
			       CORBA::ExceptionDefSeq(0));
}

CORBA::ExtAttributeDef_ptr
InterfaceDef_impl::create_ext_attribute (const char* id,
					 const char* name,
					 const char* version,
					 CORBA::IDLType_ptr type,
					 CORBA::AttributeMode modeval,
					 const CORBA::ExceptionDefSeq & ge,
					 const CORBA::ExceptionDefSeq & se)
{
  /*
   * We may overload inherited types, but not inherited operations
   * or attributes. Redefinition of types in the current scope fails
   * in insert_contained below.
   */

  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation
#ifdef USE_CCM
	||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes  || dk == CORBA::dk_Factory   ||
	dk == CORBA::dk_Finder
#endif
	) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  AttributeDef_impl * opi =
    new AttributeDef_impl (this, Contained_impl::_myrepository,
			   id, name, version);

  try {
    opi->type_def (type);
    opi->mode (modeval);
    opi->get_exceptions (ge);
    opi->set_exceptions (se);
    insert_contained (opi, id, name);
  }
  catch (...) {
    delete opi;
    throw;
  }

  CORBA::ExtAttributeDef_ptr op = opi->_this ();
  opi->_remove_ref ();
  return op;
}

CORBA::OperationDef_ptr
InterfaceDef_impl::create_operation( const char* id,
				     const char* name,
				     const char* version,
				     CORBA::IDLType_ptr result,
				     CORBA::OperationMode opmode,
				     const CORBA::ParDescriptionSeq& opparams,
				     const CORBA::ExceptionDefSeq& exceptions,
				     const CORBA::ContextIdSeq& ctxs )
{
  /*
   * We may overload inherited types, but not inherited operations
   * or attributes. Redefinition of types in the current scope fails
   * in insert_contained below.
   */

  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation
#ifdef USE_CCM
	||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes  || dk == CORBA::dk_Factory   ||
	dk == CORBA::dk_Finder
#endif
	) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  if( opmode == CORBA::OP_ONEWAY ) {
    // Check that oneway operations have no result, no out, inout parameters
    // and no raises expressions
    CORBA::TypeCode_var result_tc = result->type();
    if( result_tc->kind() != CORBA::tk_void )
      mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                     CORBA::COMPLETED_NO ) );
    if( exceptions.length() != 0 )
      mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                     CORBA::COMPLETED_NO ) );
    for( CORBA::ULong i = 0; i < opparams.length(); i++ )
      if( opparams[ i ].mode != CORBA::PARAM_IN )
	mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                       CORBA::COMPLETED_NO ) );
  }

  OperationDef_impl * opi =
    new OperationDef_impl (this, Contained_impl::_myrepository,
			   id, name, version);

  try {
    opi->result_def (result);
    opi->params (opparams);
    opi->mode (opmode);
    opi->contexts (ctxs);
    opi->exceptions (exceptions);
    insert_contained (opi, id, name);
  }
  catch (...) {
    delete opi;
    throw;
  }

  CORBA::OperationDef_ptr op = opi->_this ();
  opi->_remove_ref ();
  return op;
}

CORBA::Contained::Description *
InterfaceDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::InterfaceDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;

  d.base_interfaces.length (_base_interfaces.length());
  for (CORBA::ULong i=0; i<_base_interfaces.length(); i++) {
    d.base_interfaces[i] = _base_interfaces[i]->id();
  }

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

CORBA::TypeCode_ptr
InterfaceDef_impl::type()
{
  return CORBA::TypeCode::create_interface_tc (_id.in(), _name.in());
}

void
InterfaceDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- AbstractInterfaceDef -------------------------------------------------

AbstractInterfaceDef_impl::
AbstractInterfaceDef_impl( Container_impl * mycontainer,
			   Repository_impl * myrepository,
			   const char* id,
			   const char* name,
			   const char* version )
  : IRObject_impl (CORBA::dk_AbstractInterface),
    Container_impl    (mycontainer, myrepository),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    InterfaceDef_impl (mycontainer, myrepository, id, name, version)
{
  _type = CORBA::TypeCode::create_abstract_interface_tc( id, name );
}

//-- LocalInterfaceDef ----------------------------------------------------

LocalInterfaceDef_impl::
LocalInterfaceDef_impl( Container_impl * mycontainer,
			Repository_impl * myrepository,
			const char* id,
			const char* name,
			const char* version )
  : IRObject_impl (CORBA::dk_LocalInterface),
    Container_impl    (mycontainer, myrepository),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    InterfaceDef_impl (mycontainer, myrepository, id, name, version)
{
  _type = CORBA::TypeCode::create_local_interface_tc( id, name );
}

//-- ValueMemberDef -------------------------------------------------------

ValueMemberDef_impl::ValueMemberDef_impl ( Container_impl * mycontainer,
					   Repository_impl * myrepository,
					   const char* id,
					   const char* name,
					   const char* version )
  : IRObject_impl( CORBA::dk_ValueMember ),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
  _type_def = CORBA::IDLType::_nil ();
}

CORBA::TypeCode_ptr
ValueMemberDef_impl::type()
{
  return _type_def->type();
}

CORBA::IDLType_ptr
ValueMemberDef_impl::type_def()
{
  return CORBA::IDLType::_duplicate (_type_def);
}

void
ValueMemberDef_impl::type_def( CORBA::IDLType_ptr _new_value )
{
  _type_def = CORBA::IDLType::_duplicate (_new_value);
}

CORBA::Visibility
ValueMemberDef_impl::access()
{
  return _access;
}

void
ValueMemberDef_impl::access( CORBA::Visibility _new_value )
{
  _access = _new_value;
}

CORBA::Contained::Description *
ValueMemberDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ValueMember d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.type        = _type_def->type ();
  d.type_def    = _type_def;
  d.access      = _access;

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

//-- ValueDef -------------------------------------------------------

ValueDef_impl::ValueDef_impl ( Container_impl * mycontainer,
			       Repository_impl * myrepository,
			       const char* id,
			       const char* name,
			       const char* version )
  : IRObject_impl( CORBA::dk_Value ),
    Container_impl (mycontainer, myrepository),
    Contained_impl (mycontainer, myrepository, id, name, version)
{
  _typedirty = 1;
  _visited = 0;
}

CORBA::InterfaceDefSeq*
ValueDef_impl::supported_interfaces()
{
  return new CORBA::InterfaceDefSeq (_supported_interfaces);
}

void
ValueDef_impl::supported_interfaces( const CORBA::InterfaceDefSeq& _new_value )
{
  bool supports_concrete = false;

  for (CORBA::ULong i=0; i<_new_value.length(); i++) {
    if (_new_value[i]->def_kind() == CORBA::dk_Interface) {
      if (supports_concrete) {
	/*
	 * Attempt to let a ValueDef support more than one non-abstract if
	 */
	mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 12,
				      CORBA::COMPLETED_NO));
      }
      supports_concrete = true;
    }
  }

  _supported_interfaces = _new_value;
}

CORBA::ExtInitializerSeq*
ValueDef_impl::ext_initializers()
{
  return new CORBA::ExtInitializerSeq (_initializers);
}

void
ValueDef_impl::ext_initializers( const CORBA::ExtInitializerSeq& _new_value )
{
  _initializers = _new_value;
  for (CORBA::ULong i = 0; i < _initializers.length(); ++i) {
    for (CORBA::ULong j = 0; j < _initializers[i].members.length(); ++j) {
      _initializers[i].members[j].type =
	_initializers[i].members[j].type_def->type();
    }
  }
}

CORBA::InitializerSeq*
ValueDef_impl::initializers()
{
  CORBA::InitializerSeq * is = new CORBA::InitializerSeq;
  is->length (_initializers.length());

  for (CORBA::ULong i=0; i<_initializers.length(); i++) {
    (*is)[i].name = _initializers[i].name;
    (*is)[i].members = _initializers[i].members;
  }

  return is;
}

void
ValueDef_impl::initializers( const CORBA::InitializerSeq& _new_value )
{
  CORBA::ExtInitializerSeq eis;
  eis.length (_new_value.length());

  for (CORBA::ULong i=0; i<_new_value.length(); i++) {
    eis[i].name = _new_value[i].name;
    eis[i].members = _new_value[i].members;
  }

  ext_initializers (eis);
}

CORBA::ValueDef_ptr
ValueDef_impl::base_value()
{
  return CORBA::ValueDef::_duplicate (_base_value);
}

void
ValueDef_impl::base_value( CORBA::ValueDef_ptr _new_value )
{
  _typedirty = 1;
  _base_value = CORBA::ValueDef::_duplicate (_new_value);
}

CORBA::ValueDefSeq*
ValueDef_impl::abstract_base_values()
{
  return new CORBA::ValueDefSeq (_abstract_base_values);
}

void
ValueDef_impl::abstract_base_values( const CORBA::ValueDefSeq& _new_value )
{
  _abstract_base_values = _new_value;
}

CORBA::Boolean
ValueDef_impl::is_abstract()
{
  return _is_abstract;
}

void
ValueDef_impl::is_abstract( CORBA::Boolean _new_value )
{
  _typedirty = 1;
  _is_abstract = _new_value;
}

CORBA::Boolean
ValueDef_impl::is_custom()
{
  return _is_custom;
}

void
ValueDef_impl::is_custom( CORBA::Boolean _new_value )
{
  _typedirty = 1;
  _is_custom = _new_value;
}

CORBA::Boolean
ValueDef_impl::is_truncatable()
{
  return _is_truncatable;
}

void
ValueDef_impl::is_truncatable( CORBA::Boolean _new_value )
{
  _typedirty = 1;
  _is_truncatable = _new_value;
}

CORBA::Boolean
ValueDef_impl::is_a( const char* value_id )
{
  if( strcmp( _id, value_id ) == 0 )
    return TRUE;

  if( strcmp( "IDL:omg.org/CORBA/ValueBase:1.0", value_id ) == 0 )
    return TRUE;
   
  for( CORBA::ULong i = 0; i < _abstract_base_values.length(); i++ ) {
    if( _abstract_base_values[i]->is_a( value_id ) )
      return TRUE;
  }
  for( CORBA::ULong j = 0; j < _supported_interfaces.length(); j++ ) {
    if( _supported_interfaces[j]->is_a( value_id ) )
      return TRUE;
  }
  if( !CORBA::is_nil( _base_value ) &&
      _base_value->is_a( value_id ) )
    return TRUE;

  return FALSE;
}

CORBA::ValueDef::FullValueDescription*
ValueDef_impl::describe_value()
{
  CORBA::ValueDef::FullValueDescription *desc =
    new CORBA::ValueDef::FullValueDescription;
  
  desc->name = _name;
  desc->id = _id;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  if( !CORBA::is_nil( def_in ) )
    desc->defined_in = def_in->id();
  else
    desc->defined_in = (const char *) "";
  
  desc->version = _version;
  desc->is_abstract = _is_abstract;
  desc->is_custom = _is_custom;
  desc->is_truncatable = _is_truncatable;

  desc->abstract_base_values.length( _abstract_base_values.length() );
  for( CORBA::ULong j0 = 0; j0 < _abstract_base_values.length(); j0++ )
    desc->abstract_base_values[j0] = _abstract_base_values[j0]->id();

  desc->supported_interfaces.length( _supported_interfaces.length() );
  for( CORBA::ULong j1 = 0; j1 < _supported_interfaces.length(); j1++ )
    desc->supported_interfaces[j1] = _supported_interfaces[j1]->id();

  if (CORBA::is_nil (_base_value))
    desc->base_value = (const char *)"";
  else
    desc->base_value = _base_value->id();

  desc->type = type();

  CORBA::InitializerSeq_var inits = initializers ();
  desc->initializers = inits.in();

  // operations, attributes, members

  CORBA::ContainedSeq_var csv = contents (CORBA::dk_Operation, FALSE);
  desc->operations.length (csv->length ());
  for (CORBA::ULong i0=0; i0<csv->length(); i0++) {
    CORBA::Contained::Description_var d = csv[i0]->describe ();
    const CORBA::OperationDescription * od;
    assert (d->kind == CORBA::dk_Operation);
    CORBA::Boolean r = (d->value >>= od);
    assert (r);
    desc->operations[i0] = *od;
  }

  csv = contents (CORBA::dk_Attribute, FALSE);
  desc->attributes.length (csv->length());
  for (CORBA::ULong i1=0; i1<csv->length(); i1++) {
    CORBA::Contained::Description_var d = csv[i1]->describe ();
    const CORBA::AttributeDescription * ad;
    assert (d->kind == CORBA::dk_Attribute);
    CORBA::Boolean r = (d->value >>= ad);
    assert (r);
    desc->attributes[i1] = *ad;
  }

  csv = contents (CORBA::dk_ValueMember, FALSE);
  desc->members.length (csv->length());
  for (CORBA::ULong i2=0; i2<csv->length(); i2++) {
    CORBA::Contained::Description_var d = csv[i2]->describe ();
    const CORBA::ValueMember * vd;
    assert (d->kind == CORBA::dk_ValueMember);
    CORBA::Boolean r = (d->value >>= vd);
    assert (r);
    desc->members[i2] = *vd;
  }

  return desc;
}

CORBA::ExtValueDef::ExtFullValueDescription*
ValueDef_impl::describe_ext_value()
{
  CORBA::ExtValueDef::ExtFullValueDescription *desc =
    new CORBA::ExtValueDef::ExtFullValueDescription;
  
  desc->name = _name;
  desc->id = _id;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  if( !CORBA::is_nil( def_in ) )
    desc->defined_in = def_in->id();
  else
    desc->defined_in = (const char *) "";
  
  desc->version = _version;
  desc->is_abstract = _is_abstract;
  desc->is_custom = _is_custom;
  desc->is_truncatable = _is_truncatable;

  desc->abstract_base_values.length( _abstract_base_values.length() );
  for( CORBA::ULong j0 = 0; j0 < _abstract_base_values.length(); j0++ )
    desc->abstract_base_values[j0] = _abstract_base_values[j0]->id();

  desc->supported_interfaces.length( _supported_interfaces.length() );
  for( CORBA::ULong j1 = 0; j1 < _supported_interfaces.length(); j1++ )
    desc->supported_interfaces[j1] = _supported_interfaces[j1]->id();

  if (CORBA::is_nil (_base_value))
    desc->base_value = (const char *)"";
  else
    desc->base_value = _base_value->id();

  desc->type = type();

  desc->initializers = _initializers;

  // operations, attributes, members

  CORBA::ContainedSeq_var csv = contents (CORBA::dk_Operation, FALSE);
  desc->operations.length (csv->length ());
  for (CORBA::ULong i0=0; i0<csv->length(); i0++) {
    CORBA::Contained::Description_var d = csv[i0]->describe ();
    const CORBA::OperationDescription * od;
    assert (d->kind == CORBA::dk_Operation);
    CORBA::Boolean r = (d->value >>= od);
    assert (r);
    desc->operations[i0] = *od;
  }

  csv = contents (CORBA::dk_Attribute, FALSE);
  desc->attributes.length (csv->length());
  desc->attributes.length (csv->length());
  for (CORBA::ULong i1=0; i1<csv->length(); i1++) {
    CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (csv[i1]);
    CORBA::ExtAttributeDescription_var ad = ead->describe_attribute ();
    desc->attributes[i1] = ad.in();
  }

  csv = contents (CORBA::dk_ValueMember, FALSE);
  desc->members.length (csv->length());
  for (CORBA::ULong i2=0; i2<csv->length(); i2++) {
    CORBA::Contained::Description_var d = csv[i2]->describe ();
    const CORBA::ValueMember * vd;
    assert (d->kind == CORBA::dk_ValueMember);
    CORBA::Boolean r = (d->value >>= vd);
    assert (r);
    desc->members[i2] = *vd;
  }

  return desc;
}


CORBA::ValueMemberDef_ptr
ValueDef_impl::create_value_member( const char* id,
				    const char* name,
				    const char* version,
				    CORBA::IDLType_ptr type,
				    CORBA::Visibility accessval )
{
  CORBA::ContainedSeq_var cs;

  /*
   * We may overload inherited types, but not inherited operations
   * or attributes. Redefinition of types in the current scope fails
   * in insert_contained below.
   */

  cs = lookup_name (name, 1, CORBA::dk_Operation, 0);

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_Attribute, 0);
  }

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_ValueMember, 0);
  }

  if (cs->length()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				  CORBA::COMPLETED_NO));
  }

  ValueMemberDef_impl *memi =
    new ValueMemberDef_impl (this,
			     Contained_impl::_myrepository,
			     id, name, version);

  try {
    memi->type_def (type);
    memi->access (accessval);
    insert_contained (memi, id, name);
  }
  catch (...) {
    delete memi;
    throw;
  }

  _typedirty = 1;

  CORBA::ValueMemberDef_ptr mem = memi->_this ();
  return mem;
}


CORBA::AttributeDef_ptr
ValueDef_impl::create_attribute( const char* id,
				 const char* name,
				 const char* version,
				 CORBA::IDLType_ptr type,
				 CORBA::AttributeMode modeval )
{
  return create_ext_attribute (id, name, version, type, modeval,
			       CORBA::ExceptionDefSeq(0),
			       CORBA::ExceptionDefSeq(0));
}

CORBA::ExtAttributeDef_ptr
ValueDef_impl::create_ext_attribute (const char* id,
				     const char* name,
				     const char* version,
				     CORBA::IDLType_ptr type,
				     CORBA::AttributeMode modeval,
				     const CORBA::ExceptionDefSeq & ge,
				     const CORBA::ExceptionDefSeq & se)
{
  CORBA::ContainedSeq_var cs;

  /*
   * We may overload inherited types, but not inherited operations
   * or attributes. Redefinition of types in the current scope fails
   * in insert_contained below.
   */

  cs = lookup_name (name, 1, CORBA::dk_Operation, 0);

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_Attribute, 0);
  }

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_ValueMember, 0);
  }

  if (cs->length()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				  CORBA::COMPLETED_NO));
  }

  AttributeDef_impl * opi =
    new AttributeDef_impl (this,
			   Contained_impl::_myrepository,
			   id, name, version);

  try {
    opi->type_def (type);
    opi->mode (modeval);
    opi->get_exceptions (ge);
    opi->set_exceptions (se);
    insert_contained (opi, id, name);
  }
  catch (...) {
    delete opi;
    throw;
  }

  CORBA::ExtAttributeDef_ptr op = opi->_this ();
  opi->_remove_ref ();
  return op;
}


CORBA::OperationDef_ptr
ValueDef_impl::create_operation( const char* id,
				 const char* name,
				 const char* version,
				 CORBA::IDLType_ptr result,
				 CORBA::OperationMode opmode,
				 const CORBA::ParDescriptionSeq& opparams,
				 const CORBA::ExceptionDefSeq& exceptions,
				 const CORBA::ContextIdSeq& ctxs )
{
  CORBA::ContainedSeq_var cs;

  /*
   * We may overload inherited types, but not inherited operations
   * or attributes. Redefinition of types in the current scope fails
   * in insert_contained below.
   */

  cs = lookup_name (name, 1, CORBA::dk_Operation, 0);

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_Attribute, 0);
  }

  if (!cs->length()) {
    cs = lookup_name (name, 1, CORBA::dk_ValueMember, 0);
  }

  if (cs->length()) {
    mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				  CORBA::COMPLETED_NO));
  }

  if( opmode == CORBA::OP_ONEWAY ) {
    // Check that oneway operations have no result, no out, inout parameters
    // and no raises expressions
    CORBA::TypeCode_var result_tc = result->type();
    if( result_tc->kind() != CORBA::tk_void )
      mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                     CORBA::COMPLETED_NO ) );
    if( exceptions.length() != 0 )
      mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                     CORBA::COMPLETED_NO ) );
    for( CORBA::ULong i = 0; i < opparams.length(); i++ )
      if( opparams[ i ].mode != CORBA::PARAM_IN )
	mico_throw( CORBA::INTF_REPOS( MICO_IR_EXC_BAD_ONEWAY,
                                       CORBA::COMPLETED_NO ) );
  }

  OperationDef_impl * opi =
    new OperationDef_impl (this,
			   Contained_impl::_myrepository,
			   id, name, version);

  try {
    opi->result_def (result);
    opi->params (opparams);
    opi->mode (opmode);
    opi->contexts (ctxs);
    opi->exceptions (exceptions);
    insert_contained (opi, id, name);
  }
  catch (...) {
    delete opi;
    throw;
  }

  CORBA::OperationDef_ptr op = opi->_this ();
  opi->_remove_ref ();
  return op;
}

CORBA::TypeCode_ptr
ValueDef_impl::recursive_type ()
{
  if (_visited) {
    return CORBA::TypeCode::create_recursive_tc (_id);
  }

  _visited = 1;

  CORBA::TypeCode_var base_type;

  if (CORBA::is_nil (_base_value)) {
    base_type = CORBA::TypeCode::_nil ();
  }
  else {
    base_type = _base_value->type ();
  }

  CORBA::ValueModifier modifier = CORBA::VM_NONE;
  // only one modifier allowed
  assert (!!_is_abstract + !!_is_custom + !!_is_truncatable <= 1);
  if (_is_abstract)
    modifier = CORBA::VM_ABSTRACT;
  if (_is_custom)
    modifier = CORBA::VM_CUSTOM;
  if (_is_truncatable)
    modifier = CORBA::VM_TRUNCATABLE;

  CORBA::ValueMemberSeq members;
  NameMap::iterator it;
  CORBA::ULong j=0;

  for (it = _names.begin(); it != _names.end(); it++) {
    if (!(*it).value || (*it).value->def_kind() != CORBA::dk_ValueMember) {
      continue;
    }

    ValueMemberDef_impl * mdi =
      dynamic_cast<ValueMemberDef_impl *> ((*it).value);
    assert (mdi);

    CORBA::IDLType_var idt = mdi->type_def ();
    PortableServer::ServantBase_var serv =
      IRObject_impl::_ifrpoa->reference_to_servant (idt);
    IDLType_impl * idi = dynamic_cast<IDLType_impl *> (serv.in());
    assert (idi);

    members.length (j+1);
    members[j].type = idi->recursive_type ();
    members[j].name = mdi->name();
    members[j].id = mdi->id();
    members[j].access = mdi->access();
    j++;
  }

  _visited = 0;

  return CORBA::TypeCode::create_value_tc (_id, _name, modifier,
					   base_type, members);
}

CORBA::TypeCode_ptr
ValueDef_impl::type()
{
  if (_visited) {
    return recursive_type ();
  }

  if (_typedirty) {
    _type = recursive_type ();
    _typedirty = 0;
  }

  return CORBA::TypeCode::_duplicate (_type);
}


CORBA::Contained::Description *
ValueDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ValueDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.is_abstract = _is_abstract;
  d.is_custom   = _is_custom;
  d.is_truncatable = _is_truncatable;

  d.supported_interfaces.length (_supported_interfaces.length());
  for (CORBA::ULong i0=0; i0<_supported_interfaces.length(); i0++) {
    d.supported_interfaces[i0] = _supported_interfaces[i0]->id();
  }

  d.abstract_base_values.length (_abstract_base_values.length());
  for (CORBA::ULong i1=0; i1<_abstract_base_values.length(); i1++) {
    d.abstract_base_values[i1] = _abstract_base_values[i1]->id();
  }

  if (CORBA::is_nil (_base_value)) {
    d.base_value = (const char *) "";
  }
  else {
    d.base_value = _base_value->id();
  }

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

void
ValueDef_impl::deactivate ()
{
  Contained_impl::deactivate ();
  Container_impl::deactivate ();
}

//-- ValueBoxDef -------------------------------------------------------

ValueBoxDef_impl::ValueBoxDef_impl ( Container_impl * mycontainer,
				     Repository_impl * myrepository,
				     const char* id,
				     const char* name,
				     const char* version )
  : IRObject_impl (CORBA::dk_ValueBox),
    Contained_impl (mycontainer, myrepository, id, name, version),
    TypedefDef_impl(mycontainer, myrepository, id, name, version)
{
  _original_type_def = CORBA::IDLType::_nil ();
  _typedirty = 1;
}

CORBA::IDLType_ptr
ValueBoxDef_impl::original_type_def()
{
  return CORBA::IDLType::_duplicate (_original_type_def);
}

void
ValueBoxDef_impl::original_type_def( CORBA::IDLType_ptr _new_value )
{
  _typedirty = 1;
  _original_type_def = CORBA::IDLType::_duplicate (_new_value);
}

CORBA::TypeCode_ptr
ValueBoxDef_impl::type()
{
  if (_typedirty) {
    CORBA::TypeCode_var original_type = _original_type_def->type ();
    _type = CORBA::TypeCode::create_value_box_tc (_id, _name, original_type);
  }

  _typedirty = 0;

  return CORBA::TypeCode::_duplicate (_type);
}

/*
 * ----------------------------------------------------------------------
 * ComponentIR
 * ----------------------------------------------------------------------
 */

#ifdef USE_CCM

/*
 * ProvidesDef
 */

ProvidesDef_impl::ProvidesDef_impl (Container_impl * mycontainer,
				    Repository_impl * myrepository,
				    const char * id,
				    const char * name,
				    const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_Provides)
{
  _interface_type = CORBA::InterfaceDef::_nil ();
}

CORBA::InterfaceDef_ptr
ProvidesDef_impl::interface_type ()
{
  return CORBA::InterfaceDef::_duplicate (_interface_type);
}

void
ProvidesDef_impl::interface_type (CORBA::InterfaceDef_ptr the_interface)
{
  _interface_type = CORBA::InterfaceDef::_duplicate (the_interface);
}

CORBA::Contained::Description *
ProvidesDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ComponentIR::ProvidesDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.interface_type = _interface_type->id();

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

/*
 * UsesDef
 */

UsesDef_impl::UsesDef_impl (Container_impl * mycontainer,
			    Repository_impl * myrepository,
			    const char * id,
			    const char * name,
			    const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_Uses)
{
  _interface_type = CORBA::InterfaceDef::_nil ();
}

CORBA::InterfaceDef_ptr
UsesDef_impl::interface_type ()
{
  return CORBA::InterfaceDef::_duplicate (_interface_type);
}

void
UsesDef_impl::interface_type (CORBA::InterfaceDef_ptr the_interface)
{
  _interface_type = CORBA::InterfaceDef::_duplicate (the_interface);
}

CORBA::Boolean
UsesDef_impl::is_multiple ()
{
  return _is_multiple;
}

void
UsesDef_impl::is_multiple (CORBA::Boolean the_multiple)
{
  _is_multiple = the_multiple;
}

CORBA::Contained::Description *
UsesDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ComponentIR::UsesDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.interface_type = _interface_type->id();
  d.is_multiple = _is_multiple;

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

/*
 * EventDef
 */

EventDef_impl::EventDef_impl (Container_impl * mycontainer,
			      Repository_impl * myrepository,
			      const char * id,
			      const char * name,
			      const char * version)
  : ValueDef_impl  (mycontainer, myrepository, id, name, version),
    Contained_impl (mycontainer, myrepository, id, name, version),
    Container_impl (mycontainer, myrepository),
    IRObject_impl (CORBA::dk_Event)
{
}

/*
 * EventPortDef
 */

EventPortDef_impl::EventPortDef_impl (Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char * id,
				      const char * name,
				      const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_none)
{
  _event = CORBA::ComponentIR::EventDef::_nil ();
}

CORBA::Boolean
EventPortDef_impl::is_a (const char * event_id)
{
  return _event->is_a (event_id);
}

CORBA::ComponentIR::EventDef_ptr
EventPortDef_impl::event ()
{
  return CORBA::ComponentIR::EventDef::_duplicate (_event);
}

void
EventPortDef_impl::event (CORBA::ComponentIR::EventDef_ptr the_event)
{
  _event = CORBA::ComponentIR::EventDef::_duplicate (the_event);
}

CORBA::Contained::Description *
EventPortDef_impl::describe ()
{
  CORBA::Contained::Description * desc = new CORBA::Contained::Description;

  CORBA::Container_var cdef_in = defined_in();
  CORBA::Contained_var def_in = CORBA::Contained::_narrow (cdef_in);
  CORBA::String_var def_in_id;

  if (!CORBA::is_nil (def_in)) {
    def_in_id = def_in->id();
  }
  else {
    def_in_id = (const char *) "";
  }

  CORBA::ComponentIR::EventPortDescription d;
  d.name        = _name;
  d.id          = _id;
  d.defined_in  = def_in_id;
  d.version     = _version;
  d.event       = _event->id();

  desc->kind    = _dk;
  desc->value <<= d;

  return desc;
}

/*
 * EmitsDef
 */

EmitsDef_impl::EmitsDef_impl (Container_impl * mycontainer,
			      Repository_impl * myrepository,
			      const char * id,
			      const char * name,
			      const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    EventPortDef_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_Emits)
{
}

/*
 * PublishesDef
 */

PublishesDef_impl::PublishesDef_impl (Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char * id,
				      const char * name,
				      const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    EventPortDef_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_Publishes)
{
}

/*
 * Consumes
 */

ConsumesDef_impl::ConsumesDef_impl (Container_impl * mycontainer,
				      Repository_impl * myrepository,
				      const char * id,
				      const char * name,
				      const char * version)
  : Contained_impl (mycontainer, myrepository, id, name, version),
    EventPortDef_impl (mycontainer, myrepository, id, name, version),
    IRObject_impl (CORBA::dk_Consumes)
{
}

/*
 * Component
 */

ComponentDef_impl::
ComponentDef_impl (Container_impl * mycontainer,
		   Repository_impl * myrepository,
		   const char * id,
		   const char * name,
		   const char * version)
  : InterfaceDef_impl (mycontainer, myrepository, id, name, version),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    Container_impl    (mycontainer, myrepository),
    IRObject_impl     (CORBA::dk_Component)
{
  _base_component = CORBA::ComponentIR::ComponentDef::_nil ();
}

CORBA::InterfaceDefSeq *
ComponentDef_impl::
supported_interfaces ()
{
  return new CORBA::InterfaceDefSeq (_supported_interfaces);
}

void
ComponentDef_impl::
supported_interfaces (const CORBA::InterfaceDefSeq & ns)
{
  _supported_interfaces = ns;
}

CORBA::ComponentIR::ComponentDef_ptr
ComponentDef_impl::base_component ()
{
  return CORBA::ComponentIR::ComponentDef::_duplicate (_base_component);
}

void
ComponentDef_impl::
base_component (CORBA::ComponentIR::ComponentDef_ptr the_base)
{
  _base_component = CORBA::ComponentIR::ComponentDef::_duplicate (the_base);
}

CORBA::ComponentIR::ProvidesDef_ptr
ComponentDef_impl::
create_provides (const char * id,
		 const char * name,
		 const char * version,
		 CORBA::InterfaceDef_ptr interface_type)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  ProvidesDef_impl * pi =
    new ProvidesDef_impl (this,
			  Contained_impl::_myrepository,
			  id, name, version);

  try {
    pi->interface_type (interface_type);
    insert_contained (pi, id, name);
  }
  catch (...) {
    delete pi;
    throw;
  }

  CORBA::ComponentIR::ProvidesDef_ptr p = pi->_this ();
  pi->_remove_ref ();
  return p;
}

CORBA::ComponentIR::UsesDef_ptr
ComponentDef_impl::
create_uses (const char * id,
	     const char * name,
	     const char * version,
	     CORBA::InterfaceDef_ptr interface_type,
	     CORBA::Boolean is_multiple)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  UsesDef_impl * ui =
    new UsesDef_impl (this,
		      Contained_impl::_myrepository,
		      id, name, version);

  try {
    ui->interface_type (interface_type);
    ui->is_multiple (is_multiple);
    insert_contained (ui, id, name);
  }
  catch (...) {
    delete ui;
    throw;
  }

  CORBA::ComponentIR::UsesDef_ptr u = ui->_this ();
  ui->_remove_ref ();
  return u;
}

CORBA::ComponentIR::EmitsDef_ptr
ComponentDef_impl::
create_emits (const char * id,
	      const char * name,
	      const char * version,
	      CORBA::ComponentIR::EventDef_ptr event)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  EmitsDef_impl * ei =
    new EmitsDef_impl (this,
		       Contained_impl::_myrepository,
		       id, name, version);

  try {
    ei->event (event);
    insert_contained (ei, id, name);
  }
  catch (...) {
    delete ei;
    throw;
  }

  CORBA::ComponentIR::EmitsDef_ptr e = ei->_this ();
  ei->_remove_ref ();
  return e;
}

CORBA::ComponentIR::PublishesDef_ptr
ComponentDef_impl::
create_publishes (const char * id,
		  const char * name,
		  const char * version,
		  CORBA::ComponentIR::EventDef_ptr event)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  PublishesDef_impl * pi =
    new PublishesDef_impl (this,
			   Contained_impl::_myrepository,
			   id, name, version);

  try {
    pi->event (event);
    insert_contained (pi, id, name);
  }
  catch (...) {
    delete pi;
    throw;
  }

  CORBA::ComponentIR::PublishesDef_ptr p = pi->_this ();
  pi->_remove_ref ();
  return p;
}

CORBA::ComponentIR::ConsumesDef_ptr
ComponentDef_impl::
create_consumes (const char * id,
		 const char * name,
		 const char * version,
		 CORBA::ComponentIR::EventDef_ptr event)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Provides  || dk == CORBA::dk_Uses      ||
	dk == CORBA::dk_Emits     || dk == CORBA::dk_Publishes ||
	dk == CORBA::dk_Consumes) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  ConsumesDef_impl * ci =
    new ConsumesDef_impl (this,
			  Contained_impl::_myrepository,
			  id, name, version);

  try {
    ci->event (event);
    insert_contained (ci, id, name);
  }
  catch (...) {
    delete ci;
    throw;
  }

  CORBA::ComponentIR::ConsumesDef_ptr c = ci->_this ();
  ci->_remove_ref ();
  return c;
}

CORBA::Contained::Description *
ComponentDef_impl::describe ()
{
  assert (0);
  return 0;
}

/*
 * Factory
 */

FactoryDef_impl::
FactoryDef_impl (Container_impl * mycontainer,
		 Repository_impl * myrepository,
		 const char * id,
		 const char * name,
		 const char * version)
  : OperationDef_impl (mycontainer, myrepository, id, name, version),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    IRObject_impl     (CORBA::dk_Factory)
{
}

FinderDef_impl::
FinderDef_impl (Container_impl * mycontainer,
		Repository_impl * myrepository,
		const char * id,
		const char * name,
		const char * version)
  : OperationDef_impl (mycontainer, myrepository, id, name, version),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    IRObject_impl     (CORBA::dk_Finder)
{
}

/*
 * Home
 */

HomeDef_impl::
HomeDef_impl (Container_impl * mycontainer,
	      Repository_impl * myrepository,
	      const char * id,
	      const char * name,
	      const char * version)
  : InterfaceDef_impl (mycontainer, myrepository, id, name, version),
    Contained_impl    (mycontainer, myrepository, id, name, version),
    Container_impl    (mycontainer, myrepository),
    IRObject_impl     (CORBA::dk_Home)
{
  _base_home = CORBA::ComponentIR::HomeDef::_nil ();
  _managed_component = CORBA::ComponentIR::ComponentDef::_nil ();
  _primary_key = CORBA::ValueDef::_nil ();
}

CORBA::ComponentIR::HomeDef_ptr
HomeDef_impl::base_home ()
{
  return CORBA::ComponentIR::HomeDef::_duplicate (_base_home);
}

void
HomeDef_impl::base_home (CORBA::ComponentIR::HomeDef_ptr the_base)
{
  _base_home = CORBA::ComponentIR::HomeDef::_duplicate (the_base);
}

CORBA::InterfaceDefSeq *
HomeDef_impl::supported_interfaces ()
{
  return new CORBA::InterfaceDefSeq (_supported_interfaces);
}

void
HomeDef_impl::supported_interfaces (const CORBA::InterfaceDefSeq & ns)
{
  _supported_interfaces = ns;
}

CORBA::ComponentIR::ComponentDef_ptr
HomeDef_impl::managed_component ()
{
  return CORBA::ComponentIR::ComponentDef::_duplicate (_managed_component);
}

void
HomeDef_impl::managed_component (CORBA::ComponentIR::ComponentDef_ptr mc)
{
  _managed_component = CORBA::ComponentIR::ComponentDef::_duplicate (mc);
}

CORBA::ValueDef_ptr
HomeDef_impl::primary_key ()
{
  return CORBA::ValueDef::_duplicate (_primary_key);
}

void
HomeDef_impl::primary_key (CORBA::ValueDef_ptr the_key)
{
  _primary_key = CORBA::ValueDef::_duplicate (the_key);
}

CORBA::ComponentIR::FactoryDef_ptr
HomeDef_impl::
create_factory (const char * id,
		const char * name,
		const char * version,
		const CORBA::ParDescriptionSeq & params,
		const CORBA::ExceptionDefSeq & exceptions)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Factory   || dk == CORBA::dk_Finder) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  FactoryDef_impl * fi =
    new FactoryDef_impl (this,
			 Contained_impl::_myrepository,
			 id, name, version);

  try {
    fi->params (params);
    fi->exceptions (exceptions);
    fi->result_def (_managed_component);
    fi->mode (CORBA::OP_NORMAL);
    insert_contained (fi, id, name);
  }
  catch (...) {
    delete fi;
    throw;
  }

  CORBA::ComponentIR::FactoryDef_ptr f = fi->_this ();
  fi->_remove_ref ();
  return f;
}

CORBA::ComponentIR::FinderDef_ptr
HomeDef_impl::
create_finder (const char * id,
	       const char * name,
	       const char * version,
	       const CORBA::ParDescriptionSeq & params,
	       const CORBA::ExceptionDefSeq & exceptions)
{
  CORBA::ContainedSeq_var cs = lookup_name (name, 1, CORBA::dk_all, 0);

  for (CORBA::ULong i=0; i<cs->length(); i++) {
    CORBA::DefinitionKind dk = cs[i]->def_kind ();
    if (dk == CORBA::dk_Attribute || dk == CORBA::dk_Operation ||
	dk == CORBA::dk_Factory   || dk == CORBA::dk_Finder) {
      mico_throw (CORBA::BAD_PARAM (CORBA::OMGVMCID | 3,
				    CORBA::COMPLETED_NO));
    }
  }

  FinderDef_impl * fi =
    new FinderDef_impl (this,
			Contained_impl::_myrepository,
			id, name, version);

  try {
    fi->params (params);
    fi->exceptions (exceptions);
    fi->result_def (_managed_component);
    fi->mode (CORBA::OP_NORMAL);
    insert_contained (fi, id, name);
  }
  catch (...) {
    delete fi;
    throw;
  }

  CORBA::ComponentIR::FinderDef_ptr f = fi->_this ();
  fi->_remove_ref ();
  return f;
}

CORBA::Contained::Description *
HomeDef_impl::describe ()
{
  assert (0);
  return 0;
}

#endif // USE_CCM

/*
 * ----------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------
 */

/*
 * Create a new Interface Repository
 *
 * We create a POA that is shared by all Interface Repositories. The POA
 * uses a fresh POA Manager that is immediately activated, so that it can
 * serve requests even if the RootPOA is not active.
 *
 * If the "persistent" parameter is true, then we create another persistent
 * POA to hold the Repository itself, so that the Interface Repository will
 * have a persistent reference. This is used in the IRD program (see main),
 * and shouldn't be used anywhere else.
 *
 * Yet to do is making the whole IFR's contents persistent.
 *
 * We use the single-thread model, because at least recursive typecode
 * computation must be serialized.
 */

CORBA::Repository *
MICO::create_interface_repository (CORBA::ORB_ptr orb,
				   CORBA::Boolean persistent)
{
  CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var rootpoa = PortableServer::POA::_narrow (obj);

  /*
   * Create POA for all contents of the IFR
   */

  CORBA::PolicyList pl;
  pl.length (2);
  pl[0] = rootpoa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL);
  pl[1] = rootpoa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  PortableServer::POA_var ifrpoa;

  try {
    ifrpoa = rootpoa->create_POA ("_MICO_IFR",
				  PortableServer::POAManager::_nil(),
				  pl);
  }
  catch (PortableServer::POA::AdapterAlreadyExists &) {
    ifrpoa = rootpoa->find_POA ("_MICO_IFR", 0);
  }

  PortableServer::POAManager_var manager = ifrpoa->the_POAManager ();
  IRObject_impl::_ifrpoa = PortableServer::POA::_duplicate (ifrpoa);

  /*
   * If a persistent IFR reference is requested, create another POA,
   * else use the above.
   */

  Repository_impl * repo = new Repository_impl ();
  CORBA::Repository_ptr r;

  if (persistent) {
    pl.length (3);
    pl[1] = rootpoa->create_lifespan_policy (PortableServer::PERSISTENT);
    pl[2] = rootpoa->create_id_assignment_policy (PortableServer::USER_ID);

    PortableServer::POA_var repopoa =
      rootpoa->create_POA ("InterfaceRepository", manager, pl);

    PortableServer::ObjectId_var repooid =
      PortableServer::string_to_ObjectId ("InterfaceRepository");

    repopoa->activate_object_with_id (repooid.in(), repo);
    obj = repopoa->id_to_reference (repooid.in());
    r = CORBA::Repository::_narrow (obj);
  }
  else {
    r = repo->_this ();
  }

  repo->_remove_ref ();
  manager->activate ();

  return r;
}

