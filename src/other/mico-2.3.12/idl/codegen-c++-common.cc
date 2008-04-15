/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
#include <ctype.h>
#include <stdio.h>
#include "codegen-c++-common.h"
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

CodeGenCPPCommon::CodeGenCPPCommon( DB &db, IDLParam& params,
				    CORBA::Container_ptr con)
  : CodeGen( con ),
    CodeGenCPPUtil( db, params, con )
{
}


/*
 * In dumping everything, we make three passes over the dependency list:
 *
 * 1) forward declarations for all interfaces
 * 2) include files
 * 3) everything else
 *
 * This way, files with mutual inclusions work.
 */

void
CodeGenCPPCommon::emit_common()
{
  _current_scope = CORBA::Container::_duplicate( _container );
  
  /*
   * First pass: forward declarations for all our interfaces
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
	enter_scope (new_scope, "");
	emit_ForwardDcl (c);
      }
    }
  }
  
  // Close open modules
  enter_scope (_container, "");

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

  /*
   * Only process global objs here
   */

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    if (strcmp (_idl_objs[i]->scope.c_str(), "") != 0) {
      continue;
    }
    
    CORBA::IRObject_ptr obj = _idl_objs[i]->obj;

    if( check_for_included_defn( obj ) )
      continue;

    CORBA::Contained_var c = CORBA::Contained::_narrow( obj );

    if (!CORBA::is_nil (c)) {
      CORBA::String_var an = c->absolute_name ();
      if (_db->is_implicit (an.in())) {
	continue;
      }
    }

    if( !CORBA::is_nil( c ) ) {
      CORBA::Container_var new_scope = c->defined_in();
      enter_scope( new_scope, "" );
      set_name_prefix( new_scope );
    }

    emit_idl_obj( _idl_objs[i] );
  }

  // Close open modules
  enter_scope( _container, "" );

  /*
   * Fourth pass: POA Skeleton declarations
   */

  o << "#ifndef MICO_CONF_NO_POA" << endl << endl;
  if (_params.poa && !_params.pseudo) {
    for (CORBA::ULong i2=0; i2<_idl_objs.length(); i2++) {
      CORBA::IRObject_ptr obj = _idl_objs[i2]->obj;

      if( check_for_included_defn( obj ) )
	continue;

      CORBA::Contained_var c = CORBA::Contained::_narrow( obj );

      if (!CORBA::is_nil (c)) {
	CORBA::String_var an = c->absolute_name ();
	if (_db->is_implicit (an.in())) {
	  continue;
	}
      }

      emit_poa_obj (_idl_objs[i2]);
    }
    enter_scope (_container, "POA_");
  }
  o << "#endif // MICO_CONF_NO_POA" << endl << endl;

  /*
   * Fifth pass: OBV Skeleton declarations
   */

  for (CORBA::ULong i4=0; i4<_idl_objs.length(); i4++) {
    CORBA::IRObject_ptr obj = _idl_objs[i4]->obj;

    if( check_for_included_defn( obj ) )
      continue;

    CORBA::Contained_var c = CORBA::Contained::_narrow( obj );

    if (!CORBA::is_nil (c)) {
      CORBA::String_var an = c->absolute_name ();
      if (_db->is_implicit (an.in())) {
	continue;
      }
    }

    emit_obv_obj (_idl_objs[i4]);
  }
  enter_scope (_container, "OBV_");
}

void CodeGenCPPCommon::emit_idl_obj( IRObj_ptr no )
{
  CORBA::IRObject_ptr obj = no->obj;

  if( check_for_included_defn( obj ) )
    return;

  CORBA::Contained_var con = CORBA::Contained::_narrow (obj);
  
  if (!CORBA::is_nil (con)) {
    CORBA::String_var absname = con->absolute_name ();

    if (_db->is_implicit (absname.in())) {
      return;
    }

    if (_db->is_in_forward_dcl (absname.in())) {
      emit_ForwardDcl (con);
      return;
    }
  }

  if (no->iface_as_forward) {
    assert (!CORBA::is_nil (con));
    emit_ForwardDcl (con);
    return;
  }

  switch( obj->def_kind() ) {
  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
  case CORBA::dk_LocalInterface:
    {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( obj );
      emit_Interface( in );
      break;
    }
  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow( obj );
      emit_Struct( s );
      break;
    }
  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow( obj );
      emit_Union( u );
      break;
    }
  case CORBA::dk_Constant:
    {
      CORBA::ConstantDef_var co = CORBA::ConstantDef::_narrow( obj );
      emit_Constant( co );
      break;
    }
  case CORBA::dk_Exception:
    {
      CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow( obj );
      emit_Exception( e );
      break;
    }
  case CORBA::dk_Enum:
    {
      CORBA::EnumDef_var e = CORBA::EnumDef::_narrow( obj );
      emit_Enum( e );
      break;
    }
  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( obj );
      emit_Alias( a );
      break;
    }
  case CORBA::dk_String:
    {
      CORBA::StringDef_var s = CORBA::StringDef::_narrow( obj );
      emit_StringDef( s );
      break;
    }
  case CORBA::dk_Wstring:
    {
      CORBA::WstringDef_var ws = CORBA::WstringDef::_narrow( obj );
      emit_WstringDef( ws );
      break;
    }
  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( obj );
      emit_SequenceDef( s );
      break;
    }
  case CORBA::dk_Fixed:
    {
      CORBA::FixedDef_var f = CORBA::FixedDef::_narrow( obj );
      emit_FixedDef( f );
      break;
    }
  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow( obj );
      emit_ArrayDef( a );
      break;
    }
  case CORBA::dk_Native:
    {
      CORBA::NativeDef_var n = CORBA::NativeDef::_narrow( obj );
      emit_Native( n );
      break;
    }
  case CORBA::dk_Value:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow( obj );
      emit_Value( v );
      break;
    }
  case CORBA::dk_ValueBox:
    {
      CORBA::ValueBoxDef_var v = CORBA::ValueBoxDef::_narrow( obj );
      emit_ValueBox( v );
      break;
    }
  case CORBA::dk_Operation:
  case CORBA::dk_Attribute:
  case CORBA::dk_ValueMember:
  case CORBA::dk_Provides:
  case CORBA::dk_Uses:
  case CORBA::dk_Emits:
  case CORBA::dk_Consumes:
  case CORBA::dk_Publishes:
    // Operation & attribute declarations are generated in emitPrototypes()
    break;
  default:
    cerr << obj->def_kind() << endl;
    assert( 0 );
  }
}

void
CodeGenCPPCommon::emit_marshal_prototype( CORBA::IRObject_ptr obj )
{
  if( _params.pseudo && !_params.any )
    return;

  /*
   * Here we declare a variable that references an instance
   * of a marshaller. If obj denotes an alias, the marshaller
   * is simply a reference to the original marshaller (and
   * therefore this reference needs to be initialized right here).
   * Otherwise there exists a class for the marshaller. In this
   * case the variable is initialized in the implementation.
   */
  CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( obj );
  if( CORBA::is_nil( a ) ) {
    // obj is not an alias
    o << _params.tc_export;
    o << " CORBA::StaticTypeInfo *";
    emit_marshaller_ref( obj );
    o << ";" << endl << endl;
  } else {
    // obj is an alias
    o.switchStream( 2 );
    o << _params.tc_export;
    o << " CORBA::StaticTypeInfo *";
    o << "&";
    emit_marshaller_ref( obj );
    o << ";" << endl << endl;
  }
}

void
CodeGenCPPCommon::emit_ForwardDcl( CORBA::Contained_ptr in )
{
  // Check if we already emitted the forward decls for this interface
  CORBA::String_var repo_id;
  repo_id = in->id();

  if( _forward_dcls.find( string( repo_id ) ) != _forward_dcls.end() )
    return;
  
  _forward_dcls.insert( string( repo_id ) );
  
  if (check_for_included_defn (in))
    return;
    
  CORBA::String_var tmp = in->name();
  CORBA::String_var className = (const char *) ID(tmp);
    
  if (in->def_kind() == CORBA::dk_Interface ||
      in->def_kind() == CORBA::dk_AbstractInterface ||
      in->def_kind() == CORBA::dk_LocalInterface) {
    o << "class " << className << ";" << endl;
    o << "typedef " << className << " *" << className <<
      "_ptr;" << endl;
    o << "typedef " << className << "_ptr " << className <<
      "Ref;" << endl;
    o << "typedef ObjVar< " << className << " > " << className <<
      "_var;" << endl;
    o << "typedef ObjOut< " << className << " > " << className <<
      "_out;" << endl;
  }
  else if (in->def_kind() == CORBA::dk_Value) {
    o << "class " << className << ";" << endl;
    o << "typedef " << className << " *" << className <<
      "_ptr;" << endl;
    o << "typedef " << className << "_ptr " << className <<
      "Ref;" << endl;
    o << "typedef ValueVar< " << className << " > " << className <<
      "_var;" << endl;
    o << "typedef ValueOut< " << className << " > " << className <<
      "_out;" << endl;
  }
  else if (in->def_kind() == CORBA::dk_Struct) {
    CORBA::StructDef_var s = CORBA::StructDef::_narrow (in);
    CORBA::TypeCode_var tc = s->type ();
    o << "struct " << className << ";" << endl;
    o << "typedef ";
    if (tc->is_variable())
      o << "TVarVar< " << className << " > ";
    else
      o << "TFixVar< " << className << " > ";
    o << className << "_var;" << endl;
    o << "typedef ";
    if (tc->is_variable())
      o << "TVarOut< " << className << " > ";
    else
      o << className << "& ";
    o << className << "_out;" << endl << endl;
  }
  else if (in->def_kind() == CORBA::dk_Union) {
    CORBA::UnionDef_var u = CORBA::UnionDef::_narrow (in);
    CORBA::TypeCode_var tc = u->type ();
    o << "class " << className << ";" << endl;
    o << "typedef ";
    if (tc->is_variable())
      o << "TVarVar< " << className << " > ";
    else
      o << "TFixVar< " << className << " > ";
    o << className << "_var;" << endl;
    o << "typedef ";
    if (tc->is_variable())
      o << "TVarOut< " << className << " > ";
    else
      o << className << "& ";
    o << className << "_out;" << endl << endl;
  }
  o << endl;
}

void CodeGenCPPCommon::emitPrototypes( CORBA::Container_ptr in,
				       bool as_pure_virtual )
{
  CORBA::ContainedSeq_var c;

  // Emit attributes
  c = in->contents (CORBA::dk_Attribute, 1);
  bool b = use_rel_names (false);
  for( CORBA::ULong j = 0; j < c->length(); j++ ) {
    CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
    assert (!CORBA::is_nil(attr));
    if( as_pure_virtual )
      o << "virtual ";
    CORBA::IDLType_var type_id = attr->type_def();
    CORBA::String_var n = attr->name();
    emit_type_for_result( type_id );
    o << " " << ID( n.in() ) << "()";
    if (_params.throw_decl) {
      o << " throw (CORBA::SystemException)";
    }
    if( as_pure_virtual )
      o << " = 0";
    o << ";" << endl;
    if( attr->mode() == CORBA::ATTR_NORMAL ) {
      if( as_pure_virtual )
	o << "virtual ";
      o << "void " << ID( n.in() ) << "( ";
      emit_type_for_param( type_id, CORBA::PARAM_IN );
      o << " value )";
      if (_params.throw_decl) {
	o << " throw (CORBA::SystemException)";
      }
      if( as_pure_virtual )
	o << " = 0";
      o << ";" << endl;
    }
  }
  use_rel_names (b);

  if (c->length()) {
    o << endl;
  }

  // Emit operations
  c = in->contents (CORBA::dk_Operation, 1);
  for( CORBA::ULong j0 = 0; j0 < c->length(); j0++ ) {
    CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j0]);
    assert (!CORBA::is_nil (op));
    if( as_pure_virtual )
      o << "virtual ";
    CORBA::IDLType_var type_id = op->result_def();
    CORBA::String_var n = op->name();
    emit_type_for_result( type_id );
    o << " " << ID( n.in() ) << "(";
    CORBA::ParDescriptionSeq_var p = op->params();
    CORBA::ULong k;
    for( k = 0; k < p->length(); k++ ) {
      if( k != 0 )
	o << ",";
      o << " ";
      emit_type_for_param( p[ k ].type_def, p[ k ].mode );
      o << " " << ID(p[ k ].name);
    }

    // Do we have a context?
    CORBA::ContextIdSeq_var ctx = op->contexts();
    
    if( ctx->length() > 0 ) {
      if( p->length() > 0 )
	o << ", ";
      o << "CORBA::Context_ptr _ctx";
    }
    
    if( k != 0 || ctx->length() > 0 )
      o << " ";
    o << ")";

    if (_params.throw_decl) {
      CORBA::ExceptionDefSeq_var exs = op->exceptions ();

      o << " throw (";

      for (CORBA::ULong ei=0; ei<exs->length(); ei++) {
	emit_type_name (exs[ei]);
	o << ", ";
      }

      o << "CORBA::SystemException)";
    }

    if( as_pure_virtual )
      o << " = 0";
    o << ";" << endl;
  }

  if (c->length()) {
    o << endl;
  }

  // Emit value state
  bool pub = true;
  c = in->contents (CORBA::dk_ValueMember, 1);
  for( CORBA::ULong j1=0; j1 < c->length(); j1++ ) {
    CORBA::ValueMemberDef_var vmd = CORBA::ValueMemberDef::_narrow(c[j1]);
    assert (!CORBA::is_nil (vmd));

    if (vmd->access() == CORBA::PRIVATE_MEMBER && pub) {
      o << exdent << "protected:" << indent << endl;
      pub = false;
    }
    else if (vmd->access() == CORBA::PUBLIC_MEMBER && !pub) {
      o << exdent << "public:" << indent << endl;
      pub = true;
    }

    // mapping as for union members
    CORBA::IDLType_var m_type = vmd->type_def();
    CORBA::String_var m_name = vmd->name();
    emit_union_member_decl( m_type, m_name.in(), true );
  }

  if (c->length()) {
    o << endl;
  }

}

void
CodeGenCPPCommon::emitLocalDecls( CORBA::Container_ptr in )
{
  CORBA::Contained_var c = CORBA::Contained::_narrow (in);
  assert (!CORBA::is_nil (c));
  CORBA::String_var scope = c->absolute_name();

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    if (strcmp (_idl_objs[i]->scope.c_str(), scope.in()) == 0) {
      emit_idl_obj (_idl_objs[i]);
    }
  }
}

void
CodeGenCPPCommon::emit_StringDef( CORBA::StringDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_String );

  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );

  o.switchStream( 1 );
  
  CORBA::TypeCode_var type_tc = s->type();
	  
  // Must be a bounded string
  assert( type_tc->length() > 0 );
  o << _params.tc_export << " CORBA::TypeCodeConst _tc_string_";
  o << type_tc->length();
  o << ";" << endl << endl;

  o.switchStream( 0 );

  restore_name_prefix();
}

void
CodeGenCPPCommon::emit_WstringDef( CORBA::WstringDef_ptr ws )
{
  assert( ws->def_kind() == CORBA::dk_Wstring );

  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );
  
  o.switchStream( 1 );
  
  CORBA::TypeCode_var type_tc = ws->type();
	  
  // Must be a bounded wstring
  assert( type_tc->length() > 0 );
  o << _params.tc_export << " CORBA::TypeCodeConst _tc_wstring_";
  o << type_tc->length();
  o << ";" << endl << endl;

  o.switchStream( 0 );

  restore_name_prefix();
}

void
CodeGenCPPCommon::emit_SequenceDef( CORBA::SequenceDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_Sequence );

  if (!is_marshallable (s)) {
    return;
  }

  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );

  o.switchStream( 1 );
  
  // We need to generate the prototypes iff the option --any was given,
  // or we have a sequence of a base type. The latter need to be generated
  // because we don´t #include <mico/basic_seq.h> for compiling performance
  // reasons
  // kay: we dont need prototyepes for sequences of base types if
  //      any operator generation is turned off 
  if( _params.any ) {
    CORBA::TypeCode_var type_tc = s->type();
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    emit_sequence( s );
    o << " &_s );" << endl;
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    emit_sequence( s );
    o << " *_s );" << endl;

    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    emit_sequence( s );
    o << " &_s );" << endl;
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    emit_sequence( s );
    o << " *&_s );" << endl << endl;
  }

  if( !is_base_sequence( s ) )
    emit_marshal_prototype( s );
  
  o.switchStream( 0 );

  restore_name_prefix();
}

void
CodeGenCPPCommon::emit_FixedDef( CORBA::FixedDef_ptr f )
{
  assert( f->def_kind() == CORBA::dk_Fixed );

  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );

  o.switchStream( 1 );

  CORBA::TypeCode_var type_tc = f->type();
  string fixed_name = "Fixed_";
  fixed_name += xdec( type_tc->fixed_digits() );
  fixed_name += "_";
  if( type_tc->fixed_scale() < 0 )
    fixed_name += string( "n" ) +
      xdec( -( type_tc->fixed_scale() ) );
  else
    fixed_name += xdec( type_tc->fixed_scale() );
  
  // We have to generate _var and _out type for a Fixed,
  // because unlike anonymous types, it can be passed as a
  // parameter type [20.11.1]
  
  // Emit _var type for Fixed
  o << "typedef TFixVar< ";
  emit_fixed( f );
  o << " > " << fixed_name << "_var;" << endl;
  
  // Emit _out for Fixed
  o << "typedef ";
  emit_fixed( f );
  o << "& " << fixed_name << "_out;" << endl;
  
  // Emit TypeCode for Fixed
  if( _params.typecode ) {
    o << _params.tc_export << " CORBA::TypeCodeConst ";
    emit_type_code_name( f );
    o << ";" << endl;
  }

  // Emit any marshalling operators
  if( _params.any ) {
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    emit_fixed( f );
    o << " &_f );" << endl;
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    emit_fixed( f );
    o << " *_f );" << endl;

    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    emit_fixed( f );
    o << " &_f );" << endl;
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    emit_fixed( f );
    o << " *&_f );" << endl << endl;
  }

  emit_marshal_prototype( f );

  o.switchStream( 0 );

  restore_name_prefix();
}

void
CodeGenCPPCommon::emit_ArrayDef( CORBA::ArrayDef_ptr a )
{
  assert( a->def_kind() == CORBA::dk_Array );

  if (!is_marshallable (a)) {
    return;
  }

  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );

  o.switchStream( 1 );
  
  // SII marshaller
  emit_marshal_prototype( a );
  
  o.switchStream( 0 );

  restore_name_prefix();
}

void
CodeGenCPPCommon::emit_Interface( CORBA::InterfaceDef_ptr in )
{
  assert (in->def_kind() == CORBA::dk_Interface ||
	  in->def_kind() == CORBA::dk_AbstractInterface ||
	  in->def_kind() == CORBA::dk_LocalInterface);

  set_name_prefix( in );
  
  emit_ForwardDcl( in );
  
  CORBA::String_var n = in->name();
  string className = (const char *) ID(n);
  n = in->absolute_name();
  
  o << endl;

  o << "/*" << endl;
  o << " * Base class and common definitions for ";

  switch (in->def_kind()) {
  case CORBA::dk_Interface:         o << "interface"; break;
  case CORBA::dk_AbstractInterface: o << "abstract interface"; break;
  case CORBA::dk_LocalInterface:    o << "local interface"; break;
  default:
    break;
  }

  o << " " << className << endl;
  o << " */" << endl << endl;
  
  /*
   * Determine bases.
   */

  bool has_abstract_base = false;
  bool has_concrete_base = false;
  bool has_local_base = false;

  CORBA::InterfaceDefSeq_var bases = in->base_interfaces();

  for (CORBA::ULong j0=0; j0<bases->length(); j0++) {
    switch (bases[j0]->def_kind()) {
    case CORBA::dk_Interface:
      has_concrete_base = true;
      break;

    case CORBA::dk_AbstractInterface:
      has_abstract_base = true;
      break;

    case CORBA::dk_LocalInterface:
      has_local_base = true;
      break;
    default:
      break;
    }
  }

  /*
   * Begin class declaration
   */

  o << "class " << _params.class_export
    << className << " : " << indent << endl;

  switch (in->def_kind()) {
  case CORBA::dk_Interface:
    if (has_abstract_base && !has_concrete_base) {
      o << "virtual public CORBA::MixedBase";
      if (bases->length() || _params.reflection) o << "," << endl;
    }
    else if (!has_abstract_base && !has_concrete_base) {
      o << "virtual public CORBA::Object";
      if (bases->length() || _params.reflection) o << "," << endl;
    }
    break;

  case CORBA::dk_AbstractInterface:
    if (!has_abstract_base) {
      o << "virtual public CORBA::AbstractBase";
      if (bases->length()) o << "," << endl;
    }
    break;

  case CORBA::dk_LocalInterface:
    if (!has_local_base) {
      // parent of base class for local interface
      // was changed from LocalObject to Object
      // in ptc/03-03-09
      o << "virtual public CORBA::Object";
      if (bases->length()) o << "," << endl;
    }
    break;
  default:
    break;
  }

  if (in->def_kind() == CORBA::dk_Interface && _params.reflection) {
    o << "virtual public Reflection::IFRProvider";
    if (bases->length()) o << "," << endl;
  }

  for (CORBA::ULong j1=0; j1<bases->length(); j1++) {
    CORBA::String_var base_name = bases[ j1 ]->absolute_name();
    o << "virtual public " << ID(base_name);
    if( j1 + 1 != bases->length() ) {
      o << ",";
      o << endl;
    }
  }
  o << exdent << endl;
  o << BL_OPEN;
  
  o << "public:" << indent << endl;

  o << "virtual ~" << className << "();" << endl;
  o << endl;

  o << "#ifdef HAVE_TYPEDEF_OVERLOAD" << endl;
  o << "typedef " << className << "_ptr _ptr_type;" << endl;
  o << "typedef " << className << "_var _var_type;" << endl;
  o << "#endif" << endl;
  o << endl;

  o << "static " << className << "_ptr _narrow( CORBA::Object_ptr obj );"
    << endl;
  o << "static " << className << "_ptr _narrow( CORBA::AbstractBase_ptr obj );"
    << endl;

  o << "static " << className << "_ptr _duplicate( ";
  o << className << "_ptr _obj )" << endl << BL_OPEN;
  if (in->def_kind() != CORBA::dk_AbstractInterface) {
    o << "CORBA::Object::_duplicate (_obj);" << endl;
  }
  else {
    o << "CORBA::AbstractBase::_duplicate (_obj);" << endl;
  }
  o << "return _obj;" << endl;
  o << BL_CLOSE << endl;

  o << "static " << className << "_ptr _nil()" << endl << BL_OPEN;
  o << "return 0;" << endl << BL_CLOSE << endl;

  /*
   * If we are abstract, support downcasting from ValueBase
   */

  if (in->def_kind() == CORBA::dk_AbstractInterface) {
    o << "static " << className << "_ptr _downcast( CORBA::ValueBase * vb );"
      << endl;
  }

  /*
   * If we are concrete but have an abstract base, implement _to_object()
   */

  if (has_abstract_base && in->def_kind() == CORBA::dk_Interface) {
    o << "CORBA::Object_ptr _to_object();" << endl;
  }
  
  o << "virtual void *_narrow_helper( const char *repoid );" << endl;

  o << endl;

  // Emit local declarations
  emitLocalDecls( in );
  
  // Generate prototypes
  emitPrototypes( in, true );
  
  // Generate epilogue
  o << exdent << "protected:" << indent << endl;
  o << className << "() {};" << exdent << endl;
  o << "private:" << indent << endl;
  o << className << "( const " << className << "& );" << endl;
  o << "void operator=( const " << className << "& );" << exdent << endl;
  o << BL_CLOSE_SEMI << endl;

  // Emit TypeCode for interface
  if (!_params.pseudo && _params.typecode) {
    CORBA::Container_var def_in = in->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << className << ";" << endl << endl;
  }

  if (!_params.pseudo && in->def_kind() != CORBA::dk_LocalInterface) {
    o.switchStream( 1 );
    save_name_prefix();
    set_name_prefix( CORBA::Container::_nil() );
    if( _params.any ) {
      // copying operator<<= for interface
      o << _params.operator_export << "void operator<<=( CORBA::Any &a, const ";
      emit_idl_type_name( in );
      o << " obj );" << endl;

      // non-copying operator<<= for interface
      o << _params.operator_export << "void operator<<=( CORBA::Any &a, ";
      emit_idl_type_name( in );
      o << "* obj_ptr );" << endl;

      // operator>>= for interface
      o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &a, ";
      emit_idl_type_name( in );
      o << " &obj );" << endl << endl;
    }

    emit_marshal_prototype( in );
    
    o.switchStream( 0 );
    restore_name_prefix();
  }

  if (_params.pseudo || in->def_kind() == CORBA::dk_LocalInterface) {
    // No stubs or skeletons for PIDL or local interfaces
    o << endl;
    return;
  }

  //-----STUB--------------------------------------------------------
  o << "// Stub for interface " << className << endl;
  
  o << "class " << _params.class_export
    << className << _STUB << ":" << indent << endl;

  /*
   * For abstract interfaces, we add CORBA::Object to the stub's heritage
   */

  if (in->def_kind() == CORBA::dk_AbstractInterface) {
    o << "virtual public CORBA::Object," << endl;
  }

  o << "virtual public " << className;
  if (_params.reflection) {
    o << "," << endl << "virtual public Reflection::IFRProvider" << _STUB;
  }
  for( CORBA::ULong j2 = 0; j2 < bases->length(); j2++ ) {
    CORBA::String_var base_name = bases[ j2 ]->absolute_name();
    o << "," << endl << "virtual public " << ID(base_name) << _STUB;
  }
  o << exdent << endl;

  o << BL_OPEN;
  
  o << "public:" << indent << endl;
  o << "virtual ~" << className << _STUB << "();" << endl;
  
  // Generate prototypes
  emitPrototypes( in, false );
  
  // Generate epilogue
  o << exdent << "private:" << indent << endl;
  o << "void operator=( const " << className << _STUB << "& );" << endl;
  o << exdent << BL_CLOSE_SEMI << endl;

  if (_params.poa && _params.poa_stubs) {
    //-----STUB2-------------------------------------------------------
    // This stub for objects in a colocated POA
    o << "#ifndef MICO_CONF_NO_POA" << endl << endl;
    o << "class " << _params.class_export
      << className << _STUB << "_clp :" << indent << endl;
    o << "virtual public " << className << _STUB << "," << endl;
    
    if (_params.reflection && bases->length() == 0) {
      o << "virtual public Reflection::IFRProvider" << _STUB << "_clp"
	<< exdent << endl;
    }
    else if (bases->length() == 0) {
      o << "virtual public PortableServer::StubBase" << exdent << endl;
    }
    else {
      if (_params.reflection) {
	o << "virtual public Reflection::IFRProvider" << _STUB << "_clp," << endl;
      }
      for (CORBA::ULong i2=0; i2<bases->length(); i2++) {
	CORBA::String_var base_name = bases[i2]->absolute_name();
	o << "virtual public " << ID(base_name) << _STUB << "_clp";
	if (i2+1 != bases->length()) {
	  o << ",";
	}
	o << endl;
      }
      o << exdent;
    }
    
    o << BL_OPEN;
    o << "public:" << indent << endl;
    o << className << _STUB << "_clp (PortableServer::POA_ptr, CORBA::Object_ptr);" << endl;
    o << "virtual ~" << className << _STUB << "_clp ();" << endl;
    
    // Generate prototypes
    emitPrototypes( in, false );
    
    // Generate epilogue
    o << exdent << "protected:" << indent << endl;
    o << className << _STUB << "_clp ();" << endl;
    o << exdent << "private:" << indent << endl;
    o << "void operator=( const " << className << _STUB << "_clp & );" << endl;
    o << exdent << BL_CLOSE_SEMI << endl;
    o << "#endif // MICO_CONF_NO_POA" << endl << endl;
  }
}

/*
 * Generates default constructor, copy constructor and assignment
 * operator for structs and unions. These have to be generated due
 * to a g++ bug (g++ doesn't expand templates properly for the implicit
 * generated versions of these methods). Note that according to
 * the CORBA2 specs these shouldn't be defined (see [16.9(1)])
 */
void
CodeGenCPPCommon::emit_DefaultMembers( char *name )
{
  o << "#ifdef HAVE_EXPLICIT_STRUCT_OPS" << endl;

  // Default constructor
  o << name << "();" << endl;

  // Destructor
  o << "~" << name << "();" << endl;

  // Copy constructor
  o << name << "( const " << name << "& s );" << endl;
  
  // Assignment operator
  o << name << "& operator=( const " << name << "& s );" << endl;

  o << "#endif //HAVE_EXPLICIT_STRUCT_OPS" << endl;
  o << endl;
}
  


void CodeGenCPPCommon::emit_Struct( CORBA::StructDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_Struct );

  CORBA::String_var tmp = s->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::StructMemberSeq_var m = s->members();
  CORBA::TypeCode_var tc = s->type();

  emit_ForwardDcl (s);

  o << "struct " << _params.class_export << name
    << " " << BL_OPEN;
  o << "#ifdef HAVE_TYPEDEF_OVERLOAD" << endl;
  o << "typedef " << name << "_var _var_type;" << endl;
  o << "#endif" << endl;
  emitLocalDecls( s );
  emit_DefaultMembers( name );
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    // need typedefs for anonymous sequence members
    if (m[i].type->is_sequence() && !m[i].type->is_alias()) {
      o << "typedef ";
      emit_sequence (m[i].type_def);
      o << " _" << ID(m[i].name) << "_seq;" << endl;
      o << "_" << ID(m[i].name) << "_seq ";
      o << ID(m[i].name) << ";" << endl;
    }
    else {
      emit_type_for_variable( m[ i ].type_def, automatic );
      o << " " << ID(m[ i ].name);
      emit_array_suffix( m[ i ].type_def );
      o << ";" << endl;
    }
  }

  if (contains_a_valuetype (s)) {
    o << "// we contain a valuetype" << endl;
    o << "CORBA::Long _count_refs (CORBA::ValueBase::visited * = 0);" << endl;
    o << "void _release_members ();" << endl;
  }

  o << BL_CLOSE_SEMI << endl;

  // Emit TypeCode for struct
  if( _params.typecode && tc->is_freestanding() ) {
    CORBA::Container_var def_in = s->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << name << ";" << endl << endl;
  }
  
  if (!is_marshallable (s)) {
    return;
  }

  // operator<<=
  o.switchStream( 1 );
  if( _params.any ) {
    tmp = s->absolute_name();
    CORBA::String_var abs_name = (const char *) ID(tmp);
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs_name << " &_s );" << endl;
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    o << abs_name << " *_s );" << endl;
  
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs_name << " &_s );" << endl;
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    o << abs_name << " *&_s );" << endl << endl;
  }

  emit_marshal_prototype( s );
  
  o.switchStream( 0 );
}



void CodeGenCPPCommon::emit_Union( CORBA::UnionDef_ptr u )
{
  assert( u->def_kind() == CORBA::dk_Union );

  CORBA::String_var tmp = u->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::UnionMemberSeq_var members = u->members();
  CORBA::TypeCode_var union_tc = u->type();

  emit_ForwardDcl (u);

  o << "class " << _params.class_export
    << name << " " << BL_OPEN;
  o << "public:" << indent << endl;
  o << "#ifdef HAVE_TYPEDEF_OVERLOAD" << endl;
  o << "typedef " << name << "_var _var_type;" << endl;
  o << "#endif" << endl;
  emitLocalDecls( u );

  // Emit declaration for the discriminator
  CORBA::IDLType_var discr = u->discriminator_type_def();
  emit_type_for_variable( discr, automatic );
  o << " _discriminator;" << endl << endl;

  // Generate member variables. Note: these can't be put in a C++
  // union as one would probably expect. The reason is that MICO
  // struct's are mapped to C++ struct's with explicit default
  // constructors and destructors (this is necessary due to a
  // g++ bug). But struct's with constructors are not allowed
  // in an union.
  CORBA::String_var last_name;
  last_name = (const char *) "";
  o << "struct __m " << BL_OPEN;

  bool has_explicit_default = false;
  
  for( CORBA::ULong i = 0; i < members->length(); i++ ) {
    CORBA::UnionMember& m = members[ i ];
    CORBA::TypeCode_var tc = members[ i ].label.type();
    if( tc->is_octet() )
      has_explicit_default = true;

    if( strcmp( last_name, m.name ) == 0 )
      // We already generated a declaration for this name
      continue;
    emit_type_for_variable( m.type_def, automatic );
    o << " " << ID(m.name);
    emit_array_suffix( m.type_def );
    o << ";" << endl;
    last_name = m.name;
  }
  o << exdent << "} _m;" << endl << endl;
  
  o << exdent << "public:" << indent << endl;

  emit_DefaultMembers( name );
  o << endl;
  
  // Emit discriminator functions
  o << "void _d( ";
  emit_type_for_param( discr, CORBA::PARAM_IN );
  o << " _p );" << endl;
  emit_type_for_result( discr );
  o << " _d() const;" << endl << endl;

  // Generate member accessor functions
  last_name = (const char *) "";
  for( CORBA::ULong i0 = 0; i0 < members->length(); i0++ ) {
    CORBA::UnionMember &m = members[ i0 ];

    if( strcmp( last_name, m.name ) == 0 )
      continue;
    last_name = m.name;
    
    CORBA::IDLType_var m_type = CORBA::IDLType::_duplicate( m.type_def );
    emit_union_member_decl( m_type, m.name.in(), false );
  }
  if( !has_explicit_default ) {
    // This union has no explicit default case label
    string default_label =
      generate_union_default_case_label( discr.in(), members.inout() );
    if( default_label.length() > 0 ) {
      // We have a case label left for an implicit default member
      o << "void _default();" << endl << endl;
    }
  }

  if (contains_a_valuetype (u)) {
    o << "// we contain a valuetype" << endl;
    o << "CORBA::Long _count_refs (CORBA::ValueBase::visited * = 0);" << endl;
    o << "void _release_members ();" << endl;
  }

  o << exdent << BL_CLOSE_SEMI << endl;
      
  // Emit TypeCode for union
  if( _params.typecode && union_tc->is_freestanding() ) {
    CORBA::Container_var def_in = u->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;

    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << name << ";" << endl << endl;
  }
  
  if (!is_marshallable (u)) {
    return;
  }

  // operator<<=
  o.switchStream( 1 );
  if( _params.any ) {
    tmp = u->absolute_name();
    CORBA::String_var abs_name = (const char *) ID(tmp);
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs_name << " &_u );" << endl;
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    o << abs_name << " *_u );" << endl;
    
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs_name << " &_u );" << endl;
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    o << abs_name << " *&_u );" << endl << endl;
  }

  // SII marshaller
  emit_marshal_prototype( u );
  
  o.switchStream( 0 );
}



void CodeGenCPPCommon::emit_Constant( CORBA::ConstantDef_ptr c )
{
  assert( c->def_kind() == CORBA::dk_Constant );
  CORBA::String_var tmp = c->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::Container_var def_in = c->defined_in();

  if( def_in->def_kind() == CORBA::dk_Repository ||
      def_in->def_kind() == CORBA::dk_Interface ||
      def_in->def_kind() == CORBA::dk_AbstractInterface ||
      def_in->def_kind() == CORBA::dk_LocalInterface ||
      def_in->def_kind() == CORBA::dk_Value ) {
    o << "static const ";
  } else {
    o << "const ";
  }
  CORBA::IDLType_var type = c->type_def();
  CORBA::TypeCode_var tc = type->type ();
  emit_type_for_variable (type, manual);

  if (tc->kind() == CORBA::tk_string
      || tc->kind() == CORBA::tk_wstring) {
    // special handling of string constants
    o << " const";
  }

  o << " " << name;

  // If this const is declared on a nested scope, we need to
  // defer its initialization because some C++ compilers
  // choke on this
  if( def_in->def_kind() == CORBA::dk_Repository || 
      def_in->def_kind() == CORBA::dk_Module  ) {
    // Only initialize if we are on global scope or in an module
    o << " = ";

    if (tc->kind () == CORBA::tk_enum) {
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

    CORBA::Any_var a = c->value();
    emit_const_value (a);
  }
  o << ";" << endl;
}



void CodeGenCPPCommon::emit_Exception( CORBA::ExceptionDef_ptr e )
{
  assert( e->def_kind() == CORBA::dk_Exception );

  CORBA::String_var tmp = e->name();
  CORBA::String_var name = (const char *) ID(tmp);

  o << "struct " << _params.class_export << name
    << " : public CORBA::UserException " << BL_OPEN;

  CORBA::StructMemberSeq_var m = e->members();
  emitLocalDecls( e );
  emit_DefaultMembers( name );

  // emit constructor for member initialization
  if( m->length() > 0 ) {
    /*
     * when defining a constructor that takes some arguments, we also
     * need to explicitly define a default constructor. Such a constructor
     * is already defined by emit_DefaultMembers if
     * HAVE_EXPLICIT_STRUCT_OPTS
     */
    o << "#ifndef HAVE_EXPLICIT_STRUCT_OPS" << endl;
    o << name << "();" << endl;
    o << "#endif //HAVE_EXPLICIT_STRUCT_OPS" << endl;

    // emit constructor for member initialization
    bool b = use_rel_names (false);
    o << name << "( ";
    for( CORBA::ULong i = 0; i < m->length(); i++ ) {
      if( i != 0 )
	o << ", ";
      emit_type_for_param( m[ i ].type_def, CORBA::PARAM_IN );
      o << " _m" << i;
      emit_array_suffix( m[i].type_def );
    }
    o << " );" << endl;
    use_rel_names (b);
  }
  o << endl;

  // Emit backward compatibility operators
  o << "#ifdef HAVE_STD_EH" << endl;
  o << name << " *operator->() { return this; }" << endl;
  o << name << "& operator*() { return *this; }" << endl;
  o << "operator " << name << "*() { return this; }" << endl;
  o << "#endif // HAVE_STD_EH" << endl;
  o << endl;
  
  o << "void _throwit() const;" << endl;
  o << "const char *_repoid() const;" << endl;
  o << "void _encode( CORBA::DataEncoder &en ) const;" << endl;
  o << "void _encode_any( CORBA::Any &a ) const;" << endl;
  o << "CORBA::Exception *_clone() const;" << endl;
  o << "static " << name << " *_downcast( CORBA::Exception *ex );"
    << endl;
  o << "static const " << name
    << " *_downcast( const CORBA::Exception *ex );" << endl;
  
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    emit_type_for_variable( m[ i ].type_def, automatic );
    o << " " << ID(m[ i ].name);
    emit_array_suffix( m[ i ].type_def );
    o << ";" << endl;
  }
  o << BL_CLOSE_SEMI << endl;

  // Emit _catch types
  o << "#ifdef HAVE_STD_EH" << endl;
  o << "typedef " << name << " " << name << "_catch;" << endl;
  o << "#else" << endl;
  o << "typedef ExceptVar< " << name << " > " << name << "_var;" << endl;
  o << "typedef TVarOut< " << name << " > " << name << "_out;" << endl;
  o << "typedef " << name << "_var " << name << "_catch;" << endl;
  o << "#endif // HAVE_STD_EH" << endl;
  o << endl;

  // Emit TypeCode for exception
  if( _params.typecode ) {
    CORBA::Container_var def_in = e->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << name << ";" << endl << endl;
  }

  if (!is_marshallable (e)) {
    return;
  }

  // operator<<=
  o.switchStream( 1 );
  if( _params.any ) {
    tmp = e->absolute_name();
    CORBA::String_var abs_name = (const char *) ID(tmp);
    o << _params.operator_export << "void operator<<=( CORBA::Any &a, const ";
    o << abs_name << " &e );" << endl;
    o << _params.operator_export << "void operator<<=( CORBA::Any &a, ";
    o << abs_name << " *e );" << endl;
    
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &a, ";
    o << abs_name << " &e );" << endl << endl;
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &a, const ";
    o << abs_name << " *&e );" << endl << endl;
  }

  // SII marshaller
  emit_marshal_prototype( e );
  
  o.switchStream( 0 );
}



void CodeGenCPPCommon::emit_Enum( CORBA::EnumDef_ptr e )
{
  assert( e->def_kind() == CORBA::dk_Enum );

  CORBA::String_var tmp = e->name();
  CORBA::String_var name = (const char *) ID(tmp);
  o << "enum " << name << " " << BL_OPEN;

  CORBA::EnumMemberSeq_var m = e->members();
  
  for( CORBA::ULong i = 0; i < m->length(); i++ ) {
    if( i != 0 )
      o << "," << endl;
    o << ID(m[ i ]);
    if( i == 0 )
      o << " = 0";
  }
  o << endl;
  o << BL_CLOSE_SEMI << endl;

  // Emit _out for enum
  o << "typedef " << name << "& " << name << "_out;";
  o << endl << endl;
  
  // Emit TypeCode for enum
  if( _params.typecode ) {
    CORBA::Container_var def_in = e->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;

    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << name << ";" << endl << endl;
  }

  // operator<<=
  o.switchStream( 1 );
  if( _params.any ) {
    tmp = e->absolute_name();
    CORBA::String_var abs_name = (const char *) ID(tmp);
    o << _params.operator_export << "void operator<<=( CORBA::Any &a, const ";
    o << abs_name << " &e );" << endl << endl;
    
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &a, ";
    o << abs_name << " &e );" << endl << endl;
  }

  emit_marshal_prototype( e );
  
  o.switchStream( 0 );
}



void CodeGenCPPCommon::emit_Alias( CORBA::AliasDef_ptr a )
{
  assert( a->def_kind() == CORBA::dk_Alias );

  CORBA::IDLType_var orig = a->original_type_def();
  assert( !CORBA::is_nil( orig ) );
  
  CORBA::String_var tmp = a->name();
  CORBA::String_var name = (const char *) ID(tmp);
  
  CORBA::TypeCode_var orig_tc = orig->type();

  if( orig_tc->is_string() ) {
    o << "typedef char* " << name << ";" << endl;
    o << "typedef CORBA::String_var ";
    o << name << "_var;" << endl;
    o << "typedef CORBA::String_out ";
    o << name << "_out;" << endl << endl;
  } else if( orig_tc->is_wstring() ) {
    o << "typedef CORBA::WChar* " << name << ";" << endl;
    o << "typedef CORBA::WString_var ";
    o << name << "_var;" << endl;
    o << "typedef CORBA::WString_out ";
    o << name << "_out;" << endl << endl;
  } else if (orig_tc->is_type_code()) {
    o << "typedef CORBA::TypeCode " << name << ";" << endl;
    o << "typedef CORBA::TypeCode_ptr " << name << "_ptr;" << endl;
    o << "typedef CORBA::TypeCode_var " << name << "_var;" << endl;
    o << "typedef CORBA::TypeCode_out " << name << "_out;" << endl;
  } else if( orig_tc->is_objref() ) {
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
      if( !CORBA::is_nil( in ) ) {
	// Must be a regular object reference
	abs_name = in->absolute_name();
      } else {
	// We have an "Object" object reference
	abs_name = (const char *) "CORBA::Object";
      }
    }
    abs_name = (const char *) ID(abs_name);

    o << "typedef " << abs_name << " " << name << ";" << endl;
    o << "typedef " << abs_name << "_ptr " << name << "_ptr;" << endl;
    o << "typedef " << abs_name << "Ref " << name << "Ref;" << endl;
    o << "typedef " << abs_name << "_var " << name << "_var;" << endl;
    o << "typedef " << abs_name << "_out " << name << "_out;" << endl;
    o << endl;
  } else if( orig_tc->is_valuetype() ) {
    CORBA::String_var abs_name;
    
    if( orig_tc->kind() == CORBA::tk_alias ) {
      // we have an alias of an alias
      CORBA::AliasDef_var alias = CORBA::AliasDef::_narrow( orig );
      assert( !CORBA::is_nil( alias ) );
      abs_name = alias->absolute_name();
    } else {
      // gotta be a value or a value box
      assert( orig_tc->kind() == CORBA::tk_value ||
	      orig_tc->kind() == CORBA::tk_value_box );
      CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( orig );
      CORBA::ValueBoxDef_var vb = CORBA::ValueBoxDef::_narrow( orig );
      if( !CORBA::is_nil( val ) ) {
	abs_name = val->absolute_name();
      } else if( !CORBA::is_nil( vb ) ) {
	abs_name = vb->absolute_name();
      }
      else {
	assert (0);
      }
    }
    abs_name = (const char *) ID(abs_name);
    o << "typedef " << abs_name << " " << name << ";" << endl;
    o << "typedef " << abs_name << "_ptr " << name << "_ptr;" << endl;
    o << "typedef " << abs_name << "Ref " << name << "Ref;" << endl;
    o << "typedef " << abs_name << "_var " << name << "_var;" << endl;
    o << "typedef " << abs_name << "_out " << name << "_out;" << endl;
    o << endl;
  } else {
      /*
       * At this point we have something other than string, wstring,
       * objref, valuetype.
       */

    o << "typedef ";
    emit_type_for_variable( orig, automatic );
    o << " " << name;
    emit_array_suffix( orig );
    o << ";" << endl;

    if( orig_tc->is_array() ) {

      // Determine the base type of the array (i.e. remove all
      // aliases and array declarations)
      CORBA::IDLType_var base_type = CORBA::IDLType::_duplicate( orig );
      CORBA::DefinitionKind dk = base_type->def_kind();
      while( dk == CORBA::dk_Alias || dk == CORBA::dk_Array ) {
	if( dk == CORBA::dk_Alias ) {
	  CORBA::AliasDef_var tmp = CORBA::AliasDef::_narrow( base_type );
	  assert( !CORBA::is_nil( tmp ) );
	  base_type = tmp->original_type_def();
	}
	if( dk == CORBA::dk_Array ) {
	  CORBA::ArrayDef_var tmp = CORBA::ArrayDef::_narrow( base_type );
	  assert( !CORBA::is_nil( tmp ) );
	  base_type = tmp->element_type_def();
	}
	dk = base_type->def_kind();
      }
	
      CORBA::ULong num_elem = compute_num_array_elem( orig_tc );

      if( orig_tc->kind() != CORBA::tk_alias ) {
	
	// Array slice
	o << "typedef ";
	emit_type_for_variable( orig, automatic );
	o << " " << name << "_slice";
	emit_array_slice_suffix( orig );
	o << ";" << endl;
	
	// _var for Array
	o << "typedef Array";
	o << (orig_tc->is_variable() ? "Var" : "Fix") << "Var< ";
	emit_type_for_variable( base_type, automatic );
	o << "," << name << "_slice," << name << ",";
	o << num_elem << " > ";
	o << name << "_var;" << endl;
	
	// _out for Array
	if (orig_tc->is_variable()) {
	    o << "typedef ArrayVarOut < ";
	    emit_type_for_variable( base_type, automatic );
	    o << "," << name << "_slice," << name << ",";
	    o << num_elem << " > ";
	    o << name << "_out;" << endl;
	} else {
	    o << "typedef " << name << " " << name << "_out;" << endl;
	}
	
	// _forany for Array
	// First generate a dummy type that will be used to
	// disambiguate similar structural array types
	o << "enum _dummy_" << name << " { _dummy_" << name << "_0 };" << endl;
	o << "typedef Array";
	o << (orig_tc->is_variable() ? "Var" : "Fix") << "ForAny < ";
	emit_type_for_variable( base_type, automatic );
	o << "," << name << "_slice," << name << ",";
	o << num_elem << ",_dummy_" << name << "> ";
	o << name << "_forany;" << endl;

	// Generate helping #define for second argument since it
	// may contain "," which confuse the preprocessor
	o << "#undef MICO_ARRAY_ARG" << endl;
	o << "#define MICO_ARRAY_ARG ";
	emit_type_for_variable( base_type, automatic );
	o << endl;
	
	// T_alloc for array
	o << "DECLARE_ARRAY_ALLOC(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;

	// T_dup for array
	o << "DECLARE_ARRAY_DUP(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;

	// T_free for array
	o << "DECLARE_ARRAY_FREE(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;

	// T_copy for array (MICO extension)
	o << "DECLARE_ARRAY_COPY(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;

	if( _params.any ) {
	  o.switchStream( 1 );
	  CORBA::String_var abs_name = a->absolute_name();
	  abs_name = (const char *) ID(abs_name);
	  
	  // operator<<=()
	  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
	  o << abs_name << "_forany &_s );" << endl;
	  
	  // operator>>=()
	  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
	  o << abs_name << "_forany &_s );" << endl;
	  o.switchStream( 0 );
	}
      } else {
	// We have an alias of an array

	// Array slice
	o << "typedef ";
	emit_type_for_variable( orig, automatic );
	o << "_slice " << name << "_slice";
	o << ";" << endl;
	
	// _var for Array
	o << "typedef ";
	emit_type_for_variable( orig, automatic );
	o << "_var " << name << "_var";
	o << ";" << endl;
	
	// _out for Array
	o << "typedef " << name << "_var " << name << "_out;" << endl;
	
	// _forany for Array
	o << "typedef ";
	emit_type_for_variable( orig, automatic );
	o << "_forany " << name << "_forany";
	o << ";" << endl;
	
	// Generate helping #define for second argument since it
	// may contain "," which confuse the preprocessor
	o << "#undef MICO_ARRAY_ARG" << endl;
	o << "#define MICO_ARRAY_ARG ";
	emit_type_for_variable( base_type, automatic );
	o << endl;
	
	// T_alloc for array
	o << "DECLARE_ARRAY_ALLOC(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;
	
	// T_dup for array
	o << "DECLARE_ARRAY_DUP(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;
	
	// T_free for array
	o << "DECLARE_ARRAY_FREE(" << name << ",MICO_ARRAY_ARG,"
	  << name << "_slice," << num_elem << ")" << endl;
      }
    }
  
    if( !orig_tc->is_base_type() && !orig_tc->is_enum() &&
	!orig_tc->is_array() ) {
      // We have to generate _var and _out types as well

      // _var type
      o << "typedef ";
      if( orig_tc->is_sequence() ) {
	o << "TSeqVar< ";
	emit_type_for_variable( orig, automatic );
	o << " >";
      } else {
	emit_type_for_variable( orig, automatic );
	o << "_var";
      }
      o << " " << name << "_var;" << endl;

      // _out type
      o << "typedef ";
      if( orig_tc->is_sequence() ) {
	o << "TSeqOut< ";
	emit_type_for_variable( orig, automatic );
	o << " >";
      } else {
	emit_type_for_variable( orig, automatic );
	o << "_out";
      }
      o << " " << name << "_out;" << endl << endl;
    }
  }

  // _out for base types and enums
  if ((orig_tc->is_base_type() && !orig_tc->is_string() &&
      !orig_tc->is_wstring()) || orig_tc->is_enum()) {
      o << "typedef " << name << "& " << name << "_out;" << endl;
  }

  // Emit TypeCode for alias
  if( _params.typecode ) {
    CORBA::Container_var def_in = a->defined_in();
    assert( !CORBA::is_nil( def_in ) );
    
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
      o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << name << ";" << endl << endl;
  }
}

void CodeGenCPPCommon::emit_Native( CORBA::NativeDef_ptr n )
{
  CORBA::String_var tmp = n->name();
  CORBA::String_var name = (const char *) ID(tmp);
  tmp = n->absolute_name();

  if (strcmp (tmp.in(), "::CORBA::AbstractBase") == 0) {
    // class CORBA::AbstractBase exists in the C++ mapping
    return;
  }

  o << "typedef MICO_Native";
  emit_mangled_name (tmp.in());
  o << " " << name << ";" << endl;

  // _out for native
  o << "typedef MICO_Native";
  emit_mangled_name (tmp.in());
  o << "& " << name << "_out;" << endl << endl;
}

void CodeGenCPPCommon::emit_Value( CORBA::ValueDef_ptr val )
{
  set_name_prefix( val );
  emit_ForwardDcl( val );

  CORBA::String_var n = val->name();
  string className = (const char *) ID(n);
  
  //-----COMMON------------------------------------------------------
  o << endl;
  o << "// Common definitions for valuetype " << className << endl;
  o << "class " << _params.class_export
    << className << " : " << indent << endl;

  CORBA::ValueDef_var base_value = val->base_value ();
  CORBA::ValueDefSeq_var bases = val->abstract_base_values ();
  CORBA::InterfaceDefSeq_var supported = val->supported_interfaces ();

  /*
   * Inheritance for Valuetypes: We inherit from all base values, concrete
   * or not. According to the specs, we do not inherit from a concrete
   * supported interface (to avoid inheriting CORBA::Object), but we must
   * inherit an abstract supported interface.
   */

  if (CORBA::is_nil (base_value) && bases->length() == 0) {
    o << "virtual public CORBA::ValueBase";
  }
  else {
    if (!CORBA::is_nil (base_value)) {
      o << "virtual public ";
      CORBA::String_var base_name = base_value->absolute_name();
      o << ID(base_name);
      if (bases->length() > 0) {
	o << "," << endl;
      }
    }
    
    if (bases->length() > 0) {
      for (CORBA::ULong i=0; i<bases->length(); i++) {
	CORBA::String_var base_name = bases[i]->absolute_name();
	o << "virtual public " << ID(base_name);
	if (i+1 < bases->length()) {
	  o << "," << endl;
	}
      }
    }
  }

  if (supported->length() > 0) {
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      if (supported[i]->def_kind() == CORBA::dk_AbstractInterface) {
	CORBA::String_var supported_name = supported[i]->absolute_name();
	o << "," << endl << "virtual public " << ID(supported_name);
      }
    }
  }

  if (val->is_custom()) {
    o << "," << endl << "virtual public CORBA::CustomMarshal";
  }

  o << exdent << endl;

  o << BL_OPEN;
  o << "public:" << indent << endl;
  o << "static " << className << "* _downcast (CORBA::ValueBase *);" << endl;
  o << "static " << className << "* _downcast (CORBA::AbstractBase *);"
    << endl << endl;

  // Emit local declarations
  emitLocalDecls( val );

  // Generate prototypes
  emitPrototypes( val, true );

  // Emit prototypes for supported concrete interface
  if (supported->length() > 0) {
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      if (supported[i]->def_kind() == CORBA::dk_Interface) {
	o << exdent << "public:" << indent << endl;
	emitPrototypes( supported[i], true );
      }
    }
  }
  
  
  // Generate epilogue
  o << exdent << "public:" << indent << endl;
  if (is_marshallable(val) && !val->is_abstract()) {
    o << "CORBA::ValueBase * _copy_value ();" << endl;
  }
  o << "CORBA::ValueDef_ptr get_value_def ();" << endl;
  o << "virtual void * _narrow_helper (const char *);" << endl;
  if (is_marshallable(val)) {
    o << "void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);"
      << endl;
    o << "void _marshal_members (CORBA::DataEncoder &);" << endl;
    o << "CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);" << endl;
  }
  if (check_for_possible_circle (val)) {
    o << "CORBA::Long _count_refs (CORBA::ValueBase::visited * = 0);" << endl;
    o << "void _release_members ();" << endl;
  }

  /*
   * If we support an abstract interface, implement _to_value()
   */

  if (supported->length() > 0) {
    bool supports_abstract = false;
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      if (supported[i]->def_kind() == CORBA::dk_AbstractInterface) {
	supports_abstract = true;
	break;
      }
    }
    if (supports_abstract) {
      o << "CORBA::ValueBase * _to_value ();" << endl;
    }
  }

  o << endl << exdent << "protected:" << indent << endl;
  o << className << " ();" << endl;
  o << "virtual ~" << className << " ();" << endl;
  if (is_marshallable(val) &&
      !val->is_abstract() && !val->is_custom() &&
      !check_for_possible_circle (val)) {
    o << "void _copy_members (const " << className << "&);" << endl;
  }
  o << endl << exdent << "private:" << indent << endl;
  o << className << " (const " << className << " &);" << endl;
  o << "void operator= (const " << className << " &);" << exdent << endl;
  o << BL_CLOSE_SEMI << endl;

  /*
   * If we have initializers, dump an _init class
   */

  CORBA::InitializerSeq_var isv = val->initializers();

  if (isv->length() > 0) {
      o << "class " << _params.class_export
      << className << "_init : public CORBA::ValueFactoryBase"
      << endl;
    o << BL_OPEN;
    o << "public:" << indent << endl;
    o << "virtual ~" << className << "_init ();" << endl;
    o << "static " << className
      << "_init * _downcast (CORBA::ValueFactory);" << endl;
    o << "void * _narrow_helper (const char *);" << endl;
    o << endl;
    for (CORBA::ULong i=0; i<isv->length(); i++) {
      o << "virtual " << className << " * " << isv[i].name << " (";
      for (CORBA::ULong j=0; j<isv[i].members.length(); j++) {
	if (j) {
	  o << ", ";
	}
	emit_type_for_param (isv[i].members[j].type_def, CORBA::PARAM_IN);
	o << " " << ID(isv[i].members[j].name);
      }
      o << ") = 0;" << endl;
    }
    
    o << endl << exdent << "protected:" << indent << endl;
    o << className << "_init ();" << endl;
    o << exdent;
    o << BL_CLOSE_SEMI << endl;
  }

  // Emit TypeCode for value
  if( _params.typecode ) {
    CORBA::Container_var def_in = val->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
	  o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
	  o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " CORBA::TypeCodeConst _tc_" << className << ";" << endl << endl;
  }

  if (!is_marshallable (val)) {
    return;
  }

  o.switchStream( 1 );
  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );
  if( _params.any ) {
    // operator<<= for value (copying)
    o << _params.operator_export << "void operator<<=( CORBA::Any &a, const ";
    emit_idl_type_name( val );
    o << " val );" << endl;
    
    // operator<<= for value (non-copying)
    o << _params.operator_export << "void operator<<=( CORBA::Any &a, ";
    emit_idl_type_name( val );
    o << "* val_ptr );" << endl;
    
    // operator>>= for value
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &a, ";
    emit_idl_type_name( val );
    o << " & val_ptr );" << endl << endl;
  }

  emit_marshal_prototype( val );
  
  o.switchStream( 0 );
  restore_name_prefix();
}

void CodeGenCPPCommon::emit_ValueBox( CORBA::ValueBoxDef_ptr vb )
{
  bool oldurn = use_rel_names (false);
  CORBA::String_var n = vb->name();
  string className = (const char *) ID(n);

  //-----COMMON------------------------------------------------------
  o << endl;
  o << "// Common definitions for valuebox " << className << endl;
  o << "class " << _params.class_export
    << className << " : ";
  o << indent << endl;
  o << "virtual public ::CORBA::ValueBase," << endl;
  o << "virtual public ::CORBA::DefaultValueRefCountBase" << exdent << endl;
  o << BL_OPEN;

  CORBA::IDLType_var boxed_value = vb->original_type_def ();
  CORBA::IDLType_var orig = CORBA::IDLType::_duplicate (boxed_value);

  while (orig->def_kind() == CORBA::dk_Alias) {
    CORBA::AliasDef_var ad = CORBA::AliasDef::_narrow (orig);
    orig = ad->original_type_def ();
  }

  CORBA::TypeCode_var orig_tc = orig->type ();
  // kcg: unalias does not return duplicated TC!
  CORBA::TypeCode_ptr tc = orig_tc->unalias ();

  switch (tc->kind()) {
  case CORBA::tk_short:
  case CORBA::tk_long:
  case CORBA::tk_ushort:
  case CORBA::tk_ulong:
  case CORBA::tk_float:
  case CORBA::tk_double:
  case CORBA::tk_boolean:
  case CORBA::tk_char:
  case CORBA::tk_octet:
  case CORBA::tk_TypeCode:
  case CORBA::tk_objref:
  case CORBA::tk_enum:
  case CORBA::tk_longlong:
  case CORBA::tk_ulonglong:
  case CORBA::tk_longdouble:
  case CORBA::tk_wchar:
    // basic types, enums and object references
    {
      o << "private:" << indent << endl;
      emit_type_for_variable (boxed_value, automatic);
      o << " value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
      emit_type_for_result (boxed_value);
      o << " _value () const;" << endl;
      o << "void _value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
    }
    break;

  case CORBA::tk_struct:
    {
      o << "private:" << indent << endl;
      emit_type_name (boxed_value);
      o << "_var value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
      o << "const ";
      emit_type_name (boxed_value);
      o << "& _value () const;" << endl;
      emit_type_name (boxed_value);
      o << "& _value ();" << endl;
      o << "void _value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;

      // generate member accessor functions
      CORBA::StructDef_var sd = CORBA::StructDef::_narrow (orig);
      CORBA::StructMemberSeq_var sv = sd->members ();
      for (CORBA::ULong i=0; i<sv->length(); i++) {
	CORBA::IDLType_var mt = CORBA::IDLType::_duplicate (sv[i].type_def);
	emit_union_member_decl (mt.in(), sv[i].name.in(), false);
      }
    }
    break;
    
  case CORBA::tk_string:
  case CORBA::tk_wstring:
    {
      string char_type;
      string string_type;

      if (tc->kind() == CORBA::tk_string) {
	char_type = "char";
	string_type = "::CORBA::String_var";
      }
      else {
	char_type = "::CORBA::WChar";
	string_type = "::CORBA::WString_var";
      }

      o << "private:" << indent << endl;
      o << string_type << " value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (" << char_type << " *);" << endl;
      o << className << " (const " << char_type << " *);" << endl;
      o << className << " (const " << string_type << " &);" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (" << char_type << " *);" << endl;
      o << className << " & operator= (const " << char_type << " *);" << endl;
      o << className << " & operator= (const " << string_type << " &);"
	<< endl;
      o << endl;
      o << "const " << char_type << " * _value () const;" << endl;
      o << "void _value (" << char_type << " *);" << endl;
      o << "void _value (const " << char_type << " *);" << endl;
      o << "void _value (const " << string_type << " &);" << endl;
      o << endl;
      o << char_type << " & operator[] (::CORBA::ULong);" << endl;
      o << char_type << " operator[] (::CORBA::ULong) const;" << endl;
    }
    break;

  case CORBA::tk_union:
    {
      o << "private:" << indent << endl;
      emit_type_name (boxed_value);
      o << "_var value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
      o << "const ";
      emit_type_name (boxed_value);
      o << "& _value () const;" << endl;
      emit_type_name (boxed_value);
      o << "& _value ();" << endl;
      o << "void _value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;

      /*
       * For union member access, we overload operator->
       */

      emit_type_name (boxed_value);
      o << "* operator->() const;" << endl;
    }
    break;

  case CORBA::tk_sequence:
    {
      CORBA::SequenceDef_var sd = CORBA::SequenceDef::_narrow (orig);
      CORBA::IDLType_var mt = sd->element_type_def();

      o << "public:" << indent << endl;
      o << "typedef ";
      // emit_type_for_variable (mt, manual) seems to be wrong, as it
      // results e.g. in a pointer to the struct instead of the struct
      // itself
      emit_type_for_variable (mt, automatic);
      o << " _elem_type;" << endl;
      o << "typedef ";
      emit_type_for_variable (mt, automatic);
      o << " _auto_elem_type;" << endl;
      o << "typedef ";
      emit_type_for_variable (boxed_value, automatic);
      o << " _value_type;" << endl;
      o << "typedef TSeqVar<_value_type> _value_var;" << endl;
      o << endl << exdent << "private:" << indent << endl;
      o << "_value_var value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (const _value_type &);" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << className << " (::CORBA::ULong);" << endl;
      o << className << " (::CORBA::ULong, ::CORBA::ULong, _elem_type *, "
	<< "::CORBA::Boolean = FALSE);" << endl;

      o << endl;
      o << className << " & operator= (const _value_type &);" << endl;
      o << endl;
      o << "const _value_type & _value () const;" << endl;
      o << "_value_type & _value ();" << endl;
      o << "void _value (const _value_type &);" << endl;

      /*
       * Inherited stuff <groan>
       */

      o << endl;
      o << "const _auto_elem_type & operator[] (::CORBA::ULong) const;" 
        << endl;
      o << "_auto_elem_type & operator[] (::CORBA::ULong);" << endl;
      o << "::CORBA::ULong maximum () const;" << endl;
      o << "::CORBA::Boolean release () const;" << endl;
      o << "void replace (::CORBA::ULong, ::CORBA::ULong, "
	<<               "_elem_type *, ::CORBA::Boolean = FALSE);" << endl;
      o << "void length (::CORBA::ULong);" << endl;
      o << "::CORBA::ULong length () const;" << endl;
      o << "static _elem_type * allocbuf (::CORBA::ULong);" << endl;
      o << "static void freebuf (_elem_type *);" << endl;
    }
    break;

  case CORBA::tk_any:
    {
      CORBA::PrimitiveDef_var pd = CORBA::PrimitiveDef::_narrow (boxed_value);

      o << "private:" << indent << endl;
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << "_var value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
      o << "const ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << "& _value () const;" << endl;
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << "& _value ();" << endl;
      o << "void _value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
    }
    break;

  case CORBA::tk_array:
    {
      /*
       * The IDL grammar does not allow valueboxes of anonymous arrays,
       * i.e. we're always referring to an outside alias. This simplifies
       * our type handling somewhat wrt sequences
       */

      o << "private:" << indent << endl;
      emit_type_name (boxed_value);
      o << "_var value;" << endl;
      o << endl << exdent << "public:" << indent << endl;
      o << className << " ();" << endl;
      o << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << className << " (const " << className << " &);" << endl;
      o << endl;
      o << className << " & operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;
      o << endl;
      o << "const ";
      emit_type_name (boxed_value);
      o << "_slice * _value () const;" << endl;
      emit_type_name (boxed_value);
      o << "_slice * _value ();" << endl;
      o << "void _value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << ");" << endl;

      o << endl;
      o << "const ";
      emit_type_name (boxed_value);
      o << "_slice & operator[] (::CORBA::ULong) const;" << endl;
      emit_type_name (boxed_value);
      o << "_slice & operator[] (::CORBA::ULong);" << endl;
    }
    break;

  case CORBA::tk_fixed:
    {
      // XXX
      assert (0);
    }
    break;

  default:
    assert (0);
  }

  // same for all
  o << endl;
  if (orig->def_kind() == CORBA::dk_Sequence) {
    o << "const _value_type & _boxed_in () const;" << endl;
    o << "_value_type & _boxed_inout ();" << endl;
    o << "_value_type *& _boxed_out ();" << endl;
  }
  else if (orig->def_kind() == CORBA::dk_Array) {
    o << "const ";
    emit_type_name (boxed_value);
    o << "_slice * _boxed_in () const;" << endl;
    emit_type_name (boxed_value);
    o << "_slice * _boxed_inout ();" << endl;
    emit_type_name (boxed_value);
    if (tc->is_variable()) {
      o << "_slice *& _boxed_out ();" << endl;
    }
    else {
      o << "_slice * _boxed_out ();" << endl;
    }
  }
  else {
    emit_type_for_param (boxed_value, CORBA::PARAM_IN);
    o << " _boxed_in () const;" << endl;
    emit_type_for_param (boxed_value, CORBA::PARAM_INOUT);
    o << " _boxed_inout ();" << endl;
    emit_type_for_param (boxed_value, CORBA::PARAM_OUT);
    o << " _boxed_out ();" << endl;
  }
  o << endl;

  // standard stuff
  o << "static " << className << "* _downcast (::CORBA::ValueBase *);" << endl;
  o << "static " << className << "* _downcast (::CORBA::AbstractBase *);"
    << endl << endl;

  o << "::CORBA::ValueDef_ptr get_value_def ();" << endl;
  o << "void * _narrow_helper (const char *);" << endl;

  if (is_marshallable (vb)) {
    o << "::CORBA::ValueBase * _copy_value ();" << endl;
    o << "void _get_marshal_info (std::vector<std::string> &, ::CORBA::Boolean &);"
      << endl;
    o << "void _marshal_members (::CORBA::DataEncoder &);" << endl;
    o << "::CORBA::Boolean _demarshal_members (::CORBA::DataDecoder &);" 
      << endl;
  }

  o << endl << exdent << "protected:" << indent << endl;
  o << "~" << className << " ();" << endl;
  o << endl << exdent << "private:" << indent << endl;
  o << "void operator= (const " << className << " &);" << endl;
  o << exdent << BL_CLOSE_SEMI << endl;

  o << "typedef ValueVar< " << className << " > "
    << className << "_var;" << endl;
  o << "typedef ValueOut< " << className << " > "
    << className << "_out;" << endl;
  o << endl;

  if (_params.typecode) {
    CORBA::Container_var def_in = vb->defined_in();
    if( def_in->def_kind() == CORBA::dk_Repository )
      // We are on the toplevel
	  o << _params.tc_export;
    else if( def_in->def_kind() == CORBA::dk_Module )
      // The current scope is not top level (inside a module)
      o << _params.tc_export;
    else
      // The current scope is not top level (not inside a module)
      o << _params.static_tc_export;
    o << " ::CORBA::TypeCodeConst _tc_" << className << ";" << endl << endl;
  }

  if (!is_marshallable (vb)) {
    return;
  }

  // operator<<= and operator>>=
  o.switchStream( 1 );
  save_name_prefix();
  set_name_prefix( CORBA::Container::_nil() );

  if( _params.any ) {
    // operator<<= for value (copying)
    o << _params.operator_export << "void operator<<=( ::CORBA::Any &a, const ";
    emit_idl_type_name( vb );
    o << " vb );" << endl;
    
    // operator<<= for value (non-copying)
    o << _params.operator_export << "void operator<<=( ::CORBA::Any &a, ";
    emit_idl_type_name( vb );
    o << "* vb_ptr );" << endl;

    // operator>>= for value
    o << "::CORBA::Boolean operator>>=( const ::CORBA::Any &a, ";
    emit_idl_type_name( vb );
    o << " & vb_ptr );" << endl << endl;
  }

  emit_marshal_prototype( vb );
  o.switchStream( 0 );
  restore_name_prefix();
  use_rel_names (oldurn);
}

/*
 * Emit definitions for PortableServer skeleton classes.
 *
 * The C++ mapping dictates that the skeleton class for an interface
 * B within module A becomes POA_A::B, which is in the POA_A namespace.
 */

void
CodeGenCPPCommon::emit_poa_obj (IRObj_ptr no)
{
  CORBA::IRObject_ptr obj = no->obj;
  
  if (check_for_included_defn (obj))
    return;

  if (obj->def_kind() != CORBA::dk_Interface &&
      obj->def_kind() != CORBA::dk_AbstractInterface &&
      obj->def_kind() != CORBA::dk_LocalInterface &&
      obj->def_kind() != CORBA::dk_Value) {
    return;
  }

  if (no->iface_as_forward) {
    return;
  }

  CORBA::Contained_var c = CORBA::Contained::_narrow ( obj );

  if (!CORBA::is_nil (c)) {
    CORBA::String_var absname = c->absolute_name ();
    if (_db->is_in_forward_dcl (absname.in())) {
      return;
    }
  }

  /*
   * Produce skeletons and ties in their proper modules
   */

  if( !CORBA::is_nil( c ) ) {
    CORBA::Container_var new_scope = c->defined_in();
    enter_scope( new_scope, "POA_" );
  }

  switch (obj->def_kind()) {
  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
  case CORBA::dk_LocalInterface:
    {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( obj );
      emit_poa_skel (in);
      emit_poa_ties (in);
    }
    break;

  case CORBA::dk_Value:
    {
      CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( obj );
      CORBA::InterfaceDefSeq_var supported = val->supported_interfaces ();
      bool supports_concrete = false;
      for (CORBA::ULong i=0; i<supported->length(); i++) {
	if (supported[i]->def_kind() == CORBA::dk_Interface) {
	  supports_concrete = true;
	  break;
	}
      }
      if (supports_concrete) {
	emit_poa_skel (val);
	emit_poa_ties (val);
      }
    }
    break;
  default:
    break;
  }
}

/*
 * PortableServer Skeleton class
 */

void
CodeGenCPPCommon::emit_poa_skel (CORBA::Container_ptr c)
{
  CORBA::Contained_var con = CORBA::Contained::_narrow (c);
  CORBA::String_var n = con->name();
  CORBA::String_var a = con->absolute_name();
  string className = (const char *) ID(n);
  string absName = (const char *) ID(a);
  absName = absName.substr (2);

  if (con->def_kind() == CORBA::dk_LocalInterface) {
    return;
  }

  string prefix;
  CORBA::Container_var def_in = con->defined_in ();
  if (def_in->def_kind() == CORBA::dk_Repository) {
    prefix = "POA_";
  }
  else {
    prefix = "";
  }

  bool oldurn = use_rel_names (false);

  CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow (c);
  CORBA::ValueDef_var val = CORBA::ValueDef::_narrow (c);

  assert (!CORBA::is_nil (in) || !CORBA::is_nil (val));

  if (!CORBA::is_nil (val)) {
    CORBA::InterfaceDefSeq_var supported = val->supported_interfaces ();
    for (CORBA::ULong i=0; i<supported->length(); i++) {
      if (supported[i]->def_kind() == CORBA::dk_Interface) {
	in = CORBA::InterfaceDef::_duplicate (supported[i]);
	break;
      }
    }
  }

  if (CORBA::is_nil (val)) {
    /*
     * Inheritance for interfaces
     */

    CORBA::InterfaceDefSeq_var bc = in->base_interfaces ();

    o << "class " << _params.class_export
      << prefix << className << " : ";
    if (_params.reflection && bc->length() == 0) {
      o << "virtual public POA_Reflection::IFRProvider" << endl;
    }
    else if (bc->length() == 0) {
      o << "virtual public PortableServer::StaticImplementation" << endl;
    } else {
      o << indent << endl;
      if (_params.reflection) {
	o << "virtual public POA_Reflection::IFRProvider," << endl;
      }
      for (CORBA::ULong i=0; i<bc->length(); i++) {
	CORBA::String_var base_name = bc[i]->absolute_name();
	string absBaseName = (const char *) ID (base_name);
	absBaseName = absBaseName.substr (2);
	o << "virtual public POA_" << absBaseName;
	if (i+1 != bc->length()) {
	  o << ",";
	}
	o << endl;
      }
      o << exdent;
    }
  }
  else {
    /*
     * Inheritance for Valuetypes that support an Interface.
     * We derive from the supported Interface's skeleton classes, and
     * from the skeleton classes of a concrete base, if it also supports
     * an interface.
     */

    o << "class " << _params.class_export
      << prefix << className << " : ";
    o << indent << endl;

    CORBA::InterfaceDefSeq_var supported;
    CORBA::String_var base_name = in->absolute_name();
    string absBaseName = (const char *) ID (base_name);
    absBaseName = absBaseName.substr (2);
    o << "virtual public POA_" << absBaseName;

    CORBA::ValueDef_var base_value = val->base_value ();
    if (!CORBA::is_nil (base_value)) {
      supported = base_value->supported_interfaces ();
      bool base_supports_concrete = false;
      for (CORBA::ULong i=0; i<supported->length(); i++) {
	if (supported[i]->def_kind() == CORBA::dk_Interface) {
	  base_supports_concrete = true;
	  break;
	}
      }
      if (base_supports_concrete) {
	base_name = base_value->absolute_name();
	absBaseName = (const char *) ID (base_name);
	absBaseName = absBaseName.substr (2);
	o << "," << endl << "virtual public POA_" << absBaseName;
      }
    }

    o << "," << endl << "virtual public " << absName;
    o << exdent << endl;
  }

  o << BL_OPEN;
  o << "public:" << indent << endl;
  o << "virtual ~" << prefix << className << " ();" << endl;

  /*
   * For Value Types, _this returns a pointer to the supported
   * concrete interface
   */

  if (!CORBA::is_nil (val)) {
    CORBA::String_var base_name = in->absolute_name();
    string absBaseName = (const char *) ID (base_name);
    absBaseName = absBaseName.substr (2);
    o << absBaseName << "_ptr _this ();" << endl;
  }
  else {
    o << absName << "_ptr _this ();" << endl;
  }
  o << "bool dispatch (CORBA::StaticServerRequest_ptr);" << endl;
  o << "virtual void invoke (CORBA::StaticServerRequest_ptr);" << endl;
  o << "virtual CORBA::Boolean _is_a (const char *);" << endl;
  o << "virtual CORBA::InterfaceDef_ptr _get_interface ();" << endl;
  o << "virtual CORBA::RepositoryId _primary_interface";
  o <<   " (const PortableServer::ObjectId &,";
  o <<    " PortableServer::POA_ptr);" << endl;
  o << endl;
  o << "virtual void * _narrow_helper (const char *);" << endl;
  if (CORBA::is_nil (val)) {
    o << "static " << prefix << className << " * _narrow";
    o <<   " (PortableServer::Servant);" << endl;
  }
  if (_params.poa_stubs) {
    o << "virtual CORBA::Object_ptr _make_stub"
      <<   " (PortableServer::POA_ptr, CORBA::Object_ptr);" << endl;
  }
  if (_params.reflection) {
    o << "virtual CORBA::Any * omg_get_ifr_metadata ()";
    if (_params.throw_decl) {
      o << " throw (Reflection::FormatNotSupported, CORBA::SystemException)";
    }
    o << ";" << endl;

    o << "virtual char * omg_get_xml_metadata ()";
    if (_params.throw_decl) {
      o << " throw (Reflection::FormatNotSupported, CORBA::SystemException)";
    }
    o << ";" << endl;
  }
  o << endl;

  /*
   * Prototypes only needed for Interfaces; Valuetypes inherit them
   * from the "main" class
   */

  if (!CORBA::is_nil (in)) {
    emitPrototypes (in, true);
  }

  o << exdent << "protected:" << indent << endl;
  o << prefix << className << " () {};" << endl << endl;
  o << exdent << "private:" << indent << endl;
  o << prefix << className << " (const " << prefix << className << " &);" << endl;
  o << "void operator= (const " << prefix << className << " &);" << endl;
  o << exdent << BL_CLOSE_SEMI << endl;

  use_rel_names (oldurn);
}

/*
 * PortableServer Tie class
 */

void
CodeGenCPPCommon::emit_poa_ties (CORBA::Container_ptr c)
{
  CORBA::Contained_var con = CORBA::Contained::_narrow (c);
  CORBA::String_var n = con->name();
  string className = (const char *) ID(n);
  string mainName = className;

  if (c->def_kind() == CORBA::dk_LocalInterface) {
    return;
  }

  if (!_params.poa_ties) {
    return;
  }

  // XXX
  if (c->def_kind() == CORBA::dk_Value) {
    return;
  }

  string prefix;
  CORBA::Container_var def_in = con->defined_in ();

  if (def_in->def_kind() == CORBA::dk_Repository) {
    prefix = (const char *) "POA_";
  }
  else {
    prefix = (const char *) "";
  }

  bool oldurn = use_rel_names (false);

  /*
   * Tie class must inherit directly or indirectly from POA_Tie_Base,
   * and from the interface's base class.
   */

  CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow (c);
  CORBA::ValueDef_var val = CORBA::ValueDef::_narrow (c);

  CORBA::InterfaceDefSeq_var bc = in->base_interfaces ();

  o << "template<class T>" << endl;
  o << "class " << _params.class_export
    << prefix << className << "_tie : " << indent << endl;
  if (bc->length() == 0) {
    o << "virtual public POA_Tie_Base<T>," << endl;
  }
  else {
    for (CORBA::ULong i=0; i<bc->length(); i++) {
      CORBA::String_var base_name = bc[i]->absolute_name();
      string absBaseName = (const char *) ID (base_name);
      absBaseName = absBaseName.substr (2);

      o << "virtual public POA_" << absBaseName << "_tie<T>," << endl;
    }
  }
  o << "virtual public " << prefix << mainName << exdent << endl;
  o << BL_OPEN;
  o << "public:" << indent << endl;
  
  o << prefix << className << "_tie (T &t)" << indent << endl;
  o << ": POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)"
    << exdent << endl << "{}" << endl;
  
  o << prefix << className << "_tie (T &t, PortableServer::POA_ptr _poa)"
    << indent << endl;
  o << ": POA_Tie_Base<T> (&t, _poa, FALSE)"
    << exdent << endl << "{}" << endl;
  
  o << prefix << className << "_tie (T *t, CORBA::Boolean _rel = TRUE)"
    << indent << endl;
  o << ": POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)"
    << exdent << endl << "{}" << endl;
  
  o << prefix << className << "_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)"
    << indent << endl;
  o << ": POA_Tie_Base<T> (t, _poa, _rel)"
    << exdent << endl << "{}" << endl;
  o << "virtual ~" << prefix << className << "_tie ();" << endl;
  o << endl;
  o << "PortableServer::POA_ptr _default_POA ();" << endl;
  o << endl;
  
  emitPrototypes (in, false);
  
  o << exdent << "protected:" << indent << endl;
  o << prefix << className << "_tie () {};" << endl << endl;
  o << exdent << "private:" << indent << endl;
  o << prefix << className << "_tie (const "
    << prefix << className << "_tie<T> &);" << endl;
  o << "void operator= (const " << prefix << className << "_tie<T> &);"
    << endl;
  o << exdent << BL_CLOSE_SEMI << endl;
  
  /*
   * PortableServer Tie Implementations
   */
  
  o << "template<class T>" << endl;
  o << prefix << className << "_tie<T>::~"
    << prefix << className << "_tie ()" << endl;
  o << BL_OPEN << BL_CLOSE << endl;
  
  o << "template<class T>" << endl;
  o << "PortableServer::POA_ptr" << endl;
  o << prefix << className << "_tie<T>::_default_POA ()" << endl;
  o << BL_OPEN;
  o << "if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) " << BL_OPEN;
  o << "return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);"
    << endl;
  o << BL_CLOSE;

  string servantbase = "PortableServer::ServantBase";
  o << "return " << servantbase << "::_default_POA ();" << endl;
  o << BL_CLOSE << endl;
  
  /*
   * Tie Delegation
   */

  CORBA::Container::DescriptionSeq_var dsv =
    in->describe_contents (CORBA::dk_Attribute, TRUE, -1);

  // Emit attributes
  for (CORBA::ULong j0=0; j0<dsv->length(); j0++) {
    const CORBA::AttributeDescription * attr;
    CORBA::Boolean r = (dsv[j0].value >>= attr);
    assert (r);
    CORBA::IDLType_var type_id = lookup_attribute_by_id (attr->id.in());

    o << "template<class T>" << endl;
    emit_type_for_result (type_id);
    o << endl;
    o << prefix << className << "_tie<T>::" << ID(attr->name) << " ()" << endl;
    o << BL_OPEN;
    o << "return POA_Tie_Base<T>::ptr->" << ID(attr->name) << " ();" << endl;
    o << BL_CLOSE << endl;

    if (attr->mode == CORBA::ATTR_NORMAL) {
      o << "template<class T>" << endl;
      o << "void" << endl;
      o << prefix << className << "_tie<T>::" << ID(attr->name) << " (";
      emit_type_for_param (type_id, CORBA::PARAM_IN);
      o << " value)" << endl;
      o << BL_OPEN;
      o << "POA_Tie_Base<T>::ptr->" << ID(attr->name) << " (value);" << endl;
      o << BL_CLOSE << endl;
    }
  }

  dsv = in->describe_contents (CORBA::dk_Operation, TRUE, -1);

  // Emit operations
  for (CORBA::ULong j1=0; j1<dsv->length(); j1++) {
    const CORBA::OperationDescription * op;
    CORBA::Boolean r = (dsv[j1].value >>= op);
    assert (r);
    CORBA::IDLType_var type_id = lookup_result_by_id (op->id.in());
    CORBA::TypeCode_var tc = type_id->type();

    o << "template<class T>" << endl;
    emit_type_for_result (type_id);
    o << endl;
    o << prefix << className << "_tie<T>::" << ID(op->name) << " (";

    CORBA::ULong k;
    for (k=0; k<op->parameters.length(); k++) {
      if( k != 0 )
	o << ", ";
      emit_type_for_param (op->parameters[k].type_def.in(), op->parameters[k].mode);
      o << " " << ID(op->parameters[k].name);
    }

    // Do we have a context?
    
    if (op->contexts.length() > 0) {
      if (op->parameters.length() > 0)
	o << ", ";
      o << "CORBA::Context_ptr _ctx";
    }

    o << ")" << endl;
    o << BL_OPEN;

    if (tc->kind() != CORBA::tk_void) {
      o << "return ";
    }

    o << "POA_Tie_Base<T>::ptr->" << ID(op->name) << " (";
    
    for (k=0; k<op->parameters.length(); k++) {
      if (k != 0)
	o << ", ";
      o << ID(op->parameters[k].name);
    }

    if (op->contexts.length() > 0) {
      if (op->parameters.length() > 0)
	o << ", ";
      o << "_ctx";
    }

    o << ");" << endl;
    o << BL_CLOSE << endl;
  }

  use_rel_names (oldurn);
}

/*
 * The C++ mapping dictates that implementation classes for a valuetype
 * B within module A becomes OBV_A::B, which is in the OBV_A namespace.
 */

void
CodeGenCPPCommon::emit_obv_obj (IRObj_ptr no)
{
  CORBA::IRObject_ptr obj = no->obj;
  
  if (check_for_included_defn (obj))
    return;

  if( obj->def_kind() != CORBA::dk_Value || no->iface_as_forward )
    return;

  CORBA::Contained_var c = CORBA::Contained::_narrow ( obj );

  if (!CORBA::is_nil (c)) {
    CORBA::String_var absname = c->absolute_name ();
    if (_db->is_in_forward_dcl (absname.in())) {
      emit_ForwardDcl (c);
      return;
    }
  }

  /*
   * Produce implementation class in its proper module
   */

  if( !CORBA::is_nil( c ) ) {
    CORBA::Container_var new_scope = c->defined_in();
    enter_scope( new_scope, "OBV_" );
  }

  switch (obj->def_kind()) {
  case CORBA::dk_Value:
    {
      CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( obj );
      assert (!CORBA::is_nil (val));
      if (!val->is_abstract()) {
	emit_obv_skel (val);
      }
      break;
    }
  default:
    break;
  }
}

void
CodeGenCPPCommon::emit_obv_skel (CORBA::ValueDef_ptr val)
{
  CORBA::String_var n = val->name();
  CORBA::String_var a = val->absolute_name();
  string className = (const char *) ID(n);
  string absName = (const char *) ID(a);
  absName = absName.substr (2);

  string prefix;
  CORBA::Container_var def_in = val->defined_in ();
  if (def_in->def_kind() == CORBA::dk_Repository) {
    prefix = "OBV_";
  }
  else {
    prefix = "";
  }

  bool oldurn = use_rel_names (false);

  /*
   * A class with default member implementations
   */

  CORBA::ValueDef_var base_value = val->base_value ();
  CORBA::ValueDefSeq_var bases = val->abstract_base_values ();
  CORBA::ContainedSeq_var members = val->contents (CORBA::dk_ValueMember, 1);

  o << endl;
  o << "// OBV class for valuetype " << className << endl;
  o << "class " << _params.class_export
    << prefix << className << " :";
  if (!CORBA::is_nil (base_value)) {
    o << indent << endl;
  }
  else {
    o << " ";
  }
  o << "virtual public " << absName;
  if (!CORBA::is_nil (base_value)) {
    CORBA::String_var base_name = base_value->absolute_name();
    string abn = ID(base_name);
    abn = abn.substr (2);
    o << "," << endl << "virtual public OBV_" << abn << exdent;
  }
  o << endl << BL_OPEN;
  vector<CORBA::ValueDef_var> parents;
  if (!CORBA::is_nil(base_value)) {
      CORBA::ValueDef_var child = val;
      CORBA::ValueDef_var parent;
      while(!CORBA::is_nil((parent = child->base_value()))) {
	  parents.push_back(parent);
	  child = parent;
      }
  }
  CORBA::Boolean has_operation = FALSE;
  CORBA::ContainedSeq_var ops = val->contents(CORBA::dk_Operation, 1);
  CORBA::ContainedSeq_var attrs = val->contents(CORBA::dk_Attribute, 1);
  if (ops->length() > 0 || attrs->length() > 0)
      has_operation = TRUE;
  if (!has_operation) {
      // search for attr or oper in parents
      for (CORBA::ULong i = 0; i < parents.size(); i++) {
	  CORBA::ContainedSeq_var ops
	      = parents[i]->contents(CORBA::dk_Operation, 1);
	  CORBA::ContainedSeq_var attrs
	      = parents[i]->contents(CORBA::dk_Attribute, 1);
	  if (ops->length() > 0 || attrs->length() > 0) {
	      has_operation = TRUE;
	      break;
	  }
      }
  }
  if (has_operation)
      o << "protected:" << indent << endl;
  else
      o << "public:" << indent << endl;
  o << prefix << className << " ();" << endl;

  if (parents.size() > 0 || members->length() > 0) {
    o << prefix << className << " (";
    CORBA::ContainedSeq_var all_members = new CORBA::ContainedSeq;;
    all_members->length(0);
    for (CORBA::Long ipar = parents.size() - 1; ipar >= 0; ipar--) {
	CORBA::ContainedSeq_var parent_members
	    = parents[ipar]->contents (CORBA::dk_ValueMember, 1);
	for (CORBA::ULong imem = 0; imem < parent_members->length(); imem++) {
	    all_members->length(all_members->length() + 1);
	    all_members[all_members->length() - 1] = parent_members[imem];
	}
    }
    for (CORBA::ULong imem = 0; imem < members->length(); imem++) {
	all_members->length(all_members->length() + 1);
	all_members[all_members->length() - 1] = members[imem];
    }
    for (CORBA::ULong i0=0; i0<all_members->length(); i0++) {
      CORBA::ValueMemberDef_var vmd =
	CORBA::ValueMemberDef::_narrow (all_members[i0]);
      CORBA::IDLType_var idt = vmd->type_def ();
      CORBA::String_var name = vmd->name ();
      CORBA::TypeCode_var tc = idt->type ();
      /*
       * Must take care of anonymous types
       */
      if (tc->is_sequence() && !tc->is_alias()) {
	o << "const ";
	emit_sequence (idt);
	o << "&";
      }
      else if (tc->is_array() && !tc->is_alias()) {
	o << "const ";
	emit_array (idt);
      }
      else {
	emit_type_for_param (idt.in(), CORBA::PARAM_IN);
      }
      o << " _" << name.in();
      if (tc->is_array() && !tc->is_alias()) {
	emit_array_suffix (idt);
      }
      if (i0+1 != all_members->length()) {
	o << ", ";
      }
    }
    o << ");" << endl;
  }
  o << "virtual ~" << prefix << className << "();" << endl;
  o << endl;

  bool pub = false;
  for (CORBA::ULong i1=0; i1<members->length(); i1++) {
    CORBA::ValueMemberDef_var vmd =
      CORBA::ValueMemberDef::_narrow (members[i1]);
    CORBA::IDLType_var idt = vmd->type_def ();
    CORBA::String_var name = vmd->name ();
    
    if (vmd->access() == CORBA::PRIVATE_MEMBER && pub) {
      o << exdent << "protected:" << indent << endl;
      pub = false;
    }
    else if (vmd->access() == CORBA::PUBLIC_MEMBER && !pub) {
      o << exdent << "public:" << indent << endl;
      pub = true;
    }

    // mapping as for union members
    emit_union_member_decl( idt.in(), name.in(), false );
  }

  o << endl << exdent << "private:" << indent << endl;
  o << "struct _M " << BL_OPEN;
  for (CORBA::ULong i2=0; i2<members->length(); i2++) {
    CORBA::ValueMemberDef_var vmd =
      CORBA::ValueMemberDef::_narrow (members[i2]);
    CORBA::IDLType_var idt = vmd->type_def ();
    CORBA::String_var name = vmd->name ();
    emit_type_for_variable (idt.in(), automatic);
    o << " " << ID(name.in());
    emit_array_suffix (idt.in());
    o << ";" << endl;
  }
  o << exdent << "} _m;" << endl;
  o << exdent << BL_CLOSE_SEMI << endl;

  use_rel_names (oldurn);
}
