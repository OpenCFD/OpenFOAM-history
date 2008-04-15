/*

    Copyright (C) 1998 Stefan Westerfeld
                       stefan@space.twc.de

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Adapted by Arno Puder.
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
#include "codegen-c++-impl.h"
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH

#define _IMPL "_impl"


using namespace std;

CodeGenCPPImpl::CodeGenCPPImpl( DB &db, IDLParam &params,
				CORBA::Container_ptr con )
  : CodeGen( con ),
    CodeGenCPPUtil( db, params, con )
{
}



void CodeGenCPPImpl::emit_impl_h()
{
  headers = true;
  emit_impl();
}

void CodeGenCPPImpl::emit_impl_cpp()
{
  headers = false;
  emit_impl();
}

void CodeGenCPPImpl::emit_impl()
{
  bool old_rel_names = use_rel_names( false );

  for( CORBA::ULong i = 0; i < _idl_objs.length(); i++ ) {
    
    CORBA::InterfaceDef_var in;
    in = CORBA::InterfaceDef::_narrow( _idl_objs[ i ]->obj );
    if( CORBA::is_nil( in ) || in->def_kind() != CORBA::dk_Interface )
      continue;
    
    if( _idl_objs[ i ]->iface_as_forward )
      continue;
    
    if( check_for_included_defn( in ) )
      continue;

    CORBA::String_var absname = in->absolute_name ();
    if (_db->is_in_forward_dcl (absname.in())) {
      continue;
    }
    
    emit_Interface( in );
  }

  use_rel_names( old_rel_names );
}


void CodeGenCPPImpl::emit_impl_here(CORBA::IDLType_var  type_id)
{
    CORBA::TypeCode_var tc;
    CORBA::String_var tmp;
    string abs_name;

    if (!CORBA::is_nil (type_id))
      tc = type_id->type();
    else
      tc = CORBA::_tc_void;
    
    // implementation not in header ...
    if(headers)
    {
        o << ";" << endl;
    }
    else
    {
        o << endl << BL_OPEN;
        if(tc->kind() != CORBA::tk_void) {
            emit_type_for_result (type_id);
            o << " retval;" << endl  << endl;
        }
    
        o << "// add your implementation here" << endl;
        o << "  // REMOVE  " << endl 
          << "  mico_throw(::CORBA::NO_IMPLEMENT());" << endl 
          << "  // REMOVE " << endl  << endl;

        if(tc->kind() != CORBA::tk_void)
            o << "return retval; " << endl;

        o << BL_CLOSE << endl;
    }
}
void CodeGenCPPImpl::emitPrototypes( CORBA::InterfaceDef_ptr in,
				     bool as_pure_virtual )
{
    CORBA::String_var tmp;
    tmp = in->name();
    string implClassName="";
    if(!headers)
    {
        implClassName = ID(tmp);
        implClassName += _IMPL;
        implClassName += "::";
    }

    CORBA::Container::DescriptionSeq_var dsv =
      in->describe_contents (CORBA::dk_Attribute, TRUE, -1);

    // Emit attributes
    for( CORBA::ULong j = 0; j < dsv->length(); j++ ) {
        const CORBA::AttributeDescription * attr;
	CORBA::Boolean r = (dsv[j].value >>= attr);
	assert (r);
        if( as_pure_virtual )
            o << "virtual ";
        CORBA::IDLType_var type_id = lookup_attribute_by_id( attr->id.in() );
        emit_type_for_result( type_id );
        o << " " << implClassName << ID( attr->name ) << "()";
        if( as_pure_virtual )
            o << " = 0";
        emit_impl_here(type_id);

        if( attr->mode == CORBA::ATTR_NORMAL ) {
            if( as_pure_virtual )
                o << "virtual ";
            o << "void " << implClassName << ID( attr->name ) << "( ";
            emit_type_for_param( type_id, CORBA::PARAM_IN );
            o << " _new_value )";
            if( as_pure_virtual )
                o << " = 0";
            emit_impl_here(CORBA::IDLType::_nil());
        }
    }

    dsv = in->describe_contents (CORBA::dk_Operation, TRUE, -1);

    // Emit operations
    for( CORBA::ULong j0 = 0; j0 < dsv->length(); j0++ ) {
        const CORBA::OperationDescription * op;
	CORBA::Boolean r = (dsv[j0].value >>= op);
	assert (r);
        if( as_pure_virtual )
            o << "virtual ";
        CORBA::IDLType_var type_id = lookup_result_by_id( op->id.in() );
        CORBA::TypeCode_var tc = type_id->type();
        CORBA::Contained_var con;
        CORBA::String_var tmp;
        string ex_abs_name;

	o << endl; 
        emit_type_for_result( type_id );
        if(headers)
	    o << " ";
	else
	    o << endl; 

        o << implClassName << ID(op->name) << "(";
        
        CORBA::ParDescriptionSeq p = op->parameters;
        CORBA::ULong k;
        for( k = 0; k < p.length(); k++ ) {
            if( k != 0 )
                o << ",";
            o << " ";
            emit_type_for_param( p[ k ].type_def, p[ k ].mode );
            o << " " << ID(p[ k ].name);
        }

        // Do we have a context?
        const CORBA::ContextIdSeq& ctx = op->contexts;
    
        if( ctx.length() > 0 ) {
            if( p.length() > 0 )
                o << ", ";
            o << "CORBA::Context_ptr _ctx";
        }
    
        if( k != 0 || ctx.length() > 0 )
            o << " ";
        o << ")";

        // exceptions
        if(!_params.no_exceptions) {

            const CORBA::ExcDescriptionSeq& ex = op->exceptions;
            
	    o << endl << indent << "throw(" << endl;
	    o << indent;
	    o << "::CORBA::SystemException";
#ifndef HAVE_STD_EH
	    o << "_var";
#endif
	    for( k = 0; k < ex.length(); k++ ) {
		o << "," << endl;
		con = _repo->lookup_id(ex[k].id);
		tmp = con->absolute_name();
		o << ID(tmp);
#ifndef HAVE_STD_EH
		o << "_var";
#endif
	    }
	    o << ")" << endl << exdent << exdent;
        }
    
        if( as_pure_virtual )
            o << " = 0";
        emit_impl_here(type_id);
    }
}

void CodeGenCPPImpl::emit_Interface( CORBA::InterfaceDef_ptr in )
{
  assert( in->def_kind() == CORBA::dk_Interface );
  set_name_prefix( in );
  
  CORBA::String_var n = in->name();
  string className = (const char *) ID(n);
  string skelName;

  if(!_params.poa) {
      skelName = (const char *) ID(in->absolute_name());
      skelName = skelName.substr(2) + _SKEL;
  } else {
      skelName = (const char *) ID(in->absolute_name());
      skelName =  "POA_" + skelName.substr(2) ;
  }
 
  o << endl;

  //-----IMPL--------------------------------------------------------
  o << "// Implementation for interface " << className << endl;

  if(headers)
  {
    // note: our skeleton must be the rightmost entry in the inheritance list!
    o << "class " << className << _IMPL << " : ";
  
    CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
    if( bases->length() == 0 ) {
      // This interface has no base interfaces
      o << "virtual public " << skelName;
      o << endl;
    } else {
      // This interface *has* base interfaces
      o << indent << endl;
      for( CORBA::ULong i = 0; i < bases->length(); i++ ) {
        o << "virtual public ";
        CORBA::String_var base_name = bases[ i ]->name();
        o << ID(base_name);
        o << _IMPL;
	o << ",";
        o << endl;
      }
      o << "virtual public " << skelName << endl;
      o << exdent;
    }
  
    o << BL_OPEN;
  
    o << "public:" << indent << endl;
  }
  
  // Generate prototypes
  emitPrototypes( in, false );
 
  if(headers)
  { 
    // Generate epilogue
    o << exdent << BL_CLOSE_SEMI << endl;
  }
}
