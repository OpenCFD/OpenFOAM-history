/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/throw.h>

#endif // FAST_PCH


using namespace std;

//-------------------


CORBA::DefaultValueRefCountBase::DefaultValueRefCountBase ()
{
}

CORBA::DefaultValueRefCountBase::~DefaultValueRefCountBase ()
{
}

CORBA::SimpleValueRefCountBase::SimpleValueRefCountBase ()
{
}

CORBA::SimpleValueRefCountBase::~SimpleValueRefCountBase ()
{
}

//-------------------


CORBA::ValueBase::ValueBase ()
{
  _destructing = false;
}

CORBA::ValueBase::ValueBase (const ValueBase &)
{
  _destructing = false;
}

CORBA::ValueBase::~ValueBase ()
{
}

CORBA::ValueBase *
CORBA::ValueBase::_downcast (ValueBase *vb)
{
    return vb;
}

void *
CORBA::ValueBase::_narrow_helper (const char *)
{
  return 0;
}

CORBA::ValueBase *
CORBA::ValueBase::_create (const vector<string> &repoids,
			   const string &myrepoid)
{
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");

    if (repoids.size() == 0) {
	CORBA::ValueFactoryBase_var vf =
	    orb->lookup_value_factory (myrepoid.c_str());
	if (vf.in()) {
	  return vf->create_for_unmarshal();
	}
    } else {
	for (mico_vec_size_type i = 0; i < repoids.size(); ++i) {
	    CORBA::ValueFactoryBase_var vf =
		orb->lookup_value_factory (repoids[i].c_str());
	    if (vf.in()) {
	      return vf->create_for_unmarshal();
	    }
	}
    }
    mico_throw (MARSHAL());
    return 0;
}

void
CORBA::ValueBase::_marshal (CORBA::DataEncoder &ec, ValueBase *vb)
{
    if (!vb) {
	// nil
	ec.value_ref (0);
	return;
    }
    CORBA::DataEncoder::MapValueId::iterator it =
	ec.valuestate()->visited.find (vb);
    if (it != ec.valuestate()->visited.end()) {
	// indirection
	ec.value_ref ((*it).second);
	return;
    }

    vector<string> repoids;
    CORBA::Boolean chunked;
    vb->_get_marshal_info (repoids, chunked);

    Long vid;
    ec.value_begin ("", repoids, chunked, vid);
    ec.valuestate()->visited[vb] = vid;

    vb->_marshal_members (ec);

    ec.value_end (vid);
}

CORBA::Boolean
CORBA::ValueBase::_demarshal (CORBA::DataDecoder &dc, ValueBase *&vb,
			      const string &repoid)
{
    string url;
    vector<string> repoids;
    CORBA::Long vid;
    CORBA::Boolean is_ref;

    if (!dc.value_begin (url, repoids, vid, is_ref))
	return FALSE;

    if (is_ref) {
	if (!vid) {
	    // nil
	    vb = 0;
	    return TRUE;
	}
	// indirection
	CORBA::DataDecoder::MapIdValue::iterator it =
	    dc.valuestate()->visited.find (vid);
	if (it == dc.valuestate()->visited.end())
	    // bad encoding ...
	    return FALSE;
	vb = (*it).second;
	CORBA::add_ref (vb);
	return TRUE;
    }

    // vb is null for a non-boxed value
    // vb is non-null for a boxed value (which was constructed by a stub
    //                                   and does not need a value factory)
    if (!vb) {
        // non-boxed value: use value factory
        vb = CORBA::ValueBase::_create (repoids, repoid);
        if (!vb)
	    // no implementation ...
	    return FALSE;
    }

    dc.valuestate()->visited[vid] = vb;

    if (!vb->_demarshal_members (dc))
	return FALSE;

    return dc.value_end (vid);
}

CORBA::ValueBase *
CORBA::ValueBase::_copy_value ()
{
  MICO::CDREncoder ec;
  MICO::CDRDecoder dc (ec.buffer(), FALSE, ec.byteorder(),
		       ec.converter(), FALSE);
  CORBA::DataEncoder::ValueState evstate;
  CORBA::DataDecoder::ValueState dvstate;

  CORBA::ValueBase * res = NULL;
  CORBA::Boolean ret;
  vector<string> repoids;

  ec.valuestate (&evstate, FALSE);
  dc.valuestate (&dvstate, FALSE);
  _get_marshal_info (repoids, ret);

  _marshal (ec, this);
  ret = _demarshal (dc, res, repoids[0]);
  assert (ret);
  return res;
}

void
CORBA::ValueBase::_get_marshal_info (vector<string> &, Boolean &)
{
    // must be implemented for each valuetype class when using the SII
    assert (0);
}

void
CORBA::ValueBase::_marshal_members (DataEncoder &)
{
    // must be implemented for each valuetype class when using the SII
    assert (0);
}

CORBA::Boolean
CORBA::ValueBase::_demarshal_members (DataDecoder &)
{
    // must be implemented for each valuetype class when using the SII
    assert (0);
    return FALSE;
}

CORBA::Long
CORBA::ValueBase::_count_refs (visited *)
{
  return 0;
}

void
CORBA::ValueBase::_release_members ()
{
}

//-------------------


CORBA::ValueFactoryBase::ValueFactoryBase ()
{
}

CORBA::ValueFactoryBase::~ValueFactoryBase ()
{
}

CORBA::ValueFactory
CORBA::ValueFactoryBase::_downcast (ValueFactory vf)
{
  return vf;
}

void *
CORBA::ValueFactoryBase::_narrow_helper (const char *)
{
  return 0;
}


//-------------------


CORBA::ValueFactory
CORBA::ORB::register_value_factory (const char *repoid, ValueFactory factory)
{
    ValueFactory vf = lookup_value_factory (repoid);
    _value_facs.lock();
    factory->_add_ref();
    _value_facs[repoid] = factory;
    _value_facs.unlock();
    return vf;
}

void
CORBA::ORB::unregister_value_factory (const char *repoid)
{
    MICOMT::AutoLock l(_value_facs);
    _value_facs.erase (repoid);
}

CORBA::ValueFactory
CORBA::ORB::lookup_value_factory (const char *repoid)
{
    MICOMT::AutoLock l(_value_facs);
    ValueFactoryMap::iterator i = _value_facs.find (repoid);
    if (i == _value_facs.end())
	return 0;
    ValueFactory vf = (*i).second.in();
    vf->_add_ref();
    return vf;
}

/*
 * Abstract Interface Stuff
 */

CORBA::AbstractBase::AbstractBase ()
{
}

CORBA::AbstractBase::AbstractBase (const AbstractBase &)
{
}

CORBA::AbstractBase::~AbstractBase ()
{
}

CORBA::AbstractBase_ptr
CORBA::AbstractBase::_duplicate (AbstractBase_ptr ab)
{
  if (!CORBA::is_nil (ab)) {
    Object_ptr obj = ab->_to_object ();
    ValueBase * val = ab->_to_value ();
    if (!CORBA::is_nil (obj)) {
      obj = Object::_duplicate (obj);
    }
    else if (val != 0) {
      CORBA::add_ref (val);
    }
  }

  return ab;
}

CORBA::AbstractBase_ptr
CORBA::AbstractBase::_narrow (AbstractBase_ptr ab)
{
  return _duplicate (ab);
}

CORBA::Object_ptr
CORBA::AbstractBase::_to_object ()
{
  return Object::_nil ();
}

CORBA::ValueBase *
CORBA::AbstractBase::_to_value ()
{
  return 0;
}

void
CORBA::AbstractBase::_marshal (DataEncoder &ec, AbstractBase *ab)
{
  Object_ptr obj;
  ValueBase * val;

  if (!CORBA::is_nil (ab)) {
    obj = ab->_to_object ();
    val = ab->_to_value ();
  }
  else {
    obj = Object::_nil ();
    val = 0;
  }

  ec.union_begin();
  if (!CORBA::is_nil (obj)) {
    ec.put_boolean (TRUE);
    CORBA::_stc_Object->marshal( ec, &obj );
  }
  else {
    ec.put_boolean (FALSE);
    CORBA::_stc_ValueBase->marshal( ec, &val );
  }
  ec.union_end();
}

CORBA::Boolean
CORBA::AbstractBase::_demarshal (DataDecoder &dc, AbstractBase *&ab)
{
  Boolean is_objref;

  if (!dc.union_begin()) {
    return FALSE;
  }
  if (!CORBA::_stc_boolean->demarshal (dc, &is_objref)) {
    return FALSE;
  }
  if (is_objref) {
    Object_ptr obj;
    if (!CORBA::_stc_Object->demarshal (dc, &obj)) {
      return FALSE;
    }
    ab = new UnknownAbstract (obj, 0);
  }
  else {
    ValueBase * val = NULL;
    if (!CORBA::_stc_ValueBase->demarshal (dc, &val)) {
      return FALSE;
    }
    if (val) {
      ab = new UnknownAbstract (Object::_nil(), val);
    }
    else {
      ab = 0;
    }
  }

  return dc.union_end();
}

void *
CORBA::AbstractBase::_narrow_helper (const char *)
{
  return 0;
}

void
CORBA::release (AbstractBase_ptr ab)
{
  if (!CORBA::is_nil (ab)) {
    Object_ptr obj = ab->_to_object ();
    ValueBase * val = ab->_to_value ();
    if (!CORBA::is_nil (obj)) {
      CORBA::release (obj);
    }
    else if (val != 0) {
      CORBA::remove_ref (val);
    }
  }
}

CORBA::MixedBase::~MixedBase ()
{
}

CORBA::UnknownAbstract::UnknownAbstract (CORBA::Object_ptr _o,
					 CORBA::ValueBase * _v)
{
  obj = _o;
  val = _v;
}

CORBA::UnknownAbstract::~UnknownAbstract ()
{
}

CORBA::Object_ptr
CORBA::UnknownAbstract::_to_object ()
{
  return obj;
}

CORBA::ValueBase *
CORBA::UnknownAbstract::_to_value ()
{
  return val;
}

void *
CORBA::UnknownAbstract::_narrow_helper (const char * repoid)
{
  if (!CORBA::is_nil (obj)) {
    return obj->_narrow_helper (repoid);
  }
  if (val) {
    return val->_narrow_helper (repoid);
  }
  return 0;
}
