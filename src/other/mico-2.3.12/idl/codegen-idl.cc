/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
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
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#include <sstream>
#else
#include <fstream.h>
#include <strstream.h>
#endif
#include <ctype.h>
#include <stdio.h>
#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif
#include "codegen-idl.h"

#endif // FAST_PCH


using namespace std;

static const char *idl_copyright[] = {
  "/*",
  " *  MICO --- an Open Source CORBA implementation",
  " *  Copyright (c) 1997-2006 by The Mico Team",
  " *",
  " *  This file was automatically generated. DO NOT EDIT!",
  " */"
};

const int idl_copyright_size = sizeof( idl_copyright ) / sizeof( char * );



CodeGenIDL::CodeGenIDL( DB &db, IDLParam &params,
			CORBA::Container_ptr con )
  : CodeGen( con ),
    _params( params )
{
  _db = &db;
}


void CodeGenIDL::emit( string &fn )
{
  string fnbase = fn;
  string fnIDL  = fnbase + ".idl";
  
  for( string::size_type i = 0; i < fnbase.length(); i++ ) {
    fnbase[ i ] = isalnum( fnbase[ i ] ) ? toupper( fnbase[ i ] ) : '_';
  }

  ofstream file( fnIDL.c_str() );

  for( int i2 = 0; i2 < idl_copyright_size; i2++ )
    file << idl_copyright[ i2 ] << endl;
  
  file << endl;
  file << "#ifndef __" << fnbase << "_IDL__" << endl;
  file << "#define __" << fnbase << "_IDL__" << endl;
  file << endl;

  o.start_output( file );
  emitIDL();
  o.stop_output();

  file << "#endif" << endl;
}

string CodeGenIDL::emit( CORBA::IRObject_ptr obj )
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
  ostringstream idl;
#else
  ostrstream idl;
#endif

  CORBA::Contained_var contained = CORBA::Contained::_narrow( obj );
  if( !CORBA::is_nil( contained ) ) {
    CORBA::String_var id = contained->id();
    idl << "/*" << endl;
    idl << " * " << id.in() << endl;
    idl << " */" << endl << endl;
    o.start_output( idl );
    emitLocal (contained);
    o.stop_output();
  }

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
  string result = idl.str();
#else
  idl << ends;
  string result = idl.str();
  idl.rdbuf()->freeze( 0 );
#endif
  return result;
}

/*
 * In dumping everything, we make three passes over the dependency list:
 *
 * 1) forward declarations
 * 2) include files
 * 3) everything else
 *
 * This way, files with mutual inclusions work.
 */

void CodeGenIDL::emitIDL()
{
  _current_scope = CORBA::Container::_duplicate( _container );

  /*
   * First pass: forward declarations
   */

  for (CORBA::ULong i1=0; i1<_idl_objs.length(); i1++) {
    if (strcmp (_idl_objs[i1]->scope.c_str(), "") != 0) {
      continue;
    }

    CORBA::Contained_var c = CORBA::Contained::_narrow (_idl_objs[i1]->obj);

    if (check_for_included_defn (c, false)) {
      continue;
    }

    if (!CORBA::is_nil (c)) {
      CORBA::String_var an = c->absolute_name ();
      if (_db->is_implicit (an.in())) {
	continue;
      }
    }

    if (!CORBA::is_nil(c)) {
      if (c->def_kind() == CORBA::dk_Interface ||
	  c->def_kind() == CORBA::dk_AbstractInterface ||
	  c->def_kind() == CORBA::dk_LocalInterface) {
	CORBA::Container_var new_scope = c->defined_in();
	enter_scope (new_scope);
	emitForwardDcl (c);
      }
    }
  }
  
  // Close open modules
  enter_scope (_container);

  o << endl;

  /*
   * Second pass: include files
   */

  for (CORBA::ULong i2=0; i2<_idl_objs.length(); i2++) {
    check_for_included_defn (_idl_objs[i2]->obj);
  }

  o << endl;

  /*
   * Third pass: everything else
   */

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    /*
     * Only process global objs here
     */

    if (strcmp (_idl_objs[i]->scope.c_str(), "") != 0) {
      continue;
    }

    CORBA::Contained_var c = CORBA::Contained::_narrow( _idl_objs[ i ]->obj );

    if (check_for_included_defn (c)) {
      continue;
    }

    if (!CORBA::is_nil (c)) {
      CORBA::String_var an = c->absolute_name ();
      if (_db->is_implicit (an.in())) {
	continue;
      }
    }

    if( !CORBA::is_nil( c ) ) {
      CORBA::Container_var new_scope = c->defined_in();
      enter_scope( new_scope );
      
      if( _idl_objs[ i ]->iface_as_forward ) {
	emitForwardDcl (c);
      }
      else {
	emitLocal (c);
      }
    }
  }

  // Close open modules
  enter_scope( _container );
}

void CodeGenIDL::enter_scope( CORBA::Container_ptr c )
{
  vector<CORBA::Contained_var> trace_current_scope;
  vector<CORBA::Contained_var> trace_new_scope;
  CORBA::Container_var         scope;
  bool                         nl;
  
  scope = _current_scope;
  while( scope->def_kind() != CORBA::dk_Repository ) {
    CORBA::Contained_ptr scope_contained = CORBA::Contained::_narrow( scope );
    assert( !CORBA::is_nil( scope_contained ) );
    trace_current_scope.insert( trace_current_scope.begin(), scope_contained );
    scope = scope_contained->defined_in();
  }
  
  scope = CORBA::Container::_duplicate( c );
  while( scope->def_kind() != CORBA::dk_Repository ) {
    CORBA::Contained_ptr scope_contained = CORBA::Contained::_narrow( scope );
    assert( !CORBA::is_nil( scope_contained ) );
    trace_new_scope.insert( trace_new_scope.begin(), scope_contained );
    scope = scope_contained->defined_in();
  }
  
  vector<CORBA::Contained_var>::iterator i, j, k;
  for( i = trace_current_scope.begin(), j = trace_new_scope.begin();
       i != trace_current_scope.end() && j != trace_new_scope.end() &&
	 (*i)->_is_equivalent( *j );
       i++, j++ ) ;
  
  nl = false;
  if (i != trace_current_scope.end()) {
    k = trace_current_scope.end();
    do {
      k--;
      o << BL_CLOSE_SEMI;
      emit_repoid (*k);
      nl = true;
    }
    while (k != i);
  }

  if( nl || j != trace_new_scope.end() )
    o << endl << endl;

  nl = false;
  while( j != trace_new_scope.end() ) {
    o << "module ";
    CORBA::Identifier_var name = (*j)->name();
    o << name << " " << BL_OPEN;
    nl = true;
    j++;
  }
  if( nl )
    o << endl;
  
  _current_scope = CORBA::Container::_duplicate( c );
}

bool
CodeGenIDL::check_for_included_defn (CORBA::IRObject_ptr obj,
				     bool emitinc)
{
  string id;
  
  CORBA::Contained_var con = CORBA::Contained::_narrow( obj );
  if( !CORBA::is_nil( con ) ) {
    CORBA::String_var tmp = con->id();
    id = tmp;
  } else {
    // Check if this is an anonymous type
    CORBA::IDLType_var idl_type = CORBA::IDLType::_narrow( obj );
    if( CORBA::is_nil( idl_type ) )
      // if it is not an IDLType, then it can't be an anonymous type
      return false;
    id = _db->gen_pseudo_repoid( idl_type );
  }

  /*
   * if --gen-included-defs, generate code anyway
   */

  if (_params.gen_included_defs) {
    return false;
  }
  
  string toplev = _db->get_toplevel_fname ();

  /*
   * If the "toplevel file name" is empty, generate code for everything.
   */

  if (toplev.length() == 0) {
    return false;
  }

  /*
   * If the repo id is not registered in our file name database, we assume
   * that no code needs to be generated, for example because the data has
   * already existed in the IFR prior to loading the toplevel file.
   */

  if (!_db->is_repoid_defined (id)) {
    return true;
  }

  string fname = _db->get_fname_for_repoid (id);

  if (fname != toplev) {
    // "obj" was defined in an include file. Don't emit code for it

    if (!emitinc) {
      return true;
    }

    string::size_type pos = fname.rfind( "." );
    if( (int)pos >= 0 && pos < fname.length() )
      fname = fname.substr (0, pos);
    fname += ".idl";
    if( _include_files.count( fname ) == 0 ) {
      _include_files.insert( fname );
      // For the first time we have to generate a #include directive
      if (!_params.mico_core) {
	o << "#include ";
	o << ( _params.use_quotes ? "\"" : "<" );
	o << _params.rewrite_include_path(fname);
	o << ( _params.use_quotes ? "\"" : ">" );
	o << endl;
      }
    }
    return true;
  }

  return false;
}


void CodeGenIDL::emit_repoid( CORBA::Contained_ptr c )
{
  if( !_params.emit_repoids )
    return;
  CORBA::String_var name = c->name();
  CORBA::String_var repoid = c->id();
  o << "#pragma ID " << name << " \"" << repoid << "\"" << endl << endl;
}

void CodeGenIDL::emitLocal( CORBA::Contained_ptr con,
			    bool emit_semicolon )
{
  CORBA::String_var absname = con->absolute_name ();

  if (_db->is_implicit (absname.in())) {
    return;
  }

  if (_db->is_in_forward_dcl (absname.in())) {
    emitForwardDcl (con);
    return;
  }

  switch( con->def_kind() ) {
  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
  case CORBA::dk_LocalInterface:
    {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow(con);
      emitIDLInterface( in );
      break;
    }
  case CORBA::dk_Operation:
    {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow (con);
      emitOperation( op );
      break;
    }
    
  case CORBA::dk_Attribute:
    {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow (con);
      emitAttribute( attr );
      break;
    }

  case CORBA::dk_ValueMember:
    {
      CORBA::ValueMemberDef_var v = CORBA::ValueMemberDef::_narrow( con );
      emitValueMember( v );
      break;
    }

  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow( con );
      emitIDLStruct( s, emit_semicolon );
      break;
    }
  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow( con );
      emitIDLUnion( u, emit_semicolon );
      break;
    }
  case CORBA::dk_Constant:
    {
      CORBA::ConstantDef_var co = CORBA::ConstantDef::_narrow( con );
      emitIDLConstant( co );
      break;
    }
  case CORBA::dk_Exception:
    {
      CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow( con );
      emitIDLException( e );
      break;
    }
  case CORBA::dk_Enum:
    {
      CORBA::EnumDef_var e = CORBA::EnumDef::_narrow( con );
      emitIDLEnum( e, emit_semicolon );
      break;
    }
  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( con );
      emitIDLAlias( a );
      break;
    }
  case CORBA::dk_Native:
    {
      CORBA::NativeDef_var n = CORBA::NativeDef::_narrow( con );
      emitIDLNative( n );
      break;
    }
  case CORBA::dk_Value:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow( con );
      emitIDLValue( v );
      break;
    }
  case CORBA::dk_ValueBox:
    {
      CORBA::ValueBoxDef_var v = CORBA::ValueBoxDef::_narrow( con );
      emitIDLValueBox( v );
      break;
    }

#ifdef USE_CCM
  case CORBA::dk_Provides:
    {
      CORBA::ComponentIR::ProvidesDef_var provides =
	CORBA::ComponentIR::ProvidesDef::_narrow (con);
      emitProvides (provides);
      break;
    }

  case CORBA::dk_Uses:
    {
      CORBA::ComponentIR::UsesDef_var uses =
	CORBA::ComponentIR::UsesDef::_narrow (con);
      emitUses (uses);
      break;
    }

  case CORBA::dk_Event:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow( con );
      emitIDLValue( v );
      break;
    }      

  case CORBA::dk_Emits:
  case CORBA::dk_Publishes:
  case CORBA::dk_Consumes:
    {
      CORBA::ComponentIR::EventPortDef_var event =
	CORBA::ComponentIR::EventPortDef::_narrow (con);
      emitEventPort (event);
      break;
    }

  case CORBA::dk_Component:
    {
      CORBA::ComponentIR::ComponentDef_var comp =
	CORBA::ComponentIR::ComponentDef::_narrow (con);
      emitComponent (comp);
      break;
    }

  case CORBA::dk_Factory:
  case CORBA::dk_Finder:
    {
      CORBA::OperationDef_var factory =
	CORBA::OperationDef::_narrow (con);
      emitFactory (factory);
      break;
    }

  case CORBA::dk_Home:
    {
      CORBA::ComponentIR::HomeDef_var home =
	CORBA::ComponentIR::HomeDef::_narrow (con);
      emitHome (home);
      break;
    }
#endif

  default:
    assert( 0 );
  }

  if (emit_semicolon) {
    emit_repoid( con );
  }
}

void CodeGenIDL::emitLocalDecls( CORBA::Container_ptr in )
{
  CORBA::Contained_var c = CORBA::Contained::_narrow (in);
  assert (!CORBA::is_nil (c));
  CORBA::String_var scope = c->absolute_name();

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    if (strcmp (_idl_objs[i]->scope.c_str(), scope.in()) == 0) {
      CORBA::Contained_var co = CORBA::Contained::_narrow (_idl_objs[i]->obj);
      if (!CORBA::is_nil (co)) {
	emitLocal (co);
      }
    }
  }
}

void CodeGenIDL::emitForwardDcl( CORBA::Contained_ptr c )
{
  CORBA::String_var name = c->name();

  if (c->def_kind() == CORBA::dk_Interface ||
      c->def_kind() == CORBA::dk_AbstractInterface ||
      c->def_kind() == CORBA::dk_LocalInterface) {
    CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow (c);

    if (in->def_kind() == CORBA::dk_AbstractInterface) {
      o << "abstract ";
    }
    else if (in->def_kind() == CORBA::dk_LocalInterface) {
      o << "local ";
    }
    o << "interface " << name << ";" << endl << endl;
  }
  else if (c->def_kind() == CORBA::dk_Value) {
    CORBA::ValueDef_var val = CORBA::ValueDef::_narrow (c);
    if (val->is_abstract()) {
      o << "abstract ";
    }
    o << "valuetype " << name << ";" << endl << endl;
  }
#ifdef USE_CCM
  else if (c->def_kind() == CORBA::dk_Component) {
    o << "component " << name << ";" << endl << endl;
  }
#endif
  else if (c->def_kind() == CORBA::dk_Struct) {
    o << "struct " << name << ";" << endl << endl;
  }    
  else if (c->def_kind() == CORBA::dk_Union) {
    o << "union " << name << ";" << endl << endl;
  }    
  else {
    assert (0);
  }
}

void CodeGenIDL::emitAttribute( CORBA::AttributeDef_ptr attr )
{
  if( attr->mode() == CORBA::ATTR_READONLY )
    o << "readonly ";
  o << "attribute ";
  CORBA::IDLType_var type_id = attr->type_def();
  emit_idl_type( type_id );
  CORBA::String_var n = attr->name();
  o << " " << n;

  CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (attr);

  if (!CORBA::is_nil (ead)) {
    CORBA::ExceptionDefSeq_var get_exceps = ead->get_exceptions ();
    CORBA::ExceptionDefSeq_var set_exceps = ead->set_exceptions ();

    if (get_exceps->length() > 0) {
      o << indent << endl;
      if (attr->mode() == CORBA::ATTR_READONLY) {
	o << "raises (";
      }
      else {
	o << "getRaises (";
      }

      for (CORBA::ULong i0=0; i0<get_exceps->length(); i0++) {
	if (i0 != 0)
	  o << ", ";
	CORBA::Contained_var c = _repo->lookup_id (get_exceps[i0]->id());
	assert (!CORBA::is_nil (c));
	CORBA::String_var abs_name = c->absolute_name();
	o << abs_name;
      }
      o << ")" << exdent;
    }

    if (set_exceps->length() > 0 && attr->mode() != CORBA::ATTR_READONLY) {
      o << indent << endl << "setRaises (";

      for (CORBA::ULong i1=0; i1<get_exceps->length(); i1++) {
	if (i1 != 0)
	  o << ", ";
	CORBA::Contained_var c = _repo->lookup_id (get_exceps[i1]->id());
	assert (!CORBA::is_nil (c));
	CORBA::String_var abs_name = c->absolute_name();
	o << abs_name;
      }
      o << ")" << exdent;
    }
  }

  o << ";" << endl;
}

void CodeGenIDL::emitOperation( CORBA::OperationDef_ptr op )
{
  if( op->mode() == CORBA::OP_ONEWAY )
    o << "oneway ";
  CORBA::IDLType_var type_id = op->result_def();
  emit_idl_type( type_id );
  CORBA::String_var n = op->name();
  o << " " << n << "(";
  CORBA::ParDescriptionSeq_var p = op->params();
  CORBA::ULong k;
  for( k = 0; k < p->length(); k++ ) {
    if( k != 0 )
      o << ",";
    o << " ";
    switch( p[ k ].mode ) {
    case CORBA::PARAM_IN:
      o << "in ";
      break;
    case CORBA::PARAM_OUT:
      o << "out ";
      break;
    case CORBA::PARAM_INOUT:
      o << "inout ";
      break;
    default:
      assert( 0 );
    }
    emit_idl_type( p[ k ].type_def );
    o << " " << p[ k ].name;
  }
  if( k != 0 )
    o << " ";
  o << ")" << indent;
  
  CORBA::ExceptionDefSeq_var exs = op->exceptions();
  if( exs->length() > 0 ) {
    o << endl;
    o << "raises( ";
    for( CORBA::ULong i = 0; i < exs->length(); i++ ) {
      if( i != 0 )
	o << ", ";
      CORBA::Contained_var c = _repo->lookup_id( exs[ i ]->id() );
      assert( !CORBA::is_nil( c ) );
      CORBA::String_var abs_name = c->absolute_name();
      o << abs_name;
    }
    o << " )";
  }
  CORBA::ContextIdSeq_var ctx = op->contexts();
  if( ctx->length() > 0 ) {
    o << endl;
    o << "context( ";
    for( CORBA::ULong i = 0; i < ctx->length(); i++ ) {
      if( i != 0 )
	o << ", ";
      o << "\"" << ctx[ i ] << "\"";
    }
    o << " )";
  }
  o << exdent << ";" << endl;
}

void
CodeGenIDL::emitValueMember( CORBA::ValueMemberDef_ptr vmd )
{
  if ( vmd->access() == CORBA::PRIVATE_MEMBER ) {
    o << "private ";
  }
  else {
    o << "public ";
  }

  CORBA::IDLType_var type_id = vmd->type_def();
  emit_idl_type( type_id );
  CORBA::String_var n = vmd->name();
  o << " " << n;
  emit_array_suffix (type_id);
  o << ";" << endl;
}

void
CodeGenIDL::emitIDLInterface( CORBA::InterfaceDef_ptr in )
{
  assert (in->def_kind() == CORBA::dk_Interface ||
	  in->def_kind() == CORBA::dk_AbstractInterface ||
	  in->def_kind() == CORBA::dk_LocalInterface);

  CORBA::String_var n = in->name();
  string className = (const char *) n;

  if (in->def_kind() == CORBA::dk_AbstractInterface) {
    o << "abstract ";
  }
  else if (in->def_kind() == CORBA::dk_LocalInterface) {
    o << "local ";
  }

  o << "interface " << className;
 
  CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
  if( bases->length() != 0 ) {
    // This interface *has* base interfaces
    o << " :" << indent << endl;
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      CORBA::String_var base_name = bases[ i ]->absolute_name();
      o << base_name;
      if( i + 1 != bases->length() )
	o << ",";
      o << endl;
    }
    o << exdent;
  } else
    o << endl;
  
  o << BL_OPEN;
  
  // Emit local declarations
  emitLocalDecls( in );
  
  o << BL_CLOSE_SEMI << endl;
}

void CodeGenIDL::emitIDLStruct( CORBA::StructDef_ptr s, bool emit_semicolon )
{
  assert( s->def_kind() == CORBA::dk_Struct );

  vector<CORBA::IDLType_var> emitted_locals;
  CORBA::TypeCode_var tc = s->type();
  CORBA::String_var name = s->name();
  
  o << "struct " << name << " " << BL_OPEN;
  CORBA::StructMemberSeq_var m = s->members();
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    /*
     * If the type is defined in the same scope (meaning that we are its
     * container), and it hasn't been printed yet, emit it here.
     */

    CORBA::Contained_var cv = CORBA::Contained::_narrow (m[i].type_def);
    CORBA::Container_var con = CORBA::Container::_nil();

    for (CORBA::ULong j=0; j<emitted_locals.size(); j++) {
      if (emitted_locals[j]->_is_equivalent (m[i].type_def)) {
	cv = CORBA::Contained::_nil(); // don't print it again
	break;
      }
    }

    if (!CORBA::is_nil (cv)) {
      con = cv->defined_in ();
    }

    if (s->_is_equivalent (con)) {
      emitted_locals.push_back (CORBA::IDLType::_duplicate (m[i].type_def));
      emitLocal( cv, false );
    }
    else {
      emit_idl_type( m[ i ].type_def );
    }

    o << " " << m[ i ].name;
    emit_array_suffix( m[ i ].type_def );
    o << ";" << endl;

    if (s->_is_equivalent (con)) {
      emit_repoid (cv);
    }
  }
  o << exdent << "}";
  if( emit_semicolon )
    o << ";" << endl << endl;
}

void CodeGenIDL::emitIDLUnion( CORBA::UnionDef_ptr u, bool emit_semicolon )
{
  assert( u->def_kind() == CORBA::dk_Union );

  vector<CORBA::IDLType_var> emitted_locals;
  CORBA::TypeCode_var tc = u->type();
  CORBA::String_var name = u->name();
  
  o << "union " << name << " switch( ";
  CORBA::IDLType_var discr = u->discriminator_type_def();
  emit_idl_type( discr );
  o << " ) " << BL_OPEN;
  CORBA::UnionMemberSeq_var m = u->members();
  tc = discr->type();

  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    CORBA::TypeCode_var label_tc = m[ i ].label.type();
    if( label_tc->is_octet() )
      o << "default:";
    else {
      o << "case ";
      if( tc->is_enum() ) {
	CORBA::String_var s;
	CORBA::ULong idx;
	CORBA::Boolean r = m[ i ].label.enum_get( idx );
	assert (r);
	s = (const char *) tc->unalias()->member_name( idx );
	o << s;
      } else
	o << &m[ i ].label;
      o << ":";
      if( i + 1 != m->length() &&
	  strcmp( m[ i ].name, m[ i + 1 ].name ) == 0 ) {
	o << endl;
	continue;
      }
    }

    /*
     * If the type is defined in the same scope (meaning that we are its
     * container), and it hasn't been printed yet, emit it here.
     */

    CORBA::Contained_var cv = CORBA::Contained::_narrow (m[i].type_def);
    CORBA::Container_var con = CORBA::Container::_nil();

    for (CORBA::ULong j=0; j<emitted_locals.size(); j++) {
      if (emitted_locals[j]->_is_equivalent (m[i].type_def)) {
	cv = CORBA::Contained::_nil(); // don't print it again
	break;
      }
    }

    if (!CORBA::is_nil (cv)) {
      con = cv->defined_in ();
    }

    o << indent << endl;
    if (u->_is_equivalent (con)) {
      emitted_locals.push_back (CORBA::IDLType::_duplicate (m[i].type_def));
      emitLocal( cv, false );
    }
    else {
      emit_idl_type( m[ i ].type_def );
    }

    o << " " << m[ i ].name;
    emit_array_suffix( m[ i ].type_def );
    o << ";" << exdent << endl;

    if (u->_is_equivalent (con)) {
      emit_repoid (cv);
    }
  }
  o << exdent << "}";
  if( emit_semicolon )
    o << ";" << endl << endl;
}

void CodeGenIDL::emitIDLConstant( CORBA::ConstantDef_ptr c )
{
  assert( c->def_kind() == CORBA::dk_Constant );
  CORBA::String_var name = c->name();
  o << "const ";
  CORBA::IDLType_var type = c->type_def();
  CORBA::TypeCode_var tc = type->type();
  CORBA::Any* a = c->value();

  if( tc->kind() == CORBA::tk_fixed )
    o << "fixed";
  else
    emit_idl_type (type);

  o << " " << name << " = ";

  if (tc->kind() == CORBA::tk_wchar || tc->kind() == CORBA::tk_wstring) {
    o << "L";
  }
  else if (tc->kind() == CORBA::tk_enum) {
    CORBA::Contained_var con = CORBA::Contained::_narrow (type);
    CORBA::String_var on = con->absolute_name ();
    string scoped_name = on.in();
    unsigned long i = scoped_name.find_last_of (":");
    if (i == 1 || i == string::npos) {
      scoped_name = "";
    }
    else if (i != string::npos) {
      scoped_name = scoped_name.substr (0, i+1);
    }
    o << scoped_name;
  }

  o << a;

  delete a;

  if( tc->kind() == CORBA::tk_fixed )
    o << "D";
  o << ";" << endl << endl;
}

void CodeGenIDL::emitIDLException( CORBA::ExceptionDef_ptr e )
{
  assert( e->def_kind() == CORBA::dk_Exception );

  vector<CORBA::IDLType_var> emitted_locals;
  CORBA::String_var name = e->name();
  
  o << "exception " << name << " " << BL_OPEN;
  CORBA::StructMemberSeq_var m = e->members();
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    /*
     * If the type is defined in the same scope (meaning that we are its
     * container), and it hasn't been printed yet, emit it here.
     */

    CORBA::Contained_var cv = CORBA::Contained::_narrow (m[i].type_def);
    CORBA::Container_var con = CORBA::Container::_nil();

    for (CORBA::ULong j=0; j<emitted_locals.size(); j++) {
      if (emitted_locals[j]->_is_equivalent (m[i].type_def)) {
	cv = CORBA::Contained::_nil(); // don't print it again
	break;
      }
    }

    if (!CORBA::is_nil (cv)) {
      con = cv->defined_in ();
    }

    if (e->_is_equivalent (con)) {
      emitted_locals.push_back (CORBA::IDLType::_duplicate (m[i].type_def));
      emitLocal( cv, false );
    }
    else {
      emit_idl_type( m[ i ].type_def );
    }

    o << " " << m[ i ].name;
    emit_array_suffix( m[ i ].type_def );
    o << ";" << endl;

    if (e->_is_equivalent (con)) {
      emit_repoid (cv);
    }
  }
  o << BL_CLOSE_SEMI << endl;
}

void CodeGenIDL::emitIDLEnum( CORBA::EnumDef_ptr e, bool emit_semicolon )
{
  assert( e->def_kind() == CORBA::dk_Enum );

  CORBA::String_var name = e->name();
  o << "enum " << name << " " << BL_OPEN;

  CORBA::EnumMemberSeq_var m = e->members();
  
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    if( i != 0 )
      o << "," << endl;
    o << m[ i ];
  }
  o << exdent << endl << "}";
  if( emit_semicolon )
    o << ";" << endl << endl;
}

void CodeGenIDL::emitIDLAlias( CORBA::AliasDef_ptr a )
{
  assert( a->def_kind() == CORBA::dk_Alias );
  
  CORBA::IDLType_var orig = a->original_type_def();
  assert( !CORBA::is_nil( orig ) );
  
  CORBA::TypeCode_var orig_tc = orig->type();
  
  CORBA::String_var name = a->name();

#if 0
  if( orig_tc->is_string() ) {
    o << "typedef string " << name;
    emit_array_suffix( orig );
    o << ";" << endl << endl;
  } else
#endif
  if( orig_tc->is_objref() ) {
    CORBA::String_var abs_name;
    
    if( orig_tc->kind() == CORBA::tk_alias ) {
      // we have an alias of an alias
      CORBA::AliasDef_var alias = CORBA::AliasDef::_narrow( orig );
      assert( !CORBA::is_nil( alias ) );
      abs_name = alias->absolute_name();
    } else {
      // gotta be an interface
      assert( orig_tc->kind() == CORBA::tk_objref );
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( orig );
      if( !CORBA::is_nil( in ) )
	abs_name = in->absolute_name();
      else
	abs_name = (const char *) "Object";
    }

    o << "typedef " << abs_name << " " << name;
    emit_array_suffix( orig );
    o << ";" << endl << endl;
  } else {
    o << "typedef ";
    emit_idl_type( orig );
    o << " " << name;
    emit_array_suffix( orig );
    o << ";" << endl;
  }
  o << endl;
}

void
CodeGenIDL::emitIDLNative( CORBA::NativeDef_ptr n )
{
  CORBA::String_var name = n->name();
  o << "native " << name << ";" << endl << endl;
}

void
CodeGenIDL::emitIDLValue( CORBA::ValueDef_ptr v )
{
  CORBA::String_var n = v->name();

  if (v->is_abstract()) {
    o << "abstract ";
  }
  if (v->is_custom()) {
    o << "custom ";
  }

#ifdef HAVE_CCM
  if (v->def_kind() == CORBA::dk_Event) {
    o << "eventtype " << n;
  }
  else {
    o << "valuetype " << n;
  }
#else
  o << "valuetype " << n;
#endif

  CORBA::ValueDef_var base_value = v->base_value();
  CORBA::ValueDefSeq_var abstract_base_values = v->abstract_base_values();
  CORBA::InterfaceDefSeq_var supported_interfaces = v->supported_interfaces();

  if (!CORBA::is_nil (base_value)) {
    o << " :" << indent << endl;
    if (v->is_truncatable()) {
      o << "truncatable ";
    }
    CORBA::String_var bn = base_value->absolute_name();
    o << bn;
  }

  if (abstract_base_values->length() > 0) {
    if (!CORBA::is_nil (base_value)) {
      o << ",";
    }
    else {
      o << " :" << indent;
    }
    for( CORBA::ULong i=0; i<abstract_base_values->length(); i++ ) {
      CORBA::String_var bn = abstract_base_values[i]->absolute_name();
      o << endl << bn;
      if (i+1 != abstract_base_values->length()) {
	o << ",";
      }
    }
  }

  if (supported_interfaces->length() > 0) {
    if (!CORBA::is_nil (base_value) || abstract_base_values->length() > 0) {
      o << endl;
    }
    else {
      o << " ";
    }
    o << "supports";
    if (supported_interfaces->length() == 1) {
      o << " ";
    }
    else {
      o << endl;
    }
    for (CORBA::ULong i=0; i<supported_interfaces->length(); i++) {
      CORBA::String_var sn = supported_interfaces[i]->absolute_name();
      o << sn;
      if (i+1 != supported_interfaces->length()) {
	o << "," << endl;
      }
    }
  }

  if (!CORBA::is_nil (base_value) || abstract_base_values->length() > 0) {
    o << exdent;
  }

  o << endl << BL_OPEN;
  emitLocalDecls( v );

  // emit initializers
  CORBA::InitializerSeq_var initializers = v->initializers();
  for (CORBA::ULong i=0; i<initializers->length(); i++) {
    o << "factory " << initializers[i].name << " (";
    for (CORBA::ULong j=0; j<initializers[i].members.length(); j++) {
      if (j != 0) {
	o << ", ";
      }
      o << "in ";
      emit_idl_type( initializers[i].members[j].type_def );
      o << " " << initializers[i].members[j].name.in();
    }
    o << ");" << endl;
  }

  // emit prototypes
  o << BL_CLOSE_SEMI << endl;
}

void
CodeGenIDL::emitIDLValueBox( CORBA::ValueBoxDef_ptr vb )
{
  CORBA::IDLType_var orig = vb->original_type_def();
  CORBA::String_var n = vb->name();
  o << "valuetype " << n << " ";
  emit_idl_type( orig );
  emit_array_suffix( orig );
  o << ";" << endl << endl;
}

#ifdef USE_CCM

void
CodeGenIDL::emitComponent (CORBA::ComponentIR::ComponentDef_ptr comp)
{
  CORBA::String_var n = comp->name ();

  o << "component " << n;

  CORBA::ComponentIR::ComponentDef_var base = comp->base_component ();

  if (!CORBA::is_nil (base)) {
    CORBA::String_var bn = base->absolute_name ();
    o << " : " << bn;
  }

  CORBA::InterfaceDefSeq_var supported = comp->supported_interfaces ();

  if (supported->length() > 1) {
    o << indent << endl << "supports" << indent << endl;
  }
  else if (supported->length() == 1) {
    o << " supports ";
  }

  for (CORBA::ULong si=0; si<supported->length(); si++) {
    if (si > 0) {
      o << ", " << endl;
    }
    CORBA::String_var bn = supported[si]->absolute_name ();
    o << bn;
  }

  if (supported->length() > 1) {
    o << exdent << exdent << endl;
  }
  else {
    o << endl;
  }

  o << BL_OPEN;
  emitLocalDecls (comp);
  o << BL_CLOSE_SEMI << endl;
}

void
CodeGenIDL::emitProvides (CORBA::ComponentIR::ProvidesDef_ptr p)
{
  o << "provides ";

  CORBA::InterfaceDef_var provided = p->interface_type ();
  if (!CORBA::is_nil (provided)) {
    CORBA::String_var pn = provided->absolute_name ();
    o << pn;
  }
  else {
    o << "Object";
  }

  CORBA::String_var n = p->name ();
  o << " " << n << ";" << endl;
}

void
CodeGenIDL::emitUses (CORBA::ComponentIR::UsesDef_ptr p)
{
  o << "uses ";

  if (p->is_multiple ()) {
    o << "multiple ";
  }

  CORBA::InterfaceDef_var used = p->interface_type ();
  if (!CORBA::is_nil (used)) {
    CORBA::String_var pn = used->absolute_name ();
    o << pn;
  }
  else {
    o << "Object";
  }

  CORBA::String_var n = p->name ();
  o << " " << n << ";" << endl;
}

void
CodeGenIDL::emitEventPort (CORBA::ComponentIR::EventPortDef_ptr p)
{
  CORBA::String_var n = p->name ();
  CORBA::ComponentIR::EventDef_var event = p->event ();

  switch (p->def_kind()) {
  case CORBA::dk_Emits:
    o << "emits ";
    break;

  case CORBA::dk_Publishes:
    o << "publishes ";
    break;

  case CORBA::dk_Consumes:
    o << "consumes ";
    break;

  default:
    assert (0);
  }

  CORBA::String_var pn = event->absolute_name ();
  o << pn << " " << n << ";" << endl;
}

void
CodeGenIDL::emitHome (CORBA::ComponentIR::HomeDef_ptr home)
{
  CORBA::String_var n = home->name ();

  o << "home " << n;

  CORBA::ComponentIR::HomeDef_var base = home->base_home ();

  if (!CORBA::is_nil (base)) {
    CORBA::String_var bn = base->absolute_name ();
    o << " : " << bn;
  }

  CORBA::InterfaceDefSeq_var supported = home->supported_interfaces ();

  if (supported->length() > 1) {
    o << indent << endl << "supports" << indent << endl;
  }
  else if (supported->length() == 1) {
    o << " supports ";
  }

  for (CORBA::ULong si=0; si<supported->length(); si++) {
    if (si > 0) {
      o << ", " << endl;
    }
    CORBA::String_var bn = supported[si]->absolute_name ();
    o << bn;
  }

  if (supported->length() > 1) {
    o << exdent << exdent << endl;
  }

  CORBA::ComponentIR::ComponentDef_var managed = home->managed_component ();
  CORBA::String_var mn = managed->absolute_name ();

  if (supported->length() > 1) {
    o << indent << "manages " << mn;
  }
  else {
    o << " manages " << mn;
  }

  CORBA::ValueDef_var pk = home->primary_key ();
  if (!CORBA::is_nil (pk)) {
    CORBA::String_var kn = pk->absolute_name ();
    o << " primaryKey " << kn;
  }

  if (supported->length() > 1) {
    o << exdent;
  }

  o << endl << BL_OPEN;
  emitLocalDecls (home);
  o << BL_CLOSE_SEMI << endl;
}

void
CodeGenIDL::emitFactory (CORBA::OperationDef_ptr op)
{
  CORBA::String_var n = op->name ();

  if (op->def_kind() == CORBA::dk_Factory) {
    o << "factory " << n << "(";
  }
  else {
    o << "finder " << n << "(";
  }

  CORBA::ParDescriptionSeq_var p = op->params();
  CORBA::ULong k;

  for( k = 0; k < p->length(); k++ ) {
    if( k != 0 )
      o << ", ";
    o << "in ";
    emit_idl_type( p[ k ].type_def );
    o << " " << p[ k ].name;
  }
  o << ")";
  
  CORBA::ExceptionDefSeq_var exs = op->exceptions();
  if( exs->length() > 0 ) {
    o << indent << endl;
    o << "raises (";
    for( CORBA::ULong i = 0; i < exs->length(); i++ ) {
      if( i != 0 )
	o << ", ";
      CORBA::Contained_var c = _repo->lookup_id( exs[ i ]->id() );
      assert( !CORBA::is_nil( c ) );
      CORBA::String_var abs_name = c->absolute_name();
      o << abs_name;
    }
    o << ")" << exdent;
  }
  
  o << ";" << endl;
}

#endif

void CodeGenIDL::emit_idl_type_name( CORBA::IDLType_ptr t )
{
  CORBA::String_var tmp;
  string abs_name;
  
  switch( t->def_kind() ) {
  case CORBA::dk_Struct:
  case CORBA::dk_Union:
  case CORBA::dk_Enum:
  case CORBA::dk_Alias:
  case CORBA::dk_Interface:
  case CORBA::dk_Value:
  case CORBA::dk_ValueBox:
  case CORBA::dk_Native:
    {
      CORBA::Contained_var con = CORBA::Contained::_narrow( t );
      tmp = con->absolute_name();
      abs_name = tmp;
      break;
    }
  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow( t );
      if( p->kind() == CORBA::pk_any )
	o << "any";
      else if( p->kind() == CORBA::pk_objref )
	o << "Object";
      else if( p->kind() == CORBA::pk_Principal )
        o << "CORBA::Principal";
      else
	assert( 0 );
      return;
    }
  default:
    cout << t->def_kind() << endl;
    assert( 0 );
  }
  if( abs_name[ 0 ] == ':' )
    abs_name = abs_name.substr( 2 );
  o << (char *) abs_name.c_str();
}

void CodeGenIDL::emit_array_suffix( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  while( tc->kind() == CORBA::tk_array ) {
    o << "[ " << tc->length() << " ]";
    tc = tc->content_type();
  }
}

void CodeGenIDL::emit_array( CORBA::IDLType_ptr type )
{
  assert( type->def_kind() == CORBA::dk_Array );
  CORBA::ArrayDef_var a;
  CORBA::IDLType_var idl_type = CORBA::IDLType::_duplicate( type );
  while( idl_type->def_kind() == CORBA::dk_Array ) {
    a = CORBA::ArrayDef::_narrow( idl_type );
    assert( !CORBA::is_nil( a ) );
    idl_type = a->element_type_def();
  }
  emit_idl_type( idl_type );
}

void CodeGenIDL::emit_sequence( CORBA::IDLType_ptr type )
{
  assert( type->def_kind() == CORBA::dk_Sequence );
  CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( type );
  CORBA::IDLType_var idl_type = s->element_type_def();
  
  o << "sequence< ";
  emit_idl_type( idl_type );
  
  CORBA::ULong b = s->bound();
  if( b > 0 )
    o << ", " << b;
  
  o << " >";
}

void CodeGenIDL::emit_idl_type( CORBA::IDLType_ptr t )
{
  CORBA::String_var tmp;
  
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  if( tc->is_alias() ) {
    emit_idl_type_name( t );
    return;
  }

  CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( t );
  if( !CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_objref ) {
    o << "Object";
    return;
  }
  else if (!CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_value_base) {
    o << "ValueBase";
    return;
  }

  // What about tk_abstract_interface?  
  if( tc->is_objref() ) {
    CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( t );
    tmp = in->absolute_name();
    string abs_name;
    abs_name = tmp;
    abs_name = abs_name.substr( 2 );
    o << abs_name;
    return;
  }

  if( !emit_base_type( t ) ) {
    switch( tc->kind() ) {
    case CORBA::tk_objref:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_alias:
    case CORBA::tk_any:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
    case CORBA::tk_native:
    case CORBA::tk_Principal:
      emit_idl_type_name( t );
      break;
    case CORBA::tk_sequence:
      emit_sequence( t );
      break;
    case CORBA::tk_array:
      emit_array( t );
      break;
    case CORBA::tk_TypeCode:
      o << "TypeCode";
      break;
    default:
      assert( 0 );
    }
  }
}

bool CodeGenIDL::emit_base_type( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  // Mapping of Base Data Types [16.5]
  switch( tc->kind() ) {
  case CORBA::tk_void:
    o << "void";
    break;
  case CORBA::tk_boolean:
    o << "boolean";
    break;
  case CORBA::tk_char:
    o << "char";
    break;
  case CORBA::tk_wchar:
    o << "wchar";
    break;
  case CORBA::tk_octet:
    o << "octet";
    break;
  case CORBA::tk_ushort:
    o << "unsigned short";
    break;
  case CORBA::tk_ulong:
    o << "unsigned long";
    break;
  case CORBA::tk_short:
    o << "short";
    break;
  case CORBA::tk_long:
    o << "long";
    break;
  case CORBA::tk_longlong:
    o << "long long";
    break;
  case CORBA::tk_ulonglong:
    o << "unsigned long long";
    break;
  case CORBA::tk_float:
    o << "float";
    break;
  case CORBA::tk_double:
    o << "double";
    break;
  case CORBA::tk_longdouble:
    o << "long double";
    break;
  case CORBA::tk_string:
    o << "string";
    if( tc->length() != 0 )
      o << "<" << tc->length() << ">";
    break;
  case CORBA::tk_wstring:
    o << "wstring";
    if( tc->length() != 0 )
      o << "<" << tc->length() << ">";
    break;
  case CORBA::tk_fixed:
    o << "fixed<" << tc->fixed_digits() << "," <<
      tc->fixed_scale() << ">";
    break;
  case CORBA::tk_alias:
    if( tc->is_base_type() ) {
      emit_idl_type_name( t );
      return true;
    } else
      return false;
    break;
  default:
    return false;
  }
  return true;
}


