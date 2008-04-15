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
#include "codegen-c++-skel.h"
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

CodeGenCPPSkel::CodeGenCPPSkel( DB &db, IDLParam &params,
				CORBA::Container_ptr con )
  : CodeGen( con ),
    CodeGenCPPUtil( db, params, con )
{
}


/*
 * Get the list of operations and attributes which we need to
 * include in the dispatcher of this interface. Normally these
 * are only the operations and attributes found in this interface,
 * but with the IDL Compiler option --gen-full-dispatcher this
 * also includes all inherited operations and attributes.
 */
void CodeGenCPPSkel::get_dispatcher_list( CORBA::InterfaceDef_ptr in,
					  CORBA::OpDescriptionSeq &ops,
					  CORBA::AttrDescriptionSeq &attr )
{
    CORBA::ContainedSeq_var c;

    // Get all operations
    c = in->contents( CORBA::dk_Operation, !_params.gen_full_dispatcher );
    for( CORBA::ULong i0 = 0; i0 < c->length(); i0++ ) {
      CORBA::ULong j = ops.length();
      ops.length( j + 1 );
      CORBA::Contained::Description_var d = c[i0]->describe();
      CORBA::Boolean check;
      check = d->value >>= ops[j];
      assert(check);
    }
  
    // Get all attributes
    c = in->contents( CORBA::dk_Attribute, !_params.gen_full_dispatcher );
    for( CORBA::ULong i1 = 0; i1 < c->length(); i1++ ) {
      CORBA::ULong j = attr.length();
      attr.length( j + 1 );
      CORBA::Contained::Description_var d = c[i1]->describe();
      CORBA::Boolean check;
      check = d->value >>= attr[j];
      assert(check);
    }
}

/*
 * determine the smallest prime number >= n.
 */
CORBA::ULong
CodeGenCPPSkel::get_prime_number (CORBA::ULong n)
{
  if (n <= 3)
    return n;

  /*
   * for each n there is a prime number p with n <= p <= 2*n
   * (see sec. 22.3 in Hardy & Wright: "An introduction
   * to the theory of numbers").
   *
   * we use an optimized version of the sieve of
   * Eratosthenes for the numbers 1...2*n that
   * does not consider even numbers. then we look
   * for the first unmarked number in n...2*n, which
   * is the smallest prime number >= n.
   */
  char *c = new char[n+1];
  memset (c, 0, n+1);

  CORBA::ULong i;
  for (i = 3; i*i <= 2*n; ) {
    for (CORBA::ULong j = 3*i; j <= 2*n; j += 2*i)
      c[j/2] = 1;
    while (c[(i+=2)/2])
      ;
  }
  for (i = n|1; c[i/2]; i+=2)
    ;
  delete[] c;
  return i;
}

struct OpKey {
  CORBA::ULong hash;
  enum Kind { Op, AttrRead, AttrWrite } kind;
  CORBA::ULong index;

  OpKey (CORBA::ULong h, Kind k, CORBA::ULong i)
    : hash(h), kind(k), index(i)
  {
  }
  bool operator< (const OpKey &k) const
  {
    if (hash != k.hash)
      return hash < k.hash;
    if (kind != k.kind)
      return kind < k.kind;
    return index < k.index;
  }
};

void CodeGenCPPSkel::emit_dispatcher (CORBA::OpDescriptionSeq &ops,
				      CORBA::AttrDescriptionSeq &attr)
{
    CORBA::ULong count = attr.length() + ops.length();
    for( CORBA::ULong j3 = 0; j3 < attr.length(); j3++ ) {
      if (attr[j3].mode == CORBA::ATTR_NORMAL)
	++count;
    }
    if (count <= 3) {
      // Generate dispatcher for attributes
      for( CORBA::ULong j0 = 0; j0 < attr.length(); j0++ ) {
	emit_attr_dispatcher (attr[j0]);
      }
      
      // Generate dispatcher for methods
      for( CORBA::ULong j1 = 0; j1 < ops.length(); j1++ ) {
	CORBA::OperationDescription op;
	op = ops[ j1 ];
	string dispatch_name = (const char *) op.name;
	CORBA::IDLType_var idl_type = lookup_result_by_id( op.id );
	emit_sii_method_dispatcher( dispatch_name, op, idl_type );
      }
      return;
    }

    /*
     * generate some kind of hash table. instead of an array we
     * use a switch statement to access the right "hash bucket".
     * Sane C++ compilers will generate a jump table for the
     * switch, so accessing the hash bucket is as fast as using
     * an array. in case of a collision we have multiple methods
     * per switch case.
     *
     * we first sort the methods by ascending hash key and then
     * emit the whole thing in a second pass.
     */

    // make the hash table a bit to large so we dont get that many collisions
    CORBA::ULong p = get_prime_number (count + count/2);

    set<OpKey, less<OpKey> > sorted;

    for( CORBA::ULong j0 = 0; j0 < attr.length(); j0++ ) {
      string name = string("_get_") + attr[j0].name.in();
      sorted.insert (OpKey (mico_string_hash (name.c_str(), p),
			    OpKey::AttrRead, j0));
      if (attr[j0].mode == CORBA::ATTR_NORMAL) {
	string name = string("_set_") + attr[j0].name.in();
	sorted.insert (OpKey (mico_string_hash (name.c_str(), p),
			      OpKey::AttrWrite, j0));
      }
    }
    for( CORBA::ULong j1 = 0; j1 < ops.length(); j1++ ) {
      sorted.insert (OpKey (mico_string_hash (ops[j1].name, p),
			    OpKey::Op, j1));
    }

    o << "switch (mico_string_hash (__req->op_name(), " << p << ")) "
      << BL_OPEN;

    set<OpKey, less<OpKey> >::iterator j2;
    CORBA::ULong current_hash = p;
    for (j2 = sorted.begin(); j2 != sorted.end(); ++j2) {
      if (current_hash != (*j2).hash) {
	if (current_hash != p)
	  o << "break;" << endl;
	current_hash = (*j2).hash;
	o << exdent << "case " << current_hash << ":" << indent << endl;
      }
      switch ((*j2).kind) {
      case OpKey::Op: {
	CORBA::OperationDescription op;
	op = ops[ (*j2).index ];
	string dispatch_name = (const char *) op.name;
	CORBA::IDLType_var idl_type = lookup_result_by_id( op.id );
	emit_sii_method_dispatcher( dispatch_name, op, idl_type );
	break;
      }
      case OpKey::AttrRead:
	emit_attr_dispatcher (attr[ (*j2).index ], CORBA::PARAM_OUT);
	break;
      case OpKey::AttrWrite:
	emit_attr_dispatcher (attr[ (*j2).index ], CORBA::PARAM_IN);
	break;
      default:
	assert (0);
      }
    }
    o << "break;" << endl;
    o << BL_CLOSE;
}

void CodeGenCPPSkel::emit_skel()
{
  CORBA::IDLType_var idl_type;
  CORBA::String_var tmp;
  
  for( CORBA::ULong i0 = 0; i0 < _idl_objs.length(); i0++ ) {

    CORBA::InterfaceDef_var in;
    CORBA::ValueDef_var val;
    CORBA::Contained_var con;
    CORBA::Container_var c;
    CORBA::OpDescriptionSeq operations;
    CORBA::AttrDescriptionSeq attributes;
    
    in = CORBA::InterfaceDef::_narrow( _idl_objs[ i0 ]->obj );
    val = CORBA::ValueDef::_narrow( _idl_objs[ i0 ]->obj );
    con = CORBA::Contained::_narrow( _idl_objs[ i0 ]->obj );
    c = CORBA::Container::_narrow( _idl_objs[ i0 ]->obj );

    if( CORBA::is_nil( in ) && CORBA::is_nil( val ) )
      continue;

    if( _idl_objs[ i0 ]->iface_as_forward )
      continue;
    
    if( check_for_included_defn( in ) )
      continue;

    if (_idl_objs[i0]->obj->def_kind() == CORBA::dk_LocalInterface)
      continue;
    
    if (!CORBA::is_nil (con)) {
      CORBA::String_var an = con->absolute_name ();
      if (_db->is_implicit (an.in())) {
	continue;
      }
    }
    
    if( !CORBA::is_nil( val ) ) {
      CORBA::InterfaceDefSeq_var ids = val->supported_interfaces ();
      for (CORBA::ULong j=0; j<ids->length(); j++) {
	if (ids[j]->def_kind() == CORBA::dk_Interface) {
	  in = CORBA::InterfaceDef::_duplicate (ids[j]);
	  break;
	}
      }
      if (CORBA::is_nil( in )) {
	continue;
      }
    }

    CORBA::String_var absname = con->absolute_name ();
    if (_db->is_in_forward_dcl (absname.in())) {
      continue;
    }

    set_name_prefix( c );
    tmp = con->name();
    string className;
    className = ID(tmp);
    tmp = con->absolute_name();
    string absClassName;
    absClassName = ID(tmp);
    absClassName = absClassName.substr( 2 );
    CORBA::String_var in_id = in->id();

    if (!CORBA::is_nil (in)) {
      get_dispatcher_list( in, operations, attributes );
    }

    /*
     * PortableServer Skeleton class
     */

    if (_params.poa) {
      string prefix;
      CORBA::Container_var def_in = con->defined_in ();
      if (def_in->def_kind() == CORBA::dk_Repository) {
	prefix = (const char *) "POA_";
      }
      else {
	prefix = (const char *) "";
      }

      bool oldurn = use_rel_names (false);
      
      o << endl;
      o << "// PortableServer Skeleton Class for interface " 
	<< absClassName << endl;
      
      /*
       * virtual destructor
       */
      o << "POA_" << absClassName << "::~" << prefix << className << "()"
	<< endl;
      o << BL_OPEN << BL_CLOSE << endl;

      /*
       * _this() method
       */
      emit_idl_type_name (in);
      o << endl << "POA_" << absClassName << "::_this ()" << endl;
      o << BL_OPEN;

      // XXX win32 hack
      o << "CORBA::Object_var obj = PortableServer::ServantBase::_this();" << endl;
      o << "return ";
      emit_type_name (in);
      o << "::_narrow (obj);" << endl;
      o << BL_CLOSE << endl;

      /*
       * _is_a() method
       */
      o << "CORBA::Boolean" << endl;
      o << "POA_" << absClassName << "::_is_a (const char * repoid)" << endl;
      o << BL_OPEN;
      if (CORBA::is_nil (val)) {
	// Check interface bases
	o << "if (strcmp (repoid, \"" << in_id << "\") == 0) " << BL_OPEN;
	o << "return TRUE;" << endl;
	o << BL_CLOSE;
	CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
	for (CORBA::ULong j2=0; j2<bases->length(); j2++) {
	  tmp = bases[j2]->absolute_name();
	  string base_name;
	  base_name = ID(tmp);
	  base_name = base_name.substr( 2 );
	  o << "if (POA_" << base_name << "::_is_a (repoid)) " << BL_OPEN;
	  o << "return TRUE;" << endl;
	  o << BL_CLOSE;
	}
	if (_params.reflection) {
	  o << "if (POA_Reflection::IFRProvider::_is_a (repoid)) " << BL_OPEN;
	  o << "return TRUE;" << endl;
	  o << BL_CLOSE;
	}
      }
      else {
	/*
	 * Check Valuetype inheritance: supported concrete interface, and the
	 * base classes supported concrete interface
	 */
	tmp = in->absolute_name();
	string base_name;
	base_name = ID(tmp);
	base_name = base_name.substr( 2 );
	o << "if (POA_" << base_name << "::_is_a (repoid)) " << BL_OPEN;
	o << "return TRUE;" << endl;
	o << BL_CLOSE;

	CORBA::ValueDef_var base_value = val->base_value ();
	CORBA::InterfaceDefSeq_var base_supported;

	if (!CORBA::is_nil (base_value)) {
	  base_supported = base_value->supported_interfaces ();
	  for (CORBA::ULong i=0; i<base_supported->length(); i++) {
	    if (base_supported[i]->def_kind() == CORBA::dk_Interface) {
	      tmp = base_value->absolute_name();
	      base_name = ID(tmp);
	      base_name = base_name.substr (2);
	      o << "if (POA_" << base_name << "::_is_a (repoid)) " << BL_OPEN;
	      o << "return TRUE;" << endl;
	      o << BL_CLOSE;
	      break;
	    }
	  }
	}
      }
      o << "return FALSE;" << endl;
      o << BL_CLOSE << endl;

      /*
       * _get_interface() method
       */
      o << "CORBA::InterfaceDef_ptr" << endl;
      o << "POA_" << absClassName << "::_get_interface ()" << endl;
      o << BL_OPEN;
      // XXX Win32 hack
      o << "CORBA::InterfaceDef_ptr ifd = "
        << "PortableServer::ServantBase::_get_interface "
        << "(\"" << in_id << "\");"
	<< endl << endl;
      o << "if (CORBA::is_nil (ifd)) " << BL_OPEN;
      o << "mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));" << endl;
      o << BL_CLOSE << endl;
      o << "return ifd;" << endl;
      o << BL_CLOSE << endl;
      
      /*
       * _primary_interface() method
       */
      o << "CORBA::RepositoryId" << endl;
      o << "POA_" << absClassName << "::_primary_interface";
      o <<   " (const PortableServer::ObjectId &,";
      o <<    " PortableServer::POA_ptr)" << endl;
      o << BL_OPEN;
      o << "return CORBA::string_dup (\"" << in_id << "\");" << endl;
      o << BL_CLOSE << endl;

      if (_params.reflection) {
	try {
	  CORBA::ExtInterfaceDef_var eid = CORBA::ExtInterfaceDef::_narrow (in);
	  if (CORBA::is_nil (eid)) {
	    throw string ("oops");
	  }

	  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var efid =
	    eid->describe_ext_interface ();
	  CORBA::Any aefid;
	  aefid <<= efid.in();

	  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
	  if (CORBA::is_nil (orb)) {
	    throw string ("oops");
	  }

	  CORBA::Object_var oc = orb->resolve_initial_references ("CodecFactory");
	  IOP::CodecFactory_var cf = IOP::CodecFactory::_narrow (oc);
	  if (CORBA::is_nil (cf)) {
	    throw string ("oops");
	  }

	  IOP::Encoding enc;
	  enc.format = IOP::ENCODING_CDR_ENCAPS;
	  enc.major_version = 1;
	  enc.minor_version = 2;
	  IOP::Codec_var codec = cf->create_codec (enc);

	  CORBA::OctetSeq_var data = codec->encode_value (aefid);

	  o << "CORBA::Any *" << endl;
	  o << "POA_" << absClassName << "::omg_get_ifr_metadata ()" << endl;
	  o << BL_OPEN;

	  const CORBA::ULong maxOctetsPerArray = 16384;
	  CORBA::ULong numArrays = (data->length() + maxOctetsPerArray - 1) / maxOctetsPerArray;
	  CORBA::ULong lastDimOctets = data->length() - (numArrays - 1) * maxOctetsPerArray;

	  for (CORBA::ULong aidx=0; aidx<numArrays; aidx++) {
	    CORBA::ULong dimOctets = (aidx+1<numArrays) ? maxOctetsPerArray : lastDimOctets;

	    o << "static CORBA::Octet rawdata_" << aidx << "[" << dimOctets << "] = " << BL_OPEN;

	    for (CORBA::ULong didx=0; didx<dimOctets; didx++) {
	      o << (int) data[didx+aidx*maxOctetsPerArray];

	      if (didx+1 < dimOctets) {
		o << ",";
	      }
	      if (didx+1 == dimOctets || ((didx+1) % 16) == 0) {
		o << endl;
	      }
	      else {
		o << " ";
	      }
	    }
	    o << BL_CLOSE_SEMI;
	  }
	  o << endl;

	  o << "static CORBA::OctetSeq data;" << endl;
	  o << endl;
	  o << "if (!data.length()) " << BL_OPEN;
	  o << "data.length (" << data->length() << ");" << endl;
	  o << "CORBA::Octet * dptr = data.get_buffer ();" << endl;
	  for (CORBA::ULong aidx2=0; aidx2<numArrays; aidx2++) {
	    CORBA::ULong dimOctets = (aidx2+1<numArrays) ? maxOctetsPerArray : lastDimOctets;
	    o << "memcpy (dptr";
	    if (aidx2) {
	      o << "+" << aidx2*maxOctetsPerArray;
	    }
	    o << ", rawdata_" << aidx2 << ", " << dimOctets << ");" << endl;
	  }
	  o << BL_CLOSE << endl;
	  o << "try " << BL_OPEN;
	  o << "CORBA::ORB_var orb = CORBA::ORB_instance (\"mico-local-orb\");" << endl;
	  o << "if (CORBA::is_nil (orb)) " << BL_OPEN;
	  o << "throw Reflection::FormatNotSupported ();" << endl;
	  o << BL_CLOSE;
	  o << "CORBA::Object_var oc = orb->resolve_initial_references (\"CodecFactory\");" << endl;
	  o << "IOP::CodecFactory_var cf = IOP::CodecFactory::_narrow (oc);" << endl;
	  o << "if (CORBA::is_nil (cf)) " << BL_OPEN;
	  o << "throw Reflection::FormatNotSupported ();" << endl;
	  o << BL_CLOSE;
	  o << "IOP::Encoding enc;" << endl;
	  o << "enc.format = IOP::ENCODING_CDR_ENCAPS;" << endl;
	  o << "enc.major_version = 1;" << endl;
	  o << "enc.minor_version = 2;" << endl;
	  o << "IOP::Codec_var codec = cf->create_codec (enc);" << endl;
	  o << "return codec->decode_value (data, CORBA::InterfaceAttrExtension::_tc_ExtFullInterfaceDescription);" << endl;
	  o << BL_CLOSE;
	  o << "catch (const Reflection::FormatNotSupported &) " << BL_OPEN;
	  o << "throw;" << endl;
	  o << BL_CLOSE;
	  o << "catch (...) " << BL_OPEN;
	  o << "throw Reflection::FormatNotSupported ();" << endl;
	  o << BL_CLOSE;
	  o << BL_CLOSE << endl;
	}
	catch (...) {
	  /*
	   * Couldn't get or encode the ExtFullInterfaceDescription for this interface.
	   */

	  o << "CORBA::Any *" << endl;
	  o << "POA_" << absClassName << "::omg_get_ifr_metadata ()" << endl;
	  o << BL_OPEN;
	  o << "throw Reflection::FormatNotSupported ();" << endl;
	  o << BL_CLOSE << endl;
	}

	o << "char *" << endl;
	o << "POA_" << absClassName << "::omg_get_xml_metadata ()" << endl;
	o << BL_OPEN;
	o << "throw Reflection::FormatNotSupported ();" << endl;
	o << BL_CLOSE << endl;
      }

      /*
       * _narrow_helper() and _narrow() moved to stub
       */

      /*
       * _make_stub method
       */
      if (_params.poa_stubs) {
	o << "CORBA::Object_ptr" << endl;
	o << "POA_" << absClassName << "::_make_stub"
	  <<   " (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)"
	  << endl;
	o << BL_OPEN;
	o << "return new ";
	emit_type_name (in);
	o << _STUB << "_clp (poa, obj);" << endl;
	o << BL_CLOSE << endl;
      }

      /*
       * method dispatcher
       */
      o << "bool" << endl;
      o << "POA_" << absClassName << "::dispatch (";
      o << "CORBA::StaticServerRequest_ptr";
      o << " __req)" << endl;
      o << BL_OPEN;

      /*
       * A valuetype has no methods to dispatch
       */

      if (CORBA::is_nil (val)) {
	if( !_params.no_exceptions &&
	    attributes.length() + operations.length() > 0 ) {
	  o << "#ifdef HAVE_EXCEPTIONS" << endl;
	  o << "try {" << endl;
	  o << "#endif" << indent << endl;
	}
	
	emit_dispatcher (operations, attributes);
	
	if( !_params.no_exceptions &&
	    attributes.length() + operations.length() > 0 ) {
	  o << exdent << "#ifdef HAVE_EXCEPTIONS" << endl;
	  o << "} catch( CORBA::SystemException_catch &_ex ) " << BL_OPEN;
	  o << "__req->set_exception( _ex->_clone() );" << endl;
	  o << "__req->write_results();" << endl;
	  o << "return true;" << exdent << endl;
	  o << "} catch( ... ) " << BL_OPEN;
	  o << "CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);" << endl;
	  o << "__req->set_exception (_ex->_clone());" << endl;
	  o << "__req->write_results ();" << endl;
	  o << "return true;" << endl;
	  o << BL_CLOSE;
	  o << "#endif" << endl;
	}
	o << endl;
      }

      // call base classes' dispatchers

      if (CORBA::is_nil (val)) {
	CORBA::InterfaceDefSeq_var bases = in->base_interfaces();
	for (CORBA::ULong j5=0; j5<bases->length(); j5++) {
	  tmp = bases[j5]->absolute_name();
	  string base_name;
	  base_name = ID(tmp);
	  base_name = base_name.substr( 2 );
	  o << "if (POA_" << base_name << "::dispatch (__req)) " << BL_OPEN;
	  o << "return true;" << endl;
	  o << BL_CLOSE << endl;
	}
	if (_params.reflection) {
	  o << "if (POA_Reflection::IFRProvider::dispatch (__req)) " << BL_OPEN;
	  o << "return true;" << endl;
	  o << BL_CLOSE << endl;
	}
      }
      else {
	string base_name;
	tmp = in->absolute_name();
	base_name = ID(tmp);
	base_name = base_name.substr( 2 );
	o << "if (POA_" << base_name << "::dispatch (__req)) ";

	o << BL_OPEN << "return true;" << endl;
	o << BL_CLOSE << endl;

	CORBA::ValueDef_var base_value = val->base_value ();
	CORBA::InterfaceDefSeq_var base_supported;

	if (!CORBA::is_nil (base_value)) {
	  base_supported = base_value->supported_interfaces ();
	  for (CORBA::ULong i=0; i<base_supported->length(); i++) {
	    if (base_supported[i]->def_kind() == CORBA::dk_Interface) {
	      tmp = base_supported[i]->absolute_name();
	      base_name = ID(tmp);
	      base_name = base_name.substr (2);
	      o << "if (POA_" << base_name << "::dispatch (__req)) " << BL_OPEN;
	      o << "return TRUE;" << endl;
	      o << BL_CLOSE;
	    }
	  }
	}
      }
      o << "return false;" << endl;
      o << BL_CLOSE << endl;
      
      // invoke() method
      o << "void" << endl;
      o << "POA_" << absClassName << "::invoke (";
      o << "CORBA::StaticServerRequest_ptr";
      o << " __req)" << endl;
      o << BL_OPEN;
      o << "if (dispatch (__req)) " << BL_OPEN;
      o << "  return;" << endl;
      o << BL_CLOSE;
      o << endl;
      o << "CORBA::Exception * ex = " << endl;
      o << "  new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);" << endl;
      o << "__req->set_exception (ex);" << endl;
      o << "__req->write_results();" << endl;
      o << BL_CLOSE << endl;
      use_rel_names (oldurn);
    }
  }
}

void
CodeGenCPPSkel::emit_attr_dispatcher (CORBA::AttributeDescription &attr,
				      CORBA::Flags which)
{
  CORBA::OperationDescription op;
  CORBA::IDLType_var idl_type;
  string dispatch_name;

  CORBA::Contained_var con = _repo->lookup_id (attr.id);
  CORBA::AttributeDef_var a = CORBA::AttributeDef::_narrow (con);
  CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (a);
  CORBA::ExtAttributeDescription_var eads;
  assert (!CORBA::is_nil (a));

  if (!CORBA::is_nil (ead)) {
    eads = ead->describe_attribute();
  }

  idl_type = a->type_def();

  if( which == CORBA::PARAM_OUT || which == CORBA::PARAM_INOUT ) {
    op.name = (const char *) attr.name;
    op.id = (const char *) "";  // Not needed
    op.defined_in = attr.defined_in;
    op.version = (const char*) attr.version;
    op.result = CORBA::TypeCode::_nil();  // Not needed
    op.mode = CORBA::OP_NORMAL;
    // op.contexts not needed for attributes
    op.parameters.length( 0 );  // No parameters for _get

    if (!CORBA::is_nil (ead)) {
      op.exceptions = eads->get_exceptions;
    }

    dispatch_name = string("_get_") + (const char *)attr.name;

    emit_sii_method_dispatcher( dispatch_name, op, idl_type );
  }

  if( ( which == CORBA::PARAM_IN || which == CORBA::PARAM_INOUT ) &&
      attr.mode == CORBA::ATTR_NORMAL ) {
    // We also need the set version of the attribute
    CORBA::ParameterDescription attr_param;
    attr_param.name = (const char *) "_value";
    attr_param.type = attr.type;
    attr_param.type_def = idl_type;
    attr_param.mode = CORBA::PARAM_IN;
    
    op.name = (const char *) attr.name;
    op.id = (const char *) "";  // Not needed
    op.defined_in = attr.defined_in;
    op.version = (const char*) attr.version;
    op.result = CORBA::TypeCode::_nil();  // Not needed
    op.mode = CORBA::OP_NORMAL;
    // op.contexts not needed for attributes
    op.parameters.length( 1 );
    op.parameters[ 0 ] = attr_param;

    if (!CORBA::is_nil (ead)) {
      op.exceptions = eads->set_exceptions;
    }
    
    dispatch_name = string("_set_") + (const char *)attr.name;
    
    emit_sii_method_dispatcher( dispatch_name, op,
				_repo->get_primitive( CORBA::pk_void ) );
  }
}

void
CodeGenCPPSkel::emit_sii_method_dispatcher( string &dispatch_name,
					    CORBA::OperationDescription &op,
					    CORBA::IDLType_ptr result )
{
  o << "if( strcmp( __req->op_name(), \"" << 
    dispatch_name << "\" ) == 0 ) " << BL_OPEN;
  CORBA::ParDescriptionSeq &p = op.parameters;
  CORBA::ULong k;

  // Emit variable declarations for all parameters as well as
  // the wrappers for the array parameters
  for( k = 0; k != p.length(); k++ ) {
    // Declaration for parameter
    if( p[ k ].mode != CORBA::PARAM_OUT ) {
      emit_type_for_variable( p[ k ].type_def, automatic );
      if( !p[k].type->is_array() ) {
	o << " _par_" << ID(p[ k ].name);
	emit_array_suffix( p[ k ].type_def );
#if 0
	if (p[k].mode == CORBA::PARAM_INOUT && is_var_inout (p[k].type_def))
	  o << " ((CORBA::Boolean)FALSE)";
#endif
	o << ";" << endl;
      } else {
	o << "_slice* _par_" << ID(p[ k ].name) << " = ";
	emit_type_for_variable( p[ k ].type_def, automatic );
	o << "_alloc();" << endl;
      }
    } else {
      if( p[k].type->is_array() ) {
	emit_type_for_variable( p[ k ].type_def, automatic );
	o << "_slice* _par_" << ID(p[ k ].name) << " = ";
	emit_type_for_variable( p[ k ].type_def, automatic );
	o << "_alloc();" << endl;
      } else {
	emit_type_for_variable( p[ k ].type_def, manual );
	o << " _par_" << ID(p[ k ].name) << ";" << endl;
      }
    }

    // Emit StaticAny for argument
    o << "CORBA::StaticAny _sa_" << ID(p[k].name) << "( ";
    emit_marshaller_ref( p[k].type_def );
    if( p[k].mode != CORBA::PARAM_OUT || !is_variable_for_sii (p[k].type) ) {
      o << ", ";
      if( !p[k].type->is_array() )
	o << "&";
      o << "_par_" << ID(p[k].name);
      if (p[k].mode != CORBA::PARAM_OUT)
	emit_marshaller_suffix( p[k].type_def, TRUE );
    }
    o << " );" << endl;
  }
  if( k != 0 )
    o << endl;
  
  // Do we have a result for this operation?
  CORBA::TypeCode_var tc_result = result->type();
  if( tc_result->kind() != CORBA::tk_void ) {
    if( tc_result->is_array() ) {
      emit_type_for_variable( result, automatic );
      o << "_slice*";
    } else
      emit_type_for_variable( result, manual );
    o << " _res;" << endl;
    o << "CORBA::StaticAny __res( ";
    emit_marshaller_ref( result );
    // arrays are always variable as return types ...
    if( !(is_variable_for_sii (tc_result) || tc_result->is_array()) ) {
      o << ", &_res";
    }
    o << " );" << endl;
  }

  for( k = 0; k != p.length(); k++ ) {
    o << "__req->add_";
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
    o << "_arg( &_sa_" << ID(p[k].name) << " );" << endl;
  }

  if( tc_result->kind() != CORBA::tk_void ) {
    o << "__req->set_result( &__res );" << endl;
  }
  o << endl;

  o << "if( !__req->read_args() )" << endl;
  o << indent << "return true;" << exdent << endl << endl;
  
  if( !_params.no_exceptions && op.mode == CORBA::OP_NORMAL &&
      op.exceptions.length() > 0 ) {
    o << "#ifdef HAVE_EXCEPTIONS" << endl;
    o << "try {" << endl;
    o << "#endif" << indent << endl;
  }
  
  // Do we have a result for this operation?
  if( tc_result->kind() != CORBA::tk_void ) {
    o << "_res = ";
  }

  // Call the implementation
  o << ID(op.name) << "(";
  for( k = 0; k < p.length(); k++ ) {
    if( k != 0 )
      o << ",";
    o << " _par_" << ID(p[ k ].name);
    if (p[k].mode != CORBA::PARAM_OUT)
	emit_marshaller_suffix( p[k].type_def, FALSE );
  }
  CORBA::ContextIdSeq& ctx = op.contexts;
  if( ctx.length() > 0 ) {
    if( p.length() > 0 )
      o << ", ";
    o << "__req->ctx()";
  }
  if( k != 0 || ctx.length() > 0 )
    o << " ";
  o << ");" << endl;

  // Write back variable length out & result parameters to StaticAny
  for( k = 0; k != p.length(); k++ ) {
    if( p[k].mode == CORBA::PARAM_OUT && is_variable_for_sii (p[k].type) ) {
      o << "_sa_" << ID(p[k].name) << ".value( ";
      emit_marshaller_ref( p[k].type_def );
      o << ", _par_" << ID(p[k].name) << " );" << endl;
    }
  }
  // arrays are always variable as return types ...
  if( is_variable_for_sii (tc_result) || tc_result->is_array() ) {
    o << "__res.value( ";
    emit_marshaller_ref( result );
    o << ", " << "_res );" << endl;
  }

  if( !_params.no_exceptions && op.mode == CORBA::OP_NORMAL &&
      op.exceptions.length() > 0 ) {
    o << exdent << "#ifdef HAVE_EXCEPTIONS" << endl;
    CORBA::ExcDescriptionSeq exs = op.exceptions;
    for( CORBA::ULong i = 0; i < exs.length(); i++ ) {
      CORBA::Contained_var con = _repo->lookup_id( exs[ i ].id );
      assert( !CORBA::is_nil( con ) );
      CORBA::String_var tmp = con->absolute_name();
      CORBA::String_var ex_abs_name = (const char *) ID(tmp);
      o << "} catch( " << ex_abs_name << "_catch &_ex ) " << BL_OPEN;
      o << "__req->set_exception( _ex->_clone() );" << endl;
      o << "__req->write_results();" << endl;
      o << "return true;" << endl;
      o << exdent;
    }
    o << "}" << endl;
    o << "#endif" << endl;
  }
  
  // Write back OUT and INOUT parameters
  o << "__req->write_results();" << endl;


  // Release memory of variable length out and inout parameters
  for( k = 0; k < p.length(); k++ ) {
    if( ( p[ k ].mode == CORBA::PARAM_OUT ) || p[ k ].type->is_array() ) {
      string name = "_par_";
      name += ID(p[k].name);
      emit_release( p[k].type_def, name.c_str() );
    }
  }
  
  if( tc_result->kind() != CORBA::tk_void ) {
    emit_release (result, "_res");
  }

  o << "return true;" << endl;
  o << BL_CLOSE;
}
