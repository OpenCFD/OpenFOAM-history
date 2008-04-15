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
#include <ctype.h>
#include <stdio.h>
#include "codegen-c++-util.h"
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

/*
 * The class CPPTypeFolder removes all those anonymous types from a
 * sequence of IRObjects, whose type in the C++ language would be
 * the same. This is necessary to avoid multiple definitions for
 * the same type. E.g., the IDL specification:
 *
 *   struct S {
 *     //...
 *   };
 *
 *   typedef S S_Alias;
 *   typedef sequence<S> seq1;
 *   typedef sequence<S_Alias> seq2;;
 *
 * only code for sequence<S> should be generated, but not for 
 * sequence<S_Alias>, because under the C++ mapping, those IDL
 * types denote the same C++ type.
 */
CPPTypeFolder::CPPTypeFolder( IRObjSeq& objs )
{
  do_type_folding( objs );
}

    
/*
 * This method checks, if marshalling code needs to be generated for
 * the sequence or not (return == 'true' means no code needs to be
 * generated. This is done by checking if "seq" is contained in
 * "_seq_types". If not, it returns false and add "seq" to
 * "_seq_types". If yes, true is returned. The definition of
 * 'contained' is governed by the C++ type inference rule, whereupon:
 * sequence<string> and typedef string id; sequence<id> both refer to
 * the same C++ type. There is also no code generated for sequence<T>
 * where T is a basic data type for unbounded sequences. Those
 * marshalling routines are already part of libmico.a
 */
bool
CPPTypeFolder::check_for_sequence( CORBA::TypeCode_ptr seq )
{
  if( seq->kind() != CORBA::tk_sequence )
    return false;

  for( CORBA::ULong i = 0; i < _seq_types.length(); i++ ) {
    if( seq->equal( _seq_types[ i ], TRUE, TRUE ) )
      return true;
  }
  int len = _seq_types.length();
  _seq_types.length( len + 1 );
  _seq_types[ len ] = CORBA::TypeCode::_duplicate( seq );
  return false;
}



/*
 * This method checks, if "arr" is contained in "_arr_types". If not,
 * it returns false and adds "arr" to "_arr_types". If yes, true is
 * returned.
 */
bool
CPPTypeFolder::check_for_array( CORBA::TypeCode_ptr arr )
{
  if( arr->kind() != CORBA::tk_array )
    return false;

  for( CORBA::ULong i = 0; i < _arr_types.length(); i++ ) {
    if( arr->equal( _arr_types[ i ], TRUE, TRUE ) )
      return true;
  }
  int len = _arr_types.length();
  _arr_types.length( len + 1 );
  _arr_types[ len ] = CORBA::TypeCode::_duplicate( arr );
  return false;
}



/*
 * This method checks, if "fix" is contained in "_fix_types". If not,
 * it returns false and adds "fix" to "_fix_types". If yes, true is
 * returned.
 */
bool
CPPTypeFolder::check_for_fixed( CORBA::TypeCode_ptr fix )
{
  if( fix->kind() != CORBA::tk_fixed )
    return false;

  for( CORBA::ULong i = 0; i < _fix_types.length(); i++ ) {
    if( fix->equal( _fix_types[ i ], TRUE, TRUE ) )
      return true;
  }
  int len = _fix_types.length();
  _fix_types.length( len + 1 );
  _fix_types[ len ] = CORBA::TypeCode::_duplicate( fix );
  return false;
}

bool
CPPTypeFolder::check_for_string( CORBA::TypeCode_ptr str )
{
  for( CORBA::ULong i = 0; i < _str_types.length(); i++ ) {
    if( str->equal( _str_types[ i ], TRUE, TRUE ) )
      return true;
  }
  int len = _str_types.length();
  _str_types.length( len + 1 );
  _str_types[ len ] = CORBA::TypeCode::_duplicate( str );
  return false;
}

bool
CPPTypeFolder::check_for_wstring( CORBA::TypeCode_ptr wstr )
{
  for( CORBA::ULong i = 0; i < _wstr_types.length(); i++ ) {
    if( wstr->equal( _wstr_types[ i ], TRUE, TRUE ) )
      return true;
  }
  int len = _wstr_types.length();
  _wstr_types.length( len + 1 );
  _wstr_types[ len ] = CORBA::TypeCode::_duplicate( wstr );
  return false;
}

void
CPPTypeFolder::do_type_folding( IRObjSeq& objs )
{
  for( CORBA::ULong i = 0; i < objs.length(); i++ ) {
    CORBA::IDLType_var t = CORBA::IDLType::_narrow( objs[ i ]->obj );
    if( !CORBA::is_nil( t ) ) {

      bool redundant = false;
      
      CORBA::TypeCode_var tc = t->type();
      switch( t->def_kind() ) {
      case CORBA::dk_Sequence:
	redundant = check_for_sequence( tc );
	break;
      case CORBA::dk_Array:
	redundant = check_for_array( tc );
	break;
      case CORBA::dk_Fixed:
	redundant = check_for_fixed( tc );
	break;
      case CORBA::dk_String:
	redundant = check_for_string( tc );
	break;
      case CORBA::dk_Wstring:
	redundant = check_for_wstring( tc );
	break;
      default:
	break;
      }
      
      if( redundant ) {
        // We can remove the current anonymous type
        for( CORBA::ULong j = i; j < objs.length() - 1; j++ ) {
          objs[ j ] = objs[ j + 1 ];
        }
        objs.length( objs.length() - 1 );
        i--;
      }
    }
  }
}


//----------------------------------------------------------------------


CodeGenCPPUtil::CodeGenCPPUtil( DB &db, IDLParam& params,
				CORBA::Container_ptr con )
  : CodeGen( con ),
    _params( params )
{
  _db = &db;
  _emit_rel_names = true;
}



/*
 * The following operation checks if an IDL-identifier happens to
 * be a C++ keyword. If not, then the same identifier is returned. If
 * the identifier is a C++ keyword, then "_cxx_" is prepended
 * to avoid compiler error. See [20.1.2]
 */
char *CodeGenCPPUtil::ID( const char* ident )
{
  static const char* keywords[] =
  { "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool",
    "break", "case", "catch", "char", "class", "compl", "const",
    "const_cast", "continue", "default", "delete", "do", "double",
    "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false",
    "float", "for", "friend", "goto", "if", "inline", "int", "long",
    "mutable", "namespace", "new", "not", "not_eq", "operator", "or",
    "or_eq", "private", "protected", "public", "register",
    "reinterpret_cast", "return", "short", "signed", "sizeof", "static",
    "static_cast", "struct", "switch", "template", "this", "throw",
    "true", "try", "typedef", "typeid", "typename", "union", "unsigned",
    "using", "virtual", "void", "volatile", "wchar_t", "while", "xor",
    "xor_eq" 
  };

  static char *buf = NULL;

  string in = ident;
  string out;
  string current_id;

  while( in.length() > 0 ) {
    int colon_pos = in.find ( "::" );
    if( colon_pos == 0 ) {
      out += "::";
      in = in.substr( 2 );
      continue;
    }
    
    if( colon_pos != -1 ) {
      current_id = in.substr( 0, colon_pos );
      in = in.substr( colon_pos );
    } else {
      current_id = in;
      in = "";
    }
  
    for( size_t i = 0; i < sizeof( keywords ) / sizeof( char* ); i++ ) {
      if( current_id == keywords[ i ] ) {
	out += "_cxx_";
	break;
      }
    }
    out += current_id;
  }
  CORBA::string_free( buf );
  buf = CORBA::string_dup( (char *) out.c_str() );
  return buf;
}


void CodeGenCPPUtil::set_name_prefix( CORBA::Container_ptr c )
{
  if( CORBA::is_nil( c ) || c->def_kind() == CORBA::dk_Repository ) {
    _name_prefix = "::";
      
    return;
  }
  
  CORBA::Contained_var con = CORBA::Contained::_narrow( c );
  assert( !CORBA::is_nil( con ) );
  
  CORBA::String_var abs_name = con->absolute_name();
  _name_prefix = abs_name;
}

void CodeGenCPPUtil::save_name_prefix()
{
  _name_prefix_bak = _name_prefix;
}

void CodeGenCPPUtil::restore_name_prefix()
{
  _name_prefix = _name_prefix_bak;
}

void
CodeGenCPPUtil::enter_scope( CORBA::Container_ptr c,
			     string toplevel_prefix )
{
  vector<CORBA::Contained_var> trace_current_scope;
  vector<CORBA::Contained_var> trace_new_scope;
  CORBA::Container_var         scope;
  bool                         nl;

  // Decompose the current scope to a sequence of Contained
  // and store it in trace_current_scope
  scope = _current_scope;
  while( scope->def_kind() != CORBA::dk_Repository ) {
    CORBA::Contained_ptr scope_contained = CORBA::Contained::_narrow( scope );
    assert( !CORBA::is_nil( scope_contained ) );
    trace_current_scope.insert( trace_current_scope.begin(), scope_contained );
    scope = scope_contained->defined_in();
  }
  
  // Decompose the target scope to a sequence of Contained
  // and store it in trace_new_scope
  scope = CORBA::Container::_duplicate( c );
  while( scope->def_kind() != CORBA::dk_Repository ) {
    CORBA::Contained_ptr scope_contained = CORBA::Contained::_narrow( scope );
    assert( !CORBA::is_nil( scope_contained ) );
    trace_new_scope.insert( trace_new_scope.begin(), scope_contained );
    scope = scope_contained->defined_in();
  }
  
  // Compare both sequences and look for the first difference
  vector<CORBA::Contained_var>::iterator i, j;
  for( i = trace_current_scope.begin(), j = trace_new_scope.begin();
       i != trace_current_scope.end() && j != trace_new_scope.end() &&
	 (*i)->_is_equivalent( *j );
       i++, j++ ) ;
  
  // Backtrace and close all modules up to the difference found above
  nl = false;
  while( i != trace_current_scope.end() ) {
    o << "}" << endl;
    nl = true;
    i++;
  }
  if( nl || j != trace_new_scope.end() )
    o << endl << endl;

  // Open modules up to the target scope
  nl = false;
  while( j != trace_new_scope.end() ) {
    CORBA::Container_var def_in = (*j)->defined_in();
    CORBA::String_var tmp = (*j)->name();
    CORBA::String_var name = (const char *) ID(tmp);

    if (def_in->def_kind() == CORBA::dk_Repository) {
      o << "namespace " << toplevel_prefix << name << endl << "{" << endl;
    }
    else {
      o << "namespace " << name << endl << "{" << endl;
    }
    nl = true;
    j++;
  }
  if( nl )
    o << endl;
  
  _current_scope = CORBA::Container::_duplicate( c );

}

bool CodeGenCPPUtil::use_rel_names( bool rn )
{
  bool cur = _emit_rel_names;
  _emit_rel_names = rn;
  return cur;
}

void CodeGenCPPUtil::emit_rel_name( CORBA::Object_ptr obj )
{
  CORBA::Contained_var con = CORBA::Contained::_narrow( obj );
  assert( !CORBA::is_nil( con ) );
  
  CORBA::String_var name_tmp = con->absolute_name();
  string name = name_tmp.in();

  /*
   * First check that we don't have the case that:
   * _name_prefix = ::A::B
   *         name = ::B
   * we need to keep 'name' as an absolute name, otherwise
   * it would refer to the wrong B
   */
  unsigned int i;
  for( i = 1; i + name.length() <= _name_prefix.length(); i++ ) {
    if( _name_prefix.substr( i, name.length() ) == name ) {
      name = ID (name.c_str());
      o << name;
      return;
    }
  }
  
  int l = _name_prefix.length() < name.length() ?
      _name_prefix.length() : name.length();
  assert(l >= 0);
  for( i = 0; i < (unsigned int)l; i++ ) {
    if( _name_prefix[ i ] != name[ i ] )
      break;
  }
  
  if( i == name.length() ) {
    /*
     * We have the case that:
     *   _name_prefix = ::A::B::C::D
     *           name = ::A::B
     * in this case 'name' should be just 'B'
     */
    int j = name.find_last_of( ":" );
    name = name.substr( j + 1 );
  } else if( i == _name_prefix.length() ) {
    /*
     * We have the case that:
     * _name_prefix = ::A::B
     *         name = ::A::B::C::D
     * in this case name should be 'C::D'
     */
    if( name[ i ] != ':' ) {
      while( name[ i - 1 ] != ':' )
	i--;
    }
    else {
      i += 2;
    }
    name = name.substr( i );
  } else {
    /*
     * We have the case that:
     * _name_prefix = ::A::B::C
     *         name = ::A::D::E
     * in this case name should be 'D::E'
     */
    while( name[ --i ] != ':' )
      ;
    // keep leading "::" if name refers to global scope
    if (i > 1)
      name = name.substr( i + 1 );
  }

  name = ID (name.c_str());
  // why not use the absolute name? FP
  // if( name[ 0 ] == ':' )
  //  name = name.substr( 2 );

  o << name;
}


void CodeGenCPPUtil::emit_type_name( CORBA::Object_ptr obj )
{
  if (_emit_rel_names) {
    emit_rel_name (obj);
    return;
  }

  CORBA::Contained_var con = CORBA::Contained::_narrow( obj );
  assert( !CORBA::is_nil( con ) );
  
  CORBA::String_var name_tmp = con->absolute_name();
  string name;

  name = (const char *) ID(name_tmp);

  // why not use the absolute name? FP
  // name = name.substr (2);

  o << name;
}

void CodeGenCPPUtil::emit_mangled_name( const char* s )
{
  string str;
  int j;
  
  str = ID(s);
  while( 1 ) {
    j = str.find( "::" );
    if( j < 0 )
      break;
    str = str.replace( j, 2, "_" );
  }
  o << str;
}



/*
 * This method determines, if code has to be be generated by the definition
 * refered to "obj". No code needs to be generated if "obj" has been defined
 * in an included file. In this case we only emit an appropriate #include
 * statement. We use the map _repoid_fname_map to determine, whether "obj"
 * has been defined in an included file.
 */
bool CodeGenCPPUtil::check_for_included_defn( CORBA::IRObject_ptr obj,
					      bool emitinc )
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
    fname += ".";
    fname += _params.hh_suffix;
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


bool CodeGenCPPUtil::is_base_sequence (CORBA::SequenceDef_ptr sdef)
{
    CORBA::TypeCode_var seq_tc = sdef->type();

    CORBA::TypeCode_ptr tc = seq_tc->unalias();
    assert (tc->kind() == CORBA::tk_sequence);

    if (tc->length() != 0)
        return false;

    CORBA::TypeCode_var content_tc = tc->content_type();
    tc = content_tc->unalias();

    return (tc->is_base_type() && tc->kind() != CORBA::tk_fixed) ||
      tc->is_object() || tc->is_any();
}

bool CodeGenCPPUtil::is_variable_for_sii (CORBA::TypeCode_ptr tc)
{
    return !tc->is_string() && !tc->is_wstring() && !tc->is_objref() &&
      !tc->is_type_code() && !tc->is_valuetype() &&
      !tc->is_abstract_interface() && tc->is_variable();
}

void CodeGenCPPUtil::emit_any_param( CORBA::IDLType_ptr p,
				     char *var_name, bool is_from )
{
  const char *wrapper = is_from ? "CORBA::Any::from_" : "CORBA::Any::to_";

  CORBA::IDLType_var resolved_alias = CORBA::IDLType::_duplicate( p );
  while( resolved_alias->def_kind() == CORBA::dk_Alias ) {
    CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( resolved_alias );
    resolved_alias = a->original_type_def();
  }
  
  CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( resolved_alias );
  if( !CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_objref ) {
    o << wrapper << "object( " << ID(var_name);
    if( is_from )
      o << ", \"Object\"";
    o << " )";
    return;
  }
  
  CORBA::TypeCode_var tc = p->type();
  CORBA::TypeCode_ptr tc_resolv = tc->unalias();
  
  CORBA::TCKind tk = tc_resolv->kind();

  if( tc->is_array() && tc->is_alias() ) {
    if( is_from ) {
      emit_idl_type_name( p );
      o << "_forany( ";
      o << var_name << " )";
    } else {
      o << var_name;
    }
    return;
  }
  
  if( tk == CORBA::tk_objref ) {
    o << "(";
    emit_type_for_variable( p, manual );
    if( !is_from )
      o << "&";
    o << ") ";
  }

  if( tk == CORBA::tk_char )
    o << wrapper << "char( " << ID(var_name) << " )";
  else if( tk == CORBA::tk_wchar )
    o << wrapper << "wchar( " << ID(var_name) << " )";
  else if( tk == CORBA::tk_octet )
    o << wrapper << "octet( " << ID(var_name) << " )";
  else if( tk == CORBA::tk_boolean )
    o << wrapper << "boolean( " << ID(var_name) << " )";
  else if( tk == CORBA::tk_string ) {
    if( tc_resolv->length() == 0 ) {
      // Unbounded string
      o << ID(var_name);
    } else {
      // Bounded string
      o << wrapper << "string( " << ID(var_name) << ", "
	<< tc_resolv->length() << " )";
    }
  } else if( tk == CORBA::tk_wstring ) {
    if( tc_resolv->length() == 0 ) {
      // Unbounded wstring
      o << ID(var_name);
    } else {
      // Bounded wstring
      o << wrapper << "wstring( " << ID(var_name) << ", "
	<< tc_resolv->length() << " )";
    }
  } else
    o << ID(var_name);
}


void CodeGenCPPUtil::emit_const_value (CORBA::Any* val)
{
    CORBA::TypeCode_var tc = val->type();
    CORBA::TCKind kind = tc->kind();
    
    if (kind == CORBA::tk_fixed)
	o << "(CORBA::LongDouble) ";
    if (kind == CORBA::tk_wstring || kind == CORBA::tk_wchar)
	o << "L";
    o << val;
    if (kind == CORBA::tk_ulong)
      o << "UL";
    else if (kind == CORBA::tk_long)
      o << "L";
    else if (kind == CORBA::tk_ulonglong)
	o << "ULL";
    else if (kind == CORBA::tk_longlong)
	o << "LL";
}


void CodeGenCPPUtil::emit_type_code( CORBA::TypeCode_ptr tc )
{
  string str = tc->stringify();
  
  bool beginning_of_line = true;
  
  for( string::size_type i = 0; i < str.length(); i++ ) {
    if( beginning_of_line ) {
      o << "\"";
      beginning_of_line = false;
    }
    string c;
    c += str[ i ];
    o << c;
    if( (i+1) % 60 == 0 ) {
      o << "\"" << endl;
      beginning_of_line = true;
    }
  }
  if( !beginning_of_line )
    o << "\"";
}

void CodeGenCPPUtil::emit_type_code_name( CORBA::IRObject_ptr obj )
{
  CORBA::IDLType_var t = CORBA::IDLType::_narrow (obj);
  if (!CORBA::is_nil (t)) {
    emit_type_code_name (t);
  } else {
    CORBA::ExceptionDef_var e = CORBA::ExceptionDef::_narrow (obj);
    assert (!CORBA::is_nil (e));

    CORBA::String_var tmp = e->absolute_name();
    string abs;
    abs = ID(tmp);
    tmp = e->name();
    CORBA::String_var name = (const char *) ID(tmp);

    int colon_pos = abs.rfind( "::" );
    if( colon_pos != -1 )
      abs = abs.substr( 2, colon_pos );
    o << abs << "_tc_" << name;
  }
}

void CodeGenCPPUtil::emit_type_code_name( CORBA::IDLType_ptr t )
{
  CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( t );
  if( !CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_objref ) {
    o << "CORBA::_tc_Object";
    return;
  }
  
  CORBA::TypeCode_var tc = t->type();
  switch( tc->kind() ) {
  case CORBA::tk_void:
    o << "CORBA::_tc_void";
    break;
  case CORBA::tk_short:
    o << "CORBA::_tc_short";
    break;
  case CORBA::tk_long:
    o << "CORBA::_tc_long";
    break;
  case CORBA::tk_ushort:
    o << "CORBA::_tc_ushort";
    break;
  case CORBA::tk_ulong:
    o << "CORBA::_tc_ulong";
    break;
  case CORBA::tk_longlong:
    o << "CORBA::_tc_longlong";
    break;
  case CORBA::tk_ulonglong:
    o << "CORBA::_tc_ulonglong";
    break;
  case CORBA::tk_float:
    o << "CORBA::_tc_float";
    break;
  case CORBA::tk_double:
    o << "CORBA::_tc_double";
    break;
  case CORBA::tk_longdouble:
    o << "CORBA::_tc_longdouble";
    break;
  case CORBA::tk_boolean:
    o << "CORBA::_tc_boolean";
    break;
  case CORBA::tk_char:
    o << "CORBA::_tc_char";
    break;
  case CORBA::tk_wchar:
    o << "CORBA::_tc_wchar";
    break;
  case CORBA::tk_octet:
    o << "CORBA::_tc_octet";
    break;
  case CORBA::tk_any:
    o << "CORBA::_tc_any";
    break;
  case CORBA::tk_TypeCode:
    o << "CORBA::_tc_TypeCode";
    break;
  case CORBA::tk_Principal:
    o << "CORBA::_tc_Principal";
    break;
  case CORBA::tk_objref:
  case CORBA::tk_struct:
  case CORBA::tk_union:
  case CORBA::tk_enum:
  case CORBA::tk_alias:
  case CORBA::tk_value:
  case CORBA::tk_value_box:
  case CORBA::tk_native:
  case CORBA::tk_abstract_interface:
  case CORBA::tk_local_interface:
    {
      CORBA::Contained_var cv = CORBA::Contained::_narrow( t );
      CORBA::String_var tmp = cv->absolute_name();
      string abs_name = ID(tmp);
      int i = abs_name.rfind( "::" );
      assert( i >= 0 );
      abs_name.insert( i + 2, "_tc_" );
      abs_name = abs_name.substr( 2 );
      o << abs_name;
      break;
    }
  case CORBA::tk_string:
    if( tc->length() == 0 )
      o << "CORBA::_tc_string";
    else
      o << "_tc_string_" << tc->length();
    break;
  case CORBA::tk_wstring:
    if( tc->length() == 0 )
      o << "CORBA::_tc_wstring";
    else
      o << "_tc_wstring_" << tc->length();
    break;
  case CORBA::tk_fixed:
    o << "_tc_fixed_" << tc->fixed_digits() << "_";
    if( tc->fixed_scale() < 0 )
      o << "n" << -( tc->fixed_scale() );
    else
      o << tc->fixed_scale();
    break;
  case CORBA::tk_sequence:
    // Sequences are anonymous types and don't have a _tc_* name
    // They are only referred to indirectly (through aliases e.g.)
    assert( 0 );
  case CORBA::tk_array:
    assert( 0 );
  default:
    assert( 0 );
  }
}



void CodeGenCPPUtil::emit_idl_type_name( CORBA::IDLType_ptr t )
{
  string abs_name;

  CORBA::TypeCode_var tc = t->type();
  
  switch( t->def_kind() ) {
  case CORBA::dk_Struct:
  case CORBA::dk_Union:
  case CORBA::dk_Enum:
    {
      emit_type_name (t);
      break;
    }
  case CORBA::dk_Alias:
    {
      emit_type_name (t);
      if( tc->is_objref() )
        o << "_ptr";
      else if( tc->is_valuetype() )
	o << "*";
      break;
    }
  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
  case CORBA::dk_LocalInterface:
  case CORBA::dk_Component:
    {
      emit_type_name (t);
      o << "_ptr";
      break;
    }
  case CORBA::dk_Native:
    {
      CORBA::NativeDef_var n = CORBA::NativeDef::_narrow (t);
      CORBA::String_var tmp = n->absolute_name ();

      if (strcmp (tmp.in(), "::CORBA::AbstractBase") == 0) {
	// class CORBA::AbstractBase exists in the C++ mapping
	o << "CORBA::AbstractBase_ptr";
	break;
      }

      emit_type_name (t);
      break;
    }
  case CORBA::dk_Primitive:
    {
      // Gotta be an any, objref or a TypeCode
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow( t );
      if( p->kind() == CORBA::pk_any )
	o << "CORBA::Any";
      else if( p->kind() == CORBA::pk_objref )
	o << "CORBA::Object_ptr";
      else if( p->kind() == CORBA::pk_TypeCode )
	o << "CORBA::TypeCode_ptr";
      else if( p->kind() == CORBA::pk_value_base )
	o << "CORBA::ValueBase*";
      else {
	cout << p->kind() << endl;
	assert( 0 );
      }
      break;
    }
  case CORBA::dk_Value:
  case CORBA::dk_ValueBox:
  case CORBA::dk_Event:
    {
      emit_type_name (t);
      o << "*";
      break;
    }
  default:
    cout << t->def_kind() << endl;
    assert( 0 );
  }
}
  


/*
 * Emit a type to be used in a signature of a C++ method.
 * The ParameterMode is necessary for this.
 */
void CodeGenCPPUtil::emit_type_for_param( CORBA::IDLType_ptr t,
					  CORBA::ParameterMode mode )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  // Mapping for arrays
  if( tc->is_array() ) {
    if( mode == CORBA::PARAM_IN )
      o << "const ";
    if( !tc->is_alias() ) {
      // Arrays are normally referenced through an alias. But
      // if used in an ExceptionDef, the constructor must reference
      // the array directly. But this can only happen for IN params
      assert( mode == CORBA::PARAM_IN );
      emit_array( t );
    } else {
      emit_idl_type_name( t );
      if (mode == CORBA::PARAM_OUT)
	  o << "_out";
    }
    return;
  }

  if( tc->is_sequence() && !tc->is_alias() ) {
    // Sequences are normally referenced through an alias. But
    // if used in an ExceptionDef, the constructor must reference
    // the sequence directly. But this can only happen for IN params
    assert( mode == CORBA::PARAM_IN );
    emit_sequence( t );
    return;
  }
  
  
  // Basic argument mapping
  if( tc->is_string() ) {
    if( mode == CORBA::PARAM_IN )
      o << "const char*";
    else if( mode == CORBA::PARAM_OUT )
	o << "CORBA::String_out";
    else
      o << "char*&";
    return;
  }

  if( tc->is_wstring() ) {
    if( mode == CORBA::PARAM_IN )
      o << "const CORBA::WChar*";
    else if (mode == CORBA::PARAM_OUT)
	o << "CORBA::WString_out";
    else
      o << "CORBA::WChar*&";
    return;
  }

  if( emit_base_type( t ) ) {
    if (mode == CORBA::PARAM_OUT)
	o << "_out";
    if (mode == CORBA::PARAM_INOUT)
      o << "&";
    return;
  }

  if (tc->is_enum()) {
    emit_type_name( t );
    if (mode == CORBA::PARAM_OUT)
	o << "_out";
    else if( mode == CORBA::PARAM_INOUT )
	o << "&";
    return;
  }

  if (tc->is_native()) {
    CORBA::NativeDef_var n = CORBA::NativeDef::_narrow (t);
    CORBA::String_var tmp = n->absolute_name ();

    // class CORBA::AbstractBase exists in the C++ mapping
    // and needs special treatment

    if (strcmp (tmp.in(), "::CORBA::AbstractBase") == 0) {
      o << "CORBA::AbstractBase";

      switch (mode) {
      case CORBA::PARAM_IN:
	o << "_ptr";
	break;
      case CORBA::PARAM_OUT:
	o << "_out";
	break;
      case CORBA::PARAM_INOUT:
	o << "_ptr&";
	break;
      }
      return;
    }

    emit_type_name (t);

    if (mode == CORBA::PARAM_OUT)
      o << "_out";
    else if( mode == CORBA::PARAM_INOUT )
      o << "&";

    return;
  }

  if( tc->is_type_code() ) {
    o << "CORBA::TypeCode";
    switch (mode) {
    case CORBA::PARAM_IN:
	o << "_ptr";
	break;
    case CORBA::PARAM_OUT:
	o << "_out";
	break;
    case CORBA::PARAM_INOUT:
	o << "_ptr&";
	break;
    }
    return;
  }

  if (tc->is_object()) {
    o << "CORBA::Object";
    switch (mode) {
    case CORBA::PARAM_IN:
	o << "_ptr";
	break;
    case CORBA::PARAM_OUT:
	o << "_out";
	break;
    case CORBA::PARAM_INOUT:
	o << "_ptr&";
	break;
    }
    return;
  }

  if( tc->is_objref() || 
      tc->is_abstract_interface() ||
      tc->unalias()->kind() == CORBA::tk_local_interface ) {
    emit_type_name( t );
    if (mode == CORBA::PARAM_OUT)
	o << "_out";
    else
	o << "_ptr";
    if( mode == CORBA::PARAM_INOUT )
	o << "&";
    return;
  }

  if( tc->is_valuetype() ) {
    if (t->def_kind() == CORBA::dk_Primitive) {
      o << "CORBA::ValueBase";
    }
    else {
      emit_type_name (t);
    }
    if (mode == CORBA::PARAM_OUT)
      o << "_out";
    else if (mode == CORBA::PARAM_INOUT)
      o << "*&";
    else
      o << "*";
    return;
  }

  if( mode == CORBA::PARAM_IN )
    o << "const ";

  switch( tc->kind() ) {
  case CORBA::tk_struct:
  case CORBA::tk_union:
    emit_type_name( t );
    if( mode == CORBA::PARAM_OUT && tc->is_variable() )
	o << "_out";
    else
	o << "&";
    break;
  case CORBA::tk_alias:
    emit_type_name( t );
    if( mode == CORBA::PARAM_OUT )
	o << "_out";
    else {
	if( tc->is_objref() )
	    o << "_ptr";
	else if( tc->is_valuetype() )
	    o << "*";
	o << "&";
    }
    break;
  case CORBA::tk_any:
      o << "CORBA::Any";
      switch (mode) {
      case CORBA::PARAM_IN:
	  o << "&";
	  break;
      case CORBA::PARAM_OUT:
	  o << "_out";
	break;
      case CORBA::PARAM_INOUT:
	  o << "&";
	  break;
      }
      break;
  default:
    cout << tc->kind() << endl;
    assert( 0 );
  }
}



/*
 * Emit a TypeCode to be used as a result in a signature of a
 * C++ method. See Table 24 in [16.18(13)]
 */
void CodeGenCPPUtil::emit_type_for_result( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  if( tc->is_array() ) {
    // Mapping for arrays
    emit_idl_type_name( t );
    o << "_slice*";
    return;
  }

  if( tc->is_string() ) {
    o << "char*";
    return;
  }
  if( tc->is_wstring() ) {
    o << "CORBA::WChar*";
    return;
  }

  if( !emit_base_type( t ) ) {
    switch( tc->kind() ) {
    case CORBA::tk_objref:
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_alias:
    case CORBA::tk_enum:
    case CORBA::tk_native:
    case CORBA::tk_abstract_interface:
    case CORBA::tk_local_interface:
    case CORBA::tk_value:
    case CORBA::tk_value_box:
      emit_idl_type_name( t );
      break;
    case CORBA::tk_any:
      o << "CORBA::Any";
      break;
    case CORBA::tk_TypeCode:
      o << "CORBA::TypeCode_ptr";
      break;
    case CORBA::tk_Principal:
      o << "CORBA::Principal_ptr";
      break;
    default:
      cout << "TCKind: " << tc->kind() << endl;
      assert( 0 );
    }
  }
  
  // Basic argument mapping [16.18(12)]
  if( tc->is_variable() && !tc->is_objref() && !tc->is_type_code() &&
      !tc->is_abstract_interface() && 
      !(tc->unalias()->kind() == CORBA::tk_local_interface) &&
      !tc->is_valuetype() &&
      !tc->is_native() )
    o << "*";
}



void CodeGenCPPUtil::emit_sequence( CORBA::IDLType_ptr type )
{
  assert( type->def_kind() == CORBA::dk_Sequence );
  CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( type );
  CORBA::TypeCode_var seq_tc = s->type();
  CORBA::IDLType_var idl_type = s->element_type_def();
  CORBA::TypeCode_var tc = idl_type->type();

  if( s->bound() != 0 )
    o << "Bounded";

  if( tc->is_string() ) {
    // Use a special template for sequence of strings due to different
    // T* data constructor
    o << "StringSequenceTmpl<CORBA::String_var";
    if( s->bound() != 0 )
      o << "," << xdec( s->bound() );
    o << ">";
    return;
  }
  if( tc->is_wstring() ) {
    // Use a special template for sequence of strings due to different
    // T* data constructor
    o << "WStringSequenceTmpl<CORBA::WString_var";
    if( s->bound() != 0 )
      o << "," << xdec( s->bound() );
    o << ">";
    return;
  }

  if( tc->is_objref() || tc->is_object() ) {
    // Use a special template for sequence of interfaces due to different
    // T* data constructor
    o << "IfaceSequenceTmpl< ";
    emit_type_for_variable( idl_type, automatic );
    o << ",";
    emit_type_for_variable( idl_type, manual );
    if( s->bound() != 0 )
      o << "," << xdec( s->bound() );
    o << ">";
    return;
  }
  
  bool use_ind_template;

  if( tc->is_array() || seq_tc->is_recursive_seq() ) {
    use_ind_template = true;
    o << "SequenceIndTmpl< ";
  } else {
    use_ind_template = false;
    o << "SequenceTmpl< ";
  }

  if( tc->is_array() ) {
    CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( idl_type );
    assert( !CORBA::is_nil( a ) );
    CORBA::IDLType_var orig = a->original_type_def();
    CORBA::TypeCode_var orig_tc = orig->type();
    CORBA::ULong num_elem = compute_num_array_elem( orig_tc );
    CORBA::String_var tmp = a->absolute_name();
    CORBA::String_var name = (const char *) ID(tmp);
    emit_type_for_variable( orig, automatic );
    o << "," << name << "," << num_elem;
  } else if( seq_tc->is_recursive_seq() ) {
    emit_type_for_variable( idl_type, automatic );
    o << ",";
    emit_type_for_variable( idl_type, automatic );
    o << ",1";
  } else
    emit_type_for_variable( idl_type, automatic );

  if (tc->is_char())
    o << ",MICO_TID_CHAR";
  else if( tc->is_wchar() )
    o << ",MICO_TID_WCHAR";
  else if (tc->is_octet())
    o << ",MICO_TID_OCTET";
  else if (tc->is_bool())
    o << ",MICO_TID_BOOL";
  else if (!use_ind_template)
    o << ",MICO_TID_DEF";

  if( s->bound() != 0 )
    o << "," << s->bound();
  if( tc->kind() == CORBA::tk_fixed && s->bound() == 0 )
    o << " ";
  if( tc->kind() == CORBA::tk_sequence )
    o << " ";
  o << ">";
}
  


void CodeGenCPPUtil::emit_array( CORBA::IDLType_ptr type )
{
  assert( type->def_kind() == CORBA::dk_Array ||
	  type->def_kind() == CORBA::dk_Alias );

  CORBA::IDLType_var idl_type = CORBA::IDLType::_duplicate( type );

  CORBA::AliasDef_var alias;
  while( idl_type->def_kind() == CORBA::dk_Alias ) {
    alias = CORBA::AliasDef::_narrow( idl_type );
    assert( !CORBA::is_nil( alias ) );
    idl_type = alias->original_type_def();
  }
  assert( idl_type->def_kind() == CORBA::dk_Array );
  
  CORBA::ArrayDef_var arr;
  while( idl_type->def_kind() == CORBA::dk_Array ) {
    arr = CORBA::ArrayDef::_narrow( idl_type );
    assert( !CORBA::is_nil( arr ) );
    idl_type = arr->element_type_def();
  }
  emit_type_for_variable( idl_type, automatic );
}



void CodeGenCPPUtil::emit_fixed( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  o << "FixedTmpl< " << tc->fixed_digits() << "," << tc->fixed_scale() << ">";
}



void CodeGenCPPUtil::emit_array_suffix( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  while( tc->kind() == CORBA::tk_array ) {
    o << "[ " << tc->length() << " ]";
    tc = tc->content_type();
  }
}



void CodeGenCPPUtil::emit_array_slice_suffix( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  tc = tc->content_type();
  while( tc->kind() == CORBA::tk_array ) {
    o << "[ " << tc->length() << " ]";
    tc = tc->content_type();
  }
}



CORBA::ULong
CodeGenCPPUtil::compute_num_array_elem( CORBA::TypeCode_ptr tc )
{
  CORBA::TypeCode_var mtc = CORBA::TypeCode::_duplicate (tc);
  CORBA::ULong len = 1;

  while (mtc->kind() == CORBA::tk_array ||
	 mtc->kind() == CORBA::tk_alias) {
    if (mtc->kind() == CORBA::tk_array) {
      len *= mtc->length();
    }
    mtc = mtc->content_type();
  }

  return len;
}


void CodeGenCPPUtil::emit_assignment( const string &d, const string &s,
				      CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  if( tc->is_array() ) {
    // Need to use special array copy
    if( tc->is_alias() ) {
      // This is an alias of an array. We can therefore use a MICO
      // specific, pre-defined array copy operator
      emit_idl_type_name( t );
      o << "_copy( " << d << ", " << s << " );" << endl;
    } else {
      // This is not an alias. We therefore have to copy the elements
      // of the array manually
      string iter = string( "_iter_") + d;
      // Cure for parse error before `.' because of an iter with a dot
      CORBA::ULong _pos = iter.find('.');
      while ( _pos != (CORBA::ULong) -1 ) {   // != string::npos
	// Dot found
	iter = iter.substr(0, _pos) + string( "_" ) + iter.substr(_pos+1);
	// possibility of a recurrence of dot(s)?
	_pos = iter.find('.');
      }
      o << "for( int " << iter << " = 0; " << iter << " < ";
      CORBA::ULong num_elem = compute_num_array_elem( tc );
      o << xdec( num_elem ) << "; " << iter << "++ )" << endl << indent;
      o << d << "[ " << iter << " ] = " << s << "[ " << iter << " ];";
      o << endl << exdent;
    }
  } else {
    o << d << " = " << s << ";" << endl;
  }
}

  

/*
 * Emit a TypeCode to be used in a C++ variable declaration.
 */
void CodeGenCPPUtil::emit_type_for_variable( CORBA::IDLType_ptr t,
					     Storage s )
{
  CORBA::String_var tmp;
  
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  if( tc->is_string() ) {
    if( tc->is_alias() ) {
      emit_type_name (t);
      if( s == automatic )
	o << "_var";
    } else {
      if( s == manual )
	o << "char*";
      else
	o << "CORBA::String_var";
    }
    return;
  }

  if( tc->is_wstring() ) {
    if( tc->is_alias() ) {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( t );
      emit_type_name (t);
      if( s == automatic )
	o << "_var";
    } else {
      if( s == manual )
	o << "CORBA::WChar*";
      else
	o << "CORBA::WString_var";
    }
    return;
  }

  CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( t );
  if( !CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_objref ) {
    o << "CORBA::Object";
    if( s == manual )
      o << "_ptr";
    else
      o << "_var";
    return;
  }
  else if( !CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_value_base ) {
    o << "CORBA::ValueBase";
    if( s == manual )
      o << "*";
    else
      o << "_var";
    return;
  }
  
  if( tc->is_valuetype() ) {
    emit_type_name (t);
    if( s == manual )
      o << "*";
    else
      o << "_var";
    return;
  }

  if( tc->is_objref() || tc->is_abstract_interface() ||
      tc->unalias()->kind() == CORBA::tk_local_interface) {
    // This can be an object reference or an alias to "Object"
    emit_type_name (t);
    if( s == manual )
      o << "_ptr";
    else
      o << "_var";
    return;
  }

  if( !emit_base_type( t ) ) {
    switch( tc->kind() ) {
    case CORBA::tk_struct:
    case CORBA::tk_union:
    case CORBA::tk_enum:
    case CORBA::tk_alias:
    case CORBA::tk_any:
    case CORBA::tk_native:
      emit_idl_type_name( t );
      break;
    case CORBA::tk_sequence:
      emit_sequence( t );
      break;
    case CORBA::tk_TypeCode:
      o << "CORBA::TypeCode";
      if( s == manual )
	o << "_ptr";
      else
	o << "_var";
      return;
      break;
    case CORBA::tk_array:
      emit_array( t );
      if( s == manual )
	o << "_slice";
      // the case "s == automatic" is taken care of
      // in emit_array_suffix
      break;
    default:
      assert( 0 );
    }
  }
  if( s == manual && tc->is_variable() )
    o << "*";
}

/*
 * Emit variable initialization
 */
void CodeGenCPPUtil::emit_variable_initialization( CORBA::IDLType_ptr t,
						   Storage s )
{
    if (s == automatic) {
	// it is needed only for not _var types
	return;
    }
    CORBA::TypeCode_var tc = t->type();
    assert( !CORBA::is_nil( tc ) );

    CORBA::PrimitiveDef_var prim = CORBA::PrimitiveDef::_narrow( t );
    if ( !CORBA::is_nil(prim) && prim->kind() == CORBA::pk_objref ) {
	o << " = CORBA::Object::_nil()";
	return;
    }

    if( tc->is_object() || tc->is_objref()
	|| tc->is_abstract_interface()
	|| tc->unalias()->kind() == CORBA::tk_local_interface) {
	// This can be an object reference or an alias to "Object"
	o << " = ";
	emit_type_name (t);
	o << "::_nil()";
	return;
    }

    if( tc->is_string() || tc->is_wstring()
	|| tc->is_valuetype() || tc->is_variable()
	//|| (!CORBA::is_nil( prim ) && prim->kind() == CORBA::pk_value_base)
	) {
	o << " = NULL";
	return;
    }
    return;
}

void
CodeGenCPPUtil::emit_union_member_decl( CORBA::IDLType_ptr m_type,
					const char * name,
					bool as_pure_virtual )
{
  CORBA::TypeCode_var tc = m_type->type();
  CORBA::String_var m_name = (const char *) ID(name);
  string prefix, postfix;

  if (as_pure_virtual) {
    prefix = "virtual ";
    postfix = " = 0";
  }

  if( tc->is_string() ) {
    // Accessor functions for string
    o << prefix << "void " << m_name << "( char *_p )" << postfix << ";" << endl;
    o << prefix << "void " << m_name << "( const char *_p )" << postfix << ";" << endl;
    o << prefix << "void " << m_name << "( const CORBA::String_var &_p )" << postfix << ";" << endl;
    o << prefix << "const char *" << m_name << "() const" << postfix << ";" << endl << endl;
  } else if( tc->is_wstring() ) {
    // Accessor functions for wstring
    o << prefix << "void " << m_name << "( wchar_t *_p )" << postfix << ";" << endl;
    o << prefix << "void " << m_name << "( const wchar_t *_p )" << postfix << ";" << endl;
    o << prefix << "void " << m_name << "( const CORBA::WString_var &_p )" << postfix << ";" << endl;
    o << prefix << "const wchar_t *" << m_name << "() const" << postfix << ";" << endl << endl;
  } else if( tc->is_array() && !tc->is_alias() ) {
    // We have an anonymous array. See [16.10(7)]
    o << "typedef ";
    emit_array( m_type );
    o << " _" << ID(m_name) << "_slice";
    emit_array_slice_suffix( m_type );
    o << ";" << endl;
    o << prefix << "void " << m_name << "(const ";
    emit_array( m_type );
    o << " _p";
    emit_array_suffix( m_type );
    o << " )" << postfix << ";" << endl;
    o << prefix << "const _" << m_name << "_slice* "
      << m_name << "() const" << postfix << ";" << endl;
    o << prefix << "_" << m_name << "_slice* "
      << m_name << "()" << postfix << ";" << endl << endl;
  } else if( tc->is_array() || tc->is_objref() ||
	     tc->is_type_code() || tc->is_base_type() ||
	     tc->is_valuetype() || tc->is_abstract_interface() ||
	     tc->unalias()->kind() == CORBA::tk_local_interface) {
    // Accessor functions for array or object reference
    o << prefix << "void " << m_name << "( ";
    if( tc->is_array() ) {
      // Mapping for arrays differs from standard PARAM_IN mapping!
      o << "const ";
      emit_idl_type_name( m_type );
    }
    else
      emit_type_for_param( m_type, CORBA::PARAM_IN );
    o << " _p )" << postfix << ";" << endl;
    o << prefix;
    emit_type_for_result( m_type );
    o << " " << m_name << "() const" << postfix << ";" << endl << endl;
  } else if( tc->is_sequence() && !tc->is_alias() ) {
    // An array, which is not referred to by an alias. Note
    // That this is a special case, as emit_type_for_param
    // does not handle this situation
    o << "typedef ";
    emit_sequence( m_type );
    o << " _" << m_name << "_seq;" << endl;
    o << prefix << "void " << m_name << "( const ";
    o << "_" << m_name << "_seq & _p)" << postfix << ";" << endl;
    o << prefix << "const _" << m_name << "_seq & "
      << m_name << "() const" << postfix << ";" << endl;
    o << prefix << "_" << m_name << "_seq & "
      << m_name << "()" << postfix << ";" << endl << endl;
  } else {
    // Gotta be struct, union or sequence
    o << prefix << "void " << m_name << "( ";
    emit_type_for_param( m_type, CORBA::PARAM_IN );
    o << " _p )" << postfix << ";" << endl;
    o << prefix << "const ";
    emit_idl_type_name( m_type );
    o << "& " << m_name << "() const" << postfix << ";" << endl;
    o << prefix;
    emit_idl_type_name( m_type );
    o << "& " << m_name << "()" << postfix << ";" << endl << endl;
  }
}


bool CodeGenCPPUtil::emit_base_type( CORBA::IDLType_ptr t )
{
  CORBA::TypeCode_var tc = t->type();
  assert( !CORBA::is_nil( tc ) );

  // Mapping of Base Data Types [16.5]
  switch( tc->kind() ) {
  case CORBA::tk_void:
    o << "void";
    break;
  case CORBA::tk_boolean:
    o << "CORBA::Boolean";
    break;
  case CORBA::tk_char:
    o << "CORBA::Char";
    break;
  case CORBA::tk_wchar:
    o << "CORBA::WChar";
    break;
  case CORBA::tk_octet:
    o << "CORBA::Octet";
    break;
  case CORBA::tk_ushort:
    o << "CORBA::UShort";
    break;
  case CORBA::tk_ulong:
    o << "CORBA::ULong";
    break;
  case CORBA::tk_short:
    o << "CORBA::Short";
    break;
  case CORBA::tk_long:
    o << "CORBA::Long";
    break;
  case CORBA::tk_longlong:
    o << "CORBA::LongLong";
    break;
  case CORBA::tk_ulonglong:
    o << "CORBA::ULongLong";
    break;
  case CORBA::tk_float:
    o << "CORBA::Float";
    break;
  case CORBA::tk_double:
    o << "CORBA::Double";
    break;
  case CORBA::tk_longdouble:
    o << "CORBA::LongDouble";
    break;
  case CORBA::tk_string:
    o << "char*";
    break;
  case CORBA::tk_wstring:
    o << "CORBA::WChar*";
    break;
  case CORBA::tk_fixed:
    o << "FixedTmpl< " << tc->fixed_digits() << "," << tc->fixed_scale() << " >";
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



void CodeGenCPPUtil::emit_array_marshalling( CORBA::ArrayDef_ptr arr,
					     bool is_from,
					     int level )
{
  const char *direction = is_from ? "put" : "get";
  
  o << "_a.array_" << direction << "_begin();" << endl;
  o << "for( CORBA::ULong _i" << level << " = 0; _i" << level << " < "
    << arr->length() << "; _i" << level << "++ ) {" << indent << endl;
  CORBA::IDLType_var elem = arr->element_type_def();
  if( elem->def_kind() == CORBA::dk_Array ) {
    CORBA::ArrayDef_var nested_arr = CORBA::ArrayDef::_narrow( elem );
    assert( !CORBA::is_nil( nested_arr ) );
    emit_array_marshalling( nested_arr, is_from, ++level );
  } else {
    o << "_a ";
    if( is_from )
      o << " <<= ";
    else
      o << " >>= ";
    string var;
    var = "_s";
    for( int i = 0; i <= level; i++ ) {
      var += "[ _i";
      var += xdec( i );
      var += " ]";
    }
    emit_any_param( elem, (char *) var.c_str(), is_from );
    o << ";" << endl;
  }
  o << BL_CLOSE;
  
  o << "_a.array_" << direction << "_end();" << endl;
}



bool CodeGenCPPUtil::is_union_case_defined( CORBA::UnionMemberSeq& mem,
					    CORBA::Any &any )
{
  for( CORBA::ULong i = 0; i < mem.length(); i++ )
    if( mem[ i ].label == any )
      return true;
  return false;
}



string
CodeGenCPPUtil::generate_union_default_case_label( CORBA::IDLType_ptr discr,
						   CORBA::UnionMemberSeq &mem )
{
  string label;
  label = "";
  
  CORBA::TypeCode_var tc = discr->type();
  if( tc->is_enum() ) {
    CORBA::IDLType_var t = CORBA::IDLType::_duplicate( discr );
    while( t->def_kind() == CORBA::dk_Alias ) {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( t );
      t = a->original_type_def();
    }
    CORBA::EnumDef_var e = CORBA::EnumDef::_narrow( t );
    assert( !CORBA::is_nil( e ) );
    CORBA::EnumMemberSeq_var e_mem = e->members();
    for( CORBA::ULong i = 0; i < e_mem->length(); i++ ) {
      CORBA::Any any;
      any.set_type( tc->unalias() );
      CORBA::Boolean r = any.enum_put( i );
      assert (r);
      if( !is_union_case_defined( mem, any ) ) {
        CORBA::String_var tmp = e->absolute_name();
        label = ID(tmp);
        int colon_pos = label.rfind( "::" );
        if( colon_pos != -1 ) {
          label = label.substr( 0, colon_pos );
          label += "::";
        }
	label += e_mem[ i ];
	break;
      }
    }
    return label;
  }
  
  tc = CORBA::TypeCode::_duplicate( tc->unalias() );
  if( tc->kind() == CORBA::tk_long ) {
    CORBA::Long l = 0;
    do {
      CORBA::Any any;
      any <<= l;
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (l);
	break;
      }
    } while (++l);
    return label;
  }

  if( tc->kind() == CORBA::tk_ulong ) {
    CORBA::ULong ul = 0;
    do {
      CORBA::Any any;
      any <<= ul;
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (ul);
	break;
      }
    } while (++ul);
    return label;
  }

  if( tc->kind() == CORBA::tk_longlong ) {
    CORBA::LongLong ll = 0;
    do {
      CORBA::Any any;
      any <<= ll;
      if( !is_union_case_defined( mem, any ) ) {
	  label = xdec (ll);
	break;
      }
    } while (++ll);
    return label;
  }

  if( tc->kind() == CORBA::tk_ulonglong ) {
    CORBA::ULongLong ull = 0;
    do {
      CORBA::Any any;
      any <<= ull;
      if( !is_union_case_defined( mem, any ) ) {
	  label = xdec (ull);
	break;
      }
    } while (++ull);
    return label;
  }

  if( tc->kind() == CORBA::tk_char ) {
    CORBA::Char c = 0;
    do {
      CORBA::Any any;
      any <<= CORBA::Any::from_char( c );
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (c);
	break;
      }
    } while (++c);
    return label;
  }

  if( tc->kind() == CORBA::tk_wchar ) {
    CORBA::WChar c = 0;
    do {
      CORBA::Any any;
      any <<= CORBA::Any::from_wchar( c );
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (c);
	break;
      }
    } while (++c);
    return label;
  }

  if( tc->kind() == CORBA::tk_short ) {
    CORBA::Short s = 0;
    do {
      CORBA::Any any;
      any <<= s;
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (s);
	break;
      }
    } while (++s);
    return label;
  }

  if( tc->kind() == CORBA::tk_ushort ) {
    CORBA::UShort us = 0;
    do {
      CORBA::Any any;
      any <<= us;
      if( !is_union_case_defined( mem, any ) ) {
	label = xdec (us);
	break;
      }
    } while (++us);
    return label;
  }

  if( tc->kind() == CORBA::tk_boolean ) {
    CORBA::Any any;
    any <<= CORBA::Any::from_boolean( TRUE );
    if( is_union_case_defined( mem, any ) ) {
      any <<= CORBA::Any::from_boolean( FALSE );
      if( !is_union_case_defined( mem, any ) )
	label = "FALSE";
    } else
      label = "TRUE";
    return label;
  }

  return label;
}

void
CodeGenCPPUtil::emit_marshaller_ref( CORBA::IRObject_ptr obj,
				     bool for_an_instance )
{
  string mangled_name;
  
  gen_mangled_name( mangled_name, obj, for_an_instance );
  o << mangled_name;
}

void
CodeGenCPPUtil::emit_marshaller_suffix( CORBA::IRObject_ptr obj,
                                        CORBA::Boolean is_demarshal )
{
  CORBA::IDLType_var t = CORBA::IDLType::_narrow( obj );
  if( CORBA::is_nil( t ) )
    return;
  
  CORBA::TypeCode_var tc = t->type();
  if( tc->is_string() || tc->is_objref() || tc->is_type_code() ||
      tc->is_valuetype() || tc->is_abstract_interface() ||
      tc->is_wstring() )
    o << (is_demarshal ? "._for_demarshal()" : ".inout()");
}


void
CodeGenCPPUtil::gen_mangled_name( string &str,
				  CORBA::IRObject_ptr obj,
				  bool for_an_instance,
				  bool first_call_to_gen )
{
  CORBA::IRObject_var no = CORBA::IRObject::_duplicate( obj );

  while( no->def_kind() == CORBA::dk_Alias ) {
    CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( no );
    assert( !CORBA::is_nil( a ) );
    no = a->original_type_def();
  }

  switch( no->def_kind() ) {
  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow( no );
      CORBA::TypeCode_var tc = p->type();

      if( first_call_to_gen ) {
	assert( for_an_instance );
	str += "CORBA::_stc_";
      }
      
      switch( tc->kind() ) {
      case CORBA::tk_void:
	str += "void";
	break;
      case CORBA::tk_boolean:
	str += "boolean";
	break;
      case CORBA::tk_char:
	str += "char";
	break;
      case CORBA::tk_wchar:
	str += "wchar";
	break;
      case CORBA::tk_octet:
	str += "octet";
	break;
      case CORBA::tk_ushort:
	str += "ushort";
	break;
      case CORBA::tk_ulong:
	str += "ulong";
	break;
      case CORBA::tk_short:
	str += "short";
	break;
      case CORBA::tk_long:
	str += "long";
	break;
      case CORBA::tk_longlong:
	str += "longlong";
	break;
      case CORBA::tk_ulonglong:
	str += "ulonglong";
	break;
      case CORBA::tk_float:
	str += "float";
	break;
      case CORBA::tk_double:
	str += "double";
	break;
      case CORBA::tk_longdouble:
        str += "longdouble";
	break;
      case CORBA::tk_string:
	str += "string";
	break;
      case CORBA::tk_wstring:
	str += "wstring";
	break;
      case CORBA::tk_objref:
	str += "Object";
	break;
      case CORBA::tk_TypeCode:
	str += "TypeCode";
	break;
      case CORBA::tk_any:
	str += "any";
	break;
      case CORBA::tk_value:
	str += "ValueBase";
	break;
      case CORBA::tk_abstract_interface:
	str += "AbstractBase";
	break;
      default:
	assert( 0 );
      }
      break;
    }
  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow( no );
      CORBA::ULong bound = s->bound();
      CORBA::TypeCode_var stc = s->type();
      CORBA::Boolean isrec = stc->unalias()->is_recursive_seq();

      if( first_call_to_gen && is_base_sequence( s ) ) {
	assert( for_an_instance );
	str += "CORBA::_stcseq_";
      }
      
      if( first_call_to_gen && !is_base_sequence( s ) ) {
	if( for_an_instance )
	  str += "_marshaller";
	else
	  str += "_Marshaller";
	str += isrec ? "__rseq_" : "__seq_";
      }

      if( !first_call_to_gen )
	str += isrec ? "_rseq_" : "_seq_";
      
      if( bound > 0 ) {
	str += xdec( bound );
	str += "b_";
      }

      CORBA::IDLType_var elem = s->element_type_def();

      // resolve Alias
      while( elem->def_kind() == CORBA::dk_Alias ) {
	CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( elem );
	elem = a->original_type_def();
      }

#if 0      
      if( elem->def_kind() != CORBA::dk_Primitive )
	elem = s->element_type_def();
#endif

      gen_mangled_name( str, elem, for_an_instance, false );
      break;
    }
  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow( no );
      if( first_call_to_gen ) {
	if( for_an_instance )
	  str += "_marshaller_";
	else
	  str += "_Marshaller_";
      }
      
      str += "_a";
      str += xdec( a->length() );
      str += "_";
      CORBA::IDLType_var elem = a->element_type_def();
      gen_mangled_name( str, elem, for_an_instance, false );
      break;
    }
  case CORBA::dk_Fixed:
    {
      CORBA::FixedDef_var f = CORBA::FixedDef::_narrow( no );
      if( first_call_to_gen ) {
	if( for_an_instance )
	  str += "_marshaller_";
	else
	  str += "_Marshaller_";
      }
      
      str += "_fixed_";
      str += xdec( f->digits() );
      if( f->scale() < 0 ) {
	str += "_n";
	str += xdec( -( f->scale() ) );
      } else {
	str += "_";
	str += xdec( f->scale() );
      }
      break;
    }
  case CORBA::dk_String:
    if( first_call_to_gen ) {
      assert( for_an_instance );
      str += "CORBA::_stc_";
    }
    str += "string";
    break;
  case CORBA::dk_Wstring:
    if( first_call_to_gen ) {
      assert( for_an_instance );
      str += "CORBA::_stc_";
    }
    str += "wstring";
    break;
  case CORBA::dk_Struct:
  case CORBA::dk_Union:
  case CORBA::dk_Enum:
  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
  case CORBA::dk_Exception:
  case CORBA::dk_Value:
  case CORBA::dk_ValueBox:
  case CORBA::dk_Event:
    {
      CORBA::Contained_var c = CORBA::Contained::_narrow( no );
      CORBA::String_var n = c->absolute_name();
      string name;
      name = ID(n);
      for( int i = name.find ("::"); i >= 0; i = name.find ("::") )
	name.replace( i, 2, "_" );
      name = name.substr( 1 );
      if( first_call_to_gen ) {
	if( for_an_instance )
	  str += "_marshaller_";
	else
	  str += "_Marshaller_";
      }

      str += name;
      break;
    }

  case CORBA::dk_LocalInterface:
    assert( 0 );

  case CORBA::dk_Alias:
    assert( 0 );

  case CORBA::dk_Native:
    assert( 0 );

  default:
    assert( 0 );
  }
}

/*
 * Check if a valuetype contains a valuetype member of its own or
 * base type (a valuetype cannot contain a derived valuetype). This
 * would mean that there is a possibility of this valuetype being
 * circular.
 * We take into account that concrete valuetypes only support single
 * inheritance. Therefore, we don't need to fear that a future derivation
 * of an unrelated type can inherit from this value, too:
 *
 * valuetype ThisType { ... };
 * valuetype MemberType { ... };
 *
 * There will never be a type that inherits from both ThisType and
 * MemberType, so if ThisType contains a MemberType, this can never
 * cause a circle. But this may be well the case if either one is
 * abstract.
 *
 * This has an influence on (a) deep-copying this value, because calling
 * _copy_value() on each member might recurse infinitely, so we'll have
 * to marshal and demarshal the value instead, and (b) the behaviour
 * of _remove_ref(), which will have to check if it just orphaned a
 * circular graph that needs to be released.
 */

bool
CodeGenCPPUtil::check_for_possible_circle_rek( CORBA::ValueDef_ptr val,
					       CORBA::IRObject_ptr obj,
					       vector<CORBA::IRObject_var> & seen )
{
  /*
   * Needed to break out of recursion introduced by recursive
   * sequences or the recursion of unrelated valuetypes
   */

  for (CORBA::ULong s=0; s<seen.size(); s++) {
    if (seen[s]->_is_equivalent (obj)) {
      return false;
    }
  }

  seen.push_back (CORBA::IRObject::_duplicate (obj));

  switch (obj->def_kind()) {
  case CORBA::dk_Value:
  case CORBA::dk_Event:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow (obj);
      CORBA::String_var id = v->id ();

      if (val->is_abstract() || v->is_abstract() || val->is_a (id.in())) {
	return true;
      }

      CORBA::ContainedSeq_var c = v->contents (CORBA::dk_ValueMember, 0);

      for (CORBA::ULong i=0; i < c->length(); i++) {
	if (check_for_possible_circle_rek (val, c[i], seen)) {
	  return true;
	}
      }
    }
    break;

  case CORBA::dk_ValueMember:
    {
      CORBA::ValueMemberDef_var v = CORBA::ValueMemberDef::_narrow (obj);
      CORBA::IDLType_var tc = v->type_def ();
      if (check_for_possible_circle_rek (val, tc, seen)) {
	return true;
      }
    }
    break;

  case CORBA::dk_ValueBox:
    {
      CORBA::ValueBoxDef_var v = CORBA::ValueBoxDef::_narrow (obj);
      CORBA::IDLType_var ot = v->original_type_def ();
      if (check_for_possible_circle_rek (val, ot, seen)) {
	return true;
      }
    }
    break;

  case CORBA::dk_AbstractInterface:
    {
      return true;
    }
    break;

  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow (obj);
      CORBA::StructMemberSeq_var sms = s->members();
      for (CORBA::ULong i=0; i<sms->length(); i++) {
	if (check_for_possible_circle_rek (val, sms[i].type_def, seen)) {
	  return true;
	}
      }
    }
    break;

  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow (obj);
      CORBA::UnionMemberSeq_var ums = u->members();
      for (CORBA::ULong i=0; i<ums->length(); i++) {
	if (check_for_possible_circle_rek (val, ums[i].type_def, seen)) {
	  return true;
	}
      }
    }
    break;

  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var t = CORBA::AliasDef::_narrow (obj);
      CORBA::IDLType_var ot = t->original_type_def ();
      if (check_for_possible_circle_rek (val, ot, seen)) {
	return true;
      }
    }
    break;

  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow (obj);
      CORBA::IDLType_var et = s->element_type_def ();
      if (check_for_possible_circle_rek (val, et, seen)) {
	return true;
      }
    }
    break;

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow (obj);
      CORBA::IDLType_var et = a->element_type_def ();
      if (check_for_possible_circle_rek (val, et, seen)) {
	return true;
      }
    }
    break;

  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow (obj);
      if (p->kind() == CORBA::pk_value_base) {
	return true;
      }
    }
    break;
  default:
    break;
  }

  return false;
}

bool
CodeGenCPPUtil::check_for_possible_circle( CORBA::ValueDef_ptr v )
{
  CORBA::ContainedSeq_var c = v->contents (CORBA::dk_ValueMember, 0);
  vector<CORBA::IRObject_var> seen;

  for (CORBA::ULong i0=0; i0 < c->length(); i0++) {
    CORBA::ValueMemberDef_var vmd = CORBA::ValueMemberDef::_narrow(c[i0]);
    if (check_for_possible_circle_rek (v, vmd, seen)) {
      return true;
    }
  }

  return false;
}

/*
 * Check the parameters of an invocation if it is safe to use our
 * colocated POA stubs. Valuetype parameters are a problem, because
 * they must be deep-copied to preserve location transparency, but
 * still sharing of valuetypes must be preserved.
 *
 * The clp stubs are clever enough to detect sharing of top-level
 * parameters by themselves. However, structured types (struct, union,
 * sequence, array) that contain a valuetype are too complicated for
 * us, because using the type's copy constructor does not deep-copy
 * its member valuetypes.
 *
 * Abstract interfaces give us a headache, too, because they might
 * be shared with a different valuetype.
 */

bool
CodeGenCPPUtil::contains_a_valuetype (CORBA::IRObject_ptr obj,
				      vector<CORBA::IRObject_var> * seen)
{
  vector<CORBA::IRObject_var> myseen;

  if (!seen) {
    seen = &myseen;
  }

  for (CORBA::ULong s=0; s<seen->size(); s++) {
    if ((*seen)[s]->_is_equivalent (obj)) {
      return false;
    }
  }

  seen->push_back (CORBA::IRObject::_duplicate (obj));

  switch (obj->def_kind()) {
  case CORBA::dk_Value:
  case CORBA::dk_Event:
    {
      return true;
    }
    break;

  case CORBA::dk_ValueMember:
    {
      CORBA::ValueMemberDef_var v = CORBA::ValueMemberDef::_narrow (obj);
      CORBA::IDLType_var tc = v->type_def ();
      return contains_a_valuetype (tc, seen);
    }
    break;

  case CORBA::dk_ValueBox:
    {
      return true;
    }
    break;

  case CORBA::dk_AbstractInterface:
    {
      return true;
    }
    break;

  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow (obj);
      CORBA::StructMemberSeq_var sms = s->members();
      for (CORBA::ULong i=0; i<sms->length(); i++) {
	if (contains_a_valuetype (sms[i].type_def, seen)) {
	  return true;
	}
      }
    }
    break;

  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow (obj);
      CORBA::UnionMemberSeq_var ums = u->members();
      for (CORBA::ULong i=0; i<ums->length(); i++) {
	if (contains_a_valuetype (ums[i].type_def, seen)) {
	  return true;
	}
      }
    }
    break;

  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var t = CORBA::AliasDef::_narrow (obj);
      CORBA::IDLType_var ot = t->original_type_def ();
      return contains_a_valuetype (ot, seen);
    }
    break;

  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow (obj);
      CORBA::IDLType_var et = s->element_type_def ();
      return contains_a_valuetype (et, seen);
    }
    break;

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow (obj);
      CORBA::IDLType_var et = a->element_type_def ();
      return contains_a_valuetype (et, seen);
    }
    break;

  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow (obj);
      if (p->kind() == CORBA::pk_value_base) {
	return true;
      }
    }
    break;
  default:
    break;
  }

  return false;
}

bool
CodeGenCPPUtil::check_simple_clp_call (const CORBA::OperationDescription &op)
{
  bool value_seen = true;
  bool abs_interface_seen = false;

  const CORBA::ParDescriptionSeq &p = op.parameters;

  for (CORBA::ULong i=0; i<p.length(); i++) {
    switch (p[i].type_def->def_kind()) {
    case CORBA::dk_Value:
    case CORBA::dk_Event:
      {
	if (abs_interface_seen) {
	  return false;
	}
	value_seen = true;
      }
      break;

    case CORBA::dk_ValueBox:
      break;

    case CORBA::dk_AbstractInterface:
      {
	if (abs_interface_seen || value_seen) {
	  return false;
	}
	abs_interface_seen = true;
      }
      break;

    default:
      {
	if (contains_a_valuetype (p[i].type_def.in())) {
	  return false;
	}
      }
    }
  }

  return true;
}

bool
CodeGenCPPUtil::is_var_inout (CORBA::IDLType_ptr t)
{
    CORBA::TypeCode_var tc = t->type();
    return
        tc->is_string() || tc->is_wstring() ||
        tc->is_objref() || tc->is_abstract_interface() || 
        tc->unalias()->kind() == CORBA::tk_local_interface ||
        tc->is_type_code() || tc->is_valuetype();
}

void
CodeGenCPPUtil::emit_release (CORBA::IDLType_ptr t, const char *var_name)
{
    CORBA::TypeCode_var tc = t->type();
    if (tc->is_string()) {
	o << "CORBA::string_free( " << var_name << " );" << endl;
        return;
    }
    if (tc->is_wstring()) {
        o << "CORBA::wstring_free( " << var_name << " );" << endl;
        return;
    }
    if (tc->is_objref() || tc->is_abstract_interface() || tc->is_type_code()) {
        o << "CORBA::release( " << var_name << " );" << endl;
        return;
    }
    if (tc->is_valuetype()) {
        o << "CORBA::remove_ref( " << var_name << " );" << endl;
        return;
    }
    if (tc->is_array()) {
        emit_idl_type_name (t);
        o << "_free( " << var_name << " );" << endl;
        return;
    }
    if (tc->is_variable()) {
        o << "delete " << var_name << ";" << endl;
        return;
    }
}

bool
CodeGenCPPUtil::use_builtin_marshaller( CORBA::TypeCode_ptr tc, string &name )
{
  switch( tc->unalias()->kind() ) {
  case CORBA::tk_boolean:
    name = "booleans";
    return TRUE;
  case CORBA::tk_char:
    name = "chars";
    return TRUE;
  case CORBA::tk_wchar:
    name = "wchars";
    return TRUE;
  case CORBA::tk_octet:
    name = "octets";
    return TRUE;
  case CORBA::tk_ushort:
    name = "ushorts";
    return TRUE;
  case CORBA::tk_ulong:
    name = "ulongs";
    return TRUE;
  case CORBA::tk_short:
    name = "shorts";
    return TRUE;
  case CORBA::tk_long:
    name = "longs";
    return TRUE;
  case CORBA::tk_longlong:
    name = "longlongs";
    return TRUE;
  case CORBA::tk_ulonglong:
    name = "ulonglongs";
    return TRUE;
  case CORBA::tk_float:
    name = "floats";
    return TRUE;
  case CORBA::tk_double:
    name = "doubles";
    return TRUE;
  case CORBA::tk_longdouble:
    name = "longdoubles";
    return TRUE;
  default:
    break;
  }
  return FALSE;
}



CORBA::IDLType_ptr
CodeGenCPPUtil::resolve_alias(CORBA::IDLType_ptr alias)
{
    CORBA::IDLType_var resolved_alias = CORBA::IDLType::_duplicate( alias );
    while( resolved_alias->def_kind() == CORBA::dk_Alias ) {
	CORBA::AliasDef_var als = CORBA::AliasDef::_narrow( resolved_alias );
	resolved_alias = als->original_type_def();
    }
    return resolved_alias;
}


/*
 * A type is not marshallable if it contains a local interface.
 */

static bool
is_marshallable_rek (CORBA::IRObject_ptr obj,
		     vector<CORBA::IRObject_var> & seen)
{
  /*
   * Detect recursion
   */

  for (vector<CORBA::IRObject_var>::iterator s=seen.begin();
       s != seen.end(); s++) {
    if ((*s)->_is_equivalent (obj)) {
      return true;
    }
  }

  seen.push_back (CORBA::IRObject::_duplicate (obj));

  switch (obj->def_kind()) {
  case CORBA::dk_Struct:
    {
      CORBA::StructDef_var s = CORBA::StructDef::_narrow (obj);
      CORBA::StructMemberSeq_var sms = s->members();
      for (CORBA::ULong i=0; i<sms->length(); i++) {
	if (!is_marshallable_rek (sms[i].type_def, seen)) {
	  return false;
	}
      }
    }
    break;

  case CORBA::dk_Exception:
    {
      CORBA::ExceptionDef_var s = CORBA::ExceptionDef::_narrow (obj);
      CORBA::StructMemberSeq_var sms = s->members();
      for (CORBA::ULong i=0; i<sms->length(); i++) {
	if (!is_marshallable_rek (sms[i].type_def, seen)) {
	  return false;
	}
      }
    }
    break;


  case CORBA::dk_Union:
    {
      CORBA::UnionDef_var u = CORBA::UnionDef::_narrow (obj);
      CORBA::UnionMemberSeq_var ums = u->members();
      for (CORBA::ULong i=0; i<ums->length(); i++) {
	if (!is_marshallable_rek (ums[i].type_def, seen)) {
	  return false;
	}
      }
    }
    break;
    
  case CORBA::dk_Enum:
    return true;

  case CORBA::dk_Alias:
    {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow (obj);
      CORBA::IDLType_var idt = a->original_type_def ();
      return is_marshallable_rek (idt, seen);
    }

  case CORBA::dk_Primitive:
  case CORBA::dk_String:
  case CORBA::dk_Wstring:
  case CORBA::dk_Fixed:
    return true;

  case CORBA::dk_Sequence:
    {
      CORBA::SequenceDef_var s = CORBA::SequenceDef::_narrow (obj);
      CORBA::IDLType_var et = s->element_type_def ();
      return is_marshallable_rek (et, seen);
    }

  case CORBA::dk_Array:
    {
      CORBA::ArrayDef_var a = CORBA::ArrayDef::_narrow (obj);
      CORBA::IDLType_var et = a->element_type_def ();
      return is_marshallable_rek (et, seen);
    }

  case CORBA::dk_Interface:
  case CORBA::dk_AbstractInterface:
    return true;

  case CORBA::dk_LocalInterface:
    return false;

  case CORBA::dk_Value:
  case CORBA::dk_Event:
    {
      CORBA::ValueDef_var v = CORBA::ValueDef::_narrow (obj);
      CORBA::ContainedSeq_var c = v->contents (CORBA::dk_ValueMember, 0);

      for (CORBA::ULong i=0; i < c->length(); i++) {
	CORBA::ValueMemberDef_var vmd = CORBA::ValueMemberDef::_narrow (c[i]);
	CORBA::IDLType_var idt = vmd->type_def ();
	if (!is_marshallable_rek (idt.in(), seen)) {
	  return false;
	}
      }
    }
    break;

  case CORBA::dk_ValueBox:
    {
      CORBA::ValueBoxDef_var v = CORBA::ValueBoxDef::_narrow (obj);
      CORBA::IDLType_var ot = v->original_type_def ();
      return is_marshallable_rek (ot, seen);
    }

  default:
    assert (0);
  }

  return true;
}

bool
CodeGenCPPUtil::is_marshallable (CORBA::IRObject_ptr t)
{
  vector<CORBA::IRObject_var> seen;
  return is_marshallable_rek (t, seen);
}

