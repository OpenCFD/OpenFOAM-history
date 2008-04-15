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

#ifndef __CODEGENCPP_STUB_H__
#define __CODEGENCPP_STUB_H__

#include "codegen-c++-util.h"


class CodeGenCPPStub : virtual public CodeGenCPPUtil
{
protected:
  void emit_stub();

private:
  void emit_idl_obj( IRObj_ptr obj );
  void emit_contained( CORBA::IRObject_ptr obj );
  void emit_container( CORBA::Container_ptr obj );
  void emit_marshaller( CORBA::IRObject_ptr obj );
  void emit_sii_stub_method( std::string &absClassName,
			     std::string &dispatch_name,
			     const CORBA::OperationDescription &op,
			     CORBA::IDLType_ptr result );
  void emit_poa_stub_method( std::string &absClassName,
			     std::string &dispatch_name,
			     const CORBA::OperationDescription &op,
			     CORBA::IDLType_ptr result );
  void emit_Interface( CORBA::InterfaceDef_ptr in );
  void emit_Enum( CORBA::EnumDef_ptr s );
  void emit_Constant( CORBA::ConstantDef_ptr c );
  void emit_Exception( CORBA::ExceptionDef_ptr e );
  void emit_StructDefaultMembers( char *name,
				  char *prefix,
				  CORBA::StructMemberSeq_var &s );
  void emit_Struct( CORBA::StructDef_ptr s );
  void emit_UnionDefaultMembers( char *name,
				 char *prefix,
				 CORBA::UnionMemberSeq_var &u );
  void emit_union_case_label( CORBA::IDLType_ptr discr,
			      CORBA::UnionMember &m,
			      CORBA::UnionMemberSeq &members );
  void emit_union_member_assigner( CORBA::IDLType_ptr type,
				   const char * name,
				   const char * path = 0,
				   const char * p_name = 0 );
  void emit_union_member_accessor( const char * className,
				   CORBA::IDLType_ptr type,
				   const char * name,
				   const char * path = 0,
				   CORBA::IDLType_ptr discr =
				   CORBA::IDLType::_nil (),
				   CORBA::UnionMemberSeq * members = 0,
				   CORBA::ULong index = 0 );
  void emit_Union( CORBA::UnionDef_ptr u );
  void emit_Alias( CORBA::AliasDef_ptr a );
  void emit_StringDef( CORBA::StringDef_ptr s );
  void emit_WstringDef( CORBA::WstringDef_ptr ws );
  void emit_SequenceDef( CORBA::SequenceDef_ptr s );
  void emit_FixedDef( CORBA::FixedDef_ptr f );
  void emit_ArrayDef( CORBA::ArrayDef_ptr a );

  void emit_Value( CORBA::ValueDef_ptr v );
  void emit_ValueBox( CORBA::ValueBoxDef_ptr vb );
  void emit_ValueRekCheck( CORBA::IRObject_ptr obj,
			   std::string, CORBA::ULong );
  void emit_ValueReleaser( CORBA::IRObject_ptr obj,
			   std::string, CORBA::ULong );

  void emit_var_def( const std::string &type,
		     CORBA::Container_ptr scope,
		     const std::string &name,
		     const std::string &init );
  void emit_typecode_def( CORBA::Container_ptr scope,
			  const std::string &abs,
			  const std::string &name,
			  CORBA::TypeCode_ptr tc );
public:
  CodeGenCPPStub( DB &db, IDLParam &params, CORBA::Container_ptr con );
};


#endif
