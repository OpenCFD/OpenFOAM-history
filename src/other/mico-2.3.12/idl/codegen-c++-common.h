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

#ifndef __CODEGENCPP_COMMON_H__
#define __CODEGENCPP_COMMON_H__

#include "codegen-c++-util.h"


class CodeGenCPPCommon : virtual public CodeGenCPPUtil
{
protected:
  void emit_common();

private:

  std::set<std::string, std::less<std::string> >   _forward_dcls;

  void emit_idl_obj( IRObj_ptr obj );
  void emit_marshal_prototype( CORBA::IRObject_ptr obj );
  void emit_ForwardDcl( CORBA::Contained_ptr in );
  void emit_Interface( CORBA::InterfaceDef_ptr in );
  void emit_DefaultMembers( char *name );
  void emit_Struct( CORBA::StructDef_ptr s );
  void emit_Union( CORBA::UnionDef_ptr u );
  void emit_Constant( CORBA::ConstantDef_ptr c );
  void emit_Exception( CORBA::ExceptionDef_ptr e );
  void emit_Enum( CORBA::EnumDef_ptr e );
  void emit_Alias( CORBA::AliasDef_ptr a );
  void emit_StringDef( CORBA::StringDef_ptr s );
  void emit_WstringDef( CORBA::WstringDef_ptr ws );
  void emit_SequenceDef( CORBA::SequenceDef_ptr s );
  void emit_FixedDef( CORBA::FixedDef_ptr f );
  void emit_ArrayDef( CORBA::ArrayDef_ptr a );
  void emit_Native( CORBA::NativeDef_ptr n );
  void emit_Value( CORBA::ValueDef_ptr v );
  void emit_ValueBox( CORBA::ValueBoxDef_ptr v );
  void emitPrototypes( CORBA::Container_ptr in,
		       bool as_pure_virtual );
  void emitLocalDecls( CORBA::Container_ptr in );

  void emit_poa_obj (IRObj_ptr obj);
  void emit_poa_skel (CORBA::Container_ptr in);
  void emit_poa_ties (CORBA::Container_ptr in);

  void emit_obv_obj (IRObj_ptr obj);
  void emit_obv_skel (CORBA::ValueDef_ptr val);

public:
  CodeGenCPPCommon( DB &db, IDLParam &params, CORBA::Container_ptr con );
};


#endif
