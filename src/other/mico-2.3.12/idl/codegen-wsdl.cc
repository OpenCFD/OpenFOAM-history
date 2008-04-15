/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *  Copyright (c) 2002-2003 by ITO TU-Darmstadt
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

/* Relevant command-line options:
 * 
 *  --[no-]wsi-wsdl          Defaults to --no-wsi-wsdl (soap wsdl)
 *                           Toggle SOAP or WSI conformant WSDL output
 *  
 *  --[dont-]support-id      Defaults to --dont-support-id
 *                           When we need ID or IDREF, drop these attributes.
 *                           They show up in valuetype and are not supported
 *                           by many WSDL implementations.
 *
 *  --[not-]missing-nscorba  Defaults to --missing-nscorba
 *                           The WSDL-IDL specification names a namespace
 *                           which should have a schema, but does not. This
 *                           toggles whether we provide it ourselves.
 */
 
/* Conformance: sticks very closely to the WSDL-IDL draft, changes:
 *
 *	1. I do not name implicit types by their scope. Instead I output
 *	   a type signature like 'CORBA.sequence_5_CORBA.string_4'
 *	   This is the only way to get collision-free multi-dimensional arrays
 *
 *	2. Typedefs of implicit types output two productions.
 *
 *	3. All _SE_ hacks have been dropped. WSI conformant uses --wsi-wsdl
 *         otherwise it is SOAP compatible.
 *
 *	4. Reverse mapping documentation tags are dropped since they are 
 *	   insufficient to address the issue and thus a waste of time.
 *
 *	5. Exceptions in attribute access ARE supported (mico extension)
 *
 *	6. ValueType inheritence is supported via flattening
 *
 *	7. Renamed CORBA.SystemException to _exception.CORBA.SystemException
 *	   since the standard is buggy (see below)
 *
 *	8. We have our own scheme for fault encoding in soap since the
 *	   WSDL-IDL standard outputs non-WSDL conformant XML.
 *
 *	9. We use xsd:anyURI of xsd:url since url does not exist.
 *	
 *	10. We provide a schema for corba: since it is missing.
 *
 *	11. We include an empty NoOp request for _get_ methods since you
 *	    actually ARE requesting; not being notified.
 *	
 *	12. We include an empty NoOp respone for _set_ methods since you
 *	    actually do get feedback: exceptions!!!
 *
 * Known bugs in this file:
 *
 *	1. Namespace clash from multiple ancestors and a common method.
 *	   Different type signatures -> arbitrary method chosen.
 *
 *	2. Principal and native types unsupported. (what are these?)
 *
 * Bugs in MICO:
 *
 *	1. Set Exceptions broken codegen-idl.cc:643 s/get/set
 *
 *	2. Namespaces clash broken for all outputs
 *
 *	3. > O(n) n^2? output method with 'o <<', also never flushes till end
 *
 * Bugs in Apache AXIS WSDL->JAVA:
 *
 *	1. ID not supported
 *
 *	2. IDREF not supported
 *
 *	3. Ignores multiple fault responses in a binding
 *
 *	4. Ignores repeated elements in a type
 *
 * Problems in the standards:
 *
 *	1. The WSDL-IDL multidimensional array definition is nested backwards
 *
 *	2. The IDL standard does not address namespace clashes in interfaces
 *
 *	3. Namespaces for elements in the WSDL-IDL examples are often wrong
 *
 *	4. Contexts are ignored by the WSDL-IDL standard
 *
 *	5. Returning interfaces as Object references does not include the 
 *         WSDL doc for the type. Interfaces should either include a
 *	   CORBA.TypeCode or specify the wsdl document. Type information
 *	   is discarded right now. Breaks reverse-mapping. Hides useful info.
 *
 *	6. Does not address ValueType inheritence
 *
 *	7. CORBA.SystemException is both a message and a type (in same ns)
 *
 *	8. Union and Exception can have ValueTypes too; should be <choice>
 *
 *	9. The standard outputs malformed soap:fault bindings
 *
 *	10. No mapping for long double
 *
 *	11. The described method for multidimensional array mappings will
 *	    break. long matrix[5][3]; and long matrix2[6][4]; cannot both
 *	    define a type 'ArrayOfint'; what were they thinking?! Not to
 *	    mention the array could be of a type whose name you don't want
 *	    to embed (like a struct).
 *
 *	12. tns: must prefix all use of CORBA.* types
 *
 *	13. There is no xsd:url, maybe xsd:anyURI is meant?
 *
 *	14. _get_ and _set_ operations MUST include an empty input and output
 *	    respectively. They are NOT wsdl notify or post events. You are
 *	    requesting a response for _get_, and you are getting a successful
 *	    response (no exceptions) for _set_.
 */

#include <CORBA.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#include <sstream>
#include <set>
#else
#include <fstream.h>
#include <strstream.h>
#include <set.h>
#endif

#include <ctype.h>
#include <stdio.h>

#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif

#include "codegen-wsdl.h"

using std::cerr;
using std::endl;

//--------------------------------------------------------------------------
// Copyright for output files

static const char *wsdl_copyright[] = {
  "<!--",
  "  MICO - an Open Source CORBA implementation",
  "  Copyright (c) 1997-2001 by The Mico Team",
  "  Copyright (c) 2002-2003 by ITO TU-Darmstadt",
  "  ",
  "  This file was automatically generated. DO NOT EDIT!",
  " -->"
};

const int wsdl_copyright_size = sizeof( wsdl_copyright ) / sizeof( char * );

//--------------------------------------------------------------------------
// Some global variables which control output

// Control whether Module.Interface.NestedType -> Interface.NestedType
// (the later is wrong, but in the standard)
bool buggy_interface_namespace = false;


//--------------------------------------------------------------------------
// The namespaces used by the WSDL-IDL standard for SOAP->CORBA

static const char *ns_tns     = "http://www.omg.org/IDL-Mapped/";
static const char *ns_xsd     = "http://www.w3.org/2001/XMLSchema";
static const char *ns_corba   = "http://www.omg.org/IDL-WSDL/1.0/";
static const char *ns_wsdl    = "http://schemas.xmlsoap.org/wsdl/";
static const char *ns_soap    = "http://schemas.xmlsoap.org/wsdl/soap/";
static const char *ns_http    = "http://schemas.xmlsoap.org/wsdl/http/";
static const char *ns_soapenc = "http://schemas.xmlsoap.org/soap/encoding/";
static const char *ns_soapenv = "http://schemas.xmlsoap.org/soap/envelope/";

//--------------------------------------------------------------------------
// Implementation

CodeGenWSDL::CodeGenWSDL( DB &db, IDLParam &params,
			CORBA::Container_ptr con )
  : CodeGen( con ),
    _params( params )
{
	_db = &db;
}


void CodeGenWSDL::emit( std::string& fnbase )
{
	std::ofstream file((fnbase + ".wsdl").c_str());
	if (!file)
	{
		cerr << "error: cannot open file " << fnbase << ".wsdl for writing"
		     << endl;
		return;
	}
	
	o.start_output(file);
	emitWSDL();
	o.stop_output();
}

// Interesting method; output wsdl given an object. :-)
// That is kinda quirky, but maybe it will be useful to someone somewhere
std::string CodeGenWSDL::emit( CORBA::IRObject_ptr obj )
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
	std::ostringstream idl;
#else
	std::ostrstream idl;
#endif

	CORBA::Contained_var contained = CORBA::Contained::_narrow(obj);
	if(!CORBA::is_nil(contained)) 
	{
		CORBA::String_var id = contained->id();
	}
	
	o.start_output(idl);
	emitWSDL();
	o.stop_output();
	
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
	return idl.str();
#else
	idl << ends;
	std::string result = idl.str();
	idl.rdbuf()->freeze( 0 );
	return result;
#endif
}

void CodeGenWSDL::emitWSDL()
{
	o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	
	for (int i = 0; i < wsdl_copyright_size; ++i)
		o << wsdl_copyright[i] << "\n";
	
	o << "<definitions name=\"SOAP-IDL\"\n" 
	  << "    targetNamespace=\"" << ns_tns << "\"\n"
	  << "    xmlns:tns=\"" << ns_tns << "\"\n"
	  << "    xmlns:corba=\"" << ns_corba << "\"\n"
	  << "    xmlns:xsd=\"" << ns_xsd << "\"\n"
	  << "    xmlns:http=\"" << ns_http << "\"\n"
	  << "    xmlns:soap=\"" << ns_soap << "\"\n"
	  << "    xmlns:soapenc=\"" << ns_soapenc << "\"\n"
	  << "    xmlns:soapenv=\"" << ns_soapenv << "\"\n"
	  << "    xmlns:wsdl=\"" << ns_wsdl << "\"\n"
	  << "    xmlns=\"" << ns_wsdl << "\">\n";
	
	emitTypes();
	emitMessages();
	emitPortTypes();
	emitServices();
	
	o << "</definitions>\n";
}


//-------------------------------------------------------- Types Generation


void CodeGenWSDL::emitTypes()
{
	o << " <types>\n";
	
	if (_params.missing_nscorba)
	{
		o << "  <!-- Provide the missing corba namespace -->\n"
		  << "  <schema\n"
		  << "      targetNamespace=\"" << ns_corba << "\"\n"
		  << "      xmlns=\"" << ns_xsd << "\">\n"
		  << "   <complexType name=\"_VALREF\">\n";
		
		if (_params.support_id)
			o << "    <attribute name=\"ref\" type=\"xsd:IDREF\" use=\"optional\"/>\n";
		
		o << "   </complexType>\n"
		  << "  </schema>\n";
	}
	
	o << "  <schema\n"
	  << "      targetNamespace=\"" << ns_tns << "\"\n"
	  << "      xmlns=\"" << ns_xsd << "\">\n"
	  << "   <!-- Types common to all corba mappings -->\n"
	  << "   <complexType name=\"CORBA.ObjectReference\">\n"// -- Section 2.5
	  << "    <sequence>\n"
	  << "     <element\n"
	  << "         name=\"url\" type=\"xsd:anyURI\"\n"
	  << "         maxOccurs=\"unbounded\" minOccurs=\"1\"/>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n"
	  << "   <simpleType name=\"CORBA.char\">\n"
	  << "    <restriction base=\"xsd:string\">\n"
	  << "     <length value=\"1\" fixed=\"true\"/>\n"
	  << "    </restriction>\n"
	  << "   </simpleType>\n"
	  << "   <simpleType name=\"CORBA.wchar\">\n"
	  << "    <restriction base=\"xsd:string\"/>\n"
	  << "   </simpleType>\n"
	  << "   <complexType name=\"CORBA.TypeCode\">\n"	// -- Section 2.7.7
	  << "    <sequence>\n"
	  << "     <element\n"
	  << "         name=\"definition\" type=\"xsd:anyURI\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "     <element\n"
	  << "         name=\"typename\" type=\"xsd:string\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n"
	  << "   <complexType name=\"CORBA.Any\">\n"		// -- Section 2.7.8
	  << "    <sequence>\n"
	  << "     <element\n"
	  << "         name=\"type\" type=\"tns:CORBA.TypeCode\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "     <element\n"
	  << "         name=\"value\" type=\"xsd:anyType\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n"
	  << "   <simpleType name=\"CORBA.completion_status\">\n"// -- Section 2.8.2
	  << "    <restriction base=\"xsd:string\">\n"
	  << "     <enumeration value=\"COMPLETED_YES\"/>\n"
	  << "     <enumeration value=\"COMPLETED_NO\"/>\n"
	  << "     <enumeration value=\"COMPLETED_MAYBE\"/>\n"
	  << "    </restriction>\n"
	  << "   </simpleType>\n"
	  << "   <complexType name=\"CORBA.SystemException\">\n"
	  << "    <sequence>\n"
	  << "     <element\n"
	  << "         name=\"minor\" type=\"xsd:unsignedInt\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "     <element\n"
	  << "         name=\"completion_status\" type=\"tns:CORBA.completion_status\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n"
	  << "   <!-- Types from the source IDL -->\n";
	
	/* The unfortunate fact of life is that IDL can nest declarations.
	 * module { module { interface { struct { union { struct { enum }}}}..
	 *
	 * That is: module* interface? exception? (struct | valuetype |union)* 
	 *          (enum | struct | typedef | implicit)
	 * 
	 * We cannot nest declarations like in C++, so we have to flatten
	 * the names into m.m.i.s.u.s.u.name. Furthermore, the parse tree
	 * has them inlined. Therefore, we have to scan everything ahead
	 * of time.
	 *
	 * The module* interface? is not really a problem; it does not
	 * generate a type dependent on contents. We can just ignore it and
	 * use the scoped. The 'exception? (struct | valuetype | union)*'
	 * is the problem since we need to re-order these.
	 * 
	 * The other nastiness is that implicit types like 'long[4][6]' can
	 * occur inside typedefs, structs, attributes, operations, and unions.
	 * (but thankfully not sequences, string, or fixed types)
	 */
	
	typedef std::map<Identifier, CORBA::Contained_var> Scopeables;
	Scopeables scopeables;
	
	// First pass: collect nesting information
	for (CORBA::ULong i = 0; i < _idl_objs.length(); ++i)
	{
		CORBA::IRObject_ptr x = _idl_objs[i]->obj;
		switch (x->def_kind())
		{
		 case CORBA::dk_Exception:
		 case CORBA::dk_Union:
		 case CORBA::dk_Struct:
		 case CORBA::dk_Value:
			if (1)
			{
			 	CORBA::Contained_ptr p = CORBA::Contained::_narrow(x);
			 	scopeables[p->absolute_name()] = p;
			}
		 	break;
		 default:
		 	// noop
		 	break;
		}
	}
	
	// Second pass, output all non-implicit terminal types
	//  Including: Alias (typedef), Enum, ValueBox
	//  Always found at declaration level
	for (CORBA::ULong j = 0; j < _idl_objs.length(); ++j)
	{
		CORBA::IRObject_ptr x = _idl_objs[j]->obj;
		switch (x->def_kind())
		{
		 case CORBA::dk_Alias:
		 	if (1)
		 	{
			 	CORBA::AliasDef_ptr a = 
			 		CORBA::AliasDef::_narrow(x);
			 	emitAliasdef(a);
			}
		 	break;
		 	
		 case CORBA::dk_Enum:
		 	if (1)
		 	{
		 		CORBA::EnumDef_ptr e = 
		 			CORBA::EnumDef::_narrow(x);
		 		emitEnumdef(e);
		 	}
		 	break;
		 case CORBA::dk_ValueBox:
		 	if (1)
		 	{
		 		CORBA::ValueBoxDef_ptr b = 
		 			CORBA::ValueBoxDef::_narrow(x);
		 		emitValueBoxdef(b);
		 	}
		 	break;
		 default:
		 	// noop
		 	break;
		}
	}
	
	// Third pass, output all implicit types
	//  Including: arrays, sequence<>, fixed<>, (w)string<>,
	//  Can be found at typedef, valuebox, attribute,
	//  operation, struct, exception, union, and valuetype levels.
	for (CORBA::ULong k = 0; k < _idl_objs.length(); ++k)
	{
		CORBA::IRObject_ptr x = _idl_objs[k]->obj;
		switch (x->def_kind())
		{
		case CORBA::dk_Alias:
		 	if (1)
		 	{
			 	CORBA::AliasDef_ptr a = 
			 		CORBA::AliasDef::_narrow(x);
			 	checkImplicit(a->original_type_def());
			}
			break;
		case CORBA::dk_ValueBox:
			if (1)
			{
				CORBA::ValueBoxDef_ptr v =
					CORBA::ValueBoxDef::_narrow(x);
				checkImplicit(v->original_type_def());
			}
			break;
		case CORBA::dk_Attribute:
			if (1)
			{
				CORBA::AttributeDef_ptr at =
					CORBA::AttributeDef::_narrow(x);
				checkImplicit(at->type_def());
			}
			break;
		case CORBA::dk_Operation:
			if (1)
			{
				CORBA::OperationDef_ptr o =
					CORBA::OperationDef::_narrow(x);
				checkImplicit(o->result_def());
				checkMembers(o->params());
			}
			break;
		case CORBA::dk_Struct:
		 	if (1)
		 	{
			 	CORBA::StructDef_ptr s = 
			 		CORBA::StructDef::_narrow(x);
			 	checkMembers(s->members());
			}
			break;
		case CORBA::dk_Exception:
			if (1)
			{
				CORBA::ExceptionDef_ptr e =
					CORBA::ExceptionDef::_narrow(x);
				checkMembers(e->members());
			}
			break;
		case CORBA::dk_Value:
			if (1)
			{
				CORBA::ValueDef_ptr v =
					CORBA::ValueDef::_narrow(x);
				checkMembers(v->describe_value()->members);
			}
			break;
		case CORBA::dk_Union:
			if (1)
			{
				CORBA::UnionDef_ptr u =
					CORBA::UnionDef::_narrow(x);
				checkMembers(u->members());
			}
			break;
		default:
			// noop
			break;
		}
	}
	
	// Forth pass, output all struct types in backwards order
	for (Scopeables::reverse_iterator l  = scopeables.rbegin(); 
	                                  l != scopeables.rend(); 
	                                  ++l)
	{
		switch (l->second->def_kind())
		{
		case CORBA::dk_Struct:
			if (1)
			{
				CORBA::StructDef_ptr s = 
					CORBA::StructDef::_narrow(l->second);
				emitStructdef(s);
			}
			break;
		
		case CORBA::dk_Exception:
			if (1)
			{
				CORBA::ExceptionDef_ptr s = 
					CORBA::ExceptionDef::_narrow(l->second);
				emitExceptiondef(s);
			}
			break;
		
		case CORBA::dk_Value:
			if (1)
			{
				CORBA::ValueDef_ptr v = 
					CORBA::ValueDef::_narrow(l->second);
				emitValuedef(v);
			}
			break;
			
		case CORBA::dk_Union:
			if (1)
			{
				CORBA::UnionDef_ptr u = 
					CORBA::UnionDef::_narrow(l->second);
				emitUniondef(u);
			}
			break;
		default:
			// noop
			break;
		}
	}
	
	o << "  </schema>\n"
	  << " </types>\n";
}


//----- Nesting types


void CodeGenWSDL::emitMemberdef(const char* name, CORBA::IDLType_ptr type, bool opt)
{
	if (type->type()->kind() == CORBA::tk_value)
	{	// -- Section 2.7.10.3
		o << "      <choice>\n"
		  << "       <element name=\"" << name << "\" type=\""
		  << getTypename(type) << "\"/>\n"
		  << "       <element name=\"_REF_" << name << "\" type=\"corba:_VALREF\"/>\n"
		  << "      </choice>\n";
	}
	else
	{	// -- Section 2.7.2
		o << "      <element name=\"" << name << "\" type=\""
		  << getTypename(type) << "\"\n";
		
		if (!opt)
			o << "          maxOccurs=\"1\" minOccurs=\"1\"/>\n";
		// -- Section 2.7.4
		else	o << "          nillable=\"true\" maxOccurs=\"1\" minOccurs=\"0\"/>\n";
	}
}

void CodeGenWSDL::emitStructdef(CORBA::StructDef_ptr st)
{	// -- Section 2.7.2
	Identifier wsdl_name = idlNameToWsdl(st->absolute_name());
	o << "   <complexType name=\"" << wsdl_name << "\">\n"
	  << "    <sequence>\n";
	
	CORBA::StructMemberSeq_var m = st->members();
	for (CORBA::ULong i = 0; i < m->length(); ++i)
		emitMemberdef(m[i].name, m[i].type_def, false);
	
	o << "    </sequence>\n"
	  << "   </complexType>\n";
}

void CodeGenWSDL::emitExceptiondef(CORBA::ExceptionDef_ptr e)
{	// -- Section 2.8.5
	Identifier wsdl_name = idlNameToWsdl(e->absolute_name());
	o << "   <complexType name=\"" << wsdl_name << "\">\n"
	  << "    <sequence>\n";
	
	CORBA::StructMemberSeq_var m = e->members();
	for (CORBA::ULong i = 0; i < m->length(); ++i)
		emitMemberdef(m[i].name, m[i].type_def, false);
	
	o << "    </sequence>\n"
	  << "   </complexType>\n";
}

void CodeGenWSDL::emitValuedef(CORBA::ValueDef_ptr v)
{	// -- Section 2.7.10
	Identifier wsdl_name = idlNameToWsdl(v->absolute_name());
	o << "   <complexType name=\"" << wsdl_name << "\">\n"
	  << "    <sequence>\n";
	
	// Mico inlines parent attributes for us, so we don't need this:
	// CORBA::ValueDef_ptr base = v->base_value();
	
	CORBA::ValueMemberSeq m = v->describe_value()->members;
	for (CORBA::ULong i = 0; i < m.length(); ++i)
	{
		if (m[i].access != CORBA::PUBLIC_MEMBER)
			continue;
		
		emitMemberdef(m[i].name, m[i].type_def, false);
	}
	
	o << "    </sequence>\n";
	if (_params.support_id)
	{
		o << "    <attribute name=\"id\" type=\"xsd:ID\" use=\"optional\"/>\n";
	}
	o << "   </complexType>\n";
}

void CodeGenWSDL::emitUniondef(CORBA::UnionDef_ptr un)
{	// -- Section 2.7.4
	Identifier wsdl_name = idlNameToWsdl(un->absolute_name());
	o << "   <complexType name=\"" << wsdl_name << "\">\n"
	  << "    <sequence>\n"
	  << "     <element name=\"discriminator\" type=\""
	  << getTypename(un->discriminator_type_def()) << "\"/>\n"
	  << "     <choice>\n";
	
	CORBA::UnionMemberSeq_var m = un->members();
	for (CORBA::ULong i = 0; i < m->length(); ++i)
		emitMemberdef(m[i].name, m[i].type_def, true);
		o << "          nillable=\"true\" maxOccurs=\"1\" minOccurs=\"0\"/>\n";
	
	o << "     </choice>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n";
}


//----- Non nesting types


void CodeGenWSDL::emitEnumdef(CORBA::EnumDef_ptr e)
{	// -- Section 2.7.1
	o << "   <simpleType name=\"" << idlNameToWsdl(e->absolute_name()) << "\">\n"
	  << "    <restriction base=\"xsd:string\">\n";
	
	CORBA::EnumMemberSeq_var m = e->members();
	for (CORBA::ULong i = 0; i < m->length(); ++i)
		o << "     <enumeration value=\"" << m[i] << "\"/>\n";
	
	o << "    </restriction>\n"
	  << "   </simpleType>\n";
}

void CodeGenWSDL::emitValueBoxdef(CORBA::ValueBoxDef_ptr b)
{	// -- Section 2.7.10.3
	o << "   <complexType name=\"" << idlNameToWsdl(b->absolute_name()) << "\">\n"
	  << "    <sequence>\n"
	  << "     <element name=\"value\" type=\""
	  << getTypename(b->original_type_def()) << "\"\n"
	  << "         maxOccurs=\"1\" minOccurs=\"1\"/>\n"
	  << "    </sequence>\n"
	  << "   </complexType>\n";
}

void CodeGenWSDL::emitAliasdef(CORBA::AliasDef_ptr a)
{	// -- Section 2.7.3
	o << "   <simpleType name=\"" << idlNameToWsdl(a->absolute_name()) << "\">\n"
	  << "    <restriction base=\""
	  << getTypename(a->original_type_def()) << "\"/>\n"
	  << "   </simpleType>\n";
}


//----- Implicit scanners


void CodeGenWSDL::checkImplicit(CORBA::IDLType_ptr t)
{
	Identifier x = getTypename(t, false);
	if (_types.find(x) != _types.end())
		return;
	_types.insert(x);
	
	switch (t->def_kind())
	{
	case CORBA::dk_Fixed:
		if (1)
		{
			CORBA::FixedDef_ptr f =
				CORBA::FixedDef::_narrow(t);
			emitImplicitFixed(f);
		}
		break;
 	case CORBA::dk_String:
 		if (1)
 		{
 			CORBA::StringDef_ptr s =
 				CORBA::StringDef::_narrow(t);
 			emitImplicitString(s);
	 	}
	 	break;
 	case CORBA::dk_Wstring:
 		if (1)
 		{
 			CORBA::WstringDef_ptr s =
 				CORBA::WstringDef::_narrow(t);
 			emitImplicitWstring(s);
	 	}
	 	break;
	case CORBA::dk_Sequence:
		if (1)
		{
			CORBA::SequenceDef_ptr s =
				CORBA::SequenceDef::_narrow(t);
			emitImplicitSequence(s);
		}
		break;
	case CORBA::dk_Array:
		if (1)
		{
			CORBA::ArrayDef_ptr a =
				CORBA::ArrayDef::_narrow(t);
                	emitImplicitArray(a);
		}
		break;
	default:
		// noop
		break;
	}
}

void CodeGenWSDL::checkMembers(CORBA::ParDescriptionSeq_var s)
{
	for (CORBA::ULong i = 0; i < s->length(); ++i)
		checkImplicit(s[i].type_def);
}

void CodeGenWSDL::checkMembers(CORBA::StructMemberSeq_var s)
{
	for (CORBA::ULong i = 0; i < s->length(); ++i)
		checkImplicit(s[i].type_def);
}

void CodeGenWSDL::checkMembers(CORBA::ValueMemberSeq s)
{
	for (CORBA::ULong i = 0; i < s.length(); ++i)
		checkImplicit(s[i].type_def);
}

void CodeGenWSDL::checkMembers(CORBA::UnionMemberSeq_var s)
{
	for (CORBA::ULong i = 0; i < s->length(); ++i)
		checkImplicit(s[i].type_def);
}


//----- Implicit types


void CodeGenWSDL::emitImplicitString(CORBA::StringDef_ptr t)
{	// -- Section 2.6
	o << "   <simpleType name=\""
	  << getTypename(t, false) << "\">\n"
	  << "    <restriction base=\"xsd:string\">\n"
	  << "     <maxLength value=\"" << t->bound() << "\" fixed=\"true\"/>\n"
	  << "    </restriction>\n"
	  << "   </simpleType>\n";
}

void CodeGenWSDL::emitImplicitWstring(CORBA::WstringDef_ptr t)
{	// -- Section 2.6
	o << "   <simpleType name=\""
	  << getTypename(t, false) << "\">\n"
	  << "    <restriction base=\"xsd:string\">\n"
	  << "     <maxLength value=\"" << t->bound() << "\" fixed=\"true\"/>\n"
	  << "    </restriction>\n"
	  << "   </simpleType>\n";
}

void CodeGenWSDL::emitImplicitFixed(CORBA::FixedDef_ptr t)
{	// -- Section 2.7.9
	o << "   <simpleType name=\""
	  << getTypename(t, false) << "\">\n"
	  << "    <restriction base=\"xsd:int\">\n"
	  << "     <totalDigits value=\"" << t->digits() << "\"/>\n"
	  << "     <fractionDigits value=\"" << t->scale() << "\"/>\n"
	  << "    </restriction>\n"
	  << "   </simpleType>\n";
}

void CodeGenWSDL::emitImplicitSequence(CORBA::SequenceDef_ptr t)
{	// -- Section 2.7.5
	checkImplicit(t->element_type_def());
	o << "   <complexType name=\""
	  << getTypename(t, false) << "\">\n"
	  << "    <complexContent>\n"
	  << "     <sequence>\n"
	  << "      <element name=\"item\" minOccurs=\"0\" maxOccurs=\"";
	if (t->bound() == 0)
		o << "unbounded";
	else	o << t->bound();
	o << "\"\n"
	  << "          type=\""
	  << getTypename(t->element_type_def()) << "\"/>\n"
	  << "     </sequence>\n"
	  << "    </complexContent>\n"
	  << "   </complexType>\n";
}

void CodeGenWSDL::emitImplicitArray(CORBA::ArrayDef_ptr t)
{	// -- Section 2.7.6
	checkImplicit(t->element_type_def());
	o << "   <complexType name=\""
	  << getTypename(t, false) << "\">\n"
	  << "    <complexContent>\n";
	
	if (!_params.wsi_wsdl)
	{
		o << "     <restriction base=\"soapenc:Array\">\n";
	}
	  
	o << "      <sequence>\n"
	  << "       <element name=\"item\" minOccurs=\"" << t->length()
	  << "\" maxOccurs=\"" << t->length() << "\"\n"
	  << "           type=\""
	  << getTypename(t->element_type_def()) << "\"/>\n"
	  << "      </sequence>\n";
	  
	if (!_params.wsi_wsdl)
	{
		o << "      <attribute ref=\"soapenc:arrayType\"\n"
		  << "          wsdl:arrayType=\""
		  << getTypename(t->element_type_def())
		  << "[]\"/>\n"
		  << "     </restriction>\n";
	}
	
	o << "    </complexContent>\n"
	  << "   </complexType>\n";
}


//----- Type references


CodeGenWSDL::Identifier CodeGenWSDL::getTypename(CORBA::IDLType_ptr type, bool prefix)
{
	char buf[80];
	
	switch (type->def_kind())
	{
	 case CORBA::dk_Struct:
	 case CORBA::dk_Union:
	 case CORBA::dk_Enum:
	 case CORBA::dk_Alias:
	 case CORBA::dk_Value:
	 case CORBA::dk_ValueBox:
	 	if (1)
	 	{	// -- Section 2.4
		 	CORBA::Contained_ptr con = 
		 		CORBA::Contained::_narrow(type);
		 	return (prefix?"tns:":"") + idlNameToWsdl(con->absolute_name());
		}
		break;
	
	case CORBA::dk_Interface:
	case CORBA::dk_AbstractInterface:
	case CORBA::dk_LocalInterface:
		// -- Section 2.8.3
		return (prefix?"tns:":"") + Identifier("CORBA.ObjectReference");
		break;
	
	// I intentionally deviate from the standard with respect to
	// implicit types because the standard is stupid and promotes
	// needless redundancy.
	case CORBA::dk_Fixed:
		if (1)
		{
			CORBA::FixedDef_ptr f =
				CORBA::FixedDef::_narrow(type);
			sprintf(&buf[0], "%sCORBA.fixed_%d_%d",
				prefix?"tns:":"",
				f->digits(), f->scale());
			return &buf[0];
		}
		break;
 	case CORBA::dk_String:
 		if (1)
 		{
 			CORBA::StringDef_ptr s =
 				CORBA::StringDef::_narrow(type);
 			sprintf(&buf[0], "%sCORBA.string_%lu",
				prefix?"tns:":"",
 				s->bound());
 			return &buf[0];
	 	}
	 	break;
 	case CORBA::dk_Wstring:
 		if (1)
 		{
 			CORBA::WstringDef_ptr s =
 				CORBA::WstringDef::_narrow(type);
	 		sprintf(&buf[0], "%sCORBA.string_%lu",
				prefix?"tns:":"",
	 			s->bound());
	 		return &buf[0];
	 	}
	 	break;
	case CORBA::dk_Sequence:
		if (1)
		{
			CORBA::SequenceDef_ptr s =
				CORBA::SequenceDef::_narrow(type);
			sprintf(&buf[0], "%sCORBA.sequence_%lu_",
				prefix?"tns:":"",
				s->bound());
			return &buf[0] +  getTypename(s->element_type_def(), false);
		}
		break;
	case CORBA::dk_Array:
		if (1)
		{
			CORBA::ArrayDef_ptr a =
				CORBA::ArrayDef::_narrow(type);
			sprintf(&buf[0], "%sCORBA.array_%lu_",
				prefix?"tns:":"",
				a->length());
			return &buf[0] + getTypename(a->element_type_def(), false);
		}
		break;
	
	default:
		if (1)
		{
			CORBA::PrimitiveDef_ptr p = 
				CORBA::PrimitiveDef::_narrow(type);
			return getPrimitiveType(p->type(), prefix);
		}
		break;
	}
}

CodeGenWSDL::Identifier CodeGenWSDL::getPrimitiveType(CORBA::TypeCode_ptr tc, bool prefix)
{
	// -- Section 2.6, primitive types
	switch (tc->kind())
	{
	 case CORBA::tk_boolean:
	 	return (prefix?"xsd:":"") + Identifier("boolean");
	 	break;
	 	
	 case CORBA::tk_char:
	 	return (prefix?"tns:":"") + Identifier("CORBA.char");
	 	break;
	 case CORBA::tk_wchar:
	 	return (prefix?"tns:":"") + Identifier("CORBA.wchar");
	 	break;
	 case CORBA::tk_octet:
	 	return (prefix?"xsd:":"") + Identifier("unsignedByte");
	 	break;
	 	
	 case CORBA::tk_short:
	 	return (prefix?"xsd:":"") + Identifier("short");
	 	break;
	 case CORBA::tk_ushort:
	 	return (prefix?"xsd:":"") + Identifier("unsignedShort");
	 	break;
	 case CORBA::tk_long:
	 	return (prefix?"xsd:":"") + Identifier("int");
	 	break;
	 case CORBA::tk_ulong:
	 	return (prefix?"xsd:":"") + Identifier("unsignedInt");
	 	break;
	 case CORBA::tk_longlong:
	 	return (prefix?"xsd:":"") + Identifier("long");
	 	break;
	 case CORBA::tk_ulonglong:
	 	return (prefix?"xsd:":"") + Identifier("unsignedLong");
	 	break;
	 	
	 case CORBA::tk_float:
	 	return (prefix?"xsd:":"") + Identifier("float");
	 	break;
	 case CORBA::tk_double:
		return (prefix?"xsd:":"") + Identifier("double");
	 	break;
	 case CORBA::tk_longdouble:
	 	assert(0);		//!!! not covered by standard
	 	break;
	 	
	 // A simple string will not have a structure associated
	 case CORBA::tk_string:
	 case CORBA::tk_wstring:
	 	return (prefix?"xsd:":"") + Identifier("string");
	 	break;
	 
	 case CORBA::tk_void:
	 	return "---void---";	// never output into wsdl
	 	break;
	 	
	 case CORBA::tk_except:
	 	return "---exception---"; // never output into wsdl
	 	break;
	 	
	 case CORBA::tk_objref:
	 	// -- Section 2.5
	 	return (prefix?"tns:":"") + Identifier("CORBA.ObjectReference");
	 	break;
	 case CORBA::tk_any:
	 	// -- Section 2.7.8
	 	return (prefix?"tns:":"") + Identifier("CORBA.Any");
	 	break;
	 case CORBA::tk_TypeCode:
	 	// -- Section 2.7.7
	 	return (prefix?"tns:":"") + Identifier("CORBA.TypeCode");
	 	break;
	 
	 case CORBA::tk_null:			//!!! what is this?
	 case CORBA::tk_native:			//!!! what is this?
	 case CORBA::tk_Principal:		//!!! what is this?
	 	cerr << "null, native, Principal unimplemented; sorry!" << endl;
		assert(0);
	 	break;
	 
	 // Stuff we should never see:
	 case CORBA::tk_struct:
	 case CORBA::tk_union:
	 case CORBA::tk_enum:
	 case CORBA::tk_alias:
	 case CORBA::tk_value:
	 case CORBA::tk_value_box:
	 case CORBA::tk_abstract_interface:
	 case CORBA::tk_local_interface:
	 	// These should have been dealt with by our caller
	 case CORBA::tk_fixed:
	 case CORBA::tk_array:
		 // Nasty implicit types - dealt with by caller
		cerr << "caller of this method has not met preconditions" << endl;
		assert(0);
		 
	 default:
		cerr << "Unknown type code" << endl;
	 	assert(0);
	}
	
	return "unreachable due to assertion";
}


//-------------------------------------------------------- Message Generation


void CodeGenWSDL::emitMessages()
{
	// The standard is buggy; they fail to declare this anywhere and have
	// a clash of names for the type and the message. I fix it here.
	o << " <!-- CORBA messages -->\n"
	  << " <message name=\"_exception.CORBA.SystemException\">\n"
	  << "  <part name=\"exception\" type=\"tns:CORBA.SystemException\"/>\n"
	  << " </message>\n"
	  << " <message name=\"CORBA.NoOp\">\n"
	  << " </message>\n"
	  << " <!-- Messages from the IDL -->\n";
	
	for (CORBA::ULong i = 0; i < _idl_objs.length(); i++)
	{
		// We are outputting all the possible message that we might
		// need to send.
		
		CORBA::IRObject_ptr x = _idl_objs[i]->obj;
		switch (x->def_kind())
		{
		 case CORBA::dk_Interface:
		 case CORBA::dk_AbstractInterface:
		 case CORBA::dk_LocalInterface:
		 	o << "\n";
		 	break;
			
		 // An Operation can entail two messages -- Section 2.8.2
		 case CORBA::dk_Operation:
		 	if (1)
			{
				CORBA::OperationDef_ptr op = 
					CORBA::OperationDef::_narrow(x);
				assert (!CORBA::is_nil(op));
				emitOperation(op);
			}
			break;
		
		 // A member variable can entail two messages -- Section 2.8.2.1
		 case CORBA::dk_Attribute:
		 	if (1)
			{
				CORBA::AttributeDef_ptr at = 
					CORBA::AttributeDef::_narrow(x);
				assert (!CORBA::is_nil(at));
				emitAttribute(at);
			}
			break;
		
		 // An exception entails one message
		 case CORBA::dk_Exception:
		 	if (1)
		 	{
				CORBA::ExceptionDef_ptr at = 
					CORBA::ExceptionDef::_narrow(x);
				assert (!CORBA::is_nil(at));
				emitException(at);
			}
		 
		 default:
		 	// noop
		 	break;
		}
	}
}

void CodeGenWSDL::emitPart(const char* name, CORBA::IDLType_var type)
{
	o << "  <part name=\"" << name << "\" type=\""
	  << getTypename(type) << "\"/>\n";
}


//----- Method generators


void CodeGenWSDL::emitException(CORBA::ExceptionDef_ptr e)
{	// -- Section 2.8.5
	Identifier wsdl_name = idlNameToWsdl(e->absolute_name());
	
	o << "\n" // easier to read if seperated from other methods
	  << " <message name=\"_exception." << wsdl_name << "\">\n"
	  << "  <part name=\"exception\" type=\"tns:" << wsdl_name << "\"/>\n"
	  << " </message>\n";
}

void CodeGenWSDL::emitOperation(CORBA::OperationDef_ptr op)
{	// Section 2.8.2
	Identifier wsdl_name = idlNameToWsdl(op->absolute_name());
	
	Operation operation;
	operation.name = wsdl_name.substr(wsdl_name.rfind('.')+1, Identifier::npos);
	
	//!!! contexts are not covered by the CORBA-WSDL spec
	
	CORBA::ExceptionDefSeq_var e = op->exceptions();
	for (CORBA::ULong i = 0; i < e->length(); ++i)
	{	// -- Section 2.8.5
		Identifier ewsdl_name = idlNameToWsdl(e[i]->absolute_name());
		operation.faults.push_back("tns:_exception." + ewsdl_name);
	}
	
	CORBA::ParDescriptionSeq_var p = op->params();
	
	o << " <message name=\"" << wsdl_name << "\">\n";
	for (CORBA::ULong j = 0; j < p->length(); ++j)
	{
		if (p[j].mode == CORBA::PARAM_IN ||
		    p[j].mode == CORBA::PARAM_INOUT)
		{
			emitPart(p[j].name, p[j].type_def);
		}
	}
	o << " </message>\n";
	operation.inputs.push_back("tns:" + wsdl_name);
	
	// Only add a response if not oneway
	if (op->mode() != CORBA::OP_ONEWAY)
	{
		o << " <message name=\"" << wsdl_name << "Response\">\n";
		
		if (op->result_def()->type()->kind() != CORBA::tk_void)
			emitPart("_return", op->result_def());
		
		for (CORBA::ULong k = 0; k < p->length(); ++k)
		{
			if (p[k].mode == CORBA::PARAM_OUT ||
			    p[k].mode == CORBA::PARAM_INOUT)
			{
				emitPart(p[k].name, p[k].type_def);
			}
		}
		o << " </message>\n";
		operation.outputs.push_back("tns:" + wsdl_name + "Response");
	}
	
	_ports[wsdl_name.substr(0, wsdl_name.rfind('.'))]
		.push_back(operation);
}

void CodeGenWSDL::emitAttribute(CORBA::AttributeDef_ptr at)
{	// Section 2.8.2.1
	Identifier wsdl_getname = idlNameToPfxWsdl(at->absolute_name(), "_get_");
	Operation getoperation;
	getoperation.name = wsdl_getname.substr(
		wsdl_getname.rfind('.')+1, Identifier::npos);
	
	CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow(at);
	if (!CORBA::is_nil(ead))
	{
	
		CORBA::ExceptionDefSeq_var e = ead->get_exceptions();
		for (CORBA::ULong i = 0; i < e->length(); ++i)
		{	// -- Section 2.8.5
			Identifier ewsdl_name = idlNameToWsdl(e[i]->absolute_name());
			getoperation.faults.push_back("tns:_exception." + ewsdl_name);
		}
	}
	
	o << " <message name=\"" << wsdl_getname << "Response\">\n";
	emitPart("_return", at->type_def());
	o << " </message>\n";
	getoperation.inputs.push_back("tns:CORBA.NoOp");
	getoperation.outputs.push_back("tns:" + wsdl_getname + "Response");
	
	_ports[wsdl_getname.substr(0, wsdl_getname.rfind('.'))]
		.push_back(getoperation);
	
	if (at->mode() != CORBA::ATTR_READONLY)
	{
		Identifier wsdl_setname = idlNameToPfxWsdl(at->absolute_name(), "_set_");
		Operation setoperation;
		setoperation.name = wsdl_setname.substr(
			wsdl_setname.rfind('.')+1, Identifier::npos);
		
		if (!CORBA::is_nil(ead))
		{
		
			CORBA::ExceptionDefSeq_var e = ead->set_exceptions();
			for (CORBA::ULong i = 0; i < e->length(); ++i)
			{	// -- Section 2.8.5
				Identifier ewsdl_name = idlNameToWsdl(e[i]->absolute_name());
				setoperation.faults.push_back("tns:_exception." + ewsdl_name);
			}
		}
		
		o << " <message name=\"" << wsdl_setname << "\">\n";
		emitPart("value", at->type_def());
		o << " </message>\n";
		setoperation.inputs.push_back("tns:" + wsdl_setname);
		setoperation.outputs.push_back("tns:CORBA.NoOp");
		
		_ports[wsdl_setname.substr(0, wsdl_setname.rfind('.'))]
			.push_back(setoperation);
	}
}


//-------------------------------------------------------- PortType Generation


void CodeGenWSDL::emitPortTypes()
{
	/* Here we need to output each operation an interface provides.
	 * We also have to flatten inheritence. (but not repeat methods)
	 *
	 * First, record all the inheritence relationships.
	 * Second, output operations, using messages recorded previously.
	 */
	 
	typedef std::list<Interface>         Parents;
	typedef std::map<Interface, Parents> Ancestry;
	
	Ancestry ancestry;
	
	// Pass 1: record inheritence
	// -- Section 2.8.4
	for (CORBA::ULong i = 0; i < _idl_objs.length(); ++i)
	{
		CORBA::IRObject_ptr x = _idl_objs[i]->obj;
		switch (x->def_kind())
		{
		 case CORBA::dk_Interface:
		 case CORBA::dk_AbstractInterface:
		 case CORBA::dk_LocalInterface:
		 	if (1)
		 	{
		 		CORBA::InterfaceDef_ptr in = 
		 			CORBA::InterfaceDef::_narrow(x);
		 		
		 		Parents& r = ancestry[in->absolute_name()];
		 		CORBA::InterfaceDefSeq_var bases = 
		 			in->base_interfaces();
		 		
		 		for (CORBA::ULong j = 0; j < bases->length(); ++j)
		 			r.push_back(bases[j]->absolute_name());
		 	}
		 	break;
		 default:
		 	// noop
		 	break;
		}
	}
	
	// Pass 2: output portTypes
	Ancestry::iterator a;
	for (a = ancestry.begin(); a != ancestry.end(); ++a)
	{
		// Traverse the hierarchy for this type to build a map
		// for all it's operations
		typedef std::map<Identifier, Operation*> NameToOps;
		NameToOps nameToOps;
		std::set<Interface> done;
		std::list<Interface> todo;
		
		Identifier wsdl_name = idlNameToWsdl(a->first.c_str());
		
		// Do a simple breadth-first search over the classes
		todo.push_back(a->first);
		while (!todo.empty())
		{
			Identifier process = todo.front();
			todo.pop_front();
			
			// Have we already processed this ancestor?
			if (done.find(process) != done.end())
				continue;
			done.insert(process);
				
			//!!! There is a bug here, but even the C++ idl
			// compiler has it. Suppose you inherit from two
			// interfaces both defining a method 'bar'. If the
			// type signatures are identical, then this should
			// work, and calling either is equivalent.
			// HOWEVER, even if the type signitures differ, this
			// succeeds, although it picked arbitrarily.
			
			// Map all the methods in
			Operations& ops = _ports[idlNameToWsdl(process.c_str())];
			Operations::iterator o;
			for (o = ops.begin(); o != ops.end(); ++o)
			{
				NameToOps::iterator n = nameToOps.find(o->name);
				if (n != nameToOps.end())
					cerr << "warning: " << o->name << " ambiguous in " << a->first << endl;
				else
					nameToOps[o->name] = &(*o);
			}
			
			// Check all ancestors
			Parents& ps = ancestry[process];
			Parents::iterator p;
			for (p = ps.begin(); p != ps.end(); ++p)
				todo.push_back(*p);
		}
		
		// Now, output the portType
		o << "\n <!-- Interface " << a->first.c_str() << " -->\n"
		  << " <portType name=\"" << wsdl_name << "\">\n";
		NameToOps::iterator n;
		for (n = nameToOps.begin(); n != nameToOps.end(); ++n)
		{
			Operation* op = n->second;
			o << "  <operation name=\"" << op->name << "\">\n";
			
			// cerr << wsdl_name << " - " << op->name << endl;
			
			Inputs::iterator i;
			for (i = op->inputs.begin(); i != op->inputs.end(); ++i)
				o << "   <input message=\"" << *i << "\"/>\n";
			
			Outputs::iterator x;
			for (x = op->outputs.begin(); x != op->outputs.end(); ++x)
				o << "   <output message=\"" << *x << "\"/>\n";
				
			Exceptions::iterator f;
			for (f = op->faults.begin(); f != op->faults.end(); ++f)
				o << "   <fault message=\"" << *f << "\"/>\n";
			
			// Everybody's favourite exception
			o << "   <fault message=\"tns:_exception.CORBA.SystemException\"/>\n"
			  << "  </operation>\n";
		}
		o << " </portType>\n";
		
		
		o << " <binding name=\"" << wsdl_name << "Binding\" type=\"tns:" << wsdl_name << "\">\n"
		  << "  <soap:binding style=\"rpc\" transport=\"http://schemas.xmlsoap.org/soap/http\"/>\n";
		for (n = nameToOps.begin(); n != nameToOps.end(); ++n)
		{
			Operation* op = n->second;
			o << "  <operation name=\"" << op->name << "\">\n"
			  << "   <soap:operation soapAction=\""
			  << wsdl_name << "#" << op->name << "\"/>\n"
			  << "   <input>";
			
			if (_params.wsi_wsdl)
				o << "<soap:body namespace=\"" << wsdl_name << "\" use=\"literal\"/>";
			else	o << "<soap:body encodingStyle=\"" << ns_soapenc << "\" "
			          << "namespace=\"" << wsdl_name << "\" use=\"encoded\"/>";
				
			o << "</input>\n"
			  << "   <output>";
			  
			if (_params.wsi_wsdl)
				o << "<soap:body namespace=\"" << wsdl_name << "\" use=\"literal\"/>";
			else	o << "<soap:body encodingStyle=\"" << ns_soapenc << "\" "
			          << "namespace=\"" << wsdl_name << "\" use=\"encoded\"/>";
			
			o << "</output>\n";
			  
			Exceptions::iterator f;
			for (f = op->faults.begin(); f != op->faults.end(); ++f)
				o << "   <fault><soap:fault namespace=\"" 
				  << wsdl_name << "\" use=\"literal\" name=\""
				  << *f << "\"/></fault>\n";
			
			// Everybody's favourite exception
			o << "   <fault><soap:fault namespace=\""
			  << wsdl_name << "\" use=\"literal\" name=\""
			  << "tns:_exception.CORBA.SystemException\"/></fault>\n"
			  << "  </operation>\n";
		}
		o << " </binding>\n";
	}
}


//-------------------------------------------------------- PortType Generation


void CodeGenWSDL::emitServices()
{
	if (_params.wsdl_map == "") return;
	
	std::ifstream map(_params.wsdl_map.c_str());
	if (!map.is_open())
	{
		cerr << "Failed to open " << _params.wsdl_map << endl;
		cerr << "Skipping WSDL service output" << endl;
		return;
	}
	
	Identifier word;
	if (map >> word) do
	{
		if (word != "service")
		{
			cerr << "WSDL Map should start with keyword 'service'" << endl;
			break;
		}
		
		Identifier name, ir;
		if (!(map >> name >> word >> ir))
		{
			cerr << "Premature end of WSDL Map" << endl;
			break;
		}
		
		if (word != "at")
		{
			cerr << "keyword 'at' should follow service name" << endl;
			break;
		}
		
		o << " <service name=\"" << name << "\">\n";
		if (map >> word) do
		{
			if (word == "service") break;
			if (word != "name")
			{
				cerr << "keyword 'name' should follow service definition" << endl;
				break;
			}
			
			Identifier obj;
			if (!(map >> obj >> word))
			{
				cerr << "Premature end to object description" << endl;
				break;
			}
			
			if (word != "implements")
			{
				cerr << "keyword 'implements' missing after object name" << endl;
				break;
			}
			
			while (map >> word)
			{
				if (word == "name") break;
				if (word == "service") break;
				
				// Prune trailing comma.
				if (word[word.size()-1] == ',')
					word.resize(word.size()-1);
				
				o << "  <port name=\"" 
				  << obj << "\" binding=\"tns:"
				  << idlNameToWsdl(word.c_str()-2) << "Binding\">\n"
				  << "   <soap:address location=\""
				  << ir << obj << "\"/>\n"
				  << "  </port>\n";
			}
		} while (word == "name");
		
		o << " </service>\n";
	}
	while (word == "service");
}


//-------------------------------------------------------- Utilities


CodeGenWSDL::Identifier CodeGenWSDL::idlNameToWsdl(const char* corba_name)
{
	Identifier wsdl_name;
	for (corba_name += 2; *corba_name; ++corba_name)
	{
		if (*corba_name == ':')
		{
			wsdl_name.push_back('.');
			++corba_name;
		}
		else
		{
			wsdl_name.push_back(*corba_name);
		}
	}
	
	return wsdl_name;
}

CodeGenWSDL::Identifier CodeGenWSDL::idlNameToPfxWsdl(const char* corba_name, const char* pfx)
{
	Identifier wsdl_name = idlNameToWsdl(corba_name);
	Identifier::size_type dot = wsdl_name.rfind('.');
	
	if (dot == Identifier::npos)
		dot = 0;
	else	++dot;
	
	wsdl_name.insert(dot, pfx);
	return wsdl_name;
}
