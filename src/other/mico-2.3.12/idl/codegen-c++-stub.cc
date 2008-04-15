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
#include <ctype.h>
#include <stdio.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <sstream>
#else
#include <strstream.h>
#endif
#include "codegen-c++-stub.h"
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

CodeGenCPPStub::CodeGenCPPStub( DB &db, IDLParam &params,
				CORBA::Container_ptr con)
  : CodeGen( con ),
    CodeGenCPPUtil( db, params, con )
{
}



void CodeGenCPPStub::emit_stub()
{
  set_name_prefix( CORBA::Container::_nil() );

  /*
   * Only process global objs here
   */

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    if (strcmp (_idl_objs[i]->scope.c_str(), "") != 0) {
      continue;
    }
    emit_idl_obj( _idl_objs[ i ] );
  }
}

void CodeGenCPPStub::emit_idl_obj( IRObj_ptr no )
{
  if (!no->iface_as_forward) {
    emit_contained (no->obj);
  }
}

void
CodeGenCPPStub::emit_contained (CORBA::IRObject_ptr obj)
{
  if( check_for_included_defn( obj ) )
    return;

  CORBA::Contained_var con = CORBA::Contained::_narrow (obj);
  
  if (!CORBA::is_nil (con)) {
    CORBA::String_var absname = con->absolute_name ();

    if (_db->is_implicit (absname.in())) {
      return;
    }

    if (_db->is_in_forward_dcl (absname.in())) {
      return;
    }
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
      CORBA::ConstantDef_var cst = CORBA::ConstantDef::_narrow( obj );
      emit_Constant( cst );
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
      if(!is_base_sequence( s ))
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
  case CORBA::dk_Value:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow( obj );
      emit_Value( v );
      break;
    }
  case CORBA::dk_ValueBox:
    {
      CORBA::ValueBoxDef_var vb = CORBA::ValueBoxDef::_narrow( obj );
      emit_ValueBox( vb );
      break;
    }

  case CORBA::dk_Native:
    // no output necessary
    break;

  case CORBA::dk_Operation:
  case CORBA::dk_Attribute:
  case CORBA::dk_ValueMember:
  case CORBA::dk_Provides:
  case CORBA::dk_Uses:
  case CORBA::dk_Emits:
  case CORBA::dk_Consumes:
  case CORBA::dk_Publishes:
    // handled in emit_Interface
    break;

  default:
    assert( 0 );
  }
}

void
CodeGenCPPStub::emit_container( CORBA::Container_ptr obj )
{
  CORBA::Contained_var c = CORBA::Contained::_narrow (obj);
  assert (!CORBA::is_nil (c));
  CORBA::String_var scope = c->absolute_name();

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    if (!_idl_objs[i]->iface_as_forward &&
	strcmp (_idl_objs[i]->scope.c_str(), scope.in()) == 0) {
      emit_contained (_idl_objs[i]->obj);
    }
  }
}

void
CodeGenCPPStub::emit_StringDef( CORBA::StringDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_String );

  CORBA::TypeCode_var type_tc = s->type();
	  
  // Must be a bounded string
  assert( type_tc->length() > 0 );

  string name = "string_";
  name += xdec (type_tc->length());
  emit_typecode_def (CORBA::Container::_nil(), "", name, type_tc);
  o << endl;
}

void
CodeGenCPPStub::emit_WstringDef( CORBA::WstringDef_ptr ws )
{
  assert( ws->def_kind() == CORBA::dk_Wstring );

  CORBA::TypeCode_var type_tc = ws->type();
	  
  // Must be a bounded wstring
  assert( type_tc->length() > 0 );

  string name = "wstring_";
  name += xdec (type_tc->length());
  emit_typecode_def (CORBA::Container::_nil(), "", name, type_tc);
  o << endl;
}

void
CodeGenCPPStub::emit_SequenceDef( CORBA::SequenceDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_Sequence );

  if (!is_marshallable (s))
    return;

  emit_marshaller( s );

  if( !_params.any )
    return;

  CORBA::TypeCode_var type_tc = s->type();

  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
  CORBA::IDLType_var seq_elem;
  emit_sequence( s );
  o << " &_s )" << endl;
  o << BL_OPEN;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (s);
  o << ", &_s);" << endl;
  o << "_a.from_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;
	    
  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
  emit_sequence( s );
  o << " *_s )" << endl;
  o << BL_OPEN;
  o << "_a <<= *_s;" << endl;
  o << "delete _s;" << endl;
  o << BL_CLOSE << endl;
  
  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
  emit_sequence( s );
  o << " &_s )" << endl;
  o << BL_OPEN;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (s);
  o << ", &_s);" << endl;
  o << "return _a.to_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;

  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
  emit_sequence( s );
  o << " *&_s )" << endl;
  o << BL_OPEN;
  o << "return _a.to_static_any (";
  emit_marshaller_ref (s);
  o << ", (void *&)_s);" << endl;
  o << BL_CLOSE << endl << endl;
}

void
CodeGenCPPStub::emit_FixedDef( CORBA::FixedDef_ptr f )
{
  assert( f->def_kind() == CORBA::dk_Fixed );

  CORBA::TypeCode_var type_tc = f->type();

  string name = "fixed_";
  name += xdec (type_tc->fixed_digits());
  name += "_";
  if( type_tc->fixed_scale() < 0 ) {
      name += "n";
      name += xdec (-type_tc->fixed_scale());
  } else {
      name += xdec (type_tc->fixed_scale());
  }
  emit_typecode_def (CORBA::Container::_nil(), "", name, type_tc);
  o << endl;

  emit_marshaller( f );

  if( !_params.any )
    return;

  // operator <<=
  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
  emit_fixed( f );
  o << " &_f )" << endl << BL_OPEN;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (f);
  o << ", &_f);" << endl;
  o << "_a.from_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;
	    
  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
  emit_fixed( f );
  o << " *_s )" << endl;
  o << BL_OPEN;
  o << "_a <<= *_s;" << endl;
  o << "delete _s;" << endl;
  o << BL_CLOSE << endl;
  
  // operator >>=
  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
  emit_fixed( f );
  o << " &_f )" << endl << BL_OPEN;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (f);
  o << ", &_f);" << endl;
  o << "return _a.to_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;

  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
  emit_fixed( f );
  o << " *&_f )" << endl;
  o << BL_OPEN;
  o << "return _a.to_static_any (";
  emit_marshaller_ref (f);
  o << ", (void *&)_f);" << endl;
  o << BL_CLOSE << endl << endl;
}

void
CodeGenCPPStub::emit_ArrayDef( CORBA::ArrayDef_ptr a )
{
  assert( a->def_kind() == CORBA::dk_Array );

  if (is_marshallable (a)) {
    // SII marshaller
    emit_marshaller( a );
  }
}

void
CodeGenCPPStub::emit_marshaller( CORBA::IRObject_ptr obj )
{
  if ((_params.pseudo && !_params.any) ||
      obj->def_kind() == CORBA::dk_LocalInterface) {
    return;
  }

  assert( obj->def_kind() != CORBA::dk_Alias );

  string mangled_name;
  
  gen_mangled_name( mangled_name, obj, false );
  
  // Mangled name must be something like "_Marshaller_*"
  assert( mangled_name[ 0 ] == '_' && mangled_name[ 1 ] == 'M' );
  
  o << "class " << mangled_name
    << " : public ::CORBA::StaticTypeInfo ";
  o << BL_OPEN;
  o << indent;

  // generic typedef
  CORBA::TypeCode_var tc;

  if (_params.vc_sequence_reference_bug_workaround) {
      o << exdent << "public:" << endl << indent;
  }

  o << "typedef ";
  CORBA::IDLType_var t = CORBA::IDLType::_narrow( obj );
  if( !CORBA::is_nil( t ) ) {
    
    tc = t->type();
  
    if( tc->is_sequence() ) {
      emit_sequence( t );
      o << " _MICO_T";
    } else if( tc->kind() == CORBA::tk_fixed ) {
      emit_fixed( t );
      o << " _MICO_T";
    } else if( tc->is_array() ) {
      CORBA::ArrayDef_var arr = CORBA::ArrayDef::_narrow( obj );
      assert( !CORBA::is_nil( arr ) );

      t = arr->element_type_def();
      emit_type_for_variable( t, automatic );
      o << " _MICO_T";
      emit_array_suffix( t );
    } else {
      emit_idl_type_name( t );
      o << " _MICO_T";
    }
  } else {
    
    CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow( obj );
    assert( !CORBA::is_nil( e ) );
    tc = e->type();
    CORBA::String_var name = e->absolute_name();
    o << ID(name) << " _MICO_T";
  }

  o << ";" << endl;
  if (_params.vc_sequence_reference_bug_workaround) {
      o << exdent << "private:" << endl << indent;
  }

  
  // _tc class variable
  bool is_anonymous_type;
  bool has_typecode;
  switch (obj->def_kind()) {
  case CORBA::dk_Sequence:
      is_anonymous_type = true;
      has_typecode = !tc->is_recursive_seq();
      break;
  case CORBA::dk_Array:
  case CORBA::dk_Fixed:
  case CORBA::dk_String:
  case CORBA::dk_Wstring:
      is_anonymous_type = true;
      has_typecode = true;
      break;
  case CORBA::dk_Struct:
  case CORBA::dk_Union:
      has_typecode = tc->is_freestanding();
      is_anonymous_type = false;
      break;
  default:
      is_anonymous_type = false;
      has_typecode = true;
      break;
  }
  if (_params.any && is_anonymous_type && has_typecode)
      o << "static ::CORBA::TypeCode_ptr _tc;" << endl;

  
  o << exdent << "public:" << endl << indent;

  o << "~" << mangled_name << "();" << endl;

  o << "StaticValueType create () const;" << endl;
  o << "void assign (StaticValueType dst, const StaticValueType src) const;"
    << endl;
  o << "void free (StaticValueType) const;" << endl;
  if (tc->is_objref () || tc->is_abstract_interface()) {
      o << "void release (StaticValueType) const;" << endl;
  }
  o << "::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) "
    << "const;" << endl;
  o << "void marshal (::CORBA::DataEncoder &, StaticValueType) const;" << endl;
  if (_params.any && has_typecode)
    o << "::CORBA::TypeCode_ptr typecode ();" << endl;
  o << exdent << BL_CLOSE_SEMI << endl << endl;

  // dtor
  o << mangled_name << "::~" << mangled_name << "()" << endl;
  o << BL_OPEN;
  if (_params.any && has_typecode && is_anonymous_type) {
      o << "if (_tc)" << endl << indent;
      o << "delete _tc;" << endl << exdent;
  }
  o << BL_CLOSE << endl;

  // create
  o << "::CORBA::StaticValueType " << mangled_name << "::create() const" 
    << endl;
  o << BL_OPEN;
  o << "return (StaticValueType) ";
  if( tc->is_array() ) {
    CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow( obj );
    assert( !CORBA::is_nil( a ) );
    CORBA::ULong len = a->length();
    o << "new _MICO_T[ " << len << " ]";
  } else {
    o << "new _MICO_T";
    if( tc->is_objref() || tc->is_valuetype() || tc->is_abstract_interface())
      o << "( 0 )";
  }
  o << ";" << endl;
  o << BL_CLOSE << endl;

  // assign
  o << "void " << mangled_name
    << "::assign( StaticValueType d, const StaticValueType s ) const" << endl;
  o << BL_OPEN;
  if( tc->is_array() ) {
    // Determine the base type of the array (i.e. remove all
    // aliases and array declarations)
    CORBA::IDLType_var base_type = CORBA::IDLType::_narrow( obj );
    assert( !CORBA::is_nil( base_type ) );
    CORBA::TypeCode_var base_tc = base_type->type();
    CORBA::ULong len = compute_num_array_elem( base_tc );
    
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

    o << "for( int i = 0; i < " << len << "; i++ )" << endl;
    o << indent << "((";
    emit_type_for_variable( base_type, automatic );
    o << " *) d)[ i ] = ((";
    emit_type_for_variable( base_type, automatic );
    o << " *) s)[ i ]" << exdent;
  } else {
    if( tc->is_valuetype() ) {
      o << "::CORBA::remove_ref (*(_MICO_T*)d);" << endl;
      o << "::CORBA::add_ref (*(_MICO_T*)s);" << endl;
    }
    o << "*(_MICO_T*) d = ";
    if( tc->is_objref() || tc->is_abstract_interface() ) {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( obj );
      CORBA::String_var name = in->absolute_name();
      o << ID(name) << "::_duplicate( *(_MICO_T*) s )";
    } else {
      o << "*(_MICO_T*) s";
    }
  }
  o << ";" << endl;
  o << BL_CLOSE << endl;
  
  // free
  o << "void " << mangled_name << "::free( StaticValueType v ) const" << endl;
  o << BL_OPEN;
  if( tc->is_array() ) {
    o << "delete[] (_MICO_T *) v";
  } else {
    if( tc->is_objref() || tc->is_abstract_interface() ) {
      o << "::CORBA::release( *(_MICO_T *) v );" << endl;
    }
    else if( tc->is_valuetype() ) {
      o << "::CORBA::remove_ref (*(_MICO_T*)v);" << endl;
    }
    o << "delete (_MICO_T*) v";
  }
  o << ";" << endl;
  o << BL_CLOSE << endl;
  
  // release
  if (tc->is_objref () || tc->is_abstract_interface()) {
      o << "void " << mangled_name << "::release( StaticValueType v ) const"
	<< endl;
      o << BL_OPEN;
      o << "::CORBA::release( *(_MICO_T *) v );" << endl;
      o << BL_CLOSE << endl;
  }
  
  // demarshal
  o << "::CORBA::Boolean " << mangled_name
    << "::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const"
    << endl;
  o << BL_OPEN;

  switch( obj->def_kind() ) {
  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow( obj );
      CORBA::StructMemberSeq_var mem = s->members();
      
      o << "return" << endl << indent;
      o << "dc.struct_begin() &&" << endl;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	emit_marshaller_ref( mem[ i ].type_def );
	o << "->demarshal( dc, &((_MICO_T*)v)->" << ID(mem[ i ].name);
	emit_marshaller_suffix( mem[i].type_def, TRUE );
	o << " ) &&" << endl;
      }
      o << "dc.struct_end();" << endl << exdent;
      break;
    }
  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow( obj );
      CORBA::UnionMemberSeq_var mem = u->members();
      CORBA::IDLType_var disc = u->discriminator_type_def();
      
      o << "if( !dc.union_begin() )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      o << "if( !";
      emit_marshaller_ref( disc );
      o << "->demarshal( dc, &((_MICO_T *) v)->_discriminator";
      emit_marshaller_suffix( disc, TRUE );
      o << " ) )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      
      o << "switch( ((_MICO_T *) v)->_discriminator ) " << BL_OPEN;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	CORBA::UnionMember &m = mem[ i ];
	CORBA::TypeCode_var tc = m.label.type();
	if( tc->kind() == CORBA::tk_octet ) {
	  o << "default";
	} else {
	  o << "case ";
	  emit_union_case_label( disc.in(), m, mem.inout() );
	}
	o << ":" << indent << endl;
	o << "if( !";
	emit_marshaller_ref( m.type_def );
	o << "->demarshal( dc, &((_MICO_T *) v)->_m." << ID(m.name);
	emit_marshaller_suffix( m.type_def, TRUE );
	o << " ) )" << endl;
	o << indent << "return FALSE;" << exdent << endl;
	o << "break;" << exdent << endl;
      }
      o << BL_CLOSE;
      o << "return dc.union_end();" << endl;
      break;
    }
  case CORBA::dk_Enum:
    {
      o << "::CORBA::ULong ul;" << endl;
      o << "if( !dc.enumeration( ul ) )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      o << "*(_MICO_T*) v = (_MICO_T) ul;" << endl;
      o << "return TRUE;" << endl;
      break;
    }
  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( obj );
      CORBA::IDLType_var elem = s->element_type_def();
      CORBA::TypeCode_var tc = elem->type();
      string mname;

      o << "::CORBA::ULong len;" << endl;
      o << "if( !dc.seq_begin( len ) )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      o << "((_MICO_T *) v)->length( len );" << endl;

      if (use_builtin_marshaller (tc, mname)) {
	o << "if (len > 0) " << BL_OPEN;
	o << "if (!dc.get_" << mname << " (&(*(_MICO_T *)v)[0], len))" << endl;
	o << indent << "return FALSE;" << exdent << endl;
	o << BL_CLOSE;
      } else {
	o << "for( ::CORBA::ULong i = 0; i < len; i++ ) " << BL_OPEN;
	o << "if( !";
	emit_marshaller_ref( elem );
        if (_params.vc_sequence_reference_bug_workaround) {
            CORBA::IDLType_var el = resolve_alias (elem);
            if (el->def_kind() == CORBA::dk_Array) {
                o << "->demarshal( dc, &(";
                emit_marshaller_ref(elem, false);
                o << "::_MICO_T&)(*(_MICO_T*)v)[i]";
            }
            else {
                o << "->demarshal( dc, &(*(_MICO_T*)v)[i]";
            }
        }
        else {
            o << "->demarshal( dc, &(*(_MICO_T*)v)[i]";
        }
	emit_marshaller_suffix( elem, TRUE );
	o << " ) )" << endl;
	o << indent << "return FALSE;" << exdent << endl;
	o << BL_CLOSE;
      }
      o << "return dc.seq_end();" << endl;
      break;
    }
  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow( obj );
      CORBA::IDLType_var elem = a->element_type_def();
      CORBA::TypeCode_var tc = elem->type();
      CORBA::ULong len = a->length();
      string mname;

      o << "if( !dc.arr_begin() )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      
      if (use_builtin_marshaller (tc, mname)) {
	if (len > 0) {
	  o << "if (!dc.get_" << mname << " (&((_MICO_T *)v)[0], "
	    << len << "))" << endl;
	  o << indent << "return FALSE;" << exdent << endl;
	}
      } else {
        o << "for( ::CORBA::ULong i = 0; i < " << len << "; i++ ) " << BL_OPEN;
	o << "if( !";
	emit_marshaller_ref( elem );
	o << "->demarshal( dc, &((_MICO_T*)v)[i]";
	emit_marshaller_suffix( elem, TRUE );
	o << " ) )" << endl;
	o << indent << "return FALSE;" << exdent << endl;
	o << BL_CLOSE;
      }
      o << "return dc.arr_end();" << endl;
      break;
    }
  case CORBA::dk_Fixed:
    {
      CORBA::FixedDef_var f = CORBA::FixedDef::_narrow( obj );
      o << "FixedBase::FixedValue val;" << endl;
      o << "if( !dc.get_fixed( val, " << f->digits() << ", "
	<< f->scale() << " ) )" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      o << "((_MICO_T *) v)->from_digits( val );" << endl;
      o << "return TRUE;" << endl;
      break;
    }
  case CORBA::dk_Interface:
    {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( obj );
      CORBA::String_var name = in->absolute_name();
      CORBA::String_var id = in->id ();
      o << "::CORBA::Object_ptr obj;" << endl;
      o << "if (!::CORBA::_stc_Object->demarshal(dc, &obj))" << endl;
      o << indent << "return FALSE;" << exdent << endl;
      o << "*(_MICO_T *) v = ";
      o << ID(name) << "::_narrow( obj );" << endl;
      o << "::CORBA::Boolean ret = ::CORBA::is_nil (obj) || "
	<< "!::CORBA::is_nil (*(_MICO_T *)v);" << endl;
      o << "::CORBA::release (obj);" << endl;
      o << "return ret;" << endl;
      break;
    }
  case CORBA::dk_AbstractInterface:
    {
      CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( obj );
      CORBA::String_var name = in->absolute_name();
      CORBA::String_var id = in->id ();
      o << "::CORBA::AbstractBase_ptr ab;" << endl;
      o << "if (!::CORBA::AbstractBase::_demarshal (dc, ab)) "
	<< BL_OPEN;
      o << "return FALSE;" << endl << BL_CLOSE;
      o << "*(_MICO_T *) v = ";
      o << ID(name) << "::_narrow( ab );" << endl;
      o << "::CORBA::Boolean ret = ::CORBA::is_nil (ab) || "
	<< "!::CORBA::is_nil (*(_MICO_T *)v);" << endl;
      o << "::CORBA::release (ab);" << endl;
      o << "return ret;" << endl;
      break;
    }
  case CORBA::dk_Exception:
    {
      CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow( obj );
      CORBA::StructMemberSeq_var mem = e->members();
      
      o << "string repoid;" << endl;
      o << "return" << endl << indent;
      o << "dc.except_begin( repoid ) &&" << endl;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	emit_marshaller_ref( mem[ i ].type_def );
	o << "->demarshal( dc, &((_MICO_T*)v)->" << ID(mem[ i ].name);
	emit_marshaller_suffix( mem[i].type_def, TRUE );
	o << " ) &&" << endl;
      }
      o << "dc.except_end();" << endl << exdent;
      break;
    }
  case CORBA::dk_Value:
  case CORBA::dk_ValueBox:
    {
      CORBA::Contained_var c = CORBA::Contained::_narrow ( obj );
      CORBA::String_var name = c->absolute_name();
      CORBA::String_var id = c->id ();

      o << "::CORBA::ValueBase* vb = ";
      if ( obj->def_kind() == CORBA::dk_ValueBox ) {
        o << "new " << ID(name);
      }
      else {
	o << "NULL";
      }
      o << ";" << endl;
      
      o << "if (!::CORBA::ValueBase::_demarshal (dc, vb, \""
	<< id << "\")) " << BL_OPEN;
      o << "return FALSE;" << endl << BL_CLOSE;
      o << "::CORBA::remove_ref (*(_MICO_T *)v);" << endl;
      o << "*(_MICO_T *)v = " << ID(name) << "::_downcast (vb);" << endl;
      o << "if (vb && !*(_MICO_T *)v) " << BL_OPEN;
      o << "::CORBA::remove_ref (vb);" << endl;
      o << "return FALSE;" << endl;
      o << BL_CLOSE;
      o << "return TRUE;" << endl;
      break;
    }

  default:
    assert( 0 );
  }
  o << BL_CLOSE << endl;
  
  // marshal
  o << "void " << mangled_name
    << "::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const"
    << endl;
  o << BL_OPEN;

  switch( obj->def_kind() ) {
  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow( obj );
      CORBA::StructMemberSeq_var mem = s->members();
      
      o << "ec.struct_begin();" << endl;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	emit_marshaller_ref( mem[ i ].type_def );
	o << "->marshal( ec, &((_MICO_T*)v)->" << ID(mem[ i ].name);
	emit_marshaller_suffix( mem[i].type_def, FALSE );
	o << " );" << endl;
      }
      o << "ec.struct_end();" << endl;
      break;
    }
  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow( obj );
      CORBA::UnionMemberSeq_var mem = u->members();
      CORBA::IDLType_var disc = u->discriminator_type_def();
      
      o << "ec.union_begin();" << endl;
      emit_marshaller_ref( disc );
      o << "->marshal( ec, &((_MICO_T *) v)->_discriminator";
      emit_marshaller_suffix( disc, FALSE );
      o << " );" << endl;
      
      o << "switch( ((_MICO_T *) v)->_discriminator ) " << BL_OPEN;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	CORBA::UnionMember &m = mem[ i ];
	CORBA::TypeCode_var tc = m.label.type();
	if( tc->kind() == CORBA::tk_octet ) {
	  o << "default";
	} else {
	  o << "case ";
	  emit_union_case_label( disc.in(), m, mem.inout() );
	}
	o << ":" << indent << endl;
	emit_marshaller_ref( m.type_def );
	o << "->marshal( ec, &((_MICO_T *) v)->_m." << ID(m.name);
	emit_marshaller_suffix( m.type_def, FALSE );
	o << " );" << endl;
	o << "break;" << exdent << endl;
      }
      o << BL_CLOSE;
      o << "ec.union_end();" << endl;
      break;
    }
  case CORBA::dk_Enum:
    {
      o << "ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );" << endl;
      break;
    }
  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( obj );
      CORBA::IDLType_var elem = s->element_type_def();
      CORBA::TypeCode_var tc = elem->type();
      string mname;
      
      o << "::CORBA::ULong len = ((_MICO_T *) v)->length();" << endl;
      o << "ec.seq_begin( len );" << endl;

      if (use_builtin_marshaller (tc, mname)) {
	o << "if (len > 0) " << BL_OPEN;
	o << "ec.put_" << mname << " (&(*(_MICO_T *)v)[0], len);" << endl;
	o << BL_CLOSE;
      } else {
	o << "for( ::CORBA::ULong i = 0; i < len; i++ )" << endl;
	o << indent;
	emit_marshaller_ref( elem );
        if (_params.vc_sequence_reference_bug_workaround) {
            CORBA::IDLType_var el = resolve_alias (elem);
            if (el->def_kind() == CORBA::dk_Array) {
                o << "->marshal( ec, &(";
                emit_marshaller_ref(elem, false);
                o << "::_MICO_T&)(*(_MICO_T*)v)[i]";
            }
            else {
                o << "->marshal( ec, &(*(_MICO_T*)v)[i]";
            }
        }
        else {
            o << "->marshal( ec, &(*(_MICO_T*)v)[i]";
        }
	emit_marshaller_suffix( elem, FALSE );
	o << " );" << endl;
	o << exdent;
      }
      o << "ec.seq_end();" << endl;
      break;
    }
  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow( obj );
      CORBA::IDLType_var elem = a->element_type_def();
      CORBA::TypeCode_var tc = elem->type();
      CORBA::ULong len = a->length();
      string mname;
      
      o << "ec.arr_begin();" << endl;

      if (use_builtin_marshaller (tc, mname)) {
	if (len > 0) {
	  o << "ec.put_" << mname << " (&((_MICO_T *)v)[0], " << len << ");"
	    << endl;
	}
      } else {
        o << "for( ::CORBA::ULong i = 0; i < " << len << "; i++ )" << endl;
	o << indent;
	emit_marshaller_ref( elem );
	o << "->marshal( ec, &((_MICO_T*)v)[i]";
	emit_marshaller_suffix( elem, FALSE );
	o << " );" << endl;
	o << exdent;
      }
      o << "ec.arr_end();" << endl;
      break;
    }
  case CORBA::dk_Fixed:
    {
      CORBA::FixedDef_var f = CORBA::FixedDef::_narrow( obj );
      o << "FixedBase::FixedValue_var val = "
	<< "((_MICO_T *) v)->to_digits(); " << endl;
      o << "ec.put_fixed( val.in(), " << f->digits() << ", "
	<< f->scale() << " );" << endl;
      break;
    }
  case CORBA::dk_Interface:
    {
      o << "::CORBA::Object_ptr obj = *(_MICO_T *) v;" << endl;
      o << "::CORBA::_stc_Object->marshal( ec, &obj );" << endl;
      break;
    }
  case CORBA::dk_AbstractInterface:
    {
      o << "::CORBA::AbstractBase::_marshal (ec, *(_MICO_T *)v);" << endl;
      break;
    }
  case CORBA::dk_Exception:
    {
      CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow( obj );
      CORBA::StructMemberSeq_var mem = e->members();
      CORBA::String_var id = e->id();
      
      o << "ec.except_begin( \"" << id << "\" );" << endl;
      for( CORBA::ULong i = 0; i < mem->length(); i++ ) {
	emit_marshaller_ref( mem[ i ].type_def );
	o << "->marshal( ec, &((_MICO_T*)v)->" << ID(mem[ i ].name);
	emit_marshaller_suffix( mem[i].type_def, FALSE );
	o << " );" << endl;
      }
      o << "ec.except_end();" << endl;
      break;
    }
  case CORBA::dk_Value:
  case CORBA::dk_ValueBox:
    {
      o << "::CORBA::ValueBase::_marshal (ec, *(_MICO_T *)v);" << endl;
      break;
    }

  default:
    assert( 0 );
  }
  o << BL_CLOSE << endl;


  // typecode
  if (_params.any && has_typecode) {
      o << "::CORBA::TypeCode_ptr " << mangled_name << "::typecode()" << endl;
      o << BL_OPEN;
      if (is_anonymous_type) {
	  o << "if (!_tc)" << endl << indent;
	  o << "_tc = (new ::CORBA::TypeCode (" << endl;
	  emit_type_code (tc);
	  o << "))->mk_constant();" << endl << exdent;
	  o << "return _tc;" << endl;
      } else {
	  o << "return ";
	  emit_type_code_name (obj);
	  o << ";" << endl;
      }
      o << BL_CLOSE << endl;
  }

  // _tc class member def
  if (_params.any && is_anonymous_type && has_typecode)
      o << "::CORBA::TypeCode_ptr " << mangled_name << "::_tc = 0;" << endl;


  // Generate Marshaller definition
  o << "::CORBA::StaticTypeInfo *";
  emit_marshaller_ref( obj );
  o << ";" << endl << endl;

  // Generate Marshaller instance
#ifdef WINDOWS_TC
  o.switchStream( 1 );

  emit_marshaller_ref( obj, true );
  o << " = new ";
  emit_marshaller_ref( obj, false );
  o << ";" << endl;

  o.switchStream( 3 );
  o << "delete static_cast<";
  emit_marshaller_ref(obj, false);
  o << "*>(";
  emit_marshaller_ref(obj, true);
  o << ");" << endl;

  o.switchStream( 0 );
#else
#error Must use WINDOWS_TC here
#endif
}


void CodeGenCPPStub::emit_sii_stub_method( string &absClassName,
					   string &dispatch_name,
					   const CORBA::OperationDescription &op,
					   CORBA::IDLType_ptr result )
{
  CORBA::TypeCode_var tc_result = result->type();
  emit_type_for_result( result );
  o << " " << absClassName << _STUB << "::" << ID(op.name) << "(";
  const CORBA::ParDescriptionSeq &p = op.parameters;
  CORBA::ULong k;
  for( k = 0; k != p.length(); k++ ) {
    if( k != 0 )
      o << ",";
    o << " ";
    emit_type_for_param( p[ k ].type_def.in(), p[ k ].mode );
    o << " _par_" << ID(p[ k ].name);
  }

  // Do we have a context?
  const CORBA::ContextIdSeq& ctx = op.contexts;
    
  if( ctx.length() > 0 ) {
    if( p.length() > 0 )
      o << ", ";
    o << "CORBA::Context_ptr _ctx";
  }
    
  if( k != 0 || ctx.length() > 0 )
    o << " ";
  o << ")" << endl;
  o << BL_OPEN;

  // Generate StaticAny's for arguments and result
  for( k = 0; k < p.length(); k++ ) {
    o << "CORBA::StaticAny _sa_" << ID(p[ k ].name) << "( ";
    emit_marshaller_ref( p[ k ].type_def.in() );

    if( p[k].mode != CORBA::PARAM_OUT || !is_variable_for_sii( p[k].type.in() ) ) {
      o << ", ";
      if( !p[k].type->is_array() )
	o << "&";
      o << "_par_" << ID(p[ k ].name);
      if (p[k].mode == CORBA::PARAM_OUT && p[k].type->is_variable())
	o << ".ptr()";
    }
    
    o << " );" << endl;
  }

  // Generate variable for result (if there is one)
  if( tc_result->kind() != CORBA::tk_void ) {
    // arrays are always variable as return types ...
    if( !(is_variable_for_sii( tc_result ) || tc_result->is_array() ) ) {
      if( tc_result->is_array() ) {
        emit_type_for_variable( result, automatic );
	o << "_slice* _res;" << endl;
      } else {
        emit_type_for_variable( result, manual );
	o << " _res";
	emit_variable_initialization( result, manual );
	o << ";" << endl;
      }
      o << "CORBA::StaticAny __res( ";
      emit_marshaller_ref( result );
      o << ", &_res );";
    } else {
      o << "CORBA::StaticAny __res( ";
      emit_marshaller_ref( result );
      o << " );";
    }
    o << endl << endl;
  }
  
  // Generate marshalling code
  o << "CORBA::StaticRequest __req( this, \"" << dispatch_name
    << "\" );" << endl;

  // Marshall context
  if( ctx.length() > 0 ) {
    o << "__req.set_context( _ctx );" << endl;
    o << "CORBA::ContextList_var _clist = new CORBA::ContextList;" << endl;
    for( k = 0; k < ctx.length(); k++ )
      o << "_clist->add( \"" << ctx[ k ] << "\" );" << endl;
    o << "__req.set_context_list( _clist );" << endl;
  }

  // Marshall actual parameters
  for( k = 0; k < p.length(); k++ ) {
    o << "__req.add_";
    switch( p[ k ].mode ) {
    case CORBA::PARAM_IN:
      o << "in";
      break;
    case CORBA::PARAM_OUT:
      o << "out";
      break;
    case CORBA::PARAM_INOUT:
      o << "inout";
      break;
    default:
      assert( 0 );
    }
    o << "_arg( &_sa_" << ID(p[ k ].name) << " );" << endl;
  }
  if (tc_result->kind() != CORBA::tk_void)
    o << "__req.set_result( &__res );" << endl;
  o << endl;

  if( op.mode == CORBA::OP_NORMAL )
    o << "__req.invoke();" << endl << endl;
  else
    o << "__req.oneway();" << endl << endl;

  if( !_params.no_exceptions ) {
    o << "mico_sii_throw( &__req, " << endl << indent;
    CORBA::ExcDescriptionSeq exs = op.exceptions;
    for( CORBA::ULong i = 0; i < exs.length(); i++ ) {
      CORBA::Contained_var con = _repo->lookup_id( exs[ i ].id );
      assert( !CORBA::is_nil( con ) );
      CORBA::String_var tmp = con->absolute_name();
      CORBA::String_var ex_abs_name = (const char *) ID(tmp);
      string ex_scope;
      ex_scope = ex_abs_name;
      int idx = ex_scope.find_last_of( ":" );
      assert( idx > 0 );
      ex_scope = ex_scope.substr( 0, idx - 1 );
      if( ex_scope == "::" )
        ex_scope = "";

      emit_marshaller_ref( con );
      o << ", \"" << exs[i].id << "\"," << endl;
    }
    o << "0);" << endl << exdent;
  } else {
    o << BL_OPEN;
    o << "CORBA::Exception *_ex;" << endl;
    o << "if( (_ex = __req.exception()) )" << endl;
    o << indent << "CORBA::Exception::_throw_failed( _ex );" << exdent << endl;
    o << BL_CLOSE;
  }

  // Write back out parameters
  for( k = 0; k < p.length(); k++ ) {
    if( p[k].mode == CORBA::PARAM_OUT && is_variable_for_sii( p[k].type.in() ) ) {
      o << "_par_" << ID(p[ k ].name) << " = ";
      o << "(";
      emit_idl_type_name( p[k].type_def.in() );
      if( p[k].type->is_array() )
	o << "_slice";
      o << "*) _sa_" << ID(p[k].name) << "._retn();" << endl;
    }
  }

  // Write back result (if there is one)
  if( tc_result->kind() != CORBA::tk_void ) {
    o << "return ";
    // arrays are always variable as return types ...
    if( is_variable_for_sii( tc_result ) || tc_result->is_array() ) {
      o << "(";
      emit_idl_type_name( result );
      if( tc_result->is_array() )
	o << "_slice";
      o << "*) __res._retn();";
    } else {
      o << "_res;";
    }
    o << endl;
  }

  o << BL_CLOSE << endl << endl;
}

void
CodeGenCPPStub::emit_poa_stub_method (string &absClassName,
				      string &dispatch_name,
				      const CORBA::OperationDescription &op,
				      CORBA::IDLType_ptr result)
{
  o << "#ifndef MICO_CONF_NO_POA" << endl << endl;

  CORBA::TypeCode_var tc_result = result->type();
  emit_type_for_result( result );
  o << endl << absClassName << _STUB << "_clp::" << ID(op.name) << "(";
  const CORBA::ParDescriptionSeq &p = op.parameters;
  CORBA::ULong i, j;

  for( i = 0; i != p.length(); i++ ) {
    if( i != 0 )
      o << ",";
    o << " ";
    emit_type_for_param( p[ i ].type_def.in(), p[ i ].mode );
    o << " _par_" << ID(p[ i ].name);
  }

  // Do we have a context?
  const CORBA::ContextIdSeq& ctx = op.contexts;
    
  if( ctx.length() > 0 ) {
    if( p.length() > 0 )
      o << ", ";
    o << "CORBA::Context_ptr _ctx";
  }
    
  if( i != 0 || ctx.length() > 0 )
    o << " ";
  o << ")" << endl;
  o << BL_OPEN;

  /*
   * If valuetypes may be shared across parameters, we give up and
   * just fall back to using the stub.
   */

  if (check_simple_clp_call (op) &&
      (tc_result->is_valuetype() || !contains_a_valuetype (result))) {

    /*
     * Test if we can invoke a colocated servant
     */

    o << "PortableServer::Servant _serv = _preinvoke ();" << endl;
    o << "if (_serv) " << BL_OPEN;
    o << "POA_" << absClassName << " * _myserv = ";
    o << "POA_" << absClassName << "::_narrow (_serv);" << endl;
    o << "if (_myserv) " << BL_OPEN;
    
    if (tc_result->kind() != CORBA::tk_void) {
      if (tc_result->is_array()) {
	emit_type_for_variable (result, automatic);
	o << "_slice *";
      }
      else {
	emit_type_for_variable (result, manual);
      }
      o << " __res;" << endl << endl;
    }
    
    /*
     * in and inout Valuetypes must be copied to preserve location trans-
     * parency. Abstract Interfaces, too, if they incarnate a value.
     *
     * Take care of parameter sharing among valuetypes. We don't yet handle
     * parameter sharing across valuetypes and abstract interfaces.
     *
     * We also don't handle valuetypes contained in a structured type
     * (struct, union, sequence, array). So if this operation takes a
     * parameter with some structured type that contains a valuetype,
     * we do not enter this block at all but fall back to marshalling
     * everything, using the stub; see check_simple_clp_call() above.
     * The return value of check_simple_clp_call() must be well-adjusted
     * to our capabilities here.
     *
     * Not yet implemented is the possibility to marshal and unmarshal
     * all (or just the selected) parameters using a SimpleBuffer, which
     * should still be much faster than going all the way through the ORB.
     * But this is already much too complex for my taste.
     */
    
    for (i=0; i<p.length(); i++) {
      if (p[i].type->is_valuetype() || p[i].type->is_abstract_interface()) {
	emit_type_for_variable (p[i].type_def.in(), manual);
	o << " _copy_of_par_" << ID(p[i].name) << ";" << endl;
      }

      if (p[i].mode == CORBA::PARAM_IN || p[i].mode == CORBA::PARAM_INOUT) {
	if (p[i].type->is_valuetype()) {
	  /*
	   * Check if we've already seen this parameter
	   */
	  bool have_check = false;
	  for (j=0; j<i; j++) {
	    if (p[j].mode == CORBA::PARAM_IN ||
		p[j].mode == CORBA::PARAM_INOUT) {
	      /*
	       * Parameter type may be an alias
	       */
	      CORBA::ValueDef_var v1, v2;
	      CORBA::String_var id1, id2;
	      v1 = CORBA::ValueDef::_narrow (p[i].type_def.in());
	      v2 = CORBA::ValueDef::_narrow (p[j].type_def.in());
	      if (!CORBA::is_nil (v1)) {
		id1 = v1->id ();
	      }
	      if (!CORBA::is_nil (v2)) {
		id2 = v2->id ();
	      }
	      if (p[j].type->is_valuetype() &&
		  (CORBA::is_nil (v1) || CORBA::is_nil (v2) ||
		   v1->is_a (id2.in()) || v2->is_a (id1.in()))) {
		if (have_check) {
		  o << "else ";
		}
		o << "if (((CORBA::ValueBase*)_par_" << ID(p[i].name);
		o << ")==((CORBA::ValueBase*)_par_" << ID(p[j].name)
		  << ")) " << BL_OPEN;
		o << "_copy_of_par_" << ID(p[i].name) << " = ";
		if (p[i].type_def->def_kind() != CORBA::dk_Primitive) {
		  emit_type_name (p[i].type_def.in());
		  o << "::_downcast (_copy_of_par_" << ID(p[j].name) << ");"
		    << endl;
		}
		else {
		  o << "_par_" << ID(p[j].name) << ";" << endl;
		}
		o << "CORBA::add_ref (_copy_of_par_" << ID(p[i].name) << ");"
		  << endl;
		o << BL_CLOSE;
		have_check = true;
	      }
	    }
	  }
	  if (have_check) {
	    o << "else " << BL_OPEN;
	  }
	  o << "_copy_of_par_" << ID(p[i].name) << " = ";
	  if (p[i].type_def->def_kind() != CORBA::dk_Primitive) {
	    emit_type_name (p[i].type_def.in());
	    o << "::_downcast (_par_" << ID(p[i].name) << "->_copy_value());"
	      << endl;
	  }
	  else {
	    o << "_par_" << ID(p[i].name) << "->_copy_value();" << endl;
	  }
	  if (have_check) {
	    o << BL_CLOSE;
	  }
	}
	else if (p[i].type->is_abstract_interface()) {
	  o << "if (_par_" << ID(p[i].name) << " && ";
	  o << "_par_" << ID(p[i].name) << "->_to_value()) " << BL_OPEN;
	  o << "_copy_of_par_" << ID(p[i].name) << " = ";
	  emit_type_name (p[i].type_def.in());
	  o << "::_downcast (_par_" << ID(p[i].name)
	    << "->_to_value()->_copy_value());" << endl;
	  o << BL_CLOSE << "else " << BL_OPEN;
	  o << "_copy_of_par_" << ID(p[i].name) << " = ";
	  emit_type_name (p[i].type_def.in());
	  o << "::_duplicate (_par_" << ID(p[i].name) << ");" << endl;
	  o << BL_CLOSE;
	}
      }
    }
    
    if (!_params.no_exceptions && op.mode == CORBA::OP_NORMAL) {
      o << "#ifdef HAVE_EXCEPTIONS" << endl;
      o << "try {" << endl;
      o << "#endif" << indent << endl;
    }
    
    if (tc_result->kind() != CORBA::tk_void) {
      o << "__res = ";
    }
    o << "_myserv->" << ID(op.name) << "(";
    for (i=0; i < p.length(); i++) {
      if (i) {
	o << ", ";
      }
      if (p[i].type->is_valuetype() || p[i].type->is_abstract_interface()) {
	o << "_copy_of";
      }
      o << "_par_" << ID(p[i].name);
    }
    if (ctx.length() > 0) {
      if (p.length() > 0) {
	o << ", ";
      }
      o << "_ctx";
    }
    o << ");" << endl;
    
    if (!_params.no_exceptions && op.mode == CORBA::OP_NORMAL) {
      o << exdent << "#ifdef HAVE_EXCEPTIONS" << endl;
      o << "}" << endl;
      o << "catch (...) " << BL_OPEN;
      o << "_myserv->_remove_ref();" << endl;
      o << "_postinvoke();" << endl;
      o << "throw;" << endl;
      o << BL_CLOSE;
      o << "#endif" << endl << endl;
    }
    
    o << "_myserv->_remove_ref();" << endl;
    o << "_postinvoke ();" << endl;
    
    /*
     * Write back inout and out parameters. We must take care of shared
     * values here, too.
     */
    
    for (i=0; i<p.length(); i++) {
      if (p[i].mode == CORBA::PARAM_OUT || p[i].mode == CORBA::PARAM_INOUT) {
	if (p[i].type->is_valuetype()) {
	  /*
	   * Check if we've already seen this parameter
	   */
	  bool have_check = false;
	  for (j=0; j<i; j++) {
	    if (p[j].mode == CORBA::PARAM_OUT ||
		p[j].mode == CORBA::PARAM_INOUT) {
	      /*
	       * Parameter type may be an alias
	       */
	      CORBA::ValueDef_var v1, v2;
	      CORBA::String_var id1, id2;
	      v1 = CORBA::ValueDef::_narrow (p[i].type_def.in());
	      v2 = CORBA::ValueDef::_narrow (p[j].type_def.in());
	      if (!CORBA::is_nil (v1)) {
		id1 = v1->id ();
	      }
	      if (!CORBA::is_nil (v2)) {
		id2 = v2->id ();
	      }
	      if (p[j].type->is_valuetype() &&
		  (CORBA::is_nil (v1) || CORBA::is_nil (v2) ||
		   v1->is_a (id2.in()) || v2->is_a (id1.in()))) {
		if (have_check) {
		  o << "else ";
		}
		o << "if (((CORBA::ValueBase*)_copy_of_par_" << ID(p[i].name);
		o << ")==((CORBA::ValueBase*)_copy_of_par_" << ID(p[j].name)
		  << ")) " << BL_OPEN;
		o << "_par_" << ID(p[i].name) << " = ";
		if (p[i].type_def->def_kind() != CORBA::dk_Primitive) {
		  emit_type_name (p[i].type_def.in());
		  o << "::_downcast (_par_" << ID(p[j].name) << ");" << endl;
		}
		else {
		  o << "_par_" << ID(p[j].name) << endl;
		}
		o << "CORBA::add_ref (_par_" << ID(p[i].name) << ");" << endl;
		o << BL_CLOSE;
		have_check = true;
	      }
	    }
	  }
	  if (have_check) {
	    o << "else " << BL_OPEN;
	  }
	  o << "_par_" << ID(p[i].name) << " = ";
	  if (p[i].type_def->def_kind() != CORBA::dk_Primitive) {
	    emit_type_name (p[i].type_def.in());
	    o << "::_downcast (_copy_of_par_" << ID(p[i].name)
	      << "->_copy_value());"
	      << endl;
	  }
	  else {
	    o << "_par_" << ID(p[i].name) << "->_copy_value();" << endl;
	  }
	  if (have_check) {
	    o << BL_CLOSE;
	  }
	}
	else if (p[i].type->is_abstract_interface()) {
	  o << "if (_copy_of_par_" << ID(p[i].name) << " && _copy_of_par_";
	  o << ID(p[i].name) << "->_to_value()) " << BL_OPEN;
	  o << "_par_" << ID(p[i].name) << " = ";
	  emit_type_name (p[i].type_def.in());
	  o << "::_downcast (_copy_of_par_" << ID(p[i].name)
	    << "->_to_value()->_copy_value());" << endl;
	  o << BL_CLOSE << "else " << BL_OPEN;
	  o << "_par_" << ID(p[i].name) << " = ";
	  emit_type_name (p[i].type_def.in());
	  o << "::_duplicate (_copy_of_par_" << ID(p[i].name) << ");" << endl;
	  o << BL_CLOSE;
	}
      }
    }

    for (i=0; i<p.length(); i++) {
      if (p[i].type->is_valuetype()) {
	o << "CORBA::remove_ref (_copy_of_par_" << ID(p[i].name) << ");"
	  << endl;
      }
      else if (p[i].type->is_abstract_interface()) {
	o << "CORBA::release (_copy_of_par_" << ID(p[i].name) << ");"
	  << endl;
      }
    }

    /*
     * Valuetype return values must be deep-copied to preserve location
     * transparency
     */

    if (tc_result->is_valuetype()) {
      emit_type_for_variable (result, manual);
      o << " __res2 = ";
      if (result->def_kind() != CORBA::dk_Primitive) {
	emit_type_name (result);
	o << "::_downcast (__res->_copy_value ());" << endl;
      }
      else {
	o << "__res->_copy_value ();" << endl;
      }
      o << "CORBA::remove_ref (__res);" << endl;
      o << "return __res2;" << endl;
    }
    else if (tc_result->is_abstract_interface()) {
      emit_type_for_variable (result, manual);
      o << " __res2;" << endl;
      o << "if (__res && __res->_to_value()) " << BL_OPEN;
      o << "__res2 = ";
      emit_type_name (result);
      o << "::_downcast (__res->_copy_value());" << endl;
      o << BL_CLOSE << "else " << BL_OPEN;
      o << "__res2 = ";
      emit_type_name (result);
      o << "::_duplicate (__res);" << endl;
      o << BL_CLOSE;
      o << "CORBA::release (__res);" << endl;
      o << "return __res2;" << endl;
    }
    else {
      o << "return";
      if (tc_result->kind() != CORBA::tk_void) {
	o << " __res";
      }
      o << ";" << endl;
    }
    
    o << BL_CLOSE;
    o << "_postinvoke ();" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * Either the servant is not colocated, not realized by a POA, or
   * currently not ready to receive requests. Fall back to the usual
   * stub mechanism, and send the request by SII or DII
   */
  if (tc_result->kind() != CORBA::tk_void) {
    o << "return ";
  }
  
  o << absClassName << _STUB << "::" << ID(op.name) << "(";
  for (i=0; i < p.length(); i++) {
    if (i) {
      o << ", ";
    }
    o << "_par_" << ID(p[i].name);
  }
  if (ctx.length() > 0) {
    if (p.length() > 0) {
      o << ", ";
    }
    o << "_ctx";
  }
  o << ");" << endl;
  o << BL_CLOSE << endl;

  o << "#endif // MICO_CONF_NO_POA" << endl << endl;
}

void CodeGenCPPStub::emit_Interface( CORBA::InterfaceDef_ptr in )
{
  assert( in->def_kind() == CORBA::dk_Interface ||
	  in->def_kind() == CORBA::dk_AbstractInterface ||
	  in->def_kind() == CORBA::dk_LocalInterface );

  emit_container (in);

  CORBA::String_var id = in->id();
  bool has_abstract_base = false;
  bool has_local_base = false;

  CORBA::String_var tmp;
  tmp = in->name();
  string className;
  className = ID(tmp);
  tmp = in->absolute_name();
  string absClassName;
  absClassName = ID(tmp);
  absClassName = absClassName.substr( 2 );
  
  /*
   * Emit base stuff
   */

  o << endl;
  o << "/*" << endl
    << " * Base interface for class " << className << endl
    << " */" << endl << endl;

  // dtor
  o << absClassName << "::~" << className << "()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;

  /*
   * _narrow_helper virtual method:
   *
   * searched upwards in the inheritance hierarchy starting at the
   * real class of the object that is to be narrowed until it finds
   * the class we want to narrow to or until top is reached.
   *
   * this is used to do a downcast from a virtual base class.
   */
  o << "void *" << endl;
  o << absClassName << "::_narrow_helper( const char *_repoid )" << endl;
  o << BL_OPEN;
  o << "if( strcmp( _repoid, \"" << id << "\" ) == 0 )" << endl;
  o << indent << "return (void *)this;" << exdent << endl;
  CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
  if( bases->length() != 0 ) {
    // We have base interfaces
    set_name_prefix( in );
    for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
      if (bases[i]->def_kind() == CORBA::dk_AbstractInterface) {
	has_abstract_base = true;
      }
      else if (bases[i]->def_kind() == CORBA::dk_LocalInterface) {
	has_local_base = true;
      }
      CORBA::String_var tmp = bases[i]->absolute_name();
      string base_name;
      base_name = ID(tmp);
      base_name = base_name.substr( 2 );
      o << BL_OPEN;
      o << "void *_p;" << endl;
      o << "if ((_p = ";
      o << base_name;
      o << "::_narrow_helper( _repoid )))" << endl;
      o << indent << "return _p;" << exdent << endl;
      o << BL_CLOSE;
    }
    set_name_prefix( CORBA::Container::_nil() );
  }

  o << "return NULL;" << endl;
  o << BL_CLOSE << endl;

  /*
   * _narrow class method
   *
   * first see if we can just do a downcast by using _narrow_helper.
   * if that fails too call _obj->_is_a (_repoid) and create a
   * stub if _is_a() returns true. otherwise return _nil().
   */
  o << absClassName << "_ptr" << endl;
  o << absClassName << "::_narrow( CORBA::Object_ptr _obj )" << endl;
  o << BL_OPEN;
  if (!_params.pseudo && in->def_kind() != CORBA::dk_LocalInterface) {
    if (in->def_kind() == CORBA::dk_Interface) {
      o << absClassName << "_ptr _o;" << endl;
    }
    else {
      // Abstract interfaces don't derive from CORBA::Object, but their
      // stubs do
      o << absClassName << _STUB << "* _o;" << endl;
    }
  }
  o << "if( !CORBA::is_nil( _obj ) ) {" << indent << endl;
  o << "void *_p;" << endl;
  o << "if( (_p = _obj->_narrow_helper( \"" << id << "\" )))" << endl;
  o << indent
    << "return _duplicate( (" << absClassName << "_ptr) _p );"
    << exdent << endl;

  if (!_params.pseudo && in->def_kind() != CORBA::dk_LocalInterface) {
    o << "if (!strcmp (_obj->_repoid(), \"" << id << "\")"
      << " || _obj->_is_a_remote (\"" << id << "\")) " << BL_OPEN;
    o << "_o = new " << absClassName;
    o << _STUB << ";" << endl;
    o << "_o->";
    // Win32 hack
    o << "CORBA::Object::operator=( *_obj );" << endl;
    o << "return _o;" << endl;
    o << BL_CLOSE;
  }
  o << BL_CLOSE;
  o << "return _nil();" << endl;
  o << BL_CLOSE << endl;

  /*
   * _narrow from AbstractBase
   */
  o << absClassName << "_ptr" << endl;
  o << absClassName << "::_narrow( CORBA::AbstractBase_ptr _obj )" << endl;
  o << BL_OPEN;
  if (in->def_kind() == CORBA::dk_AbstractInterface) {
    o << className << "_ptr _o = _downcast (_obj->_to_value());" << endl;
    o << "if (CORBA::is_nil (_o)) " << BL_OPEN;
    o << "_o = _narrow (_obj->_to_object());" << endl;
    o << BL_CLOSE;
    o << "else " << BL_OPEN;
    o << "_duplicate (_o);" << endl;
    o << BL_CLOSE;
    o << "return _o;" << endl;
  }
  else {
    o << "return _narrow (_obj->_to_object());" << endl;
  }
  o << BL_CLOSE << endl;

  /*
   * _downcast class method from ValueBase for abstract interfaces
   */

  if (in->def_kind() == CORBA::dk_AbstractInterface) {
    o << absClassName << "_ptr" << endl;
    o << absClassName << "::_downcast (CORBA::ValueBase * vb)" << endl;
    o << BL_OPEN;
    o << "void * p;" << endl;
    o << "if (vb && ((p = vb->_narrow_helper (\"" << id << "\")))) "
      << BL_OPEN;
    o << "return (" << absClassName << " *) p;" << endl;
    o << BL_CLOSE;
    o << "return _nil();" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * If we are concrete but have an abstract base, implement _to_object()
   */

  if (has_abstract_base && in->def_kind() == CORBA::dk_Interface) {
    o << "CORBA::Object_ptr" << endl;
    o << absClassName << "::_to_object ()" << endl;
    o << BL_OPEN;
    o << "return this;" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * Emit helpers: typecode, any operators and marshaller
   */

  if (_params.typecode) {
    tmp = in->absolute_name();
    string abs = ID(tmp);
    int colon_pos = abs.rfind( "::" );
    if( colon_pos != -1 )
      abs = abs.substr( 2, colon_pos );

    tmp = in->name();
    CORBA::String_var name = (const char *) ID(tmp);
    CORBA::TypeCode_var tc_in = in->type();

    CORBA::Container_var container = in->defined_in();
    emit_typecode_def (container, abs, name.in(), tc_in);
  }

  // Emit SII marshaller
  if (in->def_kind() != CORBA::dk_LocalInterface) {
    emit_marshaller( in );
  }

  if (_params.any && in->def_kind() != CORBA::dk_LocalInterface) {
    // operator <<= (copying)
    o << "void" << endl;
    o << "operator<<=( CORBA::Any &_a, const ";
    emit_idl_type_name( in );
    o << " _obj )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (in);
    o << ", &_obj);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    // operator <<= (noncopying)
    o << "void" << endl;
    o << "operator<<=( CORBA::Any &_a, ";
    emit_idl_type_name( in );
    o << "* _obj_ptr )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (in);
    o << ", _obj_ptr);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << "CORBA::release (*_obj_ptr);" << endl;
    o << BL_CLOSE << endl;

    // operator >>=
    o << "CORBA::Boolean" << endl;
    o << "operator>>=( const CORBA::Any &_a, ";
    emit_idl_type_name( in );
    o << " &_obj )" << endl;
    o << BL_OPEN;
    emit_idl_type_name( in );
    o << " *p;" << endl;
    o << "if (_a.to_static_any (";
    emit_marshaller_ref (in);
    o << ", (void *&)p)) " << BL_OPEN;
    o << "_obj = *p;" << endl;
    o << "return TRUE;" << endl;
    o << BL_CLOSE;
    o << "return FALSE;" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * Local interfaces don't have stubs
   */

  if (in->def_kind() == CORBA::dk_LocalInterface || _params.pseudo) {
    o << endl;
    return;
  }

  /*
   * Emit Stub
   */

  o << endl;
  o << "/*" << endl
    << " * Stub interface for class " << className << endl
    << " */" << endl << endl;

  // dtor
  o << absClassName << _STUB << "::~" << className << _STUB << "()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;

  if (_params.poa) {
    /*
     * Skeleton's _narrow_helper and _narrow methods. Moved up here from
     * the skeleton because the colocated stubs need them.
     */

    o << "#ifndef MICO_CONF_NO_POA" << endl << endl;
    o << "void *" << endl;
    o << "POA_" << absClassName << "::_narrow_helper";
    o <<   " (const char * repoid)" << endl;
    o << BL_OPEN;
    if (bases->length()) {
      o << "void * p;" << endl;
    }
    o << "if (strcmp (repoid, \"" << id << "\") == 0) " << BL_OPEN;
    o << "return (void *) this;" << endl;
    o << BL_CLOSE;
    for (CORBA::ULong p1=0; p1<bases->length(); p1++) {
      tmp = bases[p1]->absolute_name();
      string base_name;
      base_name = ID(tmp);
      base_name = base_name.substr( 2 );

      o << "if ((p = POA_" << base_name;
      o << "::_narrow_helper (repoid)) != NULL) " << endl;
      o << BL_OPEN;
      o << "return p;" << endl;
      o << BL_CLOSE;
    }
    o << "return NULL;" << endl;
    o << BL_CLOSE << endl;
    
    /*
     * _narrow method
     */

    o << "POA_" << absClassName << " *" << endl;
    o << "POA_" << absClassName << "::_narrow";
    o <<   " (PortableServer::Servant serv) " << endl;
    o << BL_OPEN;
    o << "void * p;" << endl;
    o << "if ((p = serv->_narrow_helper (\"" << id << "\")) != NULL) "
      << BL_OPEN;
    o << "serv->_add_ref ();" << endl;
    o << "return (POA_" << absClassName << " *) p;" << endl;
    o << BL_CLOSE;
    o << "return NULL;" << endl;
    o << BL_CLOSE << endl;

    /*
     * Constructor and Destructor for colocated stubs
     */

    if (_params.poa_stubs) {
      o << absClassName << _STUB << "_clp::"
	<<    className << _STUB << "_clp ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;
      o << absClassName << _STUB << "_clp::"
	<<    className << _STUB << "_clp"
	<<  " (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)" << endl;
      o << "  : CORBA::Object(*obj), PortableServer::StubBase(poa)" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;
      o << absClassName << _STUB << "_clp::~"
	<<    className << _STUB << "_clp ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;
    }
    o << "#endif // MICO_CONF_NO_POA" << endl << endl;
  }

  CORBA::ContainedSeq_var dsv = in->contents (CORBA::dk_Attribute, TRUE);

  // Generate attributes
  for( CORBA::ULong j = 0; j < dsv->length(); j++ ) {
    CORBA::AttributeDef_var ad =
      CORBA::AttributeDef::_narrow (dsv[j]);
    assert (!CORBA::is_nil (ad));

    CORBA::ExtAttributeDescription_var attr;

    CORBA::ExtAttributeDef_var ead =
      CORBA::ExtAttributeDef::_narrow (ad);

    if (!CORBA::is_nil (ead)) {
      attr = ead->describe_attribute ();
    }
    else {
      // for backwards compatibility
      CORBA::Contained::Description_var desc = ad->describe ();
      const CORBA::AttributeDescription * oldattr;
      CORBA::Boolean b = (desc->value >>= oldattr);
      assert (b);
      attr = new CORBA::ExtAttributeDescription;
      attr->name = oldattr->name;
      attr->id = oldattr->id;
      attr->defined_in = oldattr->defined_in;
      attr->version = oldattr->version;
      attr->type = oldattr->type;
      attr->mode = oldattr->mode;
    }

    CORBA::OperationDescription op;
    op.name = (const char *) attr->name;
    op.id = (const char *) "";  // Not needed
    op.defined_in = attr->defined_in;
    op.version = (const char*) attr->version;
    op.result = CORBA::TypeCode::_nil();  // Not needed
    op.mode = CORBA::OP_NORMAL;
    // op.contexts does not need to be set for attributes
    op.parameters.length( 0 );  // No parameters for _get
    op.exceptions = attr->get_exceptions;

    string dispatch_name = string("_get_") + (const char *)attr->name;
    
    CORBA::IDLType_var idl_type = lookup_attribute_by_id( attr->id.in() );
    emit_sii_stub_method( absClassName, dispatch_name, op, idl_type );

    if (_params.poa && _params.poa_stubs) {
      emit_poa_stub_method (absClassName, dispatch_name, op, idl_type);
    }

    if( attr->mode == CORBA::ATTR_NORMAL ) {
      // We also need the set version of the attribute
      CORBA::ParameterDescription attr_param;
      attr_param.name = (const char *) "_value";
      attr_param.type = attr->type;
      attr_param.type_def = lookup_attribute_by_id( attr->id.in() );
      attr_param.mode = CORBA::PARAM_IN;
      
      op.name = (const char *) attr->name;
      op.id = (const char *) "";  // Not needed
      op.defined_in = attr->defined_in;
      op.version = (const char*) attr->version;
      op.result = CORBA::TypeCode::_nil();  // Not needed
      op.mode = CORBA::OP_NORMAL;
      // op.contexts does not need to be set for attributes
      op.parameters.length( 1 );
      op.parameters[ 0 ] = attr_param;
      op.exceptions = attr->set_exceptions;
      
      dispatch_name = string("_set_") + (const char *)attr->name;
    
      idl_type = _repo->get_primitive( CORBA::pk_void );
      emit_sii_stub_method( absClassName, dispatch_name, op, idl_type );

      if (_params.poa && _params.poa_stubs) {
	emit_poa_stub_method (absClassName, dispatch_name, op, idl_type);
      }
    }
  }

  dsv = in->contents (CORBA::dk_Operation, TRUE);

  // Generate methods
  for( CORBA::ULong j0 = 0; j0 < dsv->length(); j0++ ) {
    CORBA::Contained::Description_var desc = dsv[j0]->describe ();
    const CORBA::OperationDescription * op;
    CORBA::Boolean r = (desc->value >>= op);
    assert (r);
    string dispatch_name = (const char *) op->name;

    CORBA::IDLType_var idl_type = lookup_result_by_id( op->id.in() );
    emit_sii_stub_method( absClassName, dispatch_name, *op, idl_type );

    if (_params.poa && _params.poa_stubs) {
      emit_poa_stub_method (absClassName, dispatch_name, *op, idl_type);
    }
  }
}


/*
 * Generates default constructor, copy constructor and assignment
 * operator for structs. These have to be generated due to a g++
 * bug (g++ doesn't expand templates properly for the implicit
 * generated versions of these methods). Note that according to
 * the CORBA2 specs these shouldn't be defined (see [16.9(1)])
 */
void
CodeGenCPPStub::emit_StructDefaultMembers( char *name,
					   char *abs,
					   CORBA::StructMemberSeq_var &m )
{
  o << "#ifdef HAVE_EXPLICIT_STRUCT_OPS" << endl;

  // Default constructor
  o << abs << name << "::" << name << "()" << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;

  // Copy constructor
  o << abs << name << "::" << name << "( const " << name << "& _s )" << endl;
  o << BL_OPEN;
  for( CORBA::ULong i0 = 0; i0 < m->length(); i0++ ) {
    if (m[i0].type_def->def_kind() == CORBA::dk_Value ||
	m[i0].type_def->def_kind() == CORBA::dk_ValueBox) {
      o << ID(m[ i0 ].name) << " = ";
      emit_type_name (m[i0].type_def);
      o << "::_narrow ((" << name << "&)_s."
	<< ID(m[i0].name) << "->_copy_value());" << endl;
    }
    else {
      o << ID(m[ i0 ].name) << " = ((" << name << "&)_s).";
      o << ID(m[ i0 ].name) << ";" << endl;
    }
  }
  o << BL_CLOSE << endl;

  // Destructor (moving this out of .h file reduces template drag in a lot)
  o << abs << name << "::~" << name << "()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;
  
  // Assignment operator
  o << abs << name << "&" << endl;
  o << abs << name << "::operator=( const " << name << "& _s )" << endl;
  o << BL_OPEN;
  for( CORBA::ULong i1 = 0; i1 < m->length(); i1++ ) {
    if (m[i1].type_def->def_kind() == CORBA::dk_Value ||
	m[i1].type_def->def_kind() == CORBA::dk_ValueBox) {
      o << ID(m[ i1 ].name) << " = ";
      emit_type_name (m[i1].type_def);
      o << "::_narrow ((" << name << "&)_s."
	<< ID(m[i1].name) << "->_copy_value());" << endl;
    }
    else {
      o << ID(m[ i1 ].name) << " = ((" << name << "&)_s).";
      o << ID(m[ i1 ].name) << ";" << endl;
    }
  }
  o << "return *this;" << endl;
  o << BL_CLOSE;

  o << "#endif" << endl << endl;
}



void CodeGenCPPStub::emit_Struct( CORBA::StructDef_ptr s )
{
  assert( s->def_kind() == CORBA::dk_Struct );

  emit_container (s);

  // Get base for absolute name
  CORBA::String_var tmp = s->absolute_name();
  string abs;
  abs = ID(tmp);
  tmp = s->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::TypeCode_var tc = s->type();

  int colon_pos = abs.rfind( "::" );
  if( colon_pos != -1 )
    abs = abs.substr( 2, colon_pos );

  // Emit TypeCode for struct
  if( _params.typecode && tc->is_freestanding() ) {
    CORBA::Container_var container = s->defined_in();
    emit_typecode_def (container, abs, name.in(), tc);
    o << endl;
  }

  // Emit default ctor, copy ctor, assignment op
  CORBA::StructMemberSeq_var m = s->members();
  emit_StructDefaultMembers (name, (char *)abs.c_str(), m);

  if (contains_a_valuetype (s)) {
    o << "CORBA::Long" << endl;
    o << abs << name << "::_count_refs (CORBA::ValueBase::visited * _seen)"
      << endl;
    o << BL_OPEN;
    o << "CORBA::Long res = 0;" << endl;
    for (CORBA::ULong i0=0; i0 < m->length(); i0++) {
      string prefix = ID(m[i0].name);
      emit_ValueRekCheck (m[i0].type_def, prefix, 0);
    }
    o << "return res;" << endl;
    o << BL_CLOSE << endl;

    o << "void" << endl;
    o << abs << name << "::_release_members ()" << endl;
    o << BL_OPEN;
    for (CORBA::ULong i1=0; i1 < m->length(); i1++) {
      string prefix = ID(m[i1].name);
      emit_ValueReleaser (m[i1].type_def, prefix, 0);
    }
    o << BL_CLOSE << endl;
  }

  if (!is_marshallable (s)) {
    return;
  }

  // SII marshaller
  emit_marshaller( s );

  if( _params.any ) {
    // operator<<=
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs << name << " &_s )" << endl;
    o << BL_OPEN;
    CORBA::StructMemberSeq_var members;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (s);
    o << ", &_s);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    o << abs << name << " *_s )" << endl;
    o << BL_OPEN;
    o << "_a <<= *_s;" << endl;
    o << "delete _s;" << endl;
    o << BL_CLOSE << endl;
  
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs << name << " &_s )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (s);
    o << ", &_s);" << endl;
    o << "return _a.to_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    o << abs << name << " *&_s )" << endl;
    o << BL_OPEN;
    o << "return _a.to_static_any (";
    emit_marshaller_ref (s);
    o << ", (void *&)_s);" << endl;
    o << BL_CLOSE << endl;
  }
}



/*
 * Generates default constructor, copy constructor and assignment
 * operator for unions. These have to be generated due to a g++
 * bug (g++ doesn't expand templates properly for the implicit
 * generated versions of these methods). Note that according to
 * the CORBA2 specs these shouldn't be defined (see [16.9(1)])
 */
void
CodeGenCPPStub::emit_UnionDefaultMembers( char *name,
					  char *abs,
					  CORBA::UnionMemberSeq_var &m )
{
  o << "#ifdef HAVE_EXPLICIT_STRUCT_OPS" << endl;

  CORBA::String_var last_name;
  last_name = (const char *) "";

  // Default constructor
  o << abs << name << "::" << name << "()" << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;

  // Copy constructor
  last_name = (const char *) "";
  o << abs << name << "::" << name << "( const " << name << "&_u )" << endl;
  o << BL_OPEN;
  o << "_discriminator = _u._discriminator;" << endl;
  for( CORBA::ULong i0 = 0; i0 < m->length(); i0++ ) {
    if( strcmp( last_name, m[ i0 ].name ) == 0 )
      continue;
    last_name = m[ i0 ].name;
    if (m[i0].type_def->def_kind() == CORBA::dk_Value ||
	m[i0].type_def->def_kind() == CORBA::dk_ValueBox) {
      o << "_m." << ID(m[ i0 ].name) << " = ";
      emit_type_name (m[i0].type_def);
      o << "::_narrow (((" << name << "&)_u)._m."
	<< ID(m[i0].name) << "->_copy_value());" << endl;
    }
    else {
      string d = string("_m.") + ID(m[i0].name);
      string s = string("((") + name + "&)_u)._m." + ID(m[i0].name);
      emit_assignment( d, s, m[i0].type_def );
    }
  }
  o << BL_CLOSE << endl;

  // Destructor (moving this out of .h file reduces template drag in a lot)
  o << abs << name << "::~" << name << "()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;
  
  // Assignment operator
  o << abs << name << "&" << endl;
  o << abs << name << "::operator=( const " << name << "&_u )" << endl;
  o << BL_OPEN;
  o << "_discriminator = _u._discriminator;" << endl;
  last_name = (const char *) "";
  for( CORBA::ULong i1 = 0; i1 < m->length(); i1++ ) {
    if( strcmp( last_name, m[ i1 ].name ) == 0 )
      continue;
    last_name = m[ i1 ].name;
    if (m[i1].type_def->def_kind() == CORBA::dk_Value ||
	m[i1].type_def->def_kind() == CORBA::dk_ValueBox) {
      o << "_m." << ID(m[ i1 ].name) << " = ";
      emit_type_name (m[i1].type_def);
      o << "::_narrow (((" << name << "&)_u)._m."
	<< ID(m[i1].name) << "->_copy_value());" << endl;
    }
    else {
      o << "_m." << ID(m[ i1 ].name) << " = ((" << name << "&)_u)._m.";
      o << ID(m[ i1 ].name) << ";" << endl;
    }
  }
  o << "return *this;" << endl;
  o << BL_CLOSE;

  o << "#endif" << endl << endl;
}


    
void CodeGenCPPStub::emit_union_case_label( CORBA::IDLType_ptr discr,
					    CORBA::UnionMember &m,
					    CORBA::UnionMemberSeq &members )
{
  CORBA::TypeCode_var tc = m.label.type();
  if( tc->is_octet() ) {
    string label = generate_union_default_case_label( discr, members );
    // Test if union has already all case members listed
    assert( label.length() > 0 );
    o << label;
    return;
  }
  
  tc = discr->type();
  if( tc->is_enum() ) {
    CORBA::String_var s;
    CORBA::ULong idx;
    CORBA::Boolean r = m.label.enum_get( idx );
    assert (r);
    s = (const char *) tc->unalias()->member_name( idx );
    CORBA::Contained_var c = CORBA::Contained::_narrow( discr );
    assert( !CORBA::is_nil( c ) );
    CORBA::String_var tmp = c->absolute_name();
    string abs;
    abs = ID(tmp);
    int colon_pos = abs.rfind( "::" );
    if( colon_pos != -1 )
      abs = abs.substr( 2, colon_pos );
    o << abs << ID(s);
  } else {
    o << &m.label;
  }
}

/*
 * Emit code to assign a union member from an "in" parameter: primitive
 * types are just assigned, arrays must be copied, reference counts must
 * be increased.
 *
 * Needed by union modifiers and by OBV_ constructors
 */

void
CodeGenCPPStub::emit_union_member_assigner (CORBA::IDLType_ptr type,
					    const char * name,
					    const char * path,
					    const char * parname)
{
  CORBA::String_var m_name = (const char *) ID(name);
  CORBA::TypeCode_var tc = type->type();
  

  if (!parname) {
    parname = "_p";
  }

  if (!path) {
    path = "_m.";
  }
  
  if( tc->is_string() || tc->is_wstring() ) {
    /*
     * This works for char*, const char* and const String_var& input
     */
    o << path << m_name << " = " << parname << ";" << endl;
  } else if( tc->is_array() ) {
    /*
     * Arrays must be copied
     */

    CORBA::TypeCode_ptr orig_tc = tc->unalias();

    emit_array( type );
    o << "* _dest_" << m_name << " = (";
    emit_array( type );
    o << "*) " << path << m_name << ";" << endl;
    emit_array( type );
    o << "* _src_" << m_name << " = (";
    emit_array( type );
    o << "*) " << parname << ";" << endl;
    o << "for( int _i_" << m_name << " = 0; _i_" << m_name << " < ";
    o << compute_num_array_elem( orig_tc );
    o << "; _i_" << m_name << "++ )" << endl;
    o << indent << "*_dest_" << m_name << "++ = *_src_" << m_name << "++;"
      << exdent << endl;
  } else if( tc->is_objref() || tc->is_type_code() || tc->is_base_type() ) {
    // Accessor functions for array or object reference
    o << path << m_name << " = ";
    if( tc->is_objref() ) {
      if( type->def_kind() == CORBA::dk_Primitive ) {
	// Gotta be "Object"
	o << "CORBA::Object::_duplicate( " << parname << " );" << endl;
      } else {
	CORBA::Contained_var c = CORBA::Contained::_narrow( type );
	assert( !CORBA::is_nil( c ) );
	CORBA::String_var objref_name = c->absolute_name();
	o << ID(objref_name) << "::_duplicate( " << parname << " );"
	  << endl;
      }
    } else if( tc->is_type_code() ) {
      o << "CORBA::TypeCode::_duplicate( " << parname << " );" << endl;
    } else {
      o << parname << ";" << endl;
    }
  } else if( tc->is_sequence() && !tc->is_alias() ) {
    // Anonymous sequence
    o << path << m_name << " = " << parname << ";" << endl;
  } else if( tc->is_valuetype() ) {
    o << "CORBA::add_ref ( " << parname << " );" << endl;
    o << path << m_name << " = " << parname << ";" << endl;
  } else {
    // Gotta be struct, union or sequence
    o << path << m_name << " = " << parname << ";" << endl;
  }
}

void
CodeGenCPPStub::emit_union_member_accessor (const char * className,
					    CORBA::IDLType_ptr type,
					    const char * name,
					    const char * path,
					    CORBA::IDLType_ptr discr,
					    CORBA::UnionMemberSeq * members,
					    CORBA::ULong index)
{
  CORBA::String_var m_name = (const char *) ID(name);
  CORBA::TypeCode_var tc = type->type();

  if (!path) {
    path = "_m.";
  }
  
  if( tc->is_string() ) {
    // Accessor functions for string
    o << "void " << className << "::" << m_name << "( char *_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "void " << className << "::"
      << m_name << "( const char *_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "void " << className << "::"
      << m_name << "( const CORBA::String_var &_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "const char *" << className << "::"
      << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  } else if( tc->is_wstring() ) {
    // Accessor functions for wstring
    o << "void " << className << "::" << m_name << "( wchar_t *_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "void " << className << "::"
      << m_name << "( const wchar_t *_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "void " << className << "::"
      << m_name << "( const CORBA::WString_var &_p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "const wchar_t *" << className << "::"
      << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  } else if( tc->is_array() && !tc->is_alias() ) {
    // We have an anonymous array. See [16.10(7)]
    o << "void " << className << "::" << m_name << "(const ";
    emit_array( type );
    o << " _p";
    emit_array_suffix( type );
    o << " )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "const "
      << className << "::_" << m_name << "_slice* ";
    o << className << "::" << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;

    o << className << "::_" << m_name << "_slice* ";
    o << className << "::" << m_name << "()" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  } else if( tc->is_array() || tc->is_base_type() ) {
    // Accessor functions for array or basic types
    o << "void " << className << "::" << m_name << "( ";
    if( tc->is_array() ) {
      // Mapping for arrays differs from standard PARAM_IN mapping!
      o << "const ";
      emit_idl_type_name( type );
    }
    else
      emit_type_for_param( type, CORBA::PARAM_IN );
    o << " _p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    emit_type_for_result( type );
    o << " " << className << "::" << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return (";
    emit_type_for_result( type );
    o << ")" << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  } else if( tc->is_sequence() && !tc->is_alias() ) {
    // Anonymous sequence
    o << "void " << className << "::" << m_name << "( const ";
    emit_sequence( type );
    o << "& _p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;

    o << "const "
      << className << "::_" << m_name << "_seq& ";
    o << className << "::" << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;

    o << className << "::_" << m_name << "_seq& ";
    o << className << "::" << m_name << "()" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  } else if( tc->is_objref() || tc->is_type_code() || tc->is_valuetype() ) {
    // Accessor functions for object references etc.
    o << "void " << className << "::" << m_name << "( ";
    emit_type_for_param( type, CORBA::PARAM_IN );
    o << " _p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;

    emit_type_for_result (type);
    o << " " << className << "::" << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ".in();" << endl;
    o << BL_CLOSE << endl;
  } else {
    // Gotta be struct, union or sequence
    o << "void " << className << "::" << m_name << "( ";
    emit_type_for_param( type, CORBA::PARAM_IN );
    o << " _p )" << endl;
    o << BL_OPEN;
    if (!CORBA::is_nil (discr)) {
      o << "_discriminator = ";
      emit_union_case_label( discr, (*members)[index], *members );
      o << ";" << endl;
    }
    emit_union_member_assigner (type, name, path);
    o << BL_CLOSE << endl;
    
    o << "const ";
    emit_idl_type_name( type );
    o << "& " << className << "::" << m_name << "() const" << endl;
    o << BL_OPEN;
    o << "return (";
    emit_idl_type_name( type );
    o << "&) " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
    
    emit_idl_type_name( type );
    o << "& " << className << "::" << m_name << "()" << endl;
    o << BL_OPEN;
    o << "return " << path << m_name << ";" << endl;
    o << BL_CLOSE << endl;
  }
}

void CodeGenCPPStub::emit_Union( CORBA::UnionDef_ptr u )
{
  assert( u->def_kind() == CORBA::dk_Union );

  emit_container (u);

  // Get base for absolute name
  CORBA::String_var tmp = u->absolute_name();
  string abs, absClassName;
  abs = ID(tmp);
  tmp = u->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::TypeCode_var tc = u->type();
  CORBA::UnionMemberSeq_var members = u->members();
  CORBA::IDLType_var discr = u->discriminator_type_def();

  absClassName = abs;
  absClassName = absClassName.substr (2);

  int colon_pos = abs.rfind( "::" );
  if( colon_pos != -1 )
    abs = abs.substr( 2, colon_pos );

  // Emit TypeCode for union
  if( _params.typecode && tc->is_freestanding() ) {
    CORBA::Container_var container = u->defined_in();
    emit_typecode_def (container, abs, name.in(), tc);
    o << endl;
  }

  // Emit default ctor, copy ctor, assignment op
  emit_UnionDefaultMembers( name, (char *) abs.c_str(), members );

  // Emit discriminator functions
  o << "void " << abs << name << "::_d( ";
  emit_type_for_param( discr, CORBA::PARAM_IN );
  o << " _p )" << endl;
  o << BL_OPEN;
  o << "_discriminator = _p;" << endl;
  o << BL_CLOSE << endl;
  
  emit_type_for_result( discr );
  o << " " << abs << name << "::_d() const" << endl;
  o << BL_OPEN;
  o << "return _discriminator;" << endl;
  o << BL_CLOSE << endl;

  // Generate member accessor functions
  CORBA::String_var last_name = (const char *) "";
  bool has_explicit_default = false;
  for( CORBA::ULong i = 0; i < members->length(); i++ ) {
    CORBA::TypeCode_var tc = members[ i ].label.type();
    if( tc->is_octet() )
      has_explicit_default = true;

    if( strcmp( last_name.in(), members[i].name.in() ) == 0 )
      continue;
    last_name = members[i].name.in();
    
    emit_union_member_accessor (absClassName.c_str(),
				members[i].type_def.in(),
				members[i].name.in(),
				"_m.",
				discr.in(),
				members, i);
  }
  if( !has_explicit_default ) {
    // This union has no explicit default case label
    string default_label =
      generate_union_default_case_label( discr.in(), members.inout() );
    if( default_label.length() > 0 ) {
      o << "void " << abs << name << "::_default()" << endl;
      o << BL_OPEN;
      o << "_discriminator = " << default_label << ";" << endl;
      o << BL_CLOSE << endl;
    }
  }

  if (contains_a_valuetype (u)) {
    o << "CORBA::Long" << endl;
    o << abs << name << "::_count_refs (CORBA::ValueBase::visited * _seen)"
      << endl;
    o << BL_OPEN;
    o << "CORBA::Long res = 0;" << endl;
    for (CORBA::ULong i0=0; i0 < members->length(); i0++) {
      string prefix = "_m.";
      prefix += ID(members[i0].name);
      emit_ValueRekCheck (members[i0].type_def, prefix, 0);
    }
    o << "return res;" << endl;
    o << BL_CLOSE << endl;

    o << "void" << endl;
    o << abs << name << "::_release_members ()" << endl;
    o << BL_OPEN;
    for (CORBA::ULong i1=0; i1 < members->length(); i1++) {
      string prefix = "_m.";
      prefix += ID(members[i1].name);
      emit_ValueReleaser (members[i1].type_def, prefix, 0);
    }
    o << BL_CLOSE << endl;
  }

  if (!is_marshallable (u)) {
    return;
  }

  // SII marshaller
  emit_marshaller( u );

  if( !_params.any )
    return;

  // operator<<=
  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
  o << abs << name << " &_u )" << endl;
  o << BL_OPEN;
  string v;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (u);
  o << ", &_u);" << endl;
  o << "_a.from_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;
  
  o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
  o << abs << name << " *_s )" << endl;
  o << BL_OPEN;
  o << "_a <<= *_s;" << endl;
  o << "delete _s;" << endl;
  o << BL_CLOSE << endl;
  
  // operator>>=
  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
  o << abs << name << " &_u )" << endl;
  o << BL_OPEN;
  o << "CORBA::StaticAny _sa (";
  emit_marshaller_ref (u);
  o << ", &_u);" << endl;
  o << "return _a.to_static_any (_sa);" << endl;
  o << BL_CLOSE << endl;

  // operator>>=
  o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
  o << abs << name << " *&_u )" << endl;
  o << BL_OPEN;
  o << "return _a.to_static_any (";
  emit_marshaller_ref (u);
  o << ", (void *&)_u);" << endl;
  o << BL_CLOSE << endl;
}



void CodeGenCPPStub::emit_Constant( CORBA::ConstantDef_ptr c )
{
  assert( c->def_kind() == CORBA::dk_Constant );

  CORBA::Container_var def_in = c->defined_in();
  if( def_in->def_kind() == CORBA::dk_Repository
      || def_in->def_kind() == CORBA::dk_Module )
      // This ConstantDef was defined on global scope or in an module,
      // so we need to do nothing.
    return;

  CORBA::IDLType_var type = c->type_def();
  CORBA::TypeCode_var tc = type->type ();

  string const_type;
  {
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
#else
    ostrstream ostr;
#endif
    Output save;
    o.save_streams (save);
    o.start_output (ostr);

    o << "const ";
    emit_type_for_variable (type, manual);

    if (tc->kind() == CORBA::tk_string
	|| tc->kind() == CORBA::tk_wstring) {
      // special handling of string constants
      o << " const";
    }

    o.stop_output ();
    o.restore_streams (save);

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    const_type = ostr.str();
#else
    ostr << ends;
    const_type = ostr.str();
    ostr.rdbuf()->freeze (0);
#endif
  }

  string const_init;
  {
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
#else
    ostrstream ostr;
#endif
    Output save;
    o.save_streams (save);
    o.start_output (ostr);

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

    CORBA::Any_var val = c->value();
    emit_const_value (val);

    o.stop_output ();
    o.restore_streams (save);

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    const_init = ostr.str();
#else
    ostr << ends;
    const_init = ostr.str();
    ostr.rdbuf()->freeze (0);
#endif
  }

  CORBA::String_var const_name = c->name();
  CORBA::Container_var const_scope = c->defined_in();

  emit_var_def (const_type, const_scope, const_name.in(), const_init);
}



void CodeGenCPPStub::emit_Exception( CORBA::ExceptionDef_ptr e )
{
  assert( e->def_kind() == CORBA::dk_Exception );

  emit_container (e);

  // Get base for absolute name
  CORBA::String_var tmp = e->absolute_name();
  string abs;
  abs = ID(tmp);
  tmp = e->name();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::TypeCode_var tc = e->type();

  int colon_pos = abs.rfind( "::" );
  if( colon_pos != -1 )
    abs = abs.substr( 2, colon_pos );

  CORBA::String_var repoid = e->id();

  // Emit TypeCode for exception
  CORBA::Container_var container = e->defined_in();
  emit_typecode_def (container, abs, name.in(), tc);
  o << endl;

  // Emit default ctor, copy ctor, assignment op
  CORBA::StructMemberSeq_var m = e->members();
  emit_StructDefaultMembers( name, (char *)abs.c_str(), m );

  // Emit constructor for member initialization
  if( m->length() > 0 ) {
    /*
     * when defining a constructor that takes some arguments, we also
     * need to explicitly define a default constructor. Such a constructor
     * is already defined by emit_StructDefaultMembers if
     * HAVE_EXPLICIT_STRUCT_OPTS
     */
    o << "#ifndef HAVE_EXPLICIT_STRUCT_OPS" << endl;
    o << abs << name << "::" << name << "()" << endl;
    o << "{" << endl;
    o << "}" << endl << endl;
    o << "#endif" << endl << endl;

    // Emit constructor for member initialization
    o << abs << name << "::" << name << "( ";
    CORBA::ULong i;
    for( i = 0; i < m->length(); i++ ) {
      if( i != 0 )
	o << ", ";
      emit_type_for_param( m[ i ].type_def, CORBA::PARAM_IN );
      o << " _m" << i;
      emit_array_suffix( m[i].type_def );
    }
    o << " )" << endl;
    o << BL_OPEN;
    for( i = 0; i < m->length(); i++ ) {
      CORBA::TypeCode_ptr tc = m[i].type;
      if( tc->is_array() ) {
	string d = string( ID(m[i].name ) );
	string s = string( "_m" ) + xdec( i );
	emit_assignment( d, s, m[i].type_def );
      } else {
	o << ID(m[i].name) << " = ";
	if( m[i].type->is_objref() ) {
	  if( m[i].type->is_object() )
	    o << "CORBA::Object";
	  else
	    emit_type_name( m[i].type_def );
	  o << "::_duplicate( _m" << i << " )";
	} else {
	  o << "_m" << i;
	}
	o << ";" << endl;
      }
    }
    o << BL_CLOSE << endl;
  }

  if (is_marshallable (e)) {
    emit_marshaller( e );
  }

  if( is_marshallable (e) && _params.any ) {
    // operator<<=
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs << name << " &_e )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (e);
    o << ", &_e);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, ";
    o << abs << name << " *_s )" << endl;
    o << BL_OPEN;
    o << "_a <<= *_s;" << endl;
    o << "delete _s;" << endl;
    o << BL_CLOSE << endl;
  
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs << name << " &_e )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (e);
    o << ", &_e);" << endl;
    o << "return _a.to_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, const ";
    o << abs << name << " *&_e )" << endl;
    o << BL_OPEN;
    o << "return _a.to_static_any (";
    emit_marshaller_ref (e);
    o << ", (void *&)_e);" << endl;
    o << BL_CLOSE << endl;
  }

  o << "void " << abs << name << "::_throwit() const" << endl;
  o << BL_OPEN;
  if( !_params.no_exceptions ) {
    o << "#ifdef HAVE_EXCEPTIONS" << endl;
    o << "#ifdef HAVE_STD_EH" << endl;
    o << "throw *this;" << endl;
    o << "#else" << endl;
    o << "throw " << name << "_var( (" << abs << name << "*)_clone() );"
      << endl;
    o << "#endif" << endl;
    o << "#else" << endl;
    o << "CORBA::Exception::_throw_failed( _clone() );" << endl;
    o << "#endif" << endl;
  } else {
    o << "CORBA::Exception::_throw_failed( _clone() );" << endl;
  }
  o << BL_CLOSE << endl;
  
  o << "const char *" << abs << name << "::_repoid() const" << endl;
  o << BL_OPEN;
  o << "return \"" << repoid << "\";" << endl;
  o << BL_CLOSE << endl;

  o << "void " << abs << name
    << "::_encode( CORBA::DataEncoder &"
    << ((!is_marshallable (e) || _params.pseudo)?"":"_en")
    << " ) const" << endl;
  o << BL_OPEN;
  if (!is_marshallable (e) || _params.pseudo) {
    o << "assert(0);" << endl;
  } else {
    emit_marshaller_ref( e );
    o << "->marshal( _en, (void*) this );" << endl;
  }
  o << BL_CLOSE << endl;

  o << "void " << abs << name
    << "::_encode_any( CORBA::Any &"
    << ((is_marshallable (e) && _params.any)?"_a":"")
    << " ) const" << endl;
  o << BL_OPEN;
  if (is_marshallable (e) && _params.any) {
    o << "_a <<= *this;" << endl;
  }
  else {
    o << "// use --any to make this work!" << endl;
    o << "assert(0);" << endl;
  }
  o << BL_CLOSE << endl;
  
  o << "CORBA::Exception *" << abs << name << "::_clone() const" << endl;
  o << BL_OPEN;
  o << "return new " << name << "( *this );" << endl;
  o << BL_CLOSE << endl;
  
  o << abs << name << " *" << abs << name;
  o << "::_downcast( CORBA::Exception *_ex )" << endl;
  o << BL_OPEN;
  o << "if( _ex && !strcmp( _ex->_repoid(), \"" << repoid << "\" ) )" << endl;
  o << indent << "return (" << name << " *) _ex;" << exdent << endl;
  o << "return NULL;" << endl;
  o << BL_CLOSE << endl;

  o << "const " << abs << name << " *" << abs << name;
  o << "::_downcast( const CORBA::Exception *_ex )" << endl;
  o << BL_OPEN;
  o << "if( _ex && !strcmp( _ex->_repoid(), \"" << repoid << "\" ) )" << endl;
  o << indent << "return (" << name << " *) _ex;" << exdent << endl;
  o << "return NULL;" << endl;
  o << BL_CLOSE << endl;
}



void CodeGenCPPStub::emit_Enum( CORBA::EnumDef_ptr e )
{
  assert( e->def_kind() == CORBA::dk_Enum );

  // Get base for absolute name
  CORBA::String_var tmp;
  tmp = e->absolute_name();
  string abs;
  abs = ID(tmp);
  
  tmp = e->name();
  CORBA::String_var name = (const char *) ID(tmp);
  
  int colon_pos = abs.rfind( "::" );
  if( colon_pos != -1 )
    abs = abs.substr( 2, colon_pos );

  // Emit TypeCode for enum
  CORBA::Container_var container = e->defined_in();
  CORBA::TypeCode_var tc = e->type();
  emit_typecode_def (container, abs, name.in(), tc);
  o << endl;

  if( _params.any ) {
    // operator<<=
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs << name << " &_e )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (e);
    o << ", &_e);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
  
    // operator>>=
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs << name << " &_e )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (e);
    o << ", &_e);" << endl;
    o << "return _a.to_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
  }

  emit_marshaller( e );
}



void CodeGenCPPStub::emit_Alias( CORBA::AliasDef_ptr a )
{
  assert( a->def_kind() == CORBA::dk_Alias );

  // Get base for absolute name
  CORBA::String_var tmp = a->absolute_name();
  string abs;
  abs = ID(tmp);
  
  tmp = a->name();
  CORBA::String_var name = (const char *) ID(tmp);
  
  int colon_pos = abs.rfind( "::" );
  if( colon_pos != -1 )
    abs = abs.substr( 2, colon_pos );

  // Emit TypeCode for alias
  CORBA::Container_var container = a->defined_in();
  CORBA::TypeCode_var tc = a->type();
  emit_typecode_def (container, abs, name.in(), tc);
  o << endl;

  if (!is_marshallable (a)) {
    return;
  }

  // We don't need to generate the operator<<=() and operator>>=()
  // marshalling functions. The C++ will automatically pick the
  // appropriate operators of the original types (i.e. the original
  // type of the alias). These operators are only defined for
  // arrays. The T_forany definition for arrays, as described in
  // [16.12(5)], helps to disambiguate arrays with the same
  // structure of the type.
  // The problem with all other aliases is, that the TypeCode isn't
  // set properly. The TypeCode for these aliases always refer to
  // the element_type_def. This is clearly a problem within the
  // CORBA standard. There ought to be a T_forany for every alias.

  CORBA::TypeCode_var orig_tc = tc->content_type();
  
  if( _params.any && tc->is_array() && !orig_tc->is_alias() ) {
    // We have an alias to an array and the original type of the
    // alias is itself not an alias. Emit the marshalling
    // operators
    CORBA::AliasDef_var resolv = CORBA::AliasDef::_duplicate( a );
    CORBA::IDLType_var idl_type = resolv->original_type_def();
    CORBA::TypeCode_var idl_type_tc = idl_type->type();
    while( idl_type_tc->is_alias() ) {
      resolv = CORBA::AliasDef::_narrow( idl_type );
      idl_type = resolv->original_type_def();
      idl_type_tc = idl_type->type();
    }
    CORBA::ArrayDef_var arr = CORBA::ArrayDef::_narrow( idl_type );
    assert( !CORBA::is_nil( arr ) );
    
    o << _params.operator_export << "void operator<<=( CORBA::Any &_a, const ";
    o << abs << name << "_forany &_const_s )" << endl;
    o << BL_OPEN;
    o << "_const_s.alloc();" << endl;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (arr);
    o << ", _const_s.in());" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << "_const_s.clear();" << endl;
    o << BL_CLOSE << endl;
    
    o << _params.operator_export << "CORBA::Boolean operator>>=( const CORBA::Any &_a, ";
    o << abs << name << "_forany &_s )" << endl;
    o << BL_OPEN;
    o << abs << name << "_slice *p;" << endl;
    o << "if (_a.to_static_any (";
    emit_marshaller_ref (arr);
    o << ", (void *&)p)) " << BL_OPEN;
    o << "_s = p;" << endl;
    o << "return TRUE;" << endl;
    o << BL_CLOSE;
    o << "return FALSE;" << endl;
    o << BL_CLOSE << endl << endl;
  }
}

void
CodeGenCPPStub::emit_var_def (const string &type,
			      CORBA::Container_ptr _scope,
			      const string &_name,
			      const string &init)
{
    CORBA::Contained_var scope = CORBA::Contained::_narrow (_scope);
    string name = _name;
    vector<string> modules;

    while (!CORBA::is_nil (scope)) {
	CORBA::String_var s = scope->name();
	if (scope->def_kind() != CORBA::dk_Module) {
	    name = string(s) + "::" + name;
	} else {
	    modules.push_back (s.in());
	}
	CORBA::Container_var container = scope->defined_in();
	scope = CORBA::Contained::_narrow (container);
    }
    if (name.find ("::") == 0)
	name = name.substr (2);

    // mapping is the same for structs and namespaces if outside of modules
    if (modules.size() == 0) {
	o << type << " " << ID(name.c_str()) << init << ";" << endl;
	return;
    }

    for (mico_vec_size_type i = 0; i < modules.size(); ++i) {
	o << "namespace " << ID(modules[modules.size()-1-i].c_str()) << endl << "{" << endl;
    }
    o << type << " " << ID(name.c_str()) << init << ";" << endl;
    for (mico_vec_size_type j = 0; j < modules.size(); ++j) {
	o << "}" << endl;
    }
}

void
CodeGenCPPStub::emit_typecode_def (CORBA::Container_ptr scope,
				   const string &abs,
				   const string &_name,
				   CORBA::TypeCode_ptr tc)
{
  if( !_params.typecode )
    return;

  string name = string("_tc_") + _name;
#ifdef WINDOWS_TC
  o.switchStream( 1 );
  o << abs << name << " = " << endl;
  emit_type_code( tc );
  o << ";" << endl;
  o.switchStream( 0 );
  emit_var_def ("CORBA::TypeCodeConst", scope, name, "");
#else
  string init;
  {
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
#else
    ostrstream ostr;
#endif
    Output save;
    o.save_streams (save);
    o.start_output (ostr);

    o << " = ";
    emit_type_code (tc);

    o.stop_output ();
    o.restore_streams (save);

#ifndef HAVE_ANSI_CPLUSPLUS_HEADERS
    init = ostr.str();
#else
    ostr << ends;
    init = ostr.str();
    ostr.rdbuf()->freeze (0);
#endif
  }
  emit_var_def ("CORBA::TypeCodeConst", scope, name, init);
#endif
}

void
CodeGenCPPStub::emit_ValueRekCheck (CORBA::IRObject_ptr obj,
				    string prefix,
				    CORBA::ULong depth)
{
  if (!contains_a_valuetype (obj)) {
    return;
  }

  switch (obj->def_kind()) {
  case CORBA::dk_Value:
    {
      o << "if (" << prefix << ") " << BL_OPEN;
      o << "res += " << prefix << "->_count_refs (_seen);" << endl;
      o << BL_CLOSE;
    }
    break;

  case CORBA::dk_Struct:
    {
      o << "res += " << prefix << "._count_refs (_seen);" << endl;
    }
    break;

  case CORBA::dk_Union:
    {
      o << "res += " << prefix << "._count_refs (_seen);" << endl;
    }
    break;

  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var sd = CORBA::SequenceDef::_narrow (obj);
      CORBA::IDLType_var idt = sd->element_type_def ();
      string idx = "i";
      idx += xdec (depth);
      if (depth == 0) {
	o << BL_OPEN;
      }
      o << "for (CORBA::ULong " << idx << "=0; "
	<< idx << "<" << prefix << ".length(); "
	<< idx << "++) " << BL_OPEN;
      prefix += "[";
      prefix += idx;
      prefix += "]";
      emit_ValueRekCheck (idt, prefix, depth+1);
      if (depth == 0) {
	o << BL_CLOSE;
      }
      o << BL_CLOSE;
    }
    break;

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var ad = CORBA::ArrayDef::_narrow (obj);
      CORBA::IDLType_var idt = ad->element_type_def ();
      CORBA::ULong length = ad->length();
      string idx = "i";
      idx += xdec (depth);
      if (depth == 0) {
	o << BL_OPEN;
      }
      o << "for (CORBA::ULong " << idx << "=0; "
	<< idx << "<" << length << "; "
	<< idx << "++) " << BL_OPEN;
      prefix += "[";
      prefix += idx;
      prefix += "]";
      emit_ValueRekCheck (idt, prefix, depth+1);
      if (depth == 0) {
	o << BL_CLOSE;
      }
      o << BL_CLOSE;
    }
    break;

  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var ad = CORBA::AliasDef::_narrow (obj);
      CORBA::IDLType_var idt = ad->original_type_def ();
      emit_ValueRekCheck (idt, prefix, depth);
    }
    break;

  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow (obj);
      if (p->kind() == CORBA::pk_value_base) {
	o << "if (" << prefix << ") " << BL_OPEN;
	o << "res += " << prefix << "->_count_refs (_seen);" << endl;
	o << BL_CLOSE;
      }
    }
    break;

  case CORBA::dk_AbstractInterface:
    {
      CORBA::InterfaceDef_var p = CORBA::InterfaceDef::_narrow (obj);
      o << "if (" << prefix << "&& " << prefix << "->_to_value()) "
	<< BL_OPEN;
      o << "res += " << prefix << "->_to_value()->_count_refs (_seen);"
	<< endl;
      o << BL_CLOSE;
    }
    break;
  default:
    break;
  }
}

void
CodeGenCPPStub::emit_ValueReleaser (CORBA::IRObject_ptr obj,
				    string prefix,
				    CORBA::ULong depth)
{
  if (!contains_a_valuetype (obj)) {
    return;
  }

  switch (obj->def_kind()) {
  case CORBA::dk_Value:
    {
      o << "if (" << prefix << ") " << BL_OPEN;
      o << prefix << "->_release_members ();" << endl;
      o << BL_CLOSE;
      o << prefix << " = 0;" << endl;
    }
    break;

  case CORBA::dk_Struct:
    {
      o << prefix << "._release_members ();" << endl;
    }
    break;

  case CORBA::dk_Union:
    {
      o << prefix << "._release_members ();" << endl;
    }
    break;

  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var sd = CORBA::SequenceDef::_narrow (obj);
      CORBA::IDLType_var idt = sd->element_type_def ();
      string idx = "i";
      idx += xdec (depth);
      if (depth == 0) {
	o << BL_OPEN;
      }
      o << "for (CORBA::ULong " << idx << "=0; "
	<< idx << "<" << prefix << ".length(); "
	<< idx << "++) " << BL_OPEN;
      prefix += "[";
      prefix += idx;
      prefix += "]";
      emit_ValueReleaser (idt, prefix, depth+1);
      if (depth == 0) {
	o << BL_CLOSE;
      }
      o << BL_CLOSE;
    }
    break;

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var ad = CORBA::ArrayDef::_narrow (obj);
      CORBA::IDLType_var idt = ad->element_type_def ();
      CORBA::ULong length = ad->length();
      string idx = "i";
      idx += xdec (depth);
      if (depth == 0) {
	o << BL_OPEN;
      }
      o << "for (CORBA::ULong " << idx << "=0; "
	<< idx << "<" << length << "; "
	<< idx << "++) " << BL_OPEN;
      prefix += "[";
      prefix += idx;
      prefix += "]";
      emit_ValueReleaser (idt, prefix, depth+1);
      if (depth == 0) {
	o << BL_CLOSE;
      }
      o << BL_CLOSE;
    }
    break;

  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var ad = CORBA::AliasDef::_narrow (obj);
      CORBA::IDLType_var idt = ad->original_type_def ();
      emit_ValueReleaser (idt, prefix, depth);
    }
    break;

  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow (obj);
      if (p->kind() == CORBA::pk_value_base) {
	o << "if (" << prefix << ") " << BL_OPEN;
	o << prefix << "->_release_members ();" << endl;
	o << BL_CLOSE;
	o << prefix << " = 0;" << endl;
      }
    }
    break;

  case CORBA::dk_AbstractInterface:
    {
      CORBA::InterfaceDef_var p = CORBA::InterfaceDef::_narrow (obj);
      o << "if (" << prefix << "&& " << prefix << "->_to_value()) "
	<< BL_OPEN;
      o << prefix << "->_to_value()->_release_members ();" << endl;
      o << BL_CLOSE;
      o << prefix << " = 0;" << endl;
    }
    break;
  default:
    break;
  }
}

void
CodeGenCPPStub::emit_Value (CORBA::ValueDef_ptr val)
{
  CORBA::String_var id = val->id();
  CORBA::String_var tmp = val->name();
  string className, absClassName;
  string prefix, abs;
  className = ID(tmp);
  tmp = val->absolute_name();
  absClassName = ID(tmp);
  absClassName = absClassName.substr( 2 );

  CORBA::Container_var def_in = val->defined_in ();
  if (def_in->def_kind() == CORBA::dk_Repository) {
    prefix = (const char *) "OBV_";
  }
  else {
    prefix = (const char *) "";
  }

  tmp = val->absolute_name();
  abs = ID(tmp);
  int colon_pos = abs.rfind ("::");
  if (colon_pos != -1) {
    abs = abs.substr (2, colon_pos);
  }

  // contained decls
  emit_container (val);

  o << endl;
  o << "// valuetype " << className << endl;

  CORBA::ValueDef_var base_value = val->base_value ();
  CORBA::ValueDefSeq_var bases = val->abstract_base_values ();
  CORBA::ContainedSeq_var members = val->contents (CORBA::dk_ValueMember, 1);
  CORBA::InterfaceDefSeq_var supported = val->supported_interfaces ();
  CORBA::InterfaceDef_var supported_concrete = CORBA::InterfaceDef::_nil ();
  bool supports_abstract = false;

  for (CORBA::ULong si0=0; si0<supported->length(); si0++) {
    if (supported[si0]->def_kind() == CORBA::dk_Interface) {
      supported_concrete = CORBA::InterfaceDef::_duplicate (supported[si0]);
    }
    else {
      supports_abstract = true;
    }
  }

  string base_name;
  if (!CORBA::is_nil (base_value)) {
    tmp = base_value->absolute_name();
    base_name = ID(tmp);
    base_name = base_name.substr( 2 );
  }

  /*
   * protected default constructor
   */
  o << absClassName << "::" << className << " ()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;

  /*
   * destructor
   */
  o << absClassName << "::~" << className << " ()" << endl;
  o << "{" << endl;
  o << "}" << endl << endl;

  /*
   * _narrow_helper
   */
  o << "void *" << endl;
  o << absClassName << "::_narrow_helper (const char * repoid)" << endl;
  o << BL_OPEN;
  if (!CORBA::is_nil (base_value) || bases->length() > 0 ||
      supports_abstract) {
    o << "void * p;" << endl;
  }
  o << "if (strcmp (repoid, \"" << id << "\") == 0) " << BL_OPEN;
  o << "return (void *) this;" << endl;
  o << BL_CLOSE;
  if (!CORBA::is_nil (base_value)) {
    o << "if ((p = " << base_name <<
      "::_narrow_helper (repoid)) != NULL) " << BL_OPEN;
    o << "return p;" << endl;
    o << BL_CLOSE;
  }
  for (CORBA::ULong i0=0; i0<bases->length(); i0++) {
    tmp = bases[i0]->absolute_name();
    string bases_name;
    bases_name = ID(tmp);
    bases_name = bases_name.substr( 2 );
    o << "if ((p = " << ID(tmp) <<
      "::_narrow_helper (repoid)) != NULL) " << BL_OPEN;
    o << "return p;" << endl;
    o << BL_CLOSE;
  }
  /*
   * Only narrowing to abstract interfaces is supported. It is not possible
   * to narrow a valuetype to a concrete object!
   */
  for (CORBA::ULong i01=0; i01<supported->length(); i01++) {
    if (supported[i01]->def_kind() != CORBA::dk_AbstractInterface) {
      continue;
    }
    tmp = supported[i01]->absolute_name();
    string bases_name;
    bases_name = ID(tmp);
    bases_name = bases_name.substr( 2 );
    o << "if ((p = " << bases_name;
    o << "::_narrow_helper (repoid)) != NULL) " << BL_OPEN;
    o << "return p;" << endl;
    o << BL_CLOSE;
  }
  o << "return NULL;" << endl;
  o << BL_CLOSE << endl;
    
  /*
   * _downcast method
   */

  o << absClassName << " *" << endl;
  o << absClassName << "::_downcast (CORBA::ValueBase * vb) " << endl;
  o << BL_OPEN;
  o << "void * p;" << endl;
  o << "if (vb && ((p = vb->_narrow_helper (\"" << id << "\")))) "
    << BL_OPEN;
  o << "return (" << absClassName << " *) p;" << endl;
  o << BL_CLOSE;
  o << "return 0;" << endl;
  o << BL_CLOSE << endl;

  /*
   * _downcast from abstract base
   */

  o << absClassName << " *" << endl;
  o << absClassName << "::_downcast (CORBA::AbstractBase * vb) " << endl;
  o << BL_OPEN;
  o << "return _downcast (vb->_to_value());" << endl;
  o << BL_CLOSE << endl;

  /*
   * get_value_def
   */

  o << "CORBA::ValueDef_ptr" << endl;
  o << absClassName << "::get_value_def () " << endl;
  o << BL_OPEN;
  o << "CORBA::ORB_var orb = CORBA::ORB_instance (\"mico-local-orb\");"
    << endl;
  o << "CORBA::Object_var irobj = " << endl;
  o << "  orb->resolve_initial_references (\"InterfaceRepository\");"
    << endl;
  o << "CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);"
    << endl;
  o << "if (CORBA::is_nil (ifr)) " << BL_OPEN;
  o << "return CORBA::ValueDef::_nil ();" << endl;
  o << BL_CLOSE << endl;
  o << "CORBA::Contained_var cv = ifr->lookup_id (\"" << id << "\");"
    << endl;
  o << "return CORBA::ValueDef::_narrow (cv);" << endl;
  o << BL_CLOSE << endl;

  /*
   * _copy_members
   */

  if (is_marshallable (val) &&
      !val->is_abstract() && !val->is_custom() &&
      !check_for_possible_circle (val)) {
    o << "void" << endl;
    o << absClassName << "::_copy_members (const " << className << "& other)"
      << endl;
    o << BL_OPEN;
    for (CORBA::ULong i1=0; i1<members->length(); i1++) {
      CORBA::ValueMemberDef_var vmd =
	CORBA::ValueMemberDef::_narrow (members[i1]);
      assert (!CORBA::is_nil (vmd));
      CORBA::IDLType_var idt = vmd->type_def ();
      CORBA::String_var name = vmd->name ();
      CORBA::TypeCode_var tc = idt->type ();
      name = (const char *) ID(name.in());
      
      if (tc->is_valuetype()) {
	emit_type_for_variable (idt, automatic);
	o << " _" << name.in() << " = ";
	emit_type_name (idt);
	o << "::_downcast (other." << name.in() << "()->_copy_value());"
	  << endl;
	o << name.in() << " (_" << name.in() << ");" << endl;
      }
      else {
	o << name.in() << " (other." << name.in() << "());" << endl;
      }
    }
    o << BL_CLOSE << endl;
  }

  /*
   * _copy_value
   */

  if (is_marshallable (val) && !val->is_abstract()) {
    o << "CORBA::ValueBase *" << endl;
    o << absClassName << "::_copy_value ()" << endl;
    o << BL_OPEN;
    if (val->is_custom() || check_for_possible_circle (val)) {
      o << "return CORBA::ValueBase::_copy_value();" << endl;
    }
    else {
      o << "vector<string> _dummy;" << endl;
      o << "string _repo_id = \"" << id << "\";" << endl;
      o << className << " * _res = "
	<< "_downcast (_create (_dummy, _repo_id));" << endl;
      o << "assert (_res != 0);" << endl;
      if (!CORBA::is_nil (base_value)) {
	o << "_res->" << base_name << "::_copy_members (*this);" << endl;
      }
      o << "_res->_copy_members (*this);" << endl;
      o << "return _res;" << endl;
    }
    o << BL_CLOSE << endl;
  }

  /*
   * _get_marshal_info
   */

  if (is_marshallable (val)) {
    o << "void" << endl;
    o << absClassName << "::_get_marshal_info "
      << "(vector<string> & repoids, CORBA::Boolean & chunked)" << endl;
    o << BL_OPEN;
    o << "repoids.push_back (\"" << id << "\");" << endl;
    if (val->is_truncatable()) {
      assert (!CORBA::is_nil (base_value));
      o << base_name << "::_get_marshal_info (repoids, chunked);" << endl;
      o << "chunked = TRUE;" << endl;
    }
    else if (val->is_custom()) {
      o << "chunked = TRUE;" << endl;
    }
    else {
      o << "chunked = FALSE;" << endl;
    }
    o << BL_CLOSE << endl;
  }

  /*
   * _marshal_members
   */

  if (is_marshallable (val)) {
    o << "void" << endl;
    o << absClassName << "::_marshal_members (CORBA::DataEncoder &ec)" << endl;
    o << BL_OPEN;
    if (val->is_custom()) {
      o << "// custom marshalling" << endl;
      o << "CORBA::DataOutputStream_impl os (ec);" << endl;
      o << "marshal (&os);" << endl;
    }
    else {
      if (!CORBA::is_nil (base_value)) {
	o << base_name << "::_marshal_members (ec);" << endl;
      }
      for (CORBA::ULong i3=0; i3<members->length(); i3++) {
	CORBA::ValueMemberDef_var vmd =
	  CORBA::ValueMemberDef::_narrow (members[i3]);
	assert (!CORBA::is_nil (vmd));
	CORBA::IDLType_var idt = vmd->type_def ();
	CORBA::String_var name = vmd->name ();
	CORBA::TypeCode_var tc = idt->type ();
	name = (const char *) ID(name.in());
	
	/*
	 * Necessary variable type is different from anything else
	 */
	
	if (tc->is_string()) {
	  o << "const char *";
	}
	else if (tc->is_wstring()) {
	  o << "const wchar_t *";
	}
	else if (tc->is_array() && !tc->is_alias()) {
	  o << "_" << name << "_slice *";
	}
	else if (tc->is_array() || tc->is_objref() ||
		 tc->is_type_code() || tc->is_base_type() ||
		 tc->is_valuetype()) {
	  emit_type_for_result (idt);
	}
	else if (tc->is_sequence() && !tc->is_alias()) {
	  o << "_" << name << "_seq &";
	}
	else {
	  emit_idl_type_name (idt);
	  o << "&";
	}
	o << " _" << name.in() << " = " << name.in() << " ();" << endl;
	emit_marshaller_ref (idt.in());
	if (tc->is_array()) {
	  o << "->marshal (ec, _" << name.in();
	}
	else {
	  o << "->marshal (ec, &_" << name.in();
	}
	o << ");" << endl;
      }
    }
    o << BL_CLOSE << endl;
  }

  /*
   * _demarshal_members
   */

  if (is_marshallable (val)) {
    o << "CORBA::Boolean" << endl;
    o << absClassName << "::_demarshal_members (CORBA::DataDecoder &dc)"
      << endl;
    o << BL_OPEN;
    if (val->is_custom()) {
      o << "// custom marshalling" << endl;
      o << "CORBA::DataInputStream_impl is (dc);" << endl;
      o << "unmarshal (&is);" << endl;
      o << "return is.is_okay();" << endl;
    }
    else {
      if (!CORBA::is_nil (base_value)) {
	o << "if (!" << base_name << "::_demarshal_members (dc)) " << BL_OPEN;
	o << "  return FALSE;" << endl;
	o << BL_CLOSE;
      }
      for (CORBA::ULong i5=0; i5<members->length(); i5++) {
	CORBA::ValueMemberDef_var vmd =
	  CORBA::ValueMemberDef::_narrow (members[i5]);
	assert (!CORBA::is_nil (vmd));
	CORBA::IDLType_var idt = vmd->type_def ();
	CORBA::String_var name = vmd->name ();
	name = (const char *) ID(name.in());
	
	emit_type_for_variable (idt.in(), automatic);
	o << " _" << name.in();
	emit_array_suffix (idt);
	o << ";" << endl;
	o << "if (!";
	emit_marshaller_ref (idt.in());
	o << "->demarshal (dc, &_" << name.in();
	emit_marshaller_suffix (idt.in(), TRUE);
	o << ")) " << BL_OPEN;
	o << "  return FALSE;" << endl;
	o << BL_CLOSE;
	o << name.in() << " (_" << name.in() << ");" << endl;
      }
      o << "return TRUE;" << endl;
    }
    o << BL_CLOSE << endl;
  }

  /*
   * Implement _count_refs() and _release_mebers() if we can be part
   * of a circular graph.
   */

  if (check_for_possible_circle (val)) {
    o << "CORBA::Long" << endl;
    o << absClassName << "::_count_refs (CORBA::ValueBase::visited * _seen)"
      << endl;
    o << BL_OPEN;
    o << "if (_seen) " << BL_OPEN;
    o << "if (_seen->find (this) != _seen->end()) " << BL_OPEN;
    o << "return -1;" << endl;
    o << BL_CLOSE << BL_CLOSE << endl;
    o << "CORBA::Boolean own = FALSE;" << endl;
    o << "CORBA::Long res = _refcount_value() - 1;" << endl << endl;
    o << "if (_seen == 0) " << BL_OPEN;
    o << "_seen = new CORBA::ValueBase::visited;" << endl;
    o << "own = TRUE;" << endl;
    o << BL_CLOSE;
    o << "_seen->insert (this);" << endl << endl;
    for (CORBA::ULong i6=0; i6<members->length(); i6++) {
      vector<CORBA::IRObject_var> seen;
      if (!check_for_possible_circle_rek (val, members[i6], seen)) {
	continue;
      }
      CORBA::ValueMemberDef_var vmd =
	CORBA::ValueMemberDef::_narrow (members[i6]);
      CORBA::IDLType_var idt = vmd->type_def ();
      CORBA::String_var name = vmd->name ();
      name = (const char *) ID(name.in());
      string prefix = ID(name.in());
      prefix += "()";
      emit_ValueRekCheck (idt, prefix, 0);
    }
    o << endl;
    o << "if (own) " << BL_OPEN;
    o << "delete _seen;" << endl;
    o << BL_CLOSE << endl;
    o << "return res;" << endl;
    o << BL_CLOSE << endl;

    o << "void" << endl;
    o << absClassName << "::_release_members ()" << endl;
    o << BL_OPEN;
    o << "if (_destructing) " << BL_OPEN;
    o << "return;" << endl;
    o << BL_CLOSE;
    o << "_destructing = true;" << endl;
    for (CORBA::ULong i7=0; i7<members->length(); i7++) {
      vector<CORBA::IRObject_var> seen;
      if (!check_for_possible_circle_rek (val, members[i7], seen)) {
	continue;
      }
      CORBA::ValueMemberDef_var vmd =
	CORBA::ValueMemberDef::_narrow (members[i7]);
      assert (!CORBA::is_nil (vmd));
      CORBA::IDLType_var idt = vmd->type_def ();
      CORBA::TypeCode_var tc = vmd->type ();
      CORBA::String_var name = vmd->name ();
      name = (const char *) ID(name.in());
      if (tc->is_valuetype()) {
	o << "if (" << name << "()) " << BL_OPEN;
	o << name << "()->_release_members ();" << endl;
	o << BL_CLOSE;
	o << name << " (0);" << endl;
      }
      else {
	string prefix = name.in();
	prefix += "()";
	emit_ValueReleaser (idt, prefix, 0);
      }
    }
    o << BL_CLOSE << endl;
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
      o << "CORBA::ValueBase *" << endl;
      o << absClassName << "::_to_value ()" << endl;
      o << BL_OPEN;
      o << "return this;" << endl;
      o << BL_CLOSE << endl;
    }
  }

  /*
   * If we have initializers, dump declarations for the _init class
   */

  CORBA::InitializerSeq_var isv = val->initializers();
  
  if (isv->length()) {
    o << absClassName << "_init::" << className << "_init ()" << endl;
    o << "{" << endl;
    o << "}" << endl << endl;

    o << absClassName << "_init::~" << className << "_init ()" << endl;
    o << "{" << endl;
    o << "}" << endl << endl;

    o << "void *" << endl;
    o << absClassName << "_init::_narrow_helper (const char * repoid)" << endl;
    o << BL_OPEN;
    o << "if (strcmp (repoid, \"" << id << "\") == 0) " << BL_OPEN;
    o << "return (void *) this;" << endl;
    o << BL_CLOSE;
    o << "return NULL;" << endl;
    o << BL_CLOSE << endl;

    o << absClassName << "_init *" << endl;
    o << absClassName << "_init::_downcast (CORBA::ValueFactory vf) " << endl;
    o << BL_OPEN;
    o << "void * p;" << endl;
    o << "if (vf && ((p = vf->_narrow_helper (\"" << id << "\")))) "
      << BL_OPEN;
    o << "return (" << absClassName << "_init *) p;" << endl;
    o << BL_CLOSE;
    o << "return 0;" << endl;
    o << BL_CLOSE << endl;
  }

  if (_params.poa && !CORBA::is_nil (supported_concrete)) {
    /*
     * Skeleton's _narrow_helper method. Moved up here from
     * the skeleton because the colocated stubs need them.
     * Since we cannot narrow a valuetype's servant to this
     * type, we only need _narrow_helper, but not narrow.
     */

    string tbase_name;
    tmp = supported_concrete->absolute_name();
    tbase_name = ID(tmp);
    tbase_name = tbase_name.substr (2);

    o << "#ifndef MICO_CONF_NO_POA" << endl << endl;
    o << "void *" << endl;
    o << "POA_" << absClassName << "::_narrow_helper";
    o <<   " (const char * repoid)" << endl;
    o << BL_OPEN;
    o << "void * p;" << endl;
    o << "if ((p = POA_" << tbase_name
      << "::_narrow_helper (repoid)) != NULL) "
      << BL_OPEN;
    o << "return p;" << endl;
    o << BL_CLOSE;

    if (!CORBA::is_nil (base_value)) {
      supported = base_value->supported_interfaces ();
      for (CORBA::ULong si1=0; si1<supported->length(); si1++) {
	if (supported[si1]->def_kind() == CORBA::dk_Interface) {
	  tmp = supported[si1]->absolute_name();
	  tbase_name = ID(tmp);
	  tbase_name = tbase_name.substr (2);
	  o << "if ((p = POA_" << tbase_name
	    << "::_narrow_helper (repoid)) != NULL) "
	    << BL_OPEN;
	  o << "return p;" << endl;
	  o << BL_CLOSE;
	}
      }
    }

    o << "return NULL;" << endl;
    o << BL_CLOSE << endl;
    o << "#endif // MICO_CONF_NO_POA" << endl << endl;
  }

  /*
   * Emit TypeCode for value
   */

  tmp = val->name ();
  CORBA::String_var name = (const char *) ID(tmp);
  CORBA::TypeCode_var tc_in = val->type ();
  emit_typecode_def (def_in.in(), abs.c_str(), name.in(), tc_in.in());
  o << endl;

  if (is_marshallable (val)) {
    emit_marshaller( val );
  }

  if (is_marshallable (val) && _params.any) {
    // operator <<= (copying)
    o << "void" << endl;
    o << "operator<<=( CORBA::Any &_a, const ";
    emit_idl_type_name( val );
    o << " _val )" << endl;
    o << BL_OPEN;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (val);
    o << ", &_val);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    // operator <<= (noncopying)
    o << "void" << endl;
    o << "operator<<=( CORBA::Any &_a, ";
    emit_idl_type_name( val );
    o << "* _val_ptr )" << endl;
    o << BL_OPEN;
    o << "CORBA::ValueBase_var _val = *_val_ptr;" << endl;
    o << "CORBA::StaticAny _sa (";
    emit_marshaller_ref (val);
    o << ", _val_ptr);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;

    // operator >>=
    o << "CORBA::Boolean" << endl;
    o << "operator>>=( const CORBA::Any &_a, ";
    emit_idl_type_name( val );
    o << " &_val_ptr )" << endl;
    o << BL_OPEN;
    emit_idl_type_name( val );
    o << " *p;" << endl;
    o << "if (_a.to_static_any (";
    emit_marshaller_ref (val);
    o << ", (void *&)p)) " << BL_OPEN;
    o << "_val_ptr = *p;" << endl;
    o << "return TRUE;" << endl;
    o << BL_CLOSE;
    o << "return FALSE;" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * Emit OBV stuff for non-abstract values
   */

  if (!val->is_abstract()) {
    bool oldurn = use_rel_names (false);

    o << endl;
    o << "// OBV class for valuetype " << className << endl;

    /*
     * default constructor
     */
    o << "OBV_" << absClassName << "::"
      << prefix << className << " ()" << endl;
    o << "{" << endl;
    o << "}" << endl << endl;

    /*
     * initializing constructor
     */
    CORBA::ValueDef_var base_value = val->base_value ();
    vector<CORBA::ValueDef_var> parents;
    if (!CORBA::is_nil(base_value)) {
	CORBA::ValueDef_var child = val;
	CORBA::ValueDef_var parent;
	while(!CORBA::is_nil((parent = child->base_value()))) {
	    parents.push_back(parent);
	    child = parent;
	}
    }
    if (parents.size() > 0 || members->length() > 0) {
      o << "OBV_" << absClassName << "::" << prefix << className << " (";
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
      for (CORBA::ULong j0=0; j0<all_members->length(); j0++) {
	CORBA::ValueMemberDef_var vmd =
	  CORBA::ValueMemberDef::_narrow (all_members[j0]);
	CORBA::IDLType_var idt = vmd->type_def ();
	CORBA::String_var name = vmd->name ();
	CORBA::TypeCode_var tc = idt->type ();
	/*
	 * Must take care of anonymous types
	 */
	if (tc->is_sequence() && !tc->is_alias()) {
	  o << "const _" << ID(name.in()) << "_seq&";
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
	if (j0+1 != all_members->length()) {
	  o << ", ";
	}
      }
      o << ")" << endl;
      o << BL_OPEN;
      for (CORBA::ULong j1=0; j1<all_members->length(); j1++) {
	CORBA::ValueMemberDef_var vmd =
	  CORBA::ValueMemberDef::_narrow (all_members[j1]);
	CORBA::IDLType_var idt = vmd->type_def ();
	CORBA::String_var name = vmd->name ();
	string p_name = "_";
	p_name += name.in();
	o << "this->" << name << "(" << p_name << ");" << endl;
      }
      o << BL_CLOSE << endl;
    }
    
    /*
     * destructor
     */
    o << "OBV_" << absClassName << "::~" << prefix << className
      << " ()" << endl;
    o << "{" << endl;
    o << "}" << endl << endl;

    /*
     * modifier implementation
     */

    string complete_name = "OBV_";
    complete_name += absClassName;

    for (CORBA::ULong j3=0; j3<members->length(); j3++) {
      CORBA::ValueMemberDef_var vmd =
	CORBA::ValueMemberDef::_narrow (members[j3]);
      CORBA::IDLType_var idt = vmd->type_def ();
      CORBA::String_var name = vmd->name ();
      emit_union_member_accessor (complete_name.c_str(),
				  idt.in(), name.in());
    }

    use_rel_names (oldurn);
  }
}

void
CodeGenCPPStub::emit_ValueBox (CORBA::ValueBoxDef_ptr vb)
{
  CORBA::String_var id = vb->id();
  CORBA::String_var tmp = vb->name();
  string className, absClassName;
  string prefix, abs;
  className = ID(tmp);
  tmp = vb->absolute_name();
  absClassName = ID(tmp);
  absClassName = absClassName.substr( 2 );

  CORBA::Container_var def_in = vb->defined_in ();
  CORBA::IDLType_var boxed_value = vb->original_type_def ();
  CORBA::IDLType_var orig = CORBA::IDLType::_duplicate (boxed_value);

  bool oldurn = use_rel_names (false);

  while (orig->def_kind() == CORBA::dk_Alias) {
    CORBA::AliasDef_var ad = CORBA::AliasDef::_narrow (orig);
    orig = ad->original_type_def ();
  }

  CORBA::TypeCode_var orig_tc = orig->type ();
  // kcg: unalias does not return duplicated TC!
  CORBA::TypeCode_ptr tc = orig_tc->unalias ();

  o << endl;
  o << "// valuebox " << className << endl;

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
      CORBA::PrimitiveDef_var pd = CORBA::PrimitiveDef::_narrow (boxed_value);
      /*
       * Default, init and copy constructor
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      o << absClassName << "::" << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      if (tc->is_objref() && !CORBA::is_nil (pd) &&
	  pd->kind() == CORBA::pk_objref) {
	o << "value = ::CORBA::Object::_duplicate (_p);" << endl;
      }
      else if (tc->is_type_code() && !CORBA::is_nil (pd) &&
	       pd->kind() == CORBA::pk_TypeCode) {
	o << "value = ::CORBA::TypeCode::_duplicate (_p);" << endl;
      }
      else if (tc->is_objref() || tc->is_type_code()) {
	o << "value = ";
	emit_type_name (boxed_value);
	o << "::_duplicate (_p);" << endl;
      }
      else {
	o << "value = _p;" << endl;
      }
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = _p.value;" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      if (tc->is_objref() && !CORBA::is_nil (pd) &&
	  pd->kind() == CORBA::pk_objref) {
	o << "value = ::CORBA::Object::_duplicate (_p);" << endl;
      }
      else if (tc->is_type_code() && !CORBA::is_nil (pd) &&
	       pd->kind() == CORBA::pk_TypeCode) {
	o << "value = ::CORBA::TypeCode::_duplicate (_p);" << endl;
      }
      else if (tc->is_objref() || tc->is_type_code()) {
	o << "value = ";
	emit_type_name (boxed_value);
	o << "::_duplicate (_p);" << endl;
      }
      else {
	o << "value = _p;" << endl;
      }
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      emit_type_for_result (boxed_value);
      o << endl << absClassName << "::_value() const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      if (tc->is_objref() && !CORBA::is_nil (pd) &&
	  pd->kind() == CORBA::pk_objref) {
	o << "value = ::CORBA::Object::_duplicate (_p);" << endl;
      }
      else if (tc->is_type_code() && !CORBA::is_nil (pd) &&
	       pd->kind() == CORBA::pk_TypeCode) {
	o << "value = ::CORBA::TypeCode::_duplicate (_p);" << endl;
      }
      else if (tc->is_objref() || tc->is_type_code()) {
	o << "value = ";
	emit_type_name (boxed_value);
	o << "::_duplicate (_p);" << endl;
      }
      else {
	o << "value = _p;" << endl;
      }
      o << BL_CLOSE << endl;
    }
    break;

  case CORBA::tk_struct:
    {
      /*
       * Default, init and copy constructor
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (boxed_value);
      o << ";" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (boxed_value);
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (boxed_value);
      o << " (_p.value);" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (boxed_value);
      o << " (_p);" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      o << "const ";
      emit_type_name (boxed_value);
      o << "&" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      emit_type_name (boxed_value);
      o << "&" << endl;
      o << absClassName << "::_value ()" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (boxed_value);
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;

      // generate member accessor functions
      CORBA::StructDef_var sd = CORBA::StructDef::_narrow (orig);
      CORBA::StructMemberSeq_var sv = sd->members ();
      for (CORBA::ULong i=0; i<sv->length(); i++) {
	CORBA::IDLType_var mt = CORBA::IDLType::_duplicate (sv[i].type_def);
	emit_union_member_accessor (absClassName.c_str(),
				    mt.in(), sv[i].name.in(),
				    "value->");
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

      /*
       * Default, init and copy constructor
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className
	<< " (" << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className
	<< " (const " << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className
	<< " (const " << string_type << " & _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = _p.value;" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator=" 
	<< " (" << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << " &" << endl;
      o << absClassName << "::operator="
	<< " (const " << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << " &" << endl;
      o << absClassName << "::operator="
	<< " (const " << string_type << " & _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      o << "const " << char_type << " *" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value" 
	<< " (" << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value"
	<< " (const " << char_type << " * _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value"
	<< " (const " << string_type << " & _p)" << endl;
      o << BL_OPEN;
      o << "value = _p;" << endl;
      o << BL_CLOSE << endl;

      /*
       * inherited index operators
       */
      o << char_type << " &" << endl;
      o << absClassName << "::operator[] (::CORBA::ULong idx)" << endl;
      o << BL_OPEN;
      o << "return value[idx];" << endl;
      o << BL_CLOSE << endl;

      o << char_type << endl;
      o << absClassName << "::operator[] (::CORBA::ULong idx) const" << endl;
      o << BL_OPEN;
      o << "return value[idx];" << endl;
      o << BL_CLOSE << endl;
    }
    break;

  case CORBA::tk_union:
    {
      /*
       * Default, init and copy constructor
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (orig);
      o << ";" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (";
      emit_type_for_param (orig, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (orig);
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (orig);
      o << " (_p.value);" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (";
      emit_type_for_param (orig, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (orig);
      o << " (_p);" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      o << "const ";
      emit_type_name (orig);
      o << "&" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      emit_type_name (orig);
      o << "&" << endl;
      o << absClassName << "::_value ()" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (";
      emit_type_for_param (orig.in(), CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      emit_type_name (orig);
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;

      /*
       * For union member access, we overload operator->
       */

      emit_type_name (orig);
      o << "*" << endl;
      o << absClassName << "::operator-> () const" << endl;
      o << BL_OPEN;
      o << "return value.operator-> ();" << endl;
      o << BL_CLOSE << endl;
    }
    break;

  case CORBA::tk_sequence:
    {
      CORBA::SequenceDef_var sd = CORBA::SequenceDef::_narrow (orig);
      CORBA::IDLType_var mt = sd->element_type_def();
      CORBA::IDLType_var omt = CORBA::IDLType::_duplicate (mt);

      while (omt->def_kind() == CORBA::dk_Alias) {
	CORBA::AliasDef_var adv = CORBA::AliasDef::_narrow (omt);
	omt = adv->original_type_def();
      }

      /*
       * Default, init and copy constructor, plus some inherited ones
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << "value = new _value_type;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className
	<< " (const _value_type & _p)" << endl;
      o << BL_OPEN;
      o << "value = new _value_type (_p);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = new _value_type (_p.value);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (::CORBA::ULong max)" 
        << endl;
      o << BL_OPEN;
      o << "value = new _value_type (max);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className
	<< " (::CORBA::ULong max, ::CORBA::ULong length, " 
	<<   "_elem_type * data, ::CORBA::Boolean rel)" << endl;
      o << BL_OPEN;
      o << "value = new _value_type (max, length, data, rel);" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (const _value_type & _p)" << endl;
      o << BL_OPEN;
      o << "value = new _value_type (_p);" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */

      o << "const " << absClassName << "::_value_type &" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::_value_type &" << endl;
      o << absClassName << "::_value ()" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (const _value_type & _p)" << endl;
      o << BL_OPEN;
      o << "value = new _value_type (_p);" << endl;
      o << BL_CLOSE << endl;

      /*
       * Inherited stuff
       */

      o << "const " << absClassName << "::_auto_elem_type &" << endl;
      o << absClassName << "::operator[] (::CORBA::ULong i) const" << endl;
      o << BL_OPEN;
      o << "return value->operator[] (i);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::_auto_elem_type &" << endl;
      o << absClassName << "::operator[] (::CORBA::ULong i)" << endl;
      o << BL_OPEN;
      o << "return value->operator[] (i);" << endl;
      o << BL_CLOSE << endl;

      o << "::CORBA::ULong" << endl;
      o << absClassName << "::maximum () const" << endl;
      o << BL_OPEN;
      o << "return value->maximum ();" << endl;
      o << BL_CLOSE << endl;

      o << "::CORBA::Boolean" << endl;
      o << absClassName << "::release () const" << endl;
      o << BL_OPEN;
      o << "return value->release ();" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::replace (::CORBA::ULong max, "
        <<                            "::CORBA::ULong length, "
	<<                            "_elem_type * data, "
        <<                            "::CORBA::Boolean rel)"
	<< endl;
      o << BL_OPEN;
      o << "value->replace (max, length, data, rel);" << endl;
      o << BL_CLOSE << endl;

      o << "::CORBA::ULong" << endl;
      o << absClassName << "::length () const" << endl;
      o << BL_OPEN;
      o << "return value->length ();" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::length (::CORBA::ULong nl)" << endl;
      o << BL_OPEN;
      o << "value->length (nl);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::_elem_type *" << endl;
      o << absClassName << "::allocbuf (::CORBA::ULong len)" << endl;
      o << BL_OPEN;
      o << "return new _elem_type[len];" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::freebuf (_elem_type * data)" << endl;
      o << BL_OPEN;
      o << "delete [] data;" << endl;
      o << BL_CLOSE << endl;
    }
    break;

  case CORBA::tk_any:
    {
      CORBA::PrimitiveDef_var pd = CORBA::PrimitiveDef::_narrow (boxed_value);

      /*
       * Default, init and copy constructor
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << "value = new ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << ";" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << " (_p.value);" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << " (_p);" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      o << "const ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << "&" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << "&" << endl;
      o << absClassName << "::_value ()" << endl;
      o << BL_OPEN;
      o << "return value;" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = new ";
      if (!CORBA::is_nil (pd) && pd->kind() == CORBA::pk_any) {
	o << "::CORBA::Any";
      }
      else {
	emit_type_name (boxed_value);
      }
      o << " (_p);" << endl;
      o << BL_CLOSE << endl;
    }
    break;

  case CORBA::tk_array:
    {
      /*
       * Default, init and copy constructor, plus some inherited ones
       */
      o << absClassName << "::" << className << " ()" << endl;
      o << BL_OPEN;
      o << "value = ";
      emit_type_name (boxed_value);
      o << "_alloc();" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = ";
      emit_type_name (boxed_value);
      o << "_dup (_p);" << endl;
      o << BL_CLOSE << endl;

      o << absClassName << "::" << className << " (const "
	<< className << "& _p)" << endl;
      o << BL_OPEN;
      o << "value = ";
      emit_type_name (boxed_value);
      o << "_dup (_p.value.in());" << endl;
      o << BL_CLOSE << endl;

      /*
       * destructor
       */
      o << absClassName << "::~" << className << " ()" << endl;
      o << "{" << endl;
      o << "}" << endl << endl;

      /*
       * assignment operator
       */
      o << absClassName << " &" << endl;
      o << absClassName << "::operator= (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = ";
      emit_type_name (boxed_value);
      o << "_dup (_p);" << endl;
      o << "return *this;" << endl;
      o << BL_CLOSE << endl;

      /*
       * Accessor
       */
      o << "const ";
      emit_type_name (boxed_value);
      o << "_slice *" << endl;
      o << absClassName << "::_value () const" << endl;
      o << BL_OPEN;
      o << "return value.in();" << endl;
      o << BL_CLOSE << endl;

      emit_type_name (boxed_value);
      o << "_slice *" << endl;
      o << absClassName << "::_value ()" << endl;
      o << BL_OPEN;
      o << "return value.inout();" << endl;
      o << BL_CLOSE << endl;

      o << "void" << endl;
      o << absClassName << "::_value (";
      emit_type_for_param (boxed_value, CORBA::PARAM_IN);
      o << " _p)" << endl;
      o << BL_OPEN;
      o << "value = ";
      emit_type_name (boxed_value);
      o << "_dup (_p);" << endl;
      o << BL_CLOSE << endl;

      /*
       * Inherited stuff
       */

      o << "const ";
      emit_type_name (boxed_value);
      o << "_slice &" << endl;
      o << absClassName << "::operator[] (::CORBA::ULong i) const" << endl;
      o << BL_OPEN;
      o << "return value[i];" << endl;
      o << BL_CLOSE << endl;

      emit_type_name (boxed_value);
      o << "_slice &" << endl;
      o << absClassName << "::operator[] (::CORBA::ULong i)" << endl;
      o << BL_OPEN;
      o << "return value[i];" << endl;
      o << BL_CLOSE << endl;
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

  /*
   * _boxed_in
   */

  if (orig->def_kind() == CORBA::dk_Sequence) {
    o << "const " << absClassName << "::_value_type &";
  }
  else if (orig->def_kind() == CORBA::dk_Array) {
    o << "const ";
    emit_type_name (boxed_value);
    o << "_slice *";
  }
  else {
    emit_type_for_param (boxed_value, CORBA::PARAM_IN);
  }
  o << endl << absClassName << "::_boxed_in() const" << endl;
  o << BL_OPEN;
  o << "return value";
  if ((!tc->is_base_type() && !tc->is_enum()) ||
      (tc->is_string() || tc->is_wstring() || tc->is_any())) {
    o << ".in()";
  }
  o << ";" << endl << BL_CLOSE << endl;
  
  /*
   * _boxed_inout
   */

  if (orig->def_kind() == CORBA::dk_Sequence) {
    o << absClassName << "::_value_type &";
  }
  else if (orig->def_kind() == CORBA::dk_Array) {
    emit_type_name (boxed_value);
    o << "_slice *";
  }
  else {
    emit_type_for_param (boxed_value, CORBA::PARAM_INOUT);
  }
  o << endl << absClassName << "::_boxed_inout()" << endl;
  o << BL_OPEN;
  o << "return value";
  if ((!tc->is_base_type() && !tc->is_enum()) ||
      (tc->is_string() || tc->is_wstring() || tc->is_any())) {
    o << ".inout()";
  }
  o << ";" << endl << BL_CLOSE << endl;
  
  /*
   * _boxed_out
   */

  if (orig->def_kind() == CORBA::dk_Sequence) {
    o << absClassName << "::_value_type *&";
  }
  else if (orig->def_kind() == CORBA::dk_Array) {
    emit_type_name (boxed_value);
    if (tc->is_variable()) {
      o << "_slice *&";
    }
    else {
      o << "_slice *";
    }
  }
  else {
    emit_type_for_param (boxed_value, CORBA::PARAM_OUT);
  }
  o << endl << absClassName << "::_boxed_out()" << endl;
  o << BL_OPEN;
  o << "return value";
  if ((!tc->is_base_type() && !tc->is_enum()) ||
      (tc->is_string() || tc->is_wstring() || tc->is_any())) {
    o << ".out()";
  }
  o << ";" << endl << BL_CLOSE << endl;

  /*
   * _narrow_helper
   */

  o << "void *" << endl;
  o << absClassName << "::_narrow_helper (const char * repoid)" << endl;
  o << BL_OPEN;
  o << "if (strcmp (repoid, \"" << id << "\") == 0) " << BL_OPEN;
  o << "return (void *) this;" << endl;
  o << BL_CLOSE;
  o << "return NULL;" << endl;
  o << BL_CLOSE << endl;
    
  /*
   * downcast
   */

  o << absClassName << " *" << endl;
  o << absClassName << "::_downcast (::CORBA::ValueBase *vb)" << endl;
  o << BL_OPEN;
  o << "void * p;" << endl;
  o << "if (vb && ((p = vb->_narrow_helper (\"" << id << "\")))) "
    << BL_OPEN;
  o << "return (::" << absClassName << " *) p;" << endl;
  o << BL_CLOSE;
  o << "return 0;" << endl;
  o << BL_CLOSE << endl;

  /*
   * downcast from abstract base; just for the sake of completeness,
   * value boxes cannot inherit from abstract bases anyway.
   */

  o << absClassName << " *" << endl;
  o << absClassName << "::_downcast (::CORBA::AbstractBase *)" << endl;
  o << BL_OPEN;
  o << "return 0;" << endl;
  o << BL_CLOSE << endl;

  /*
   * get_value_def
   */

  o << "::CORBA::ValueDef_ptr" << endl;
  o << absClassName << "::get_value_def () " << endl;
  o << BL_OPEN;
  o << "return ::CORBA::ValueDef::_nil ();" << endl;
  o << BL_CLOSE << endl;

  /*
   * _copy_value
   */

  if (is_marshallable (vb)) {
    o << "::CORBA::ValueBase *" << endl;
    o << absClassName << "::_copy_value ()" << endl;
    o << BL_OPEN;
    o << "return new " << className << " (*this);" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * _get_marshal_info
   */

  if (is_marshallable (vb)) {
    o << "void" << endl;
    o << absClassName << "::_get_marshal_info "
      << "(vector<string> & repoids, ::CORBA::Boolean & chunked)" << endl;
    o << BL_OPEN;
    o << "repoids.push_back (\"" << id << "\");" << endl;
    o << "chunked = FALSE;" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * _marshal_members
   */

  if (is_marshallable (vb)) {
    o << "void" << endl;
    o << absClassName << "::_marshal_members (::CORBA::DataEncoder &ec)" 
      << endl;
    o << BL_OPEN;
    emit_marshaller_ref (orig.in());
    o << "->marshal (ec, &value";
    if (orig->def_kind() == CORBA::dk_Sequence 
        || orig->def_kind() == CORBA::dk_Struct
        || orig->def_kind() == CORBA::dk_Union) {
      o << ".inout()";
    }
    else {
      emit_marshaller_suffix (orig.in(), 0);
    }
    o << ");" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * _demarshal_members
   */

  if (is_marshallable (vb)) {
    o << "::CORBA::Boolean" << endl;
    o << absClassName << "::_demarshal_members (::CORBA::DataDecoder &ec)"
      << endl;
    o << BL_OPEN;
    o << "return ";
    emit_marshaller_ref (orig.in());
    o << "->demarshal (ec, &value";
    if (orig->def_kind() == CORBA::dk_Sequence
        || orig->def_kind() == CORBA::dk_Struct
        || orig->def_kind() == CORBA::dk_Union) {
      o << ".inout()";
    }
    else {
      emit_marshaller_suffix (orig.in(), 1);
    }
    o << ");" << endl;
    o << BL_CLOSE << endl;
  }

  /*
   * Emit TypeCode for value
   */

  tmp = vb->name ();
  CORBA::String_var name = (const char *) ID(tmp);

  tmp = vb->absolute_name();
  abs = ID(tmp);
  int colon_pos = abs.rfind ("::");
  if (colon_pos != -1) {
    abs = abs.substr (2, colon_pos);
  }

  CORBA::TypeCode_var tc_in = vb->type ();
  emit_typecode_def (def_in.in(), abs.c_str(), name.in(), tc_in.in());
  o << endl;

  if (!is_marshallable (vb)) {
    return;
  }

  emit_marshaller( vb );

  if (_params.any) {
    // operator <<= (copying)
    o << "void" << endl;
    o << "operator<<=( ::CORBA::Any &_a, const ";
    emit_idl_type_name( vb );
    o << " _val )" << endl;
    o << BL_OPEN;
    o << "::CORBA::StaticAny _sa (";
    emit_marshaller_ref (vb);
    o << ", &_val);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;
    
    // operator <<= (noncopying)
    o << "void" << endl;
    o << "operator<<=( ::CORBA::Any &_a, ";
    emit_idl_type_name( vb );
    o << "* _val_ptr )" << endl;
    o << BL_OPEN;
    o << "::CORBA::ValueBase_var _val = *_val_ptr;" << endl;
    o << "::CORBA::StaticAny _sa (";
    emit_marshaller_ref (vb);
    o << ", _val_ptr);" << endl;
    o << "_a.from_static_any (_sa);" << endl;
    o << BL_CLOSE << endl;

    // operator >>=
    o << "::CORBA::Boolean" << endl;
    o << "operator>>=( const ::CORBA::Any &_a, ";
    emit_idl_type_name( vb );
    o << " &_val_ptr )" << endl;
    o << BL_OPEN;
    emit_idl_type_name( vb );
    o << " *p;" << endl;
    o << "if (_a.to_static_any (";
    emit_marshaller_ref (vb);
    o << ", (void *&)p)) " << BL_OPEN;
    o << "_val_ptr = *p;" << endl;
    o << "return TRUE;" << endl;
    o << BL_CLOSE;
    o << "return FALSE;" << endl;
    o << BL_CLOSE << endl;
  }

  use_rel_names (oldurn);

}
