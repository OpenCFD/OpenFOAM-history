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

#ifndef __CODEGENMIDL_H__
#define __CODEGENMIDL_H__

#include "codegen.h"
#include "db.h"


class CodeGenMIDL : public CodeGen
{
private:
  
  CORBA::ULong _lun_string_bound; // indica se ho string<N>

  // afxTypeLib
  // used for TypeLib code
  std::vector<CORBA::InterfaceDef_var> _coclasslist;

  // used for the scope of the objects
  char _midl_absolute_name[1000];

  bool                 _emit_repoids;
  CORBA::Container_var _current_scope;
  
  typedef SequenceTmpl<CORBA::TypedefDef_var,MICO_TID_DEF> TypedefDefSeq;
  
  void enter_scope( CORBA::Container_ptr c );
  
  void emit_repoid( CORBA::Contained_ptr c );
  void emitIDL();
  void emitIDLInterface( CORBA::InterfaceDef_ptr in );
  void emitIDLStruct( CORBA::StructDef_ptr s, bool emit_semicolon = true );
  void emitIDLUnion( CORBA::UnionDef_ptr u, bool emit_semicolon = true );
  void emitIDLConstant( CORBA::ConstantDef_ptr co );
  void emitIDLException( CORBA::ExceptionDef_ptr e );
  void emitIDLEnum( CORBA::EnumDef_ptr e, bool emit_semicolon = true );
  void emitIDLAlias( CORBA::AliasDef_ptr a );
  void emitIDLNative( CORBA::NativeDef_ptr n );
  void emitIDLValue( CORBA::ValueDef_ptr v );
  void emitIDLValueBox( CORBA::ValueBoxDef_ptr v );

  void emitAttribute( CORBA::AttributeDef_ptr attr );
  void emitOperation( CORBA::OperationDef_ptr op );
  void emitValueMember( CORBA::ValueMemberDef_ptr vmd );
  void emitPrototypes( CORBA::Container_ptr in );
  void emitForwardDcl( CORBA::Container_ptr in );
  void emitLocalDecls( CORBA::Container_ptr in );
  void emitLocal( CORBA::Contained_ptr in, bool emit_semicolon = true );

  void emit_idl_type_name( CORBA::IDLType_ptr t );
  void emit_sequence( CORBA::IDLType_ptr t );
  void emit_idl_type( CORBA::IDLType_ptr t );
  void emit_array( CORBA::IDLType_ptr t );
  void emit_array_suffix( CORBA::IDLType_ptr t );
  bool emit_base_type( CORBA::IDLType_ptr t );

  bool Is_Alias_Interface(CORBA::IDLType_ptr t);
  bool Is_Interface(CORBA::IDLType_ptr t);
  void emitInterfaceHeader();
  void emitHeader();
  void emitTypeLib(std::string &fnbase);
  const char* midl_absolute_name(const char *abs_name);
  char *recursive_container(CORBA::IDLType_ptr memb);
  void insert_guid();
  int Compare(const char* ps1, const char* ps2);
  void SortName(CORBA::ContainedSeq_var c, CORBA::ULong* pv, CORBA::ULong n);


public:
  CodeGenMIDL( DB &db, CORBA::Container_ptr con, bool emit_repoids );
  void emit( std::string &fnbase );
  std::string emit( CORBA::IRObject_ptr obj );
};



#endif
