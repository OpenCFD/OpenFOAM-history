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

#ifndef __CODEGENWSDL_H__
#define __CODEGENWSDL_H__

#include "codegen.h"
#include "params.h"
#include "db.h"

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <map>
#include <list>
#include <set>
#else
#include <map.h>
#include <list.h>
#include <set.h>
#endif

class CodeGenWSDL : public CodeGen
{
 private:
 	typedef std::string	Identifier;	// IDL qualified form
 	typedef Identifier	Interface;	// XML qualified form
 	typedef Identifier	Message;	// XML qualified form
 	
 	typedef std::list<Message>	Inputs;
 	typedef std::list<Message>	Outputs;
 	typedef std::list<Message>	Exceptions;
 	
#if defined(__SUNPRO_CC)
	struct Operation;
// explicitly grant permission
	friend struct Operation;
#endif

	struct Operation
 	{
 		Identifier	name;	// unqualified
 		Inputs		inputs;
 		Outputs		outputs;
 		Exceptions	faults;
 	};
 	
 	typedef std::list<Operation>		Operations;
 	typedef std::map<Interface, Operations>	Ports;
 	typedef std::set<Identifier>		ImplicitTypes;
 	
 	DB*		_db;
 	IDLParam&	_params;
 	
 	Ports		_ports;
 	ImplicitTypes	_types;
 
 protected:
 	void emitWSDL();
 	
 	
 	void emitTypes();
 	
 	void emitMemberdef(const char* name, CORBA::IDLType_ptr type, bool opt);
 	void emitStructdef(CORBA::StructDef_ptr s);
 	void emitExceptiondef(CORBA::ExceptionDef_ptr e);
 	void emitValuedef(CORBA::ValueDef_ptr v);
 	void emitUniondef(CORBA::UnionDef_ptr u);
 	
 	void checkImplicit(CORBA::IDLType_ptr t);
 	void checkMembers(CORBA::ParDescriptionSeq_var s);
 	void checkMembers(CORBA::StructMemberSeq_var s);
 	void checkMembers(CORBA::ValueMemberSeq s);
 	void checkMembers(CORBA::UnionMemberSeq_var s);
 	
 	void emitImplicitString(CORBA::StringDef_ptr t);
 	void emitImplicitWstring(CORBA::WstringDef_ptr t);
 	void emitImplicitFixed(CORBA::FixedDef_ptr t);
 	void emitImplicitSequence(CORBA::SequenceDef_ptr t);
 	void emitImplicitArray(CORBA::ArrayDef_ptr t);
 	
 	void emitEnumdef(CORBA::EnumDef_ptr e);
 	void emitValueBoxdef(CORBA::ValueBoxDef_ptr b);
 	void emitAliasdef(CORBA::AliasDef_ptr a);
 	
 	Identifier getTypename(CORBA::IDLType_ptr type, bool prefix = true);
 	Identifier getPrimitiveType(CORBA::TypeCode_ptr type, bool prefix);
 	
 	
 	void emitMessages();
 	
 	void emitException(CORBA::ExceptionDef_ptr e);
 	void emitOperation(CORBA::OperationDef_ptr op);
 	void emitAttribute(CORBA::AttributeDef_ptr at);
 	void emitPart(const char* name, CORBA::IDLType_var type);
 	 	
 	
 	void emitPortTypes();
 	
 	
 	void emitServices();
 	
 	
 	Identifier idlNameToWsdl(const char* corba_name);
 	Identifier idlNameToPfxWsdl(const char* corba_name, const char* pfx);
 
 public:
 	CodeGenWSDL( DB &db, IDLParam & params, CORBA::Container_ptr con );
 	void emit( std::string& fnbase );
 	std::string emit( CORBA::IRObject_ptr obj );
};

#endif
