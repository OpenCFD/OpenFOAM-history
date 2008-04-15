/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <mico/util.h>
#include <mico/template_impl.h>
#include "parsenode.h"
#include "db.h"

#endif // FAST_PCH


using namespace std;


DB::PseudoRepoId::PseudoRepoId()
{
}

DB::PseudoRepoId::~PseudoRepoId()
{
}

void DB::PseudoRepoId::set_full_id( string str )
{
    _full_id = str;
}

string DB::PseudoRepoId::get_full_id()
{
    return _full_id;
}

void DB::PseudoRepoId::add_prefix( string str )
{
    _full_id = str + _full_id;
    _prefix = str + _prefix;
}

string DB::PseudoRepoId::get_prefix()
{
    return _prefix;
}

void DB::PseudoRepoId::add_suffix( string str )
{
    _full_id = _full_id + str;
    _suffix = _suffix + str;
}

string DB::PseudoRepoId::get_suffix()
{
    return _suffix;
}

void DB::PseudoRepoId::set_contained_id( string str )
{
    _contained_id = _full_id = str;
}

string DB::PseudoRepoId::get_contained_id()
{
    return _contained_id;
}

bool DB::PseudoRepoId::contains_id()
{
    return (_contained_id != "");
}


DB::DB()
{
  vector<string> empty_scope;
  _current_scope.push_back (empty_scope);

  string empty;
  _current_prefix.push_back( empty );
}

DB::~DB()
{
}

/*
 * Generate a pseudo repoid for an IRObject that is not a
 * Contained (like SequenceDef)
 */
void DB::gen_pseudo_repoid( CORBA::IDLType_ptr t, PseudoRepoId &pseudo_id )
{
  string s;

  CORBA::Contained_var con = CORBA::Contained::_narrow( t );
  if( !CORBA::is_nil( con ) ) {
    CORBA::String_var tmp = con->id();
    s = tmp;
    pseudo_id.set_contained_id( s );
    return;
  }
  
  CORBA::StringDef_var str = CORBA::StringDef::_narrow( t );
  if( !CORBA::is_nil( str ) ) {
    s = "string_";
    s += xdec( str->bound() );
    pseudo_id.set_full_id( s );
    return;
  }

  CORBA::WstringDef_var wstr = CORBA::WstringDef::_narrow( t );
  if( !CORBA::is_nil( wstr ) ) {
    s = "wstring_";
    s += xdec( wstr->bound() );
    pseudo_id.set_full_id( s );
    return;
  }

  CORBA::SequenceDef_var seq = CORBA::SequenceDef::_narrow( t );
  if( !CORBA::is_nil( seq ) ) {
    CORBA::IDLType_var elem = seq->element_type_def();
    gen_pseudo_repoid( elem, pseudo_id );
    pseudo_id.add_prefix( "sequence_" );
    s = "_";
    s += xdec( seq->bound() );
    pseudo_id.add_suffix( s );
    return;
  }

  CORBA::FixedDef_var fix = CORBA::FixedDef::_narrow( t );
  if( !CORBA::is_nil( fix ) ) {
    s = "fixed_";
    CORBA::TypeCode_var tc = t->type();
    s += xdec( tc->fixed_digits() );
    s += '_';
    s += xdec( tc->fixed_scale() );
    pseudo_id.set_full_id( s );
    return;
  }
  
  CORBA::ArrayDef_var arr = CORBA::ArrayDef::_narrow( t );
  if( !CORBA::is_nil( arr ) ) {
    CORBA::IDLType_var elem = arr->element_type_def();
    gen_pseudo_repoid( elem, pseudo_id );
    pseudo_id.add_prefix( "array_" );
    pseudo_id.add_suffix( xdec( arr->length() ) );
    return;
  }
  
  CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( t );
  assert( !CORBA::is_nil( prim ) );
  s = "primitive_";
  s += xdec( (long) prim->kind() );
  pseudo_id.set_full_id( s );
}

/*
 * Generate a pseudo repoid for an IRObject that is not a
 * Contained (like SequenceDef)
 */
string DB::gen_pseudo_repoid( CORBA::IDLType_ptr t )
{
  PseudoRepoId pseudo_id;

  gen_pseudo_repoid( t, pseudo_id );
  return pseudo_id.get_full_id();
}

void DB::register_repoid( ParseNode* node, const string &id)
{
  _repoid_fname_map[ id ] = node->getFname();
}

void DB::register_repoid( const string &id, const string &fname )
{
  _repoid_fname_map[ id ] = fname;
}

void DB::register_anonym_type( ParseNode* node, CORBA::IDLType_ptr t )
{
  PseudoRepoId pseudo_id;
  gen_pseudo_repoid( t, pseudo_id );

  if (is_repoid_defined( pseudo_id.get_full_id() )) {
    return;
  }

  _repoid_fname_map[ pseudo_id.get_full_id() ] = node->getFname();

  if (pseudo_id.contains_id()) {
    IdToPseudoIdMap::value_type entry = 
      IdToPseudoIdMap::value_type( pseudo_id.get_contained_id(), pseudo_id );
    _id_to_pseudo_id_map.insert( entry );
  }
}

bool DB::is_repoid_defined( const string& id ) const
{
  return _repoid_fname_map.count( id ) != 0;
}

string DB::get_fname_for_repoid( const string &id )
{
  return _repoid_fname_map[ id ];
}

string DB::get_toplevel_fname () const
{
  return _toplevel_fname;
}

void DB::set_toplevel_fname (const char * fname)
{
  _toplevel_fname = fname;
}

void DB::enter_scope( const string& name )
{
  _current_scope.back().push_back( name );
  string last_prefix = _current_prefix.back();
  _current_prefix.push_back( last_prefix );
}

void DB::leave_scope()
{
  assert( _current_scope.back().size() > 0 );
  assert( _current_prefix.size() > 0 );
  _current_scope.back().pop_back();
  _current_prefix.pop_back();
}

void DB::enter_include_file ()
{
  string empty_prefix;
  _current_prefix.push_back( empty_prefix );
}

void DB::leave_include_file ()
{
  assert (_current_prefix.size() > 0);
  _current_prefix.pop_back();
}

void DB::switch_prefix( const string& name )
{
  _current_prefix.pop_back();
  _current_prefix.push_back( name );
}

string DB::get_current_scope() const
{
  string scope = "::";
  
  for (VecVecString::size_type i=0; i < _current_scope.back().size(); i++) {
    scope += (_current_scope.back())[i];
    scope += "::";
  }
  
  return scope;
}

string DB::get_current_prefix() const
{
  string prefix = _current_prefix.back();
  
  for (VecVecString::size_type i=0; i < _current_scope.back().size(); i++) {
    if (prefix.length() > 0)
      prefix += "/";
    prefix += (_current_scope.back())[i];
  }

  if (prefix.length() > 0)
    prefix += "/";
  
  return prefix;
}

void DB::set_repo_version( const string& name, const string& version )
{
  _version_name.push_back( name );
  _version.push_back( version );
}

void DB::set_repo_id( const string& name, const string& id )
{
  _id_name.push_back( name );
  _id.push_back( id );
}

void DB::set_prefix( const string& name, const string& prefix )
{
  _prefix_name.push_back( name );
  _prefix.push_back( prefix );
}

/*
 * Look for a typePrefix to use for some type and its descendants
 * Prefixes can be recursively re-defined, so we must continue to look
 * for the most specific match.
 */

string DB::get_prefix (const char * absolute_name) const
{
  CORBA::ULong longest_match=0;
  CORBA::ULong length;
  string prefix = "";

  for (vector<string>::size_type i=0; i<_prefix_name.size(); i++) {
    length = _prefix_name[i].length();
    if (strncmp (_prefix_name[i].data(), absolute_name, length) == 0 &&
	(absolute_name[length] == '\0' ||
	 absolute_name[length] == ':'  ||
	 (length == 2 && absolute_name[0]==':' && absolute_name[1]==':')) &&
	length > longest_match) {
      longest_match = length;
      prefix = _prefix[i];
    }
  }

  if (longest_match == 0) {
    prefix = _current_prefix.back ();
  }

  for (VecVecString::size_type j=0; j < _current_scope.back().size(); j++) {
    if (prefix.length() > 0)
      prefix += "/";
    prefix += (_current_scope.back())[j];
  }

  if (prefix.length() > 0)
    prefix += "/";

  return prefix;
}

void DB::set_repoids( CORBA::Repository_ptr repo )
{
  /*
   * Also update the id when setting the version. But generation of a
   * new id is problematic. At this point, we don't know what prefix
   * was in use when this definition was read. So we do a hack and
   * just replace the last three characters of the current id.
   */
  for( vector<string>::size_type i = 0; i < _version.size(); i++ ) {
    CORBA::Contained_var c =
      repo->lookup( _version_name[ i ].c_str() );
    assert( !CORBA::is_nil( c ) );
    c->version( _version[ i ].c_str() );

    CORBA::String_var cid = c->id ();
    string oldid = cid.in();
    string fname = get_fname_for_repoid (oldid);
    CORBA::ULong ilen = oldid.length ();
    assert (ilen > 3 && _version[i].length() == 3 &&
	    cid[ilen-4] == ':' && cid[ilen-2] == '.');
    string newid = oldid;
    newid.replace (ilen - 3, 3, _version[i]);
    register_repoid (newid, fname);
    c->id (newid.c_str());
  }
  for( vector<string>::size_type i0 = 0; i0 < _id.size(); i0++ ) {
    CORBA::Contained_var c = repo->lookup( _id_name[ i0 ].c_str() );
    assert( !CORBA::is_nil( c ) );
    CORBA::String_var oldid = c->id ();
    string soldid = oldid.in();
    string fname = get_fname_for_repoid (soldid);
    register_repoid (_id[i0], fname);
    c->id( _id[ i0 ].c_str() );

    // Update pseudo_ids of anonymous types that depend on the updated id.
    for (IdToPseudoIdMap::iterator it = _id_to_pseudo_id_map.find( soldid );
	 it != _id_to_pseudo_id_map.end(); it++) {
      PseudoRepoId pseudo_id = (*it).second;
      string new_pseudo_id = 
        pseudo_id.get_prefix() + _id[0] + pseudo_id.get_suffix();
      if (!is_repoid_defined( new_pseudo_id )) {
        register_repoid( new_pseudo_id, 
                         get_fname_for_repoid( pseudo_id.get_full_id() ) );
      }
    }
  }
}

void DB::add_forward_dcl( const char* scoped_name )
{
  _forward_dcl.insert( scoped_name );
}

void DB::remove_forward_dcl( const char* scoped_name )
{
  _forward_dcl.erase( scoped_name );
}

bool DB::is_in_forward_dcl( const char* scoped_name )
{
  return _forward_dcl.find( scoped_name ) != _forward_dcl.end();
}

void DB::add_implicit (const char *scoped_name)
{
  _name_implicit_map[scoped_name] = true;
}

bool DB::is_implicit (const char *scoped_name)
{
  NameImplicitMap::iterator it = _name_implicit_map.find (scoped_name);
  if (it == _name_implicit_map.end()) {
    return false;
  }
  return (*it).second;
}

void DB::clear_implicits ()
{
  _name_implicit_map.clear ();
}

