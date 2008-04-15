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
#include <mico/throw.h>
#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif
#include <mico/ir_creator.h>
#include <mico/template_impl.h>
#include <mico/os-misc.h>
#include "idlparser.h"
#include "error.h"

#endif // FAST_PCH


using namespace std;

CORBA::Container_ptr
IRCopier (DB & db, IDLParam & params,
	  CORBA::Repository_ptr repo,
	  CORBA::Container_ptr cont);


//-- IDLParser ----------------------------------------------------------

IDLParser::IDLParser( DB& db, IDLParam & params )
{
  _current_parse_node = NULL;
  _feed_included_defs = params.feed_included_defs;
  _repo               = CORBA::Repository::_nil ();
  _db                 = &db;
  _params             = &params;
  _version            = "1.0";
}

void
IDLParser::collect (CORBA::Container_ptr cont, ParseNode * root)
{
  assert (!CORBA::is_nil (cont));

  _repo = CORBA::Repository::_narrow (cont);

  if (CORBA::is_nil (_repo)) {
    CORBA::Contained_var temp = CORBA::Contained::_narrow (cont);
    _repo = temp->containing_repository ();
  }

  // root->print ();

  /*
   * Traverse the parse tree
   */

#ifdef HAVE_EXCEPTIONS
  try {
#endif
    scan( cont, root );
#ifdef HAVE_EXCEPTIONS
  } catch( CORBA::BAD_PARAM &bp) {
    ParseNode *node = current_parse_node();
    assert( node != NULL );
    char *ident = node->getIdent();
    assert( ident != NULL );
    switch( bp.minor() & (~CORBA::OMGVMCID)) {
    case 2:
      cerr << IDLError(13, node, ident) << endl;
      break;
    case 3:
      cerr << IDLError(12, node, ident) << endl;
      break;
    case 4:
      cerr << IDLError(17, node, ident) << endl;
      break;
    case 5:
      cerr << IDLError(16, node, ident) << endl;
      break;
    default:
      cerr << IDLError( 31, node ) << endl;
      bp._print (cerr);
      break;
    }
    exit( 1 );
  } catch( CORBA::SystemException &ex ) {
    ParseNode *node = current_parse_node();
    assert( node != NULL );
    char *ident = node->getIdent();
    assert( ident != NULL );
    switch( ex.minor() ) {
    case MICO_IR_EXC_DUPLICATE_NAME:
      cerr << IDLError( 12, node, ident ) << endl;
      break;
    case MICO_IR_EXC_DUPLICATE_REPOID:
      cerr << IDLError( 13, node, ident ) << endl;
      break;
    case MICO_IR_EXC_DUPLICATE_LABEL:
      cerr << IDLError( 14, node, ident ) << endl;
      break;
    case MICO_IR_EXC_AMBIGUOUS_NAME:
      cerr << IDLError( 15, node, ident ) << endl;
      break;
    case MICO_IR_EXC_OVERLOADING:
      cerr << IDLError( 16, node, ident ) << endl;
      break;
    case MICO_IR_EXC_BAD_RECURSION:
      cerr << IDLError( 17, node, ident ) << endl;
      break;
    case MICO_IR_EXC_BAD_ONEWAY:
      cerr << IDLError( 18, node, ident ) << endl;
      break;
    case MICO_IR_EXC_DUPLICATE_MEMBER:
      cerr << IDLError( 19, node, ident ) << endl;
      break;
    case MICO_IR_EXC_BAD_UNION:
      cerr << IDLError( 21, node ) << endl;
      break;
    case MICO_IR_EXC_BAD_TYPE:
      // This error indicates that we tried to create an interface within
      // an interface. This error can not occur when the IR is fed via
      // the IDL compiler (because the IDL grammar already inhibits this).
      assert( 0 );
    default:
      cerr << IDLError( 31, node ) << endl;
      ex._print (cerr);
      cerr << endl;
      break;
    }
    exit( 1 );
  }
#endif
  
  bool undefined_references = false;

  for (set<string, less<string> >::iterator it = _db->_forward_dcl.begin();
       it != _db->_forward_dcl.end(); it++) {
    CORBA::Contained_var cv = _repo->lookup ((*it).c_str());
    if (CORBA::is_nil (cv) ||
	(cv->def_kind() != CORBA::dk_Interface &&
	 cv->def_kind() != CORBA::dk_AbstractInterface &&
	 cv->def_kind() != CORBA::dk_LocalInterface &&
#ifdef USE_CCM
	 cv->def_kind() != CORBA::dk_Component &&
	 cv->def_kind() != CORBA::dk_Home &&
#endif
	 cv->def_kind() != CORBA::dk_Value)) {
      if (!undefined_references) {
	cerr << "undefined forward references:" << endl;
      }
      cerr << "    " << (*it) << endl;
      undefined_references = true;
    }
  }

  if (undefined_references) {
    cerr << IDLError( 1 )  << endl;
    exit( 1 );
  }
}

bool IDLParser::is_iface_defined( const char* scoped_name )
{
  return _seen_iface.find( scoped_name ) != _seen_iface.end();
}

void IDLParser::current_parse_node( ParseNode* node )
{
  _current_parse_node = node;
}

ParseNode* IDLParser::current_parse_node()
{
  return _current_parse_node;
}

char *IDLParser::gen_scoped_name( ParseNode *node )
{
  string name = node->getIdent();

  current_parse_node( node );

  if ( name[0] == ':' ) {
      /* It is an absolute name */
      return CORBA::string_dup( name.c_str() );
  } else {
      string scoped_name =_db->get_current_scope();
      scoped_name += node->getIdent();
      return CORBA::string_dup( scoped_name.c_str() );
  }
}

char *IDLParser::gen_repo_id( ParseNode *node )
{
  string id;

  current_parse_node( node );
  CORBA::String_var absname = gen_scoped_name (node);

  id = "IDL:" + _db->get_prefix(absname.in());
  id += node->getIdent();
  id += ":";
  id += _version;

  _db->register_repoid( node, id );

  return CORBA::string_dup( id.c_str() );
}


void IDLParser::scan( CORBA::Container_ptr con, ParseNode *node )
{
  if( node == NULL )
    return;

  ParseNodeType t = node->getType();
  
  if( t != exports && t != definitions && t != t_module &&
      t != interface_dcl && t != forward_dcl &&
      t != abstract_forward_dcl && t != local_forward_dcl &&
      t != value_forward_dcl && t != abstract_value_forward_dcl &&
      t != value_dcl && t != abstract_value_dcl && t != t_include &&
      t != union_forward_decl && t != struct_forward_decl &&
      t != component_dcl && t != component_forward_dcl &&
      t != home_dcl &&
      !(node->is_toplevel()) && !_feed_included_defs )
    return;
  
  switch( node->getType() ) {
  case t_pragma:
  case t_typeid:
  case t_typeprefix:
    collectPragma( con, node );
    break;
  case t_include:
    collectInclude( con, node );
    break;
  case t_import:
    collectImport (con, node);
    break;
  case exports:
  case definitions:
    scan( con, node->getBranch( 1 ) );
    scan( con, node->getBranch( 2 ) );
    break;
  case t_module:
    collectModule( con, node );
    break;
  case struct_type:
    {
      CORBA::StructDef_var s = collectStruct( con, node );
      break;
    }
  case union_type:
    {
      CORBA::UnionDef_var u = collectUnion( con, node );
      break;
    }
  case const_dcl:
    collectConstDcl( con, node );
    break;
  case except_dcl:
    collectException( con, node );
    break;
  case forward_dcl:
  case abstract_forward_dcl:
  case local_forward_dcl:
  case struct_forward_decl:
  case union_forward_decl:
#ifdef USE_CCM
  case component_forward_dcl:
#endif
    collectForwardDcl (con, node);
    break;
  case interface_dcl:
    collectInterface( con, node );
    break;
  case t_typedef:
    collectTypedef( con, node );
    break;
  case t_native:
    collectNative( con, node );
    break;
  case op_dcl:
    collectMethod( con, node );
    break;
  case t_attribute:
    collectAttribute( con, node, CORBA::ATTR_NORMAL );
    break;
  case t_readonly_attribute:
    collectAttribute( con, node, CORBA::ATTR_READONLY );
    break;
  case enum_type:
    {
      CORBA::EnumDef_var e = collectEnum( con, node );
      break;
    }
  case value_forward_dcl:
  case abstract_value_forward_dcl:
#ifdef USE_CCM
  case event_forward_dcl:
  case abstract_event_forward_dcl:
#endif
    collectValueForwardDcl( con, node );
    break;
  case value_box_dcl:
    collectValueBoxDcl( con, node );
    break;
  case value_dcl:
  case abstract_value_dcl:
#ifdef USE_CCM
  case event_dcl:
  case abstract_event_dcl:
#endif
    collectValueDcl( con, node );
    break;
  case public_state_member:
    collectStateMember (con, node, CORBA::PUBLIC_MEMBER);
    break;
  case private_state_member:
    collectStateMember (con, node, CORBA::PRIVATE_MEMBER);
    break;
#ifdef USE_CCM
  case component_dcl:
    collectComponent (con, node);
    break;
  case provides_dcl:
    collectProvides (con, node);
    break;
  case uses_dcl:
  case uses_multiple_dcl:
    collectUses (con, node);
    break;
  case emits_dcl:
  case publishes_dcl:
  case consumes_dcl:
    collectEventPort (con, node);
    break;
  case home_dcl:
    collectHome (con, node);
    break;
  case factory_dcl:
  case finder_dcl:
    collectFactory (con, node);
    break;
#endif
  default:
    node->print();
    assert( 0 );
  }
}

/*
 * Process import statement:
 *    import "Repository Id";
 *    import <scoped-name>
 * Imports are always at the beginning of a file, outside any scope.
 */

void
IDLParser::collectImport (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node);
  assert (node->getType() == t_import);

  node = node->getBranch (1);

  assert (node);
  assert (node->getType() == string_literals ||
	  node->getType() == scoped_name);

  CORBA::String_var to_be_imported = CORBA::string_dup (node->getIdent ());

  /*
   * Import statements must be at toplevel. Well, actually, import
   * statements can appear only at the beginning of a file, but
   * adhering to that would screw up the grammar with respect to
   * includes.
   */

  if (con->def_kind() != CORBA::dk_Repository) {
    cerr << IDLError (40, node) << endl;
    exit (1);
  }

  /*
   * We may need the Interface Repository
   */

  CORBA::ORB_var myorb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var myobj =
    myorb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var myrepo = CORBA::Repository::_narrow (myobj);

  /*
   * Try to look up Repository Id or scoped name in the Repository
   */

  CORBA::Contained_var icont = CORBA::Contained::_nil ();

  if (node->getType() == string_literals) {
    if (CORBA::is_nil (myrepo)) {
      cerr << IDLError (36, node, to_be_imported.in()) << endl;
      exit (1);
    }

    icont = myrepo->lookup_id (to_be_imported.in());

    if (CORBA::is_nil (icont)) {
      cerr << IDLError (37, node, to_be_imported.in()) << endl;
      exit (1);
    }
  }
  else {
    /*
     * We try to look up the named scope in the Repository, too.
     * If this fails, we go on searching for a matching file, see
     * below.
     */
    if (!CORBA::is_nil (myrepo)) {
      icont = myrepo->lookup (to_be_imported.in());
    }
  }

  /*
   * If the import was found in the Interface Repository, employ the
   * IR copier to pull it in.
   */

  if (!CORBA::is_nil (icont)) {
    CORBA::Container_var import_container = CORBA::Container::_narrow (icont);

    if (CORBA::is_nil (import_container)) {
      cerr << IDLError (39, node, to_be_imported.in()) << endl;
      exit (1);
    }

    CORBA::Container_var imported =
      IRCopier (*_db, *_params, _repo, import_container);
    assert (!CORBA::is_nil (imported));

    return;
  }

  /*
   * Okay, to_be_imported is a scoped name. Translate this to a file name
   * by replacing '::' with '/'.
   */

  string file_name = to_be_imported.in();

  if (file_name[0] == ':' && file_name[1] == ':') {
    file_name = file_name.substr (2);
  }

  int pos = file_name.find ("::");

  while (pos != -1) {
    file_name = file_name.replace (pos, 2, "/");
    pos = file_name.find ("::");
  }

  file_name += ".idl";

  /*
   * Now traverse our include paths and see if we can find this file.
   */

  string absolute_file_name;
  vector<string> tested_paths;
  vector<string>::size_type i;

  for (i=0; i<_params->inc_paths.size(); i++) {
    absolute_file_name = _params->inc_paths[i];
    if (absolute_file_name.length() > 0 &&
	absolute_file_name[absolute_file_name.length()-1] != '/') {
      absolute_file_name += '/';
    }

    absolute_file_name += file_name;
    tested_paths.push_back (absolute_file_name);

    if (!OSMisc::access (absolute_file_name.c_str(), OSMisc::ACCESS_READ)) {
      break;
    }
  }

  if (i == _params->inc_paths.size()) {
    cerr << IDLError (38, node, to_be_imported.in()) << endl;
    cerr << "The following locations were tried:" << endl;
    for (i=0; i<tested_paths.size(); i++) {
      cerr << "  " << tested_paths[i] << endl;
    }
    exit (1);
  }

  /*
   * File was found. Load it.
   */

  // cout << "loading " << absolute_file_name << endl;

  /*
   * Step 1: Invoke the Preprocessor
   */
#ifndef _WIN32
  string cmd = "mico-cpp -D __MICO_IDL__ ";
#else // _WIN32
  string cmd = "mico-cpp.exe -D __MICO_IDL__ ";
  char sz_path[256] = "";
  GetModuleFileName(0, sz_path, 255);
  string p(sz_path);
  p.erase(p.find_last_of("\\") + 1, p.length());
  cmd = "\"" + p + cmd + "\"";
#endif // _WIN32

  cmd += _params->cpp_options;
  cmd += ' ';

  string includepath;
  if (_params->base_dir.empty()) {
    includepath = ABSEXECDIR;
    if (!includepath.empty() && includepath[includepath.length() - 1] != '/')
      includepath += '/';
  } else {
    includepath = _params->base_dir;
  }
  includepath += "include/";
  string micopath = includepath + "mico/";

  cmd += "-isystem ";
  cmd += includepath;
  cmd += ' ';
  cmd += " -isystem ";
  cmd += micopath;
  cmd += ' ';

  cmd += absolute_file_name;

  FILE * inp_file = OSMisc::popen (cmd.c_str(), "r");

  if (inp_file == NULL) {
    cerr << "error: cannot open " << absolute_file_name << endl;
    exit (1);
  }

  int dummy_char = fgetc (inp_file);

  if (dummy_char == EOF) {
    cerr << "error: cannot execute mico-cpp for import: " << cmd << endl;
    exit( 1 );
  }

  ungetc (dummy_char, inp_file);
  
  /*
   * Step 2: Invoke the Parser (bison)
   */
    
  Parser parser (inp_file, file_name.c_str());
  parser.parse();
  
  OSMisc::pclose (inp_file);

  /*
   * Step 3: Traverse the Parse Tree and load Interface Repository
   */
  
  IDLParser idlParser (*_db, *_params);
  idlParser.collect (_repo, parser.getRootNode());
  _db->set_repoids (_repo);
  return;
}

/*
 * responsible for #pragma, typeId and typePrefix
 */

void
IDLParser::collectPragma( CORBA::Container_ptr con, ParseNode *node )
{
  string s, dir, name, value;
  s = node->getIdent();
  
  if (node->getType() == t_typeid) {
    dir = "ID";
    name = node->getBranch(1)->getIdent();
    value = node->getIdent();
  }
  else if (node->getType() == t_typeprefix) {
    dir = "prefix";
    name = node->getBranch(1)->getIdent();
    value = node->getIdent();
  }
  else {
    assert (node->getType() == t_pragma);

    /*
     * handle plain old #pragma
     */

    // Strip off "#pragma"
    s = s.substr( 7 );
    
    int i = s.find_first_not_of( " \t" );
    if( !(i > 0) )
      return;
    int j = s.find_first_of( " \t", i );
    if( !(j > i) )
      return;

    // Get pragma directive
    dir = s.substr( i, j - i );
    i = s.find_first_not_of( " \t", j );
    check_pragma_syntax( i > 0, node );

    if( dir == "prefix" ) {
      check_pragma_syntax( s[ i ] == '"', node );
      j = s.find( '"', i + 1 );
      check_pragma_syntax( j > i, node );
   
      // Get prefix
      name = "";
      value = s.substr( i + 1, j - i - 1 );
    }
    else if (dir == "version") {
      j = s.find( ' ', i );
      check_pragma_syntax( j > i, node );
      name = s.substr( i, j - i );

      i = s.find_first_not_of( " \t", j );
      j = s.find_first_not_of( "0123456789.", i );
      if( j < 0 )
	j = s.length();
      value = s.substr( i, j - i );

      /*
       * version must be <major>.<minor>
       */

      if (value.length() != 3 ||
	  (value[0] < '0' && value[0] > '9') ||
	  value[1] != '.' ||
	  (value[2] < '0' && value[2] > '9')) {
	cerr << IDLError (22, node) << endl;
	exit (1);
      }
    }
    else if( dir == "ID" ) {
      j = s.find( ' ', i );
      check_pragma_syntax( j > i, node );
      name = s.substr( i, j - i );

      i = s.find_first_not_of( " \t", j );
      check_pragma_syntax( s[ i ] == '"', node );
      j = s.find( '"', i + 1 );
      check_pragma_syntax( j > i, node );
      value = s.substr( i + 1, j - i - 1 );
    }
    else {
      /*
       * ignore unknown pragma
       */
      return;
    }
  }

  // Strip leading '_'
  if (name.length() > 0 && name[0] == '_')
      name = name.substr (1);
  
  if (name.length() > 0 && name[0] != ':') {
    // Name is relative, make it absolute
    CORBA::Contained_var in = con->lookup (name.c_str());
    if (CORBA::is_nil (in)) {
      /* The name wasn't defined. Assume its relative to the
	 current scope */
      name = _db->get_current_scope() + name;
    } else {
      name = in->absolute_name();
    }
  }

  if (dir == "prefix") {
    if (name == "") {
      _db->switch_prefix (value);
    }
    else {
      _db->set_prefix (name, value);
    }
  }
  else if (dir == "version") {
    _db->set_repo_version (name, value);
  }
  else if (dir == "ID") {
    _db->set_repo_id (name, value);
  }
}

void IDLParser::check_pragma_syntax( bool condition, ParseNode *node )
{
  if( condition )
    return;
  cerr << IDLError( 22, node ) << endl;
  exit( 1 );
}

void IDLParser::collectInclude( CORBA::Container_ptr con, ParseNode *node )
{
  string s = node->getIdent();
  string num;
  string name;
  int i, j;

  assert( s[0] == '#' );
 
  if (s.find ("#line ") == 0)
    s = s.substr (6);
  else if (s[1] == ' ') 
    s = s.substr( 2 );
  else
  {
      cerr << IDLError( 32, node, node->getIdent() ) << endl;
      exit( 1 );
  }

  // Get line number
  i = s.find_first_of( "0123456789" );
  assert( i >= 0 );
  j = s.find_first_not_of( "0123456789", i );
  assert( j > i );

  // strip off line number
  s = s.substr( j );

  // Get file name
  i = s.find( '"' );
  assert (i >= 0);

  j = s.find( '"', ++i );
  assert( j >= i );

  if (s.length() > (unsigned int)j+2) {
    if (s[j+2] == '1')
      _db->enter_include_file();

    if (s[j+2] == '2')
      _db->leave_include_file();
  }
}

void IDLParser::collectModule( CORBA::Container_ptr con, ParseNode *node )
{
  CORBA::ModuleDef_var m = CORBA::ModuleDef::_nil ();
  
  assert( node->getType() == t_module );

  CORBA::String_var repo_id = gen_repo_id( node );

  string name;
  name = node->getIdent();
  _db->enter_scope( name );

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var c = con->lookup_name (name.c_str(), 1,
						  CORBA::dk_all, 1);

    assert (c->length() <= 1);

    if (c->length() == 1) {
      // We re-open an existing module
      m = CORBA::ModuleDef::_narrow( c[ (CORBA::ULong) 0] );
      assert( !CORBA::is_nil( m ) );
    } else {
      // We create a new module
      m = con->create_module( repo_id, node->getIdent(), _version );
    }
    scan( m, node->getBranch( 1 ) );
  }

  _db->leave_scope();

  // regenerate repo id in case it was redefined with a typeprefix
  if (!CORBA::is_nil (m)) {
    repo_id = gen_repo_id (node);
    m->id (repo_id);
  }
}

void IDLParser::collectTypedef( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == t_typedef );
  CORBA::IDLType_var orig = collectType( con, node->getBranch( 1 ) );
  node = node->getBranch( 2 );
  while( node != NULL ) {
    assert( node->getType() == declarators );
    ParseNode *dcl = node->getBranch( 1 );
    if( dcl->getType() == simple_declarator ) {
      CORBA::String_var repo_id = gen_repo_id( dcl );
      CORBA::AliasDef_var a = con->create_alias( repo_id,
						 dcl->getIdent(),
						 _version,
						 orig );
    } else if( dcl->getType() == array_declarator ) {
      CORBA::ArrayDef_var arr = collectArray( con, orig, dcl->getBranch( 1 ) );
      CORBA::String_var repo_id = gen_repo_id( dcl );
      CORBA::AliasDef_var a = con->create_alias( repo_id,
						 dcl->getIdent(),
						 _version,
						 arr );
    } else
      assert( 0 );
    node = node->getBranch( 2 );
  }
}

void
IDLParser::collectNative( CORBA::Container_ptr con, ParseNode *node)
{
  assert( node->getType() == t_native );
  ParseNode *dcl = node->getBranch( 1 );
  assert( dcl->getType() == simple_declarator );
  CORBA::String_var repo_id = gen_repo_id( dcl );
  CORBA::NativeDef_var n = con->create_native( repo_id,
					       dcl->getIdent(),
					       _version );
}

CORBA::ArrayDef_ptr
IDLParser::collectArray( CORBA::Container_ptr con,
			 CORBA::IDLType_ptr orig, ParseNode *node )
{
  assert( node->getType() == fixed_array_sizes );
  Const c = collectConst( con, node->getBranch( 1 ) );
  CORBA::ULong length = c.get_int();
  CORBA::IDLType_var elem;
  if( node->getBranch( 2 ) != NULL ) {
    elem = collectArray( con, orig, node->getBranch( 2 ) );
  } else {
    elem = CORBA::IDLType::_duplicate( orig );
  }
  CORBA::ArrayDef_ptr arr = _repo->create_array( length, elem );
  _db->register_anonym_type( node, arr );
  return arr;
}

void IDLParser::collectConstDcl( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == const_dcl );

  ParseNode* t = node->getBranch( 1 );

  if( t->getType() == fixed_pt_type ) {
    collectConstFixedDcl( con, node );
    return;
  }
  
  CORBA::IDLType_var type = collectType( con, t );
  CORBA::TypeCode_var origtc = type->type ();
  CORBA::TypeCode_var tc = origtc;
  CORBA::Any val;

  while (tc->kind() == CORBA::tk_alias)
    tc = tc->content_type ();

  switch( tc->kind() ) {
  case CORBA::tk_float:
  case CORBA::tk_double:
  case CORBA::tk_long:
  case CORBA::tk_short:
  case CORBA::tk_ulong:
  case CORBA::tk_ushort:
  case CORBA::tk_boolean:
  case CORBA::tk_string:
  case CORBA::tk_wstring:
  case CORBA::tk_char:
  case CORBA::tk_octet:
  case CORBA::tk_wchar:
  case CORBA::tk_longlong:
  case CORBA::tk_ulonglong:
  case CORBA::tk_longdouble:
    {
      Const const_value = collectConst( con, node->getBranch( 2 ) );
      val = const_value.get_value( origtc );
      break;
    }

  case CORBA::tk_enum:
    {
      string scoped_name = collectScopedName (node->getBranch(2));
      unsigned long i = scoped_name.find_last_of (":");

      if (i != string::npos) {
	scoped_name = scoped_name.substr (i+1);
      }

      for (i=0; i<tc->member_count(); i++) {
	if (strcmp (scoped_name.c_str(), tc->member_name (i)) == 0) {
	  break;
	}
      }

      if (i >= tc->member_count()) {
	cerr << IDLError( 4, t, t->getIdent() ) << endl;
	exit( 1 );
      }

      val.set_type (origtc);
      val.enum_put (i);

      break;
    }

  default:
    cerr << IDLError( 4, t, t->getIdent() ) << endl;
    exit( 1 );
  }

  CORBA::String_var repo_id = gen_repo_id( node );
  CORBA::ConstantDef_var c;

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if (cs->length() > 0) {
      c = CORBA::ConstantDef::_narrow (cs[(MICO_ULong)0]);

      if ( CORBA::is_nil( c ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      
      if ( strcmp( c->id(), repo_id ) != 0 ) {
	cerr << IDLError( 33, node, repo_id.in() ) << endl;
	exit( 1 );
      }

      c->type_def (type);
      c->value (val);
    }
    else {
      c = con->create_constant (repo_id,
				node->getIdent(),
				_version,
				type,
				val);
    }
  }
}

/*
 * Const dcl of a fixed has to be handled differently. Consider:
 *   const fixed f = 123.45D;
 * here, the type dcl is incomplete. The consequence is, that
 * for fixed const dcl we have to collect the value first
 * and then create a type in the IR.
 */
void IDLParser::collectConstFixedDcl( CORBA::Container_ptr con,
				      ParseNode *node )
{
  assert( node->getType() == const_dcl );

  CORBA::IDLType_var type;
  ParseNode* t = node->getBranch( 1 );

  Const const_value = collectConst( con, node->getBranch( 2 ) );

  CORBA::LongDouble ld;
  CORBA::Boolean r = ( const_value.get_any() >>= ld );
  assert (r);

  FixedBase f (ld);
  
  type = _repo->create_fixed( f.fixed_digits(), f.fixed_scale() );
  _db->register_anonym_type( t, type );
  
  CORBA::TypeCode_var tc = type->type();

  CORBA::Any val = const_value.get_value( tc );
  
  CORBA::String_var repo_id = gen_repo_id( node );
  CORBA::ConstantDef_var c = con->create_constant( repo_id,
						   node->getIdent(),
						   _version,
						   type,
						   val );
}

void IDLParser::collectException( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == except_dcl );

  CORBA::String_var repo_id = gen_repo_id( node );

  string name;
  name = node->getIdent();
  _db->enter_scope( name );
  
  CORBA::StructMemberSeq members;
  CORBA::ExceptionDef_var e;

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if (cs->length() > 0) {
      e = CORBA::ExceptionDef::_narrow (cs[(MICO_ULong)0]);

      if ( CORBA::is_nil( e ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      
      if ( strcmp( e->id(), repo_id ) != 0 ) {
	cerr << IDLError( 33, node, repo_id.in() ) << endl;
	exit( 1 );
      }
    }
    else {
      e = con->create_exception( repo_id,
				 node->getIdent(),
				 _version,
				 members );
    }
  }

  collectStructMembers( e, node->getBranch( 1 ), members );
  e->members( members );
  current_parse_node( node );
  _db->leave_scope();
}

CORBA::StructDef_ptr
IDLParser::collectStruct( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == struct_type );

  CORBA::String_var scoped_name = gen_scoped_name( node );
  CORBA::String_var repo_id = gen_repo_id( node );

  string name;
  name = node->getIdent();
  _db->enter_scope( name );

  CORBA::StructMemberSeq members;
  CORBA::StructDef_ptr s;

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if (cs->length() > 0) {
      scoped_name = cs[ (MICO_ULong)0 ]->absolute_name();
      if( !_db->is_in_forward_dcl( scoped_name ) ) {
        cerr << IDLError( 12, node, node->getIdent() ) << endl;
        exit( 1 );
      }
    }

    if ( _db->is_in_forward_dcl( scoped_name ) ) {
	/* This struct was forward declared, we should be able to
	   find it, already created in the repository */
      CORBA::Contained_var c = _repo->lookup( scoped_name );

      if ( CORBA::is_nil( c ) ) {
	cerr << IDLError( 2, node, scoped_name.in() ) << endl;
	exit( 1 );
      }
      
      s = CORBA::StructDef::_narrow( c );
      
      if ( CORBA::is_nil( s ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      
      if ( strcmp( s->id(), repo_id ) != 0 ) {
	cerr << IDLError( 33, node, repo_id.in() ) << endl;
	exit( 1 );
      }
    }
    else {
      s = con->create_struct( repo_id,
			      node->getIdent(),
			      _version,
			      members );
    }
  }
  else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * interface with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      s = CORBA::StructDef::_narrow( cs[(CORBA::ULong)0] );
      if( CORBA::is_nil( s ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      _db->leave_scope ();
      return s;
    }

    _db->leave_scope ();
    return CORBA::StructDef::_nil ();
  }

  scoped_name = s->absolute_name();
  _db->remove_forward_dcl( scoped_name );
  _seen_iface.insert( scoped_name.in() );

  collectStructMembers( s, node->getBranch( 1 ), members );
  s->members( members );
  current_parse_node( node );
  _db->leave_scope();
  return s;
}

void IDLParser::collectStructMembers( CORBA::Container_ptr con,
				      ParseNode *n, CORBA::StructMemberSeq &m )
{
  if( n == NULL )
    return;
  assert( n->getType() == member_list );
  collectStructMember( con, n->getBranch( 1 ), m );
  n = n->getBranch( 2 );
  if( n != NULL )
    collectStructMembers( con, n, m );
}

void IDLParser::collectStructMember( CORBA::Container_ptr con,
				     ParseNode *n, CORBA::StructMemberSeq &m )
{
  if (n->getType() == t_pragma ||
      n->getType() == t_typeid ||
      n->getType() == t_typeprefix) {
    collectPragma (con, n);
    return;
  }
  if (n->getType() == t_include) {
    collectInclude (con, n);
    return;
  }
  assert( n->getType() == member );
  CORBA::IDLType_var t = collectType( con, n->getBranch( 1 ) );
  collectStructMemberDeclarators( con, n->getBranch( 2 ), t, m );
}

void IDLParser::collectStructMemberDeclarators( CORBA::Container_ptr con,
						ParseNode *n,
						CORBA::IDLType_ptr t,
						CORBA::StructMemberSeq &m )
{
  assert( n->getType() == declarators );
  collectStructMemberDeclarator( con, n->getBranch( 1 ), t, m );
  n = n->getBranch( 2 );
  if( n != NULL )
    collectStructMemberDeclarators( con, n, t, m );
}

void IDLParser::collectStructMemberDeclarator( CORBA::Container_ptr con,
					       ParseNode *n,
					       CORBA::IDLType_ptr t,
					       CORBA::StructMemberSeq &m )
{
  CORBA::StructMember member;
  int i = m.length();
  m.length( i + 1 );

  switch( n->getType() ) {
  case simple_declarator:
    member.name     = CORBA::string_dup( n->getIdent() );
    member.type     = CORBA::_tc_void;   //[6.5.10]
    member.type_def = CORBA::IDLType::_duplicate( t );
    m[ i ] = member;
    break;
  case array_declarator:
    member.name     = CORBA::string_dup( n->getIdent() );
    member.type     = CORBA::_tc_void;   //[6.5.10]
    member.type_def = collectArray( con, t, n->getBranch( 1 ) );
    m[ i ] = member;
    break;
  default:
    assert( 0 );
  }

  current_parse_node( n );

  CORBA::StructDef_var sd = CORBA::StructDef::_narrow (con);
  CORBA::ExceptionDef_var ed = CORBA::ExceptionDef::_narrow (con);

  if (!CORBA::is_nil (sd)) {
    sd->members (m);
  }
  else if (!CORBA::is_nil (ed)) {
    ed->members (m);
  }
  else {
    assert (0);
  }
}

CORBA::UnionDef_ptr
IDLParser::collectUnion( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == union_type );

  CORBA::String_var scoped_name = gen_scoped_name( node );
  CORBA::String_var repo_id = gen_repo_id( node );

  string name;
  name = node->getIdent();
  _db->enter_scope( name );

  CORBA::IDLType_var switch_type = CORBA::IDLType::_nil();
  CORBA::UnionMemberSeq members;
  CORBA::UnionDef_ptr u;

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if (cs->length() > 0) {
      scoped_name = cs[ (MICO_ULong)0 ]->absolute_name();
      if( !_db->is_in_forward_dcl( scoped_name ) ) {
        cerr << IDLError( 12, node, node->getIdent() ) << endl;
        exit( 1 );
      }
    }

    CORBA::Contained_var c;

    if ( _db->is_in_forward_dcl( scoped_name ) ) {
	/* This struct was forward declared, we should be able to
	   find it, already created in the repository */
      c = _repo->lookup( scoped_name );

      if ( CORBA::is_nil( c ) ) {
	cerr << IDLError( 2, node, scoped_name.in() ) << endl;
	exit( 1 );
      }
      
      u = CORBA::UnionDef::_narrow( c );
      
      if ( CORBA::is_nil( u ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      
      if ( strcmp( u->id(), repo_id ) != 0 ) {
	cerr << IDLError( 33, node, repo_id.in() ) << endl;
	exit( 1 );
      }
    }
    else {
      u = con->create_union( repo_id,
			     node->getIdent(),
			     _version,
			     switch_type,
			     members );
    }
  }
  else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * interface with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      u = CORBA::UnionDef::_narrow( cs[(CORBA::ULong)0] );
      if( CORBA::is_nil( u ) ) {
	cerr << IDLError( 23, node, node->getIdent() ) << endl;
	exit( 1 );
      }
      _db->leave_scope ();
      return u;
    }

    _db->leave_scope ();
    return CORBA::UnionDef::_nil();
  }

  scoped_name = u->absolute_name();
  _db->remove_forward_dcl( scoped_name );
  _seen_iface.insert( scoped_name.in() );

  switch_type = collectUnionSwitchTypeSpec( u, node->getBranch( 1 ) );
  u->discriminator_type_def( switch_type );
  
  collectUnionMembers( u, node->getBranch( 2 ), switch_type, members );
  current_parse_node( node );
  u->members( members );
  _db->leave_scope();
  return u;
}

CORBA::IDLType_ptr
IDLParser::collectUnionSwitchTypeSpec( CORBA::Container_ptr con,
				       ParseNode *node )
{
  CORBA::IDLType_ptr switch_type = collectType( con, node );
  CORBA::IDLType_var resolv_type = CORBA::IDLType::_duplicate( switch_type );
  while( resolv_type->def_kind() == CORBA::dk_Alias ) {
    CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( resolv_type );
    assert( !CORBA::is_nil( a ) );
    resolv_type = a->original_type_def();
  }
  switch( resolv_type->def_kind() ) {
  case CORBA::dk_Enum:
    break;
  case CORBA::dk_Primitive:
    {
      CORBA::PrimitiveDef_var p = CORBA::PrimitiveDef::_narrow( resolv_type );
      assert( !CORBA::is_nil( p ) );
      switch( p->kind() ) {
      case CORBA::pk_long:
      case CORBA::pk_ulong:
      case CORBA::pk_longlong:
      case CORBA::pk_ulonglong:
      case CORBA::pk_short:
      case CORBA::pk_ushort:
      case CORBA::pk_char:
      case CORBA::pk_wchar:
      case CORBA::pk_boolean:
	break;
      default:
	cerr << IDLError( 5, node ) << endl;
	exit( 1 );
      }
      break;
    }
  default:
    cerr << IDLError( 5, node ) << endl;
    exit( 1 );
  }
  return switch_type;
}

void
IDLParser::collectUnionElementSpec( CORBA::Container_ptr con,
				    ParseNode *node,
				    CORBA::UnionMember& member )
{
  assert( node->getType() == _case );
  while( node->getBranch( 3 ) == NULL ) {
    node = node->getBranch( 2 );
    assert( node->getType() == _case );
  }
  node = node->getBranch( 3 );
  assert( node->getType() == element_spec );
  CORBA::IDLType_var type_spec = collectType( con, node->getBranch( 1 ) );
  node = node->getBranch( 2 );

  switch( node->getType() ) {
  case simple_declarator:
    member.name     = CORBA::string_dup( node->getIdent() );
    member.type     = CORBA::_tc_void;   //[6.5.11]
    member.type_def = CORBA::IDLType::_duplicate( type_spec );
    break;
  case array_declarator:
    member.name     = CORBA::string_dup( node->getIdent() );
    member.type     = CORBA::_tc_void;   //[6.5.11]
    member.type_def = collectArray( con, type_spec, node->getBranch( 1 ) );
    break;
  default:
    assert( 0 );
  }
}

void
IDLParser::collectUnionCaseLabel( CORBA::Container_ptr con,
				  ParseNode *node,
				  CORBA::IDLType_ptr switch_type,
				  CORBA::UnionMember& member ) 
{
  if( node->getType() == t_default ) {
    member.label <<= CORBA::Any::from_octet( 0 );
  } else {
    CORBA::IDLType_var resolv_type = CORBA::IDLType::_duplicate( switch_type );
    while( resolv_type->def_kind() == CORBA::dk_Alias ) {
      CORBA::AliasDef_var a = CORBA::AliasDef::_narrow( resolv_type );
      assert( !CORBA::is_nil( a ) );
      resolv_type = a->original_type_def();
    }
    switch( resolv_type->def_kind() ) {
    case CORBA::dk_Enum:
      {
	if( node->getType() != scoped_name ) {
	  cerr << IDLError( 20, node ) << endl;
	  exit( 1 );
	}
	CORBA::TypeCode_var enum_tc = resolv_type->type();
	enum_tc = CORBA::TypeCode::_duplicate( enum_tc->unalias() );

	string label;
	string scoped_name = node->getIdent();
	if( (int) scoped_name.find( "::" ) >= 0 ) {
	  // We have a scoped name. Check that the discriminator
	  // type is defined in the scope that is defined through
	  // this scoped name
	  string scoped_enum = scoped_name;
	  int i = scoped_enum.find_last_of( ":" );
	  scoped_enum = scoped_enum.substr( 0, i+1 );
	  CORBA::EnumDef_var enum_switch_type =
	    CORBA::EnumDef::_narrow( resolv_type );
	  assert( !CORBA::is_nil( enum_switch_type ) );
	  CORBA::String_var enum_name = enum_switch_type->name();
	  scoped_enum += enum_name;
	  CORBA::Contained_var e = con->lookup (scoped_enum.c_str());
	  if( CORBA::is_nil( e ) || !e->_is_equivalent( resolv_type ) ) {
	    cerr << IDLError( 11, node, scoped_name.c_str(),
  			                scoped_enum.c_str() ) << endl;
	    exit( 1 );
	  }
	  label = scoped_name.substr( i+1 );
	} else {
	  label = scoped_name;
	}

	CORBA::ULong j;
	for( j = 0; j < enum_tc->member_count(); j++ ) {
	  if( !strcmp( enum_tc->member_name( j ), label.c_str() ) ) {
	    member.label.set_type( enum_tc );
	    CORBA::Boolean r = member.label.enum_put( j );
	    assert (r);
	    break;
	  }
	}
	if( j >= enum_tc->member_count() ) {
	  CORBA::Contained_var c = CORBA::Contained::_narrow( switch_type );
	  assert( !CORBA::is_nil( c ) );
	  CORBA::String_var name = c->absolute_name();
	  cerr << IDLError( 11, node, label.c_str(), (char*) name ) << endl;
	  exit( 1 );
	}
      }
      break;
    case CORBA::dk_Primitive:
      {
	Const c = collectConst( con, node );
	CORBA::TypeCode_var tc = resolv_type->type();
	CORBA::Any a = c.get_value( tc );
	member.label = a;
      }
      break;
    default:
      break;
    }
  }
}
  
void
IDLParser::collectUnionCases( CORBA::Container_ptr con,
			      ParseNode *node,
			      CORBA::IDLType_ptr switch_type,
			      CORBA::UnionMemberSeq& members )
{
  if (node->getBranch( 4 ) != NULL) {
    collectPragma (con, node->getBranch( 4 ));
  }

  assert( node->getType() == _case );

  CORBA::UnionMember member;
  collectUnionElementSpec( con, node, member );
  collectUnionCaseLabel( con, node->getBranch( 1 ), switch_type, member );

  int i = members.length();
  members.length( i + 1 );
  members[ i ] = member;

  if( node->getBranch( 2 ) != NULL )
    collectUnionCases( con, node->getBranch( 2 ), switch_type, members );
}

void
IDLParser::collectUnionMembers( CORBA::Container_ptr con,
				ParseNode *node,
				CORBA::IDLType_ptr switch_type,
				CORBA::UnionMemberSeq& members )
{
  assert( node->getType() == switch_body );
  collectUnionCases( con, node->getBranch( 1 ), switch_type, members );
  if( node->getBranch( 2 ) != NULL )
    collectUnionMembers( con, node->getBranch( 2 ), switch_type, members );
}

void
IDLParser::collectForwardDcl (CORBA::Container_ptr con, ParseNode *node)
{
  assert( node->getType() == forward_dcl ||
	  node->getType() == abstract_forward_dcl ||
	  node->getType() == local_forward_dcl ||
	  node->getType() == struct_forward_decl ||
	  node->getType() == union_forward_decl ||
	  node->getType() == component_forward_dcl);
  
  CORBA::String_var scoped_name = gen_scoped_name( node );
  CORBA::String_var repo_id = gen_repo_id( node );
  CORBA::InterfaceDefSeq base_interfaces; // Leave empty for now

  if( is_iface_defined( scoped_name ) )
    return;

  if( !_db->is_in_forward_dcl( scoped_name ) ) {
    CORBA::Contained_var in;
 
    if( node->is_toplevel() || _feed_included_defs ) {
      switch (node->getType()) {
      case forward_dcl: {
        in = con->create_interface( repo_id,
				    node->getIdent(),
				    _version,
				    base_interfaces);
	break;
      }
      case abstract_forward_dcl: {
	CORBA::AbstractInterfaceDefSeq abi; // Leave empty for now
        in = con->create_abstract_interface( repo_id,
					     node->getIdent(),
					     _version,
					     abi);
	break;
      }
      case local_forward_dcl: {
        in = con->create_local_interface( repo_id,
					  node->getIdent(),
					  _version,
					  base_interfaces);
	break;
      }
      case struct_forward_decl: {
	CORBA::StructMemberSeq sms;
	in = con->create_struct (repo_id,
				 node->getIdent(),
				 _version,
				 sms);
	break;
      }
      case union_forward_decl: {
	CORBA::UnionMemberSeq sms;
	CORBA::Repository_var therepo = CORBA::Repository::_narrow (con);
	if (CORBA::is_nil (therepo)) {
	  CORBA::Contained_var cont = CORBA::Contained::_narrow (con);
	  therepo = cont->containing_repository ();
	}
	CORBA::IDLType_var dummydisc =
	  therepo->get_primitive (CORBA::pk_short);
	in = con->create_union (repo_id,
				node->getIdent(),
				_version,
				dummydisc,
				sms);
	break;
      }
#ifdef USE_CCM
      case component_forward_dcl: {
	CORBA::ComponentIR::Container_var ccv =
	  CORBA::ComponentIR::Container::_narrow (con);
	assert (!CORBA::is_nil (ccv));
	CORBA::InterfaceDefSeq supported_interfaces;
	in = ccv->create_component (repo_id,
				    node->getIdent(),
				    _version,
				    CORBA::ComponentIR::ComponentDef::_nil(),
				    supported_interfaces);
	break;
      }
#endif
      default: {
	assert (0);
      }
      }
    } else {
      /*
       * Since we don't feed included defs, we only have to make
       * sure that no other IDL type with the same identifier
       * exists (the only exception being that there is another
       * interface with the same name).
       */

      CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						     CORBA::dk_all, 1);

      if (cs->length() == 0) {
	return;
      }
    }

    _db->add_forward_dcl( scoped_name );
  }
}

void
IDLParser::collectInterface( CORBA::Container_ptr con, ParseNode *node )
{
  CORBA::InterfaceDef_var in;
 
  ParseNode *header = node->getBranch( 1 );

  CORBA::String_var scoped_name = gen_scoped_name( header );
  CORBA::String_var repo_id = gen_repo_id( header );

  string name;
  name = header->getIdent();
  _db->enter_scope( name );

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if( cs->length() == 1 ) {
      scoped_name = cs[(CORBA::ULong)0]->absolute_name();
      if( !_db->is_in_forward_dcl( scoped_name ) ) {
	cerr << IDLError( 12, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    if ( _db->is_in_forward_dcl( scoped_name ) ) {
	/* This interface was forward declared, we should be able to
	   find it, already created in the repository */
      CORBA::Contained_var c = _repo->lookup( scoped_name );
	if ( CORBA::is_nil( c ) ) {
	    cerr << IDLError( 2, header, scoped_name.in() ) << endl;
	    exit( 1 );
	}
	in = CORBA::InterfaceDef::_narrow( c );
	if ( CORBA::is_nil( in ) ) {
	    cerr << IDLError( 23, header, header->getIdent() ) << endl;
	    exit( 1 );
	}
	if ( strcmp( in->id(), repo_id ) != 0 ) {
	    cerr << IDLError( 33, header, repo_id.in() ) << endl;
	    exit( 1 );
	}
    } else {
      CORBA::InterfaceDefSeq base_interfaces; // Leave empty for now
      if (header->getType() == interface_header) {
	in = con->create_interface( repo_id,
				    header->getIdent(),
				    _version,
				    base_interfaces);
      }
      else if (header->getType() == abstract_interface_header) {
	CORBA::AbstractInterfaceDefSeq abi; // Leave empty for now
	in = con->create_abstract_interface( repo_id,
					     header->getIdent(),
					     _version,
					     abi);
      }
      else if (header->getType() == local_interface_header) {
	in = con->create_local_interface( repo_id,
					  header->getIdent(),
					  _version,
					  base_interfaces);
      }
    }
  } else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * interface with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      in = CORBA::InterfaceDef::_narrow( cs[(CORBA::ULong)0] );
      if( CORBA::is_nil( in ) ) {
	cerr << IDLError( 23, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    _db->leave_scope();
    return;
  }

  scoped_name = in->absolute_name();
  _db->remove_forward_dcl( scoped_name );
  _seen_iface.insert( scoped_name.in() );

  header = header->getBranch( 1 );

  CORBA::InterfaceDefSeq base_interfaces;
  
  while( header != NULL ) {
    ParseNode* node_base = header->getBranch( 1 );
    CORBA::IDLType_var t = collectType( con, node_base );
    CORBA::InterfaceDef_var base =
      CORBA::InterfaceDef::_narrow( t );
    if( CORBA::is_nil( base ) ) {
      cerr << IDLError( 8, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    scoped_name = base->absolute_name();
    if( _db->is_in_forward_dcl( scoped_name ) ) {
      cerr << IDLError( 9, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    if( in->_is_equivalent( base ) ) {
      cerr << IDLError( 25, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    if (in->def_kind()   == CORBA::dk_AbstractInterface &&
	base->def_kind() != CORBA::dk_AbstractInterface) {
      cerr << IDLError( 30, node_base, node_base->getIdent(), 
			scoped_name.in() )
	   << endl;
      exit( 1 );
    }
    int i = base_interfaces.length();
    base_interfaces.length( i + 1 );
    base_interfaces[ i ] = base;
    header = header->getBranch( 2 );
  }
  
  if( node->is_toplevel() || _feed_included_defs ) {
    in->base_interfaces( base_interfaces );
    scan( in, node->getBranch( 2 ) );
  }

  _db->leave_scope();

  // regenerate repo id in case it was redefined with a typeprefix
  header = node->getBranch( 1 );
  repo_id = gen_repo_id (header);
  in->id (repo_id);

#ifdef USE_CCM
  /*
   * make implicit CCM_<name> interface, so that user code can derive
   * from it. We don't care about its contents; the interface will be
   * re-generated by the IDL 3 to IDL 2 stage.
   */

  if (in->def_kind() == CORBA::dk_Interface) {
    make_ccm_interface (in);
  }
#endif
}

void
IDLParser::collectRaisesExpr( CORBA::Container_ptr con, ParseNode *node,
			      CORBA::ExceptionDefSeq &exs )
{
  if( node == NULL )
    return;
  
  assert( node->getType() == raises_expr );
  node = node->getBranch( 1 );
  while( node != NULL ) {
    CORBA::Contained_var c;
    c = con->lookup ( collectScopedName( node->getBranch( 1 ) ) );
    if( CORBA::is_nil( c ) ) {
      cerr << IDLError( 2, node->getBranch( 1 ),
			node->getBranch( 1 )->getIdent() ) << endl;
      exit( 1 );
    }
    CORBA::ExceptionDef_ptr ex = CORBA::ExceptionDef::_narrow( c );
    if( CORBA::is_nil( ex ) ) {
      cerr << IDLError( 6, node->getBranch( 1 ),
			node->getBranch( 1 )->getIdent() ) << endl;
      exit( 1 );
    }
    int i = exs.length();
    exs.length( i + 1 );
    exs[ i ] = ex;
    node = node->getBranch( 2 );
  }
}

void
IDLParser::collectContextExpr( ParseNode *node, CORBA::ContextIdSeq &ctx )
{
  while( node != NULL ) {
    assert( node->getType() == string_literals );
    int i = ctx.length();
    ctx.length( i + 1 );
    ctx[ i ] = (const char *) node->getIdent();
    node = node->getBranch( 1 );
  }
}

void
IDLParser::collectMethod( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == op_dcl );
  // con must be an InterfaceDef or a ValueDef
  CORBA::InterfaceDef_var interf = CORBA::InterfaceDef::_narrow( con );
  CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( con );
  assert( !CORBA::is_nil( interf ) || !CORBA::is_nil( val ) );

  // Get operation attribute
  CORBA::OperationMode op_mode;
  if( node->getBranch( 1 ) == NULL )
    op_mode = CORBA::OP_NORMAL;
  else
    op_mode = CORBA::OP_ONEWAY;
  
  CORBA::ParDescriptionSeq params;
  ParseNode *p = node->getBranch( 3 );
  if( p != NULL ) {
    // This method has parameters
    int i = 0;
    
    while( p != NULL ) {
      assert( p->getType() == param_dcls );
      ParseNode *q;
      CORBA::ParameterMode mode;    // in, out, inout attribute
      CORBA::IDLType_ptr idl_type;  // type specification
      
      q = p->getBranch( 1 );
      assert( q->getType() == param_dcl );
      switch( q->getBranch( 1 )->getType() ) {
      case t_in:
	mode = CORBA::PARAM_IN;
	break;
      case t_out:
	mode = CORBA::PARAM_OUT;
	break;
      case t_inout:
	mode = CORBA::PARAM_INOUT;
	break;
      default:
	assert( 0 );
      }
      idl_type = collectType( con, q->getBranch( 2 ) );
      CORBA::ParameterDescription param;
      param.name = CORBA::string_dup( q->getBranch( 3 )->getIdent() );
      param.mode = mode;
      param.type = idl_type->type();
      param.type_def = idl_type;

      params.length( i + 1 );
      params[ i++ ] = param;
      p = p->getBranch( 2 );
    }
  }

  // Look for return value
  CORBA::TypeCode_var op_result_type;
  CORBA::IDLType_var op_result;
  
  p = node->getBranch( 2 );
  assert( p != NULL );
  if( p->getType() == t_void ) {
    op_result = _repo->get_primitive( CORBA::pk_void );
  } else {
    assert( p->getType() == op_type_spec );
    op_result = collectType( con, p->getBranch( 1 ) );
  }

  CORBA::ExceptionDefSeq exceptions;
  if( node->getBranch( 4 ) ) {
    collectRaisesExpr( con, node->getBranch( 4 ), exceptions );
  }
    
  CORBA::ContextIdSeq contexts;
  if( node->getBranch( 5 ) )
    collectContextExpr( node->getBranch( 5 ), contexts );
  
  CORBA::String_var repo_id = gen_repo_id( node );;
  CORBA::OperationDef_var op;
  
  if (!CORBA::is_nil (interf)) {
    op = interf->create_operation( repo_id,
				   node->getIdent(),
				   _version,
				   op_result,
				   op_mode,
				   params,
				   exceptions,
				   contexts );
  }
  else {
    op = val->create_operation( repo_id,
				node->getIdent(),
				_version,
				op_result,
				op_mode,
				params,
				exceptions,
				contexts );
  }
}

void
IDLParser::collectAttribute( CORBA::Container_ptr con, ParseNode *node,
			     CORBA::AttributeMode mode )
{
  assert( node->getType() == t_attribute ||
	  node->getType() == t_readonly_attribute );
  
  // con must be an InterfaceDef or a ValueDef
  CORBA::InterfaceDef_var interf = CORBA::InterfaceDef::_narrow( con );
  CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( con );
  assert( !CORBA::is_nil( interf ) || !CORBA::is_nil( val ) );
  
  CORBA::IDLType_var idl_type;
  
  idl_type = collectType( con, node->getBranch( 1 ) );

  ParseNode * p = node->getBranch (2);
  ParseNode * exceps = p->getBranch (3);

  do {
    assert( p->getType() == simple_declarators );
    char *name = p->getBranch( 1 )->getIdent();
    CORBA::String_var repo_id = gen_repo_id( p->getBranch( 1 ) );
    CORBA::AttributeDef_var attr;

    if (!CORBA::is_nil (interf)) {
      attr = interf->create_attribute( repo_id,
				       name,
				       _version,
				       idl_type,
				       mode );
    }
    else {
      attr = val->create_attribute( repo_id,
				    name,
				    _version,
				    idl_type,
				    mode );
    }

    if (exceps) {
      CORBA::ExceptionDefSeq get_exceps, set_exceps;
      collectRaisesExpr (con, exceps->getBranch (1), get_exceps);
      collectRaisesExpr (con, exceps->getBranch (2), set_exceps);

      CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (attr);
      assert (!CORBA::is_nil (ead));

      ead->get_exceptions (get_exceps);
      ead->set_exceptions (set_exceps);
    }

    p = p->getBranch( 2 );
  } while( p != NULL );
}

void
IDLParser::collectValueForwardDcl (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node->getType() == value_forward_dcl ||
	  node->getType() == abstract_value_forward_dcl ||
	  node->getType() == event_forward_dcl ||
	  node->getType() == abstract_event_forward_dcl);
  
  CORBA::String_var scoped_name = gen_scoped_name( node );
  CORBA::String_var repo_id = gen_repo_id( node );
  CORBA::Boolean is_abstract;

  if (node->getType() == value_forward_dcl ||
      node->getType() == event_forward_dcl) {
    is_abstract = 0;
  }
  else {
    is_abstract = 1;
  }

  if( is_iface_defined( scoped_name ) )
    return;

  if( !_db->is_in_forward_dcl( scoped_name ) ) {
    CORBA::ValueDef_var in;
 
    if( node->is_toplevel() || _feed_included_defs ) {
      CORBA::ValueDefSeq abstract_base_values; // Leave empty for now
      CORBA::InterfaceDefSeq supported_interfaces; // dito
      CORBA::InitializerSeq initializers;

      if (node->getType() == value_forward_dcl ||
	  node->getType() == abstract_value_forward_dcl) {
	in = con->create_value( repo_id,
				node->getIdent(),
				_version,
				FALSE,
				is_abstract,
				CORBA::ValueDef::_nil(),
				FALSE,
				abstract_base_values,
				supported_interfaces,
				initializers );
      }
      else {
#ifdef USE_CCM
	CORBA::ComponentIR::Container_var ccm_con =
	  CORBA::ComponentIR::Container::_narrow (con);
	CORBA::ExtInitializerSeq ext_initializers;
	assert (!CORBA::is_nil (ccm_con));

	in = ccm_con->create_event( repo_id,
				    node->getIdent(),
				    _version,
				    FALSE,
				    is_abstract,
				    CORBA::ValueDef::_nil(),
				    FALSE,
				    abstract_base_values,
				    supported_interfaces,
				    ext_initializers );
#endif
      }
    } else {
      /*
       * Since we don't feed included defs, we only have to make
       * sure that no other IDL type with the same identifier
       * exists (the only exception being that there is another
       * interface with the same name).
       */

      CORBA::ContainedSeq_var cs = con->lookup_name (node->getIdent(), 1,
						     CORBA::dk_all, 1);
      assert (cs->length() <= 1);

      if (cs->length() == 1) {
	in = CORBA::ValueDef::_narrow( cs[(CORBA::ULong)0] );
	if( CORBA::is_nil( in ) ) {
	  cerr << IDLError( 23, node, node->getIdent() ) << endl;
	  exit( 1 );
	}
      }
      else {
	return;
      }
    }
    
    _db->add_forward_dcl( scoped_name );
  }
}

void
IDLParser::collectValueBoxDcl( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == value_box_dcl );
  CORBA::IDLType_var orig = collectType( con, node->getBranch( 1 ) );

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::String_var repo_id = gen_repo_id( node );
    CORBA::ValueBoxDef_var v = con->create_value_box ( repo_id,
						       node->getIdent(),
						       _version,
						       orig );
  }
}

void
IDLParser::collectValueDcl( CORBA::Container_ptr con, ParseNode *node )
{
  assert( node->getType() == value_dcl ||
	  node->getType() == abstract_value_dcl ||
	  node->getType() == event_dcl ||
	  node->getType() == abstract_event_dcl );

  ParseNode * header = node->getBranch (1);

  assert (header->getType() == value_header ||
	  header->getType() == custom_value_header);

  CORBA::String_var sname   = gen_scoped_name( header );
  CORBA::String_var repo_id = gen_repo_id( header );

  string name;
  name = header->getIdent();
  _db->enter_scope (name);

  CORBA::ValueDef_var val;
  CORBA::InterfaceDefSeq supported_interfaces;
  CORBA::ValueDef_var base_value;
  CORBA::ValueDefSeq abstract_base_values;
  CORBA::InitializerSeq initializers;
  CORBA::Boolean is_custom;
  CORBA::Boolean is_truncatable;
  CORBA::Boolean is_abstract;

  if (header->getType() == custom_value_header) {
    is_custom = TRUE;
  }
  else {
    is_custom = FALSE;
  }

  if (node->getType() == value_dcl || node->getType() == event_dcl) {
    is_abstract = 0;
  }
  else {
    is_abstract = 1;
  }

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if (cs->length() > 0) {
      sname = cs[(CORBA::ULong)0]->absolute_name();
      if( !_db->is_in_forward_dcl( sname ) ) {
	cerr << IDLError( 12, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    if ( _db->is_in_forward_dcl( sname ) ) {
	/* This value was forward declared, we should be able to
	   find it, already created in the repository */
      CORBA::Contained_var c = _repo->lookup( sname );
	if ( CORBA::is_nil( c ) ) {
	    cerr << IDLError( 2, header, sname.in() ) << endl;
	    exit( 1 );
	}
	val = CORBA::ValueDef::_narrow( c );
	if ( CORBA::is_nil( val ) ) {
	    cerr << IDLError( 34, header, header->getIdent() ) << endl;
	    exit( 1 );
	}
	if ( strcmp( val->id(), repo_id ) != 0 ) {
	    cerr << IDLError( 35, header, repo_id.in() ) << endl;
	    exit( 1 );
	}
    } else {
        if (node->getType() == value_dcl ||
	    node->getType() == abstract_value_dcl) {
	  val = con->create_value( repo_id,
				   header->getIdent(),
				   _version,
				   FALSE,
				   is_abstract,
				   CORBA::ValueDef::_nil(),
				   FALSE,
				   abstract_base_values,
				   supported_interfaces,
				   initializers );
	}
	else {
#ifdef USE_CCM
	  CORBA::ComponentIR::Container_var ccm_con =
	    CORBA::ComponentIR::Container::_narrow (con);
	  CORBA::ExtInitializerSeq ext_initializers;
	  assert (!CORBA::is_nil (ccm_con));

	  val = ccm_con->create_event( repo_id,
				       header->getIdent(),
				       _version,
				       FALSE,
				       is_abstract,
				       CORBA::ValueDef::_nil(),
				       FALSE,
				       abstract_base_values,
				       supported_interfaces,
				       ext_initializers );
#endif
	}
    }
  } else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * interface with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      val = CORBA::ValueDef::_narrow( cs[(CORBA::ULong)0] );
      if( CORBA::is_nil( val ) ) {
	cerr << IDLError( 23, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }
    else {
      _db->leave_scope();
      return;
    }
  }

  CORBA::String_var id = val->absolute_name();
  _db->remove_forward_dcl( id );
  _seen_iface.insert( id.in() );

  header = header->getBranch (1);
  // first element may contain supported interfaces
  if (header != NULL) {
    assert (header->getType() == value_inheritance_spec);
    ParseNode * sins = header->getBranch (4);
    bool have_concrete_if = false;

    while (sins != NULL) {
      assert (sins->getType() == scoped_names);
      ParseNode * sin = sins->getBranch (1);
      CORBA::IDLType_var t = collectType (con, sin);
      CORBA::InterfaceDef_ptr sif = CORBA::InterfaceDef::_narrow (t);

      if (sif->def_kind() == CORBA::dk_Interface) {
	if (have_concrete_if) {
	  cerr << IDLError( 31 ) << endl;
	  exit( 1 );
	}
	have_concrete_if = true;
      }

      CORBA::String_var bname = sif->absolute_name();
      if( _db->is_in_forward_dcl( bname ) ) {
	cerr << IDLError( 9, sin, sin->getIdent() ) << endl;
	exit( 1 );
      }

      CORBA::ULong i = supported_interfaces.length();
      supported_interfaces.length (i+1);
      supported_interfaces[i] = sif;
      sins = sins->getBranch (2);
    }
  }

  // first element may be a concrete (non-abstract) value

  if (header != NULL) {
    ParseNode * cvt;
    if ((cvt = header->getBranch (1)) == NULL) {
      if ((cvt = header->getBranch (2)) != NULL) {
	is_truncatable = TRUE;
      }
      else {
	is_truncatable = FALSE;
      }
    }
    else {
      is_truncatable = FALSE;
    }
    
    if (cvt != NULL) {
      if (is_custom && is_truncatable) {
	cerr << IDLError(26, cvt, name.c_str()) << endl;
	exit (1);
      }
    
      if (is_abstract && is_truncatable) {
	cerr << IDLError(27, cvt, name.c_str()) << endl;
	exit (1);
      }

      if (cvt->getType() == scoped_names) {
	cvt = cvt->getBranch (1);
      }
    
      assert (cvt->getType() == scoped_name ||
	      cvt->getType() == t_valuebase);
    
      if (cvt->getType() == scoped_name) {
	CORBA::IDLType_var t = collectType (con, cvt);
	base_value = CORBA::ValueDef::_narrow (t);
	if (CORBA::is_nil (base_value)) {
	  cerr << IDLError (8, cvt, cvt->getIdent()) << endl;
	  exit (1);
	}
	CORBA::String_var bname = base_value->absolute_name();
	if ( _db->is_in_forward_dcl(bname)) {
	  cerr << IDLError (9, cvt, cvt->getIdent()) << endl;
	  exit (1);
	}
	if (base_value->_is_equivalent (val)) {
	  cerr << IDLError (25, cvt, cvt->getIdent()) << endl;
	  exit (1);
	}
	if (is_abstract && !base_value->is_abstract()) {
	  cerr << IDLError (28, cvt, name.c_str(), cvt->getIdent()) << endl;
	  exit (1);
	}
	if (is_truncatable && base_value->is_abstract()) {
	  cerr << IDLError (29, cvt, name.c_str(), cvt->getIdent()) << endl;
	  exit (1);
	}
	if (base_value->is_abstract()) {
	  // not a concrete base
	  abstract_base_values.length (1);
	  abstract_base_values[(CORBA::ULong)0] =
	    CORBA::ValueDef::_duplicate (base_value);
	  base_value = CORBA::ValueDef::_nil ();
	}
      }
      else {
	// inherits from ValueBase
	base_value = CORBA::ValueDef::_nil ();
      }
    }
    header = header->getBranch (3);
  }
  else {
    // does not inherit anything
    base_value = CORBA::ValueDef::_nil ();
    is_truncatable = FALSE;
  }

  // scan abstract base values
  while (header != NULL) {
    assert (header->getType() == scoped_names);
    ParseNode * node_base = header->getBranch (1);
    CORBA::IDLType_var t = collectType (con, node_base);
    CORBA::ValueDef_ptr base =
      CORBA::ValueDef::_narrow (t);

    if (CORBA::is_nil (base)) {
      cerr << IDLError( 8, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    CORBA::String_var bname = base->absolute_name();
    if ( _db->is_in_forward_dcl (bname)) {
      cerr << IDLError( 9, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    if (val->_is_equivalent (base)) {
      cerr << IDLError( 25, node_base, node_base->getIdent() ) << endl;
      exit( 1 );
    }
    CORBA::ULong i = abstract_base_values.length();
    abstract_base_values.length (i+1);
    abstract_base_values[i] = base;
    header = header->getBranch (2);
  }

  /*
   * Must set base values and supported interfaces before scanning
   * the valuetype's contents, so that inherited symbols are found.
   */

  if (node->is_toplevel() || _feed_included_defs) {
    val->supported_interfaces (supported_interfaces);
    val->base_value (base_value);
    val->abstract_base_values (abstract_base_values);
    val->is_abstract (is_abstract);
    val->is_custom (is_custom);
    val->is_truncatable (is_truncatable);
  }

  /*
   * Done with header
   *
   * We can't call scan() recursively, because we must collect
   * initializers separately.
   */

  if (node->is_toplevel() || _feed_included_defs) {
    // scan members
    ParseNode * elements = node->getBranch (2);
    while (elements != NULL) {
      assert (elements->getType() == value_elements ||
	      elements->getType() == exports);

      ParseNode * element = elements->getBranch (1);

      if (element->getType() == init_dcl) {
	CORBA::Initializer initializer;
	CORBA::ULong icount;

	initializer.name = (const char *) element->getIdent();
	ParseNode * idcl = element->getBranch (1);

	while (idcl != NULL) {
	  assert (idcl->getType() == init_param_decls);
	  ParseNode * ipd = idcl->getBranch (1);
	  CORBA::IDLType_var t = collectType (val, ipd->getBranch (1));
	  assert (ipd->getBranch(2)->getType() == simple_declarator);
	  const char * name = ipd->getBranch(2)->getIdent();
	  icount = initializer.members.length();
	  initializer.members.length (icount+1);
	  CORBA::StructMember inelem;
	  inelem.name = CORBA::string_dup (name);
	  inelem.type = CORBA::_tc_void;
	  inelem.type_def = CORBA::IDLType::_duplicate (t);
	  initializer.members[icount] = inelem;
	  idcl = idcl->getBranch (2);
	}
	icount = initializers.length();
	initializers.length (icount + 1);
	initializers[icount] = initializer;
      }
      else {
	scan (val, element);
      }

      elements = elements->getBranch (2);
    }

    // set initializers
    val->initializers (initializers);
  }

  _db->leave_scope();

  // regenerate repo id in case it was redefined with a typeprefix
  header = node->getBranch (1);
  repo_id = gen_repo_id (header);
  val->id (repo_id);
}

void
IDLParser::collectStateMember (CORBA::Container_ptr con, ParseNode *node,
			       CORBA::Visibility vis)
{
  assert (node->getType() == public_state_member ||
	  node->getType() == private_state_member);

  // con must be a ValueDef
  CORBA::ValueDef_var val = CORBA::ValueDef::_narrow( con );
  assert( !CORBA::is_nil( val ) );

  CORBA::IDLType_var idl_type = collectType (con, node->getBranch (1));
  node = node->getBranch (2);

  while (node != NULL) {
    assert (node->getType() == declarators);
    ParseNode * dcl = node->getBranch (1);
    if (dcl->getType() == simple_declarator) {
      CORBA::String_var repo_id = gen_repo_id (dcl);
      CORBA::ValueMemberDef_var vm =
	val->create_value_member (repo_id,
				  dcl->getIdent(),
				  _version,
				  idl_type,
				  vis);
    }
    else if (dcl->getType() == array_declarator) {
      CORBA::ArrayDef_var arr = collectArray (con, idl_type,
					      dcl->getBranch(1));
      CORBA::String_var repo_id = gen_repo_id (dcl);
      CORBA::ValueMemberDef_var vm =
	val->create_value_member (repo_id,
				  dcl->getIdent(),
				  _version,
				  arr,
				  vis);
    }
    else {
      assert (0);
    }
    node = node->getBranch (2);
  }
}

CORBA::EnumDef_ptr
IDLParser::collectEnum( CORBA::Container_ptr con, ParseNode *p )
{
  assert( p->getType() == enum_type );

  CORBA::String_var repo_id = gen_repo_id( p );

  CORBA::EnumMemberSeq members;
  CORBA::EnumDef_ptr enum_def = con->create_enum( repo_id, p->getIdent(), 
						  _version, members );
  
  collectEnumMembers( con, enum_def, p->getBranch( 1 ) );

  current_parse_node( p );

  return enum_def;
}

void IDLParser::collectEnumMembers( CORBA::Container_ptr con,
				    CORBA::EnumDef_ptr e, ParseNode *n )
{
  
  switch( n->getType() ) {
  case t_pragma: 
  case t_typeid:
  case t_typeprefix:
    collectPragma( con, n );
    break;
  case enumerator: {
    CORBA::EnumMemberSeq_var m = e->members ();
    CORBA::ULong i = m->length ();
    current_parse_node (n);
    m->length (i+1);
    m[ i ] = CORBA::string_dup( n->getIdent() );
    e->members (m.in());
    break;
  }
  case enumerators:
    collectEnumMembers( con, e, n->getBranch( 1 ) );
    collectEnumMembers( con, e, n->getBranch( 2 ) );
    break;
  default:
    assert( 0 );
  }
}

CORBA::IDLType_ptr IDLParser::collectType( CORBA::Container_ptr con,
					   ParseNode *p )
{
  switch( p->getType() ) {
  case t_void:
    return _repo->get_primitive( CORBA::pk_void );
  case t_float:
    return _repo->get_primitive( CORBA::pk_float );
  case t_double:
    return _repo->get_primitive( CORBA::pk_double );
  case t_long:
    return _repo->get_primitive( CORBA::pk_long );
  case t_short:
    return _repo->get_primitive( CORBA::pk_short );
  case t_unsigned_long:
    return _repo->get_primitive( CORBA::pk_ulong );
  case t_unsigned_short:
    return _repo->get_primitive( CORBA::pk_ushort );
  case t_char:
    return _repo->get_primitive( CORBA::pk_char );
  case t_boolean:
    return _repo->get_primitive( CORBA::pk_boolean );
  case t_octet:
    return _repo->get_primitive( CORBA::pk_octet );
  case t_any:
    return _repo->get_primitive( CORBA::pk_any );
  case t_object:
    return _repo->get_primitive( CORBA::pk_objref );
  case t_longlong:
    return _repo->get_primitive( CORBA::pk_longlong );
  case t_unsigned_longlong:
    return _repo->get_primitive( CORBA::pk_ulonglong );
  case t_long_double:
    return _repo->get_primitive( CORBA::pk_longdouble );
  case t_wide_char:
    return _repo->get_primitive( CORBA::pk_wchar );
  case t_valuebase:
    return _repo->get_primitive( CORBA::pk_value_base );
  case t_string:
    if( p->getBranch( 1 ) == NULL )
      // Unbounded string
      return _repo->get_primitive( CORBA::pk_string );
    else {
      // Bounded string
      Const c = collectConst( con, p->getBranch( 1 ) );
      CORBA::ULong bound = c.get_int();
      CORBA::StringDef_ptr str = _repo->create_string( bound );
      _db->register_anonym_type( p, str );
      return str;
    }
    break;
  case t_wide_string:
    if( p->getBranch( 1 ) == NULL )
      // Unbounded wstring
      return _repo->get_primitive( CORBA::pk_wstring );
    else {
      // Bounded wstring
      Const c = collectConst( con, p->getBranch( 1 ) );
      CORBA::ULong bound = c.get_int();
      CORBA::WstringDef_ptr wstr = _repo->create_wstring( bound );
      _db->register_anonym_type( p, wstr );
      return wstr;
    }
    break;
  case t_sequence:
    {
      CORBA::IDLType_var element_type = collectType( con, p->getBranch( 1 ) );
      CORBA::ULong bound = 0;
      if( p->getBranch( 2 ) != NULL ) {
	// Bounded sequence
	Const c = collectConst( con, p->getBranch( 2 ) );
	bound = c.get_int();
      }
      CORBA::SequenceDef_ptr seq = _repo->create_sequence( bound, element_type );
      _db->register_anonym_type( p, seq );
      return seq;
    }
  case fixed_pt_type:
    {
      CORBA::FixedDef_ptr f;
      /*
       * A fixed type dcl without digits and scale can only occur
       * in const dcl. This is handled in IDLParser::collectConstFixedDcl()
       */
      assert( p->getBranch( 1 ) != NULL );
      Const c = collectConst( con, p->getBranch( 1 ) );
      CORBA::ULong digits = c.get_int();
      f = _repo->create_fixed( digits, p->getIntegerLiteral() );
      _db->register_anonym_type( p, f );
      return f;
    }
  case scoped_name:
    {
      CORBA::Contained_var c;
      c = con->lookup ( collectScopedName( p ) );
      if( CORBA::is_nil( c ) ) {
	if( is_TypeCode( p ) )
	  return _repo->get_primitive( CORBA::pk_TypeCode );
	if( is_Principal( p ) )
	  return _repo->get_primitive( CORBA::pk_Principal );
	cerr << IDLError( 2, p, p->getIdent() ) << endl;
	exit( 1 );
      }
      CORBA::IDLType_ptr t = CORBA::IDLType::_narrow( c );
      if( CORBA::is_nil( t ) ) {
	cerr << IDLError( 7, p, p->getIdent() ) << endl;
	exit( 1 );
      }
      return t;
    }
  case struct_type:
    {
      return collectStruct( con, p );
    }
  case union_type:
    {
      return collectUnion( con, p );
    }
  case enum_type:
    {
      return collectEnum( con, p );
    }

  default:
    p->print();
    assert( 0 );
  }
  return 0;
}

CORBA::ScopedName IDLParser::collectScopedName( ParseNode *n )
{
  assert( n->getType() == scoped_name );
  
  current_parse_node( n );
  return n->getIdent();
}

bool IDLParser::is_TypeCode( ParseNode *n )
{
  assert( n->getType() == scoped_name );
  return strcmp( n->getIdent(), "TypeCode" ) == 0 ||
    strcmp( n->getIdent(), "CORBA::TypeCode" ) == 0;
}

bool IDLParser::is_Principal( ParseNode *n )
{
  assert( n->getType() == scoped_name );
  return strcmp( n->getIdent(), "CORBA::Principal" ) == 0;
}

Const IDLParser::collectConst( CORBA::Container_ptr con, ParseNode *p )
{
  if( p == NULL )
    return Const();

  Const c1( collectConst( con, p->getBranch( 1 ) ) );
  Const c2( collectConst( con, p->getBranch( 2 ) ) );
  
  switch( p->getType() ) {
  case t_floating_pt_literal:
    return Const( CORBA::_tc_longdouble, p->getFloatingPtLiteral(), p );
  case t_fixed_pt_literal:
    return Const( CORBA::_tc_longdouble, p->getFixedPtLiteral(), p );
  case t_integer_literal:
    return Const( CORBA::_tc_longlong, p->getIntegerLiteral(), p );
  case t_string_literal:
    return Const( CORBA::_tc_string, p->getStringLiteral(), p );
  case t_wstring_literal:
    return Const( CORBA::_tc_wstring, p->getWStringLiteral(), p );
  case t_character_literal:
    return Const( CORBA::_tc_char,
		  CORBA::Any::from_char ( p->getCharacterLiteral() ), p );
  case t_wcharacter_literal:
    return Const( CORBA::_tc_wchar,
		  CORBA::Any::from_wchar ( p->getWCharacterLiteral() ), p );
  case t_boolean_literal:
    return Const( CORBA::_tc_boolean,
		  CORBA::Any::from_boolean ( p->getBooleanLiteral() ), p );
  case t_vertical_line:
    return c1 | c2;
  case t_circumflex:
    return c1 ^ c2;
  case t_ampersand:
    return c1 & c2;
  case t_shiftright:
    return c1 >> c2;
  case t_shiftleft:
    return c1 << c2;
  case t_plus_sign:
    if( p->getBranch( 2 ) == NULL )
      return c1;
    else
      return c1 + c2;
  case t_minus_sign:
    if (p->getBranch (2) == NULL) {
      return -c1;
    }
    else {
      return c1 - c2;
    }
  case t_asterik:
    return c1 * c2;
  case t_solidus:
    return c1 / c2;
  case t_percent_sign:
    return c1 % c2;
  case t_tilde:
    return ~c1;
  case scoped_name:
    {
      CORBA::Contained_var contained;
      contained = con->lookup ( collectScopedName( p ) );
      if( CORBA::is_nil( contained ) ) {
	cerr << IDLError( 2, p, p->getIdent() ) << endl;
	exit( 1 );
      }
      CORBA::ConstantDef_var c = CORBA::ConstantDef::_narrow( contained );
      if( CORBA::is_nil( c ) ) {
	cerr << IDLError( 24, p, p->getIdent() ) << endl;
	exit( 1 );
      }
      return Const( c->value(), p );
    }
  default:
    p->print();
    assert( 0 );
  }
  return Const();
}

#ifdef USE_CCM

void
IDLParser::collectComponent (CORBA::Container_ptr con, ParseNode * node)
{
  CORBA::ComponentIR::ComponentDef_var comp;

  ParseNode *header = node->getBranch( 1 );

  CORBA::String_var scoped_name = gen_scoped_name( header );
  CORBA::String_var repo_id = gen_repo_id( header );

  string name;
  name = header->getIdent();
  _db->enter_scope( name );

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if( cs->length() == 1 ) {
      scoped_name = cs[(CORBA::ULong)0]->absolute_name();
      if( !_db->is_in_forward_dcl( scoped_name ) ) {
	cerr << IDLError( 12, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    if ( _db->is_in_forward_dcl( scoped_name ) ) {
	/* This interface was forward declared, we should be able to
	   find it, already created in the repository */
        CORBA::Contained_var c = _repo->lookup( scoped_name );
	if ( CORBA::is_nil( c ) ) {
	    cerr << IDLError( 2, header, scoped_name.in() ) << endl;
	    exit( 1 );
	}
	comp = CORBA::ComponentIR::ComponentDef::_narrow( c );
	if ( CORBA::is_nil( comp ) ) {
	    cerr << IDLError( 23, header, header->getIdent() ) << endl;
	    exit( 1 );
	}
    } else {
      CORBA::ComponentIR::Container_var ccv =
	CORBA::ComponentIR::Container::_narrow (con);
      assert (!CORBA::is_nil (ccv));
      comp = ccv->create_component (repo_id,
				    header->getIdent(),
				    _version,
				    CORBA::ComponentIR::ComponentDef::_nil(),
				    CORBA::InterfaceDefSeq (0));
    }
  } else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * component with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      comp = CORBA::ComponentIR::ComponentDef::_narrow(cs[(CORBA::ULong)0]);
      if( CORBA::is_nil( comp ) ) {
	cerr << IDLError( 23, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    _db->leave_scope();
    return;
  }

  scoped_name = comp->absolute_name();
  _db->remove_forward_dcl( scoped_name );
  _seen_iface.insert( scoped_name.in() );

  if (!node->is_toplevel() && !_feed_included_defs) {
    _db->leave_scope();
    return;
  }

  if (header->getBranch (1)) {
    ParseNode * node_base = header->getBranch (1);
    CORBA::IDLType_var t = collectType (con, node_base);
    CORBA::ComponentIR::ComponentDef_var base_component =
      CORBA::ComponentIR::ComponentDef::_narrow (t);
    if (CORBA::is_nil (base_component)) {
      cerr << IDLError (8, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    scoped_name = base_component->absolute_name();
    if (_db->is_in_forward_dcl (scoped_name)) {
      cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    comp->base_component (base_component);
  }

  if (header->getBranch (2)) {
    CORBA::InterfaceDefSeq supported_interfaces;
    ParseNode * supports = header->getBranch (2);
    CORBA::ULong length = 0;

    while (supports != 0) {
      ParseNode* node_base = supports->getBranch( 1 );
      CORBA::IDLType_var t = collectType( con, node_base );
      CORBA::InterfaceDef_var supported = CORBA::InterfaceDef::_narrow (t);
      if (CORBA::is_nil (supported)) {
	cerr << IDLError( 8, node_base, node_base->getIdent() ) << endl;
	exit (1);
      }
      scoped_name = supported->absolute_name();
      if (_db->is_in_forward_dcl (scoped_name)) {
	cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
	exit (1);
      }
      supported_interfaces.length (length+1);
      supported_interfaces[length++] = supported;
      supports = supports->getBranch (2);
    }

    comp->supported_interfaces (supported_interfaces);
  }

  scan (comp, node->getBranch (2));
  _db->leave_scope();

  // regenerate repo id in case it was redefined with a typeprefix
  repo_id = gen_repo_id (header);
  comp->id (repo_id);

  /*
   * make implicit CCM_<name> interface, so that user code can derive
   * from it. We don't care about its contents; the interface will be
   * re-generated by the IDL 3 to IDL 2 stage.
   */

  make_ccm_interface (comp);
}

void
IDLParser::collectProvides (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node->getType() == provides_dcl);
  CORBA::ComponentIR::ComponentDef_var comp =
    CORBA::ComponentIR::ComponentDef::_narrow (con);
  assert (!CORBA::is_nil (comp));

  CORBA::InterfaceDef_var provided;

  if (node->getBranch(1)->getType() == scoped_name) {
    CORBA::Contained_var c =
      con->lookup (collectScopedName(node->getBranch(1)));
    provided = CORBA::InterfaceDef::_narrow (c);
    if (CORBA::is_nil (provided)) {
      cerr << IDLError (2, node->getBranch(1), node->getBranch(1)->getIdent())
	   << endl;
      exit (1);
    }
  }
  else {
    provided = CORBA::InterfaceDef::_nil ();
  }

  char * name = node->getIdent ();
  CORBA::String_var repo_id = gen_repo_id (node);

  CORBA::ComponentIR::ProvidesDef_var p =
    comp->create_provides (repo_id, name, _version, provided);
}

void
IDLParser::collectUses (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node->getType() == uses_dcl ||
	  node->getType() == uses_multiple_dcl);
  CORBA::ComponentIR::ComponentDef_var comp =
    CORBA::ComponentIR::ComponentDef::_narrow (con);
  assert (!CORBA::is_nil (comp));

  CORBA::InterfaceDef_var used;

  if (node->getBranch(1)->getType() == scoped_name) {
    CORBA::Contained_var c =
      con->lookup (collectScopedName(node->getBranch(1)));
    used = CORBA::InterfaceDef::_narrow (c);
    if (CORBA::is_nil (used)) {
      cerr << IDLError (2, node->getBranch(1), node->getBranch(1)->getIdent())
	   << endl;
      exit (1);
    }
  }
  else {
    used = CORBA::InterfaceDef::_nil ();
  }

  char * name = node->getIdent ();
  CORBA::String_var repo_id = gen_repo_id (node);

  CORBA::ComponentIR::UsesDef_var p =
    comp->create_uses (repo_id, name, _version, used,
		       (node->getType() == uses_dcl) ? 0 : 1);
}

void
IDLParser::collectEventPort (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node->getType() == emits_dcl ||
	  node->getType() == publishes_dcl ||
	  node->getType() == consumes_dcl);

  CORBA::ComponentIR::ComponentDef_var comp =
    CORBA::ComponentIR::ComponentDef::_narrow (con);
  assert (!CORBA::is_nil (comp));

  CORBA::Contained_var c =
    con->lookup (collectScopedName(node->getBranch(1)));
  CORBA::ComponentIR::EventDef_var event =
    CORBA::ComponentIR::EventDef::_narrow (c);

  if (CORBA::is_nil (event)) {
    cerr << IDLError (2, node->getBranch(1), node->getBranch(1)->getIdent())
	 << endl;
    exit (1);
  }

  char * name = node->getIdent ();
  CORBA::String_var repo_id = gen_repo_id (node);

  CORBA::ComponentIR::EventPortDef_var e;

  switch (node->getType()) {
  case emits_dcl:
    e = comp->create_emits (repo_id, name, _version, event);
    break;
  case publishes_dcl:
    e = comp->create_publishes (repo_id, name, _version, event);
    break;
  case consumes_dcl:
    e = comp->create_consumes (repo_id, name, _version, event);
    break;
  default:
    assert (0);
  }
}

void
IDLParser::collectHome (CORBA::Container_ptr con, ParseNode * node)
{
  CORBA::ComponentIR::HomeDef_var home;

  ParseNode *header = node->getBranch( 1 );

  CORBA::String_var scoped_name = gen_scoped_name( header );
  CORBA::String_var repo_id = gen_repo_id( header );

  string name;
  name = header->getIdent();
  _db->enter_scope( name );

  if( node->is_toplevel() || _feed_included_defs ) {
    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);
    assert (cs->length() <= 1);

    if( cs->length() == 1 ) {
      scoped_name = cs[(CORBA::ULong)0]->absolute_name();
      if( !_db->is_in_forward_dcl( scoped_name ) ) {
	cerr << IDLError( 12, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    if ( _db->is_in_forward_dcl( scoped_name ) ) {
	/* This interface was forward declared, we should be able to
	   find it, already created in the repository */
        CORBA::Contained_var c = _repo->lookup( scoped_name );
	if ( CORBA::is_nil( c ) ) {
	    cerr << IDLError( 2, header, scoped_name.in() ) << endl;
	    exit( 1 );
	}
	home = CORBA::ComponentIR::HomeDef::_narrow( c );
	if ( CORBA::is_nil( home ) ) {
	    cerr << IDLError( 23, header, header->getIdent() ) << endl;
	    exit( 1 );
	}
    } else {
      CORBA::ComponentIR::Container_var ccv =
	CORBA::ComponentIR::Container::_narrow (con);
      CORBA::InterfaceDefSeq supported_interfaces;
      assert (!CORBA::is_nil (ccv));
      home = ccv->create_home (repo_id,
			       header->getIdent(),
			       _version,
			       CORBA::ComponentIR::HomeDef::_nil(),
			       CORBA::ComponentIR::ComponentDef::_nil(),
			       CORBA::InterfaceDefSeq (0),
			       CORBA::ValueDef::_nil());
    }
  } else {
    /*
     * Since we don't feed included defs, we only have to make
     * sure that no other IDL type with the same identifier
     * exists (the only exception being that there is another
     * component with the same name).
     */

    CORBA::ContainedSeq_var cs = con->lookup_name (header->getIdent(), 1,
						   CORBA::dk_all, 1);

    if (cs->length() == 1) {
      home = CORBA::ComponentIR::HomeDef::_narrow(cs[(CORBA::ULong)0]);
      if( CORBA::is_nil( home ) ) {
	cerr << IDLError( 23, header, header->getIdent() ) << endl;
	exit( 1 );
      }
    }

    _db->leave_scope();
    return;
  }

  scoped_name = home->absolute_name();
  _db->remove_forward_dcl( scoped_name );
  _seen_iface.insert( scoped_name.in() );

  if (!node->is_toplevel() && !_feed_included_defs) {
    _db->leave_scope();
    return;
  }

  if (header->getBranch (1)) {
    ParseNode * node_base = header->getBranch (1);
    CORBA::IDLType_var t = collectType (con, node_base);
    CORBA::ComponentIR::HomeDef_var base_home =
      CORBA::ComponentIR::HomeDef::_narrow (t);
    if (CORBA::is_nil (base_home)) {
      cerr << IDLError (8, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    scoped_name = base_home->absolute_name();
    if (_db->is_in_forward_dcl (scoped_name)) {
      cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    home->base_home (base_home);
  }

  if (header->getBranch (2)) {
    ParseNode * node_base = header->getBranch (2);
    CORBA::IDLType_var t = collectType (con, node_base);
    CORBA::ComponentIR::ComponentDef_var managed_component =
      CORBA::ComponentIR::ComponentDef::_narrow (t);
    if (CORBA::is_nil (managed_component)) {
      cerr << IDLError (8, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    scoped_name = managed_component->absolute_name();
    if (_db->is_in_forward_dcl (scoped_name)) {
      cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    home->managed_component (managed_component);
  }

  if (header->getBranch (3)) {
    ParseNode * node_base = header->getBranch (3);
    CORBA::IDLType_var t = collectType (con, node_base);
    CORBA::ValueDef_var primary_key = CORBA::ValueDef::_narrow (t);
    if (CORBA::is_nil (primary_key)) {
      cerr << IDLError (8, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    scoped_name = primary_key->absolute_name();
    if (_db->is_in_forward_dcl (scoped_name)) {
      cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
      exit (1);
    }
    home->primary_key (primary_key);
  }

  if (header->getBranch (4)) {
    CORBA::InterfaceDefSeq supported_interfaces;
    ParseNode * supports = header->getBranch (4);
    CORBA::ULong length = 0;

    while (supports != 0) {
      ParseNode* node_base = supports->getBranch( 1 );
      CORBA::IDLType_var t = collectType( con, node_base );
      CORBA::InterfaceDef_var supported = CORBA::InterfaceDef::_narrow (t);
      if (CORBA::is_nil (supported)) {
	cerr << IDLError( 8, node_base, node_base->getIdent() ) << endl;
	exit (1);
      }
      scoped_name = supported->absolute_name();
      if (_db->is_in_forward_dcl (scoped_name)) {
	cerr << IDLError (9, node_base, node_base->getIdent()) << endl;
	exit (1);
      }
      supported_interfaces.length (length+1);
      supported_interfaces[length++] = supported;
      supports = supports->getBranch (2);
    }
    
    home->supported_interfaces (supported_interfaces);
  }

  scan (home, node->getBranch (2));
  _db->leave_scope();

  // regenerate repo id in case it was redefined with a typeprefix
  repo_id = gen_repo_id (header);
  home->id (repo_id);

  /*
   * make implicit CCM_<name> interface, so that user code can derive
   * from it. We don't care about its contents; the interface will be
   * re-generated by the IDL 3 to IDL 2 stage.
   */

  make_ccm_interface (home);
}

void
IDLParser::collectFactory (CORBA::Container_ptr con, ParseNode *node)
{
  assert (node->getType() == factory_dcl ||
	  node->getType() == finder_dcl);
  CORBA::ComponentIR::HomeDef_var home =
    CORBA::ComponentIR::HomeDef::_narrow (con);
  assert (!CORBA::is_nil (home));

  ParseNode * p = node->getBranch (1);
  CORBA::ParDescriptionSeq params;
  CORBA::ULong len=0;

  while (p != 0) {
    assert (p->getType() == init_param_decls);
    ParseNode * q = p->getBranch (1);
    assert (q->getType() == init_param_decl);

    CORBA::IDLType_var idl_type = collectType (con, q->getBranch(1));
    params.length (len+1);
    params[len].mode = CORBA::PARAM_IN;
    params[len].name = CORBA::string_dup (q->getBranch(2)->getIdent());
    params[len].type = idl_type->type ();
    params[len].type_def = idl_type;
    p = p->getBranch (2);
    len++;
  }

  CORBA::ExceptionDefSeq exceptions;
  if (node->getBranch (2)) {
    collectRaisesExpr (con, node->getBranch (2), exceptions);
  }

  char * name = node->getIdent ();
  CORBA::String_var repo_id = gen_repo_id (node);

  if (node->getType() == factory_dcl) {
    CORBA::ComponentIR::FactoryDef_var f =
      home->create_factory (repo_id, name, _version,
			    params, exceptions);
  }
  else {
    CORBA::ComponentIR::FinderDef_var f =
      home->create_finder (repo_id, name, _version,
			   params, exceptions);
  }
}

/*
 * Make a placeholder for the CCM_<name> equivalent interface that is
 * generated for components and homes, and mark this interface as im-
 * plicit, so that no code gets generated for it.
 *
 * This is done so that the user can write IDL interfaces that inherit
 * from the equivalent interface.
 *
 * The CCM Transformator will then generate the "real" equivalent
 * interfaces and ignore the one that is generated here.
 *
 * We make the implicit interface inherit from the component or home
 * itself. This is ugly, but causes the dependency generator to work
 * correctly.
 */

void
IDLParser::make_ccm_interface (CORBA::InterfaceDef_ptr iface)
{
  CORBA::Container_var co = iface->defined_in ();

  CORBA::String_var id = iface->id ();
  CORBA::String_var name = iface->name ();
  CORBA::String_var version = iface->version ();

  CORBA::InterfaceDef_var dummyres;
  CORBA::InterfaceDefSeq dummybases;
  dummybases.length (1);
  dummybases[0] = CORBA::InterfaceDef::_duplicate (iface);

  string newid (id.in());
  newid.insert (0, "CCM_");

  string newname (name.in());
  newname.insert (0, "CCM_");

  dummyres = co->create_local_interface (newid.c_str(), newname.c_str(),
					 version.in(), dummybases);

  string oldid (id.in());
  if (_db->is_repoid_defined (oldid)) {
    string fname = _db->get_fname_for_repoid (oldid);
    _db->register_repoid (newid, fname);
  }

  CORBA::String_var an = dummyres->absolute_name ();
  _db->add_implicit (an.in());
}

#endif

