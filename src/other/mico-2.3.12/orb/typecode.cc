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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <ctype.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/throw.h>

#endif // FAST_PCH


using namespace std;

#define TK_RECURSIVE ((int)0xffffffff)


/********************************************************************/


/****************************** TypeCode ******************************/


CORBA::TypeCode::TypeCode ()
    : tckind (tk_null), isconst (FALSE)
{
    init ();
}

CORBA::TypeCode::TypeCode (DataDecoder &dc)
    : tckind (tk_null), isconst (FALSE)
{
    init ();
    CORBA::Boolean r = dc.get_typecode (*this);
    assert (r);
}

CORBA::TypeCode::TypeCode (const string &s)
    : tckind (tk_null), isconst (FALSE)
{
    init ();
    CORBA::Boolean r = from_string (s.c_str());
    assert (r);
}

string
CORBA::TypeCode::stringify () const
{
    MICO::CDREncoder ec;
    ec.put_octet (ec.byteorder() == CORBA::BigEndian ? FALSE : TRUE);
    ec.put_typecode (*this);

    string s;
    Octet o;
    for (Long i = ec.buffer()->length(); --i >= 0; ) {
	ec.buffer()->get (o);
	s += mico_to_xdigit ((o >> 4) & 0xf);
	s += mico_to_xdigit (o & 0xf);
    }
    return s;
}

CORBA::TypeCode::TypeCode (TCKind kindval)
    : tckind (kindval), len (0), isconst (FALSE)
{
    init ();
}

CORBA::TypeCode::TypeCode (const TypeCode &tc)
{
    init ();
    copy (tc);
}

CORBA::TypeCode::~TypeCode ()
{
    free ();
}

CORBA::TypeCode &
CORBA::TypeCode::operator= (const TypeCode &tc)
{
    if (this != &tc) {
	free ();
	copy (tc);
    }
    return *this;
}

CORBA::Boolean
CORBA::TypeCode::from_string (const char *_ior)
{
    string s (_ior);

    free ();
    if ((s.length() & 1) != 0)
	return FALSE;
    Buffer b;
    for (mico_vec_size_type i = 0; i < s.length(); i += 2) {
	if (!isxdigit (s[i]) || !isxdigit (s[i+1]))
	    return FALSE;
        b.put ((mico_from_xdigit (s[i]) << 4) | mico_from_xdigit (s[i+1]));
    }

    MICO::CDRDecoder dc (&b, FALSE);

    Octet bo;
    if (!dc.get_octet (bo))
	return FALSE;
    dc.byteorder (bo ? CORBA::LittleEndian : CORBA::BigEndian);

    if (!dc.get_typecode (*this)) {
	free ();
	return FALSE;
    }
    return TRUE;
}

void
CORBA::TypeCode::connect (TypeCode_ptr parent, Long depth)
{
    /*
     * connect nested recursive types when they are embedded into
     * a new constructed type (parent).
     */
    if (tckind == TK_RECURSIVE) {
	if (!CORBA::is_nil (recurse_tc))
	    return;
	if (recurse_depth == depth ||
	    (repoid.length() > 0 && repoid == parent->repoid)) {
	    recurse_tc = parent;
	    recurse_depth = depth;
	}
	return;
    }

    if (!CORBA::is_nil (content))
	content->connect (parent, depth+1);

    for (mico_vec_size_type i = 0; i < tcvec.size(); ++i)
	tcvec[i]->connect (parent, depth+1);
}

void
CORBA::TypeCode::disconnect (TypeCode_ptr parent)
{
    /*
     * disconnect nested recursive types when the enclosing
     * constructed type (parent) is deleted.
     */
    if (CORBA::is_nil (parent))
	return;

    if (tckind == TK_RECURSIVE) {
	if (recurse_tc == parent) {
	    recurse_tc = _nil();
	    if (repoid.length() > 0)
		recurse_depth = -1;
	}
	return;
    }

    if (!CORBA::is_nil (content))
	content->disconnect (parent);

    for (mico_vec_size_type i = 0; i < tcvec.size(); ++i)
	tcvec[i]->disconnect (parent);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_basic_tc (TCKind k)
{
    return new TypeCode (k);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_struct_tc (const char *rep_id,
				   const char *nameval,
				   const StructMemberSeq &members)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_struct);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    for (CORBA::ULong i = 0; i < members.length(); ++i) {
        tc->namevec.push_back (members[i].name.in());
        tc->tcvec.push_back (CORBA::TypeCode::_duplicate (members[i].type.in()));
	tc->tcvec.back()->connect (tc);
    }
    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_exception_tc (const char *rep_id,
				      const char *nameval,
				      const StructMemberSeq &members)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_except);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    for (CORBA::ULong i = 0; i < members.length(); ++i) {
        tc->namevec.push_back (members[i].name.in());
        tc->tcvec.push_back (CORBA::TypeCode::_duplicate (members[i].type.in()));
	tc->tcvec.back()->connect (tc);
    }
    return tc;
}

CORBA::Any *
CORBA::TypeCode::convert_case_label (CORBA::TypeCode_ptr disc,
				     const CORBA::Any &l)
{
    CORBA::TypeCode_var t = l.type();

    if (disc->equal (t, TRUE))
	return new CORBA::Any (l);

    switch (t->unalias()->kind()) {
    case CORBA::tk_octet:
	return new CORBA::Any (l);

    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
	break;
    default:
	mico_throw (CORBA::BAD_PARAM());
    }

    switch (disc->unalias()->kind()) {
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_ulong:
    case CORBA::tk_longlong:
    case CORBA::tk_ulonglong:
	break;
    default:
	mico_throw (CORBA::BAD_PARAM());
    }

    CORBA::Any_var nl = new CORBA::Any (disc, 0);
    if (!l.coerce (nl.inout()))
	mico_throw (CORBA::BAD_PARAM());
    return nl._retn();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_union_tc (const char *rep_id, const char *nameval,
				  TypeCode_ptr disc_type,
				  const UnionMemberSeq &members)
{
    MICO_OBJ_CHECK (disc_type);

    CORBA::TypeCode_var tc = new TypeCode (tk_union);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";
    tc->defidx = -1;

    tc->discriminator = CORBA::TypeCode::_duplicate (disc_type);
    TypeCode_var labeltc;
    for (CORBA::ULong i = 0; i < members.length(); ++i) {
        tc->labelvec.push_back (convert_case_label (disc_type, members[i].label));
	labeltc = tc->labelvec.back()->type();
	if (labeltc->kind() == tk_octet) {
	    if (tc->defidx >= 0)
		mico_throw (CORBA::BAD_PARAM());
	    tc->defidx = i;
	}
        tc->namevec.push_back (members[i].name.in());
        tc->tcvec.push_back (CORBA::TypeCode::_duplicate (members[i].type.in()));
        tc->tcvec.back()->connect (tc);
    }
    return tc._retn();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_enum_tc (const char *rep_id, const char *nameval,
				 const EnumMemberSeq &members)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_enum);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    for (CORBA::ULong i = 0; i < members.length(); ++i)
        tc->namevec.push_back (members[i].in());

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_alias_tc (const char *rep_id, const char *nameval,
				  TypeCode_ptr orig_type)
{
    MICO_OBJ_CHECK (orig_type);

    CORBA::TypeCode_ptr tc = new TypeCode (tk_alias);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    tc->content = CORBA::TypeCode::_duplicate (orig_type);
    tc->content->connect (tc);

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_interface_tc (const char *rep_id, const char *nameval)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_objref);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_string_tc (ULong bound)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_string);
    tc->len = bound;

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_wstring_tc (ULong bound)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_wstring);
    tc->len = bound;

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_fixed_tc (UShort _digits, Short _scale)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_fixed);
    tc->digits = _digits;
    tc->scale = _scale;

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_array_tc (ULong bound, TypeCode_ptr el_type)
{
    MICO_OBJ_CHECK (el_type);

    CORBA::TypeCode_ptr tc = new TypeCode (tk_array);
    tc->len = bound;

    tc->content = CORBA::TypeCode::_duplicate (el_type);
    tc->content->connect (tc);

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_sequence_tc (ULong bound, TypeCode_ptr el_type)
{
    MICO_OBJ_CHECK (el_type);

    CORBA::TypeCode_ptr tc = new TypeCode (tk_sequence);
    tc->len = bound;

    tc->content = CORBA::TypeCode::_duplicate (el_type);
    tc->content->connect (tc);

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_recursive_sequence_tc (ULong bound, ULong offset)
{
    CORBA::TypeCode_ptr tc = new TypeCode (tk_sequence);
    tc->len = bound;
    // avoid "constant overflow" compiler warning
    CORBA::ULong tk_recursive = TK_RECURSIVE;
    tc->content = new TypeCode ((CORBA::TCKind)tk_recursive);
    tc->content->recurse_depth = offset;
    tc->content->connect (tc);

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_value_tc (const char *rep_id, const char *nameval,
				  ValueModifier mod,
				  TypeCode_ptr concrete_base,
				  const ValueMemberSeq &members)
{
    if (!CORBA::is_nil (concrete_base))
	MICO_OBJ_CHECK (concrete_base);

    CORBA::TypeCode_ptr tc = new CORBA::TypeCode (tk_value);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";
    tc->value_mod = mod;
    tc->len = -1;
    if (!CORBA::is_nil (concrete_base)) {
	tc->content = CORBA::TypeCode::_duplicate (concrete_base);
	tc->content->connect (tc);
    } else {
	tc->content = CORBA::TypeCode::_nil();
    }

    for (CORBA::ULong i = 0; i < members.length(); ++i) {
        tc->visvec.push_back (members[i].access);
        tc->namevec.push_back (members[i].name.in());
        tc->tcvec.push_back (CORBA::TypeCode::_duplicate (members[i].type.in()));
        tc->tcvec.back()->connect (tc);
    }

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_value_box_tc (const char *rep_id,
				      const char *nameval,
				      TypeCode_ptr boxed_type)
{
    MICO_OBJ_CHECK (boxed_type);

    CORBA::TypeCode_ptr tc = new CORBA::TypeCode (tk_value_box);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    tc->content = CORBA::TypeCode::_duplicate (boxed_type);
    tc->content->connect (tc);

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_native_tc (const char *rep_id,
				   const char *nameval)
{
    CORBA::TypeCode_ptr tc = new CORBA::TypeCode (tk_native);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_recursive_tc (const char *rep_id)
{
    if (!rep_id)
	mico_throw (CORBA::BAD_PARAM());

    // avoid "constant overflow" compiler warning
    CORBA::ULong tk_recursive = TK_RECURSIVE;
    CORBA::TypeCode_ptr tc = new CORBA::TypeCode ((CORBA::TCKind)tk_recursive);
    tc->recurse_depth = -1;
    tc->repoid = rep_id;

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_abstract_interface_tc (const char *rep_id,
					       const char *nameval)
{
    if (!rep_id || !nameval)
	mico_throw (CORBA::BAD_PARAM());

    CORBA::TypeCode_ptr tc = new CORBA::TypeCode (tk_abstract_interface);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = nameval ? nameval : "";

    return tc;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::create_local_interface_tc (const char *rep_id,
					    const char *name)
{
    if (!rep_id || !name)
	mico_throw (CORBA::BAD_PARAM());

    CORBA::TypeCode_ptr tc = new CORBA::TypeCode (tk_local_interface);
    tc->repoid = rep_id ? rep_id : "";
    tc->tcname = name ? name : "";

    return tc;
}

void
CORBA::TypeCode::init ()
{
    content = TypeCode::_nil ();
    discriminator = TypeCode::_nil ();
    recurse_tc = TypeCode::_nil ();
}

void
CORBA::TypeCode::copy (const TypeCode &tc)
{
    isconst = FALSE;
    tckind = tc.tckind;
    tcname = tc.tcname;
    repoid = tc.repoid;
    defidx = tc.defidx;
    len = tc.len;
    digits = tc.digits;
    scale = tc.scale;
    recurse_depth = tc.recurse_depth;
    recurse_tc = TypeCode::_nil();
    namevec = tc.namevec;
    visvec = tc.visvec;
    value_mod = tc.value_mod;

    if (CORBA::is_nil (tc.content))
        content = TypeCode::_nil();
    else {
        content = new TypeCode (*tc.content);
        content->connect (this);
    }

    if (CORBA::is_nil (tc.discriminator))
        discriminator = TypeCode::_nil();
    else {
        discriminator = new TypeCode (*tc.discriminator);
    }

    for (mico_vec_size_type i0 = 0; i0 < tc.tcvec.size(); ++i0) {
        tcvec.push_back (new TypeCode (*tc.tcvec[i0]));
        tcvec.back()->connect (this);
    }

    for (mico_vec_size_type i1 = 0; i1 < tc.labelvec.size(); ++i1)
        labelvec.push_back (new Any (*tc.labelvec[i1]));
}

void
CORBA::TypeCode::free ()
{
    if (!CORBA::is_nil (content)) {
        content->disconnect (this);
        CORBA::release (content);
    }
    if (!CORBA::is_nil (discriminator)) {
        CORBA::release (discriminator);
    }
    // do not free parent
    content = TypeCode::_nil();
    discriminator = TypeCode::_nil();
    recurse_tc = TypeCode::_nil();

    for (mico_vec_size_type i0 = 0; i0 < tcvec.size(); ++i0) {
        tcvec[i0]->disconnect (this);
        CORBA::release (tcvec[i0]);
    }

    for (mico_vec_size_type i1 = 0; i1 < labelvec.size(); ++i1)
        delete labelvec[i1];

    namevec.erase (namevec.begin(), namevec.end());
    tcvec.erase (tcvec.begin(), tcvec.end());
    labelvec.erase (labelvec.begin(), labelvec.end());
    visvec.erase (visvec.begin(), visvec.end());

    tckind = tk_null;
}

CORBA::TCKind
CORBA::TypeCode::kind () const
{
    MICO_OBJ_CHECK (this);

    return tckind;
}

CORBA::Boolean
CORBA::TypeCode::equal (TypeCode_ptr tc,
			Boolean remove_aliases,
			Boolean ignore_string_bounds) const
{
    MICO_OBJ_CHECK (this);

    if (this == tc)
	return TRUE;

    if (remove_aliases) {
	tc = tc->unalias();
	if (tckind == tk_alias)
	    return ((TypeCode_ptr)this)->unalias()->equal (tc, remove_aliases,
							   ignore_string_bounds);
    }

    if (tckind == TK_RECURSIVE || tc->tckind == TK_RECURSIVE) {
	if ((!remove_aliases || 1) && tckind != tc->tckind)
	    return FALSE;
	if (tckind == TK_RECURSIVE && CORBA::is_nil (recurse_tc))
	    return FALSE;
	if (tc->tckind == TK_RECURSIVE && CORBA::is_nil (tc->recurse_tc))
	    return FALSE;
	CORBA::TypeCode_ptr metc = resolve_recursion();
	CORBA::TypeCode_ptr hetc = tc->resolve_recursion();
	return metc->tckind == hetc->tckind &&
	    !strcmp (metc->id(), hetc->id());
    }

    if (tckind != tc->tckind)
	return FALSE;

    /*
     * [12-12] says we should not take into account the "name" for
     * type equivalence checking ...
     */
    switch (tckind) {
    case tk_wstring:
    case tk_string:
        if( ignore_string_bounds )
	  return true;
	return len == tc->len;

    case tk_fixed:
	return digits == tc->digits && scale == tc->scale;

    case tk_sequence:
    case tk_array:
	/*
	 * do not use content_type(), since that would lead to
	 * infinite recursion for recursive sequences ...
	 */
        return len == tc->len && content->equal (tc->content, remove_aliases,
						 ignore_string_bounds);

    case tk_alias:
        return repoid == tc->repoid && content->equal (tc->content,
						       remove_aliases,
						       ignore_string_bounds);

    case tk_objref:
    case tk_abstract_interface:
    case tk_local_interface:
    case tk_native:
        return repoid == tc->repoid;

    case tk_except:
    case tk_struct:
	{
	    if (!(repoid == tc->repoid &&
		  namevec.size() == tc->namevec.size())) {
		return FALSE;
	    }
	    assert (namevec.size() == tcvec.size());
	    assert (tc->namevec.size() == tc->tcvec.size());
	    for (mico_vec_size_type i = 0; i < tcvec.size(); ++i) {
		if (!tcvec[i]->equal (tc->tcvec[i], remove_aliases,
                                                    ignore_string_bounds)) {
		    return FALSE;
		}
	    }
	    return TRUE;
	}

    case tk_value_box:
        return repoid == tc->repoid && content->equal (tc->content,
						       remove_aliases,
						       ignore_string_bounds);

    case tk_value:
	{
	    if (!(repoid == tc->repoid &&
                  value_mod == tc->value_mod &&
		  namevec.size() == tc->namevec.size())) {
		return FALSE;
	    }
	    assert (namevec.size() == tcvec.size());
	    assert (namevec.size() == visvec.size());
	    assert (tc->namevec.size() == tc->tcvec.size());
	    assert (tc->namevec.size() == tc->visvec.size());
	    for (mico_vec_size_type i = 0; i < tcvec.size(); ++i) {
		if (!(tcvec[i]->equal (tc->tcvec[i], remove_aliases,
                                                     ignore_string_bounds) &&
                      visvec[i] == tc->visvec[i])) {
		    return FALSE;
		}
	    }
	    return TRUE;
	}

    case tk_union:
	{
	    if (!(repoid == tc->repoid &&
		  namevec.size() == tc->namevec.size() &&
		  discriminator->equal (tc->discriminator, remove_aliases,
                                                      ignore_string_bounds) &&
		((defidx == tc->defidx) || (defidx < 0 && tc->defidx < 0)))) {
		return FALSE;
	    }
	    assert (namevec.size() == tcvec.size());
	    assert (namevec.size() == labelvec.size());
	    assert (tc->namevec.size() == tc->tcvec.size());
	    assert (tc->namevec.size() == tc->labelvec.size());
	    for (mico_vec_size_type i = 0; i < tcvec.size(); ++i) {
		if (!(tcvec[i]->equal (tc->tcvec[i], remove_aliases,
                                                      ignore_string_bounds) &&
		    *labelvec[i] == *tc->labelvec[i])) {
		    return FALSE;
		}
	    }
	    return TRUE;
	}
    case tk_enum:
        return repoid == tc->repoid && namevec == tc->namevec;

    case TK_RECURSIVE:
	assert (0);

    default:
	// base type
	return TRUE;
    }
}

CORBA::Boolean
CORBA::TypeCode::equivalent (TypeCode_ptr tc)
{
  return equaltype (tc);
}

CORBA::Boolean
CORBA::TypeCode::equaltype (TypeCode_ptr tc, SetTC *_cache)
{
    if (this == tc)
	return TRUE;

    TypeCode_ptr me = unalias()->resolve_recursion();
    while (me->tckind == tk_alias)
	me = me->unalias()->resolve_recursion();

    TypeCode_ptr he = tc->unalias()->resolve_recursion();
    while (he->tckind == tk_alias)
	he = he->unalias()->resolve_recursion();

    /*
     * kinds must be the same to be of equal type
     */
    if (me->tckind != he->tckind)
	return FALSE;

    /*
     * first check for simple types ...
     */
    switch (me->tckind) {
    case tk_void:
    case tk_null:
    case tk_short:
    case tk_ushort:
    case tk_octet:
    case tk_boolean:
    case tk_char:
    case tk_wchar:
    case tk_any:
    case tk_TypeCode:
    case tk_Principal:
    case tk_long:
    case tk_longlong:
    case tk_ulong:
    case tk_ulonglong:
    case tk_float:
    case tk_double:
    case tk_longdouble:
	return TRUE;

    case tk_string:
    case tk_wstring:
	return me->len == he->len;

    case tk_objref:
    case tk_abstract_interface:
    case tk_local_interface:
    case tk_native:
        return me->repoid == he->repoid;

    case tk_enum:
	return me->namevec == he->namevec;

    case tk_fixed:
        return me->digits == he->digits && me->scale == he->scale;

    case tk_alias:
    case TK_RECURSIVE:
	// cannot happen
	assert (0);

    default:
	break;
    }

    /*
     * check the cache. this is needed for recursive types ...
     */
    if (_cache && _cache->count (make_pair (me, he)))
	return TRUE;

    /*
     * insert ourself into cache
     */
    SetTC *cache = _cache ? _cache : new SetTC;
    cache->insert (make_pair (me, he));

    Boolean retval = FALSE;

    switch (me->tckind) {
    case tk_sequence:
    case tk_array:
	retval = (me->len == he->len &&
		  me->content->equaltype (he->content, cache));
	break;

    case tk_except:
    case tk_struct:
	{
	    if (me->namevec.size() != he->namevec.size())
		break;
	    assert (me->namevec.size() == me->tcvec.size());
	    assert (he->namevec.size() == he->tcvec.size());
	    retval = TRUE;
	    for (mico_vec_size_type i=0; i < me->tcvec.size(); ++i) {
		if (!me->tcvec[i]->equaltype (he->tcvec[i], cache)) {
		    retval = FALSE;
		    break;
		}
	    }
	    break;
	}
    case tk_value_box:
	retval = me->content->equaltype (he->content, cache);
	break;

    case tk_value:
	{
	    if (me->value_mod != he->value_mod ||
                me->namevec.size() != he->namevec.size())
                break;

	    assert (me->namevec.size() == me->tcvec.size());
	    assert (me->namevec.size() == me->visvec.size());
	    assert (he->namevec.size() == he->tcvec.size());
	    assert (he->namevec.size() == he->visvec.size());
            retval = TRUE;
	    for (mico_vec_size_type i = 0; i < me->tcvec.size(); ++i) {
		if (!me->tcvec[i]->equaltype (he->tcvec[i]) ||
                    me->visvec[i] != he->visvec[i]) {
		    retval = FALSE;
                    break;
		}
	    }
            break;
	}
    case tk_union:
	{
	    if (me->namevec.size() != he->namevec.size() ||
		!me->discriminator->equaltype (he->discriminator, cache))
		break;
	    
	    assert (me->namevec.size() == me->tcvec.size());
	    assert (me->namevec.size() == me->labelvec.size());
	    assert (he->namevec.size() == he->tcvec.size());
	    assert (he->namevec.size() == he->labelvec.size());
	    retval = TRUE;
	    for (mico_vec_size_type i=0; i < me->tcvec.size(); ++i) {
		if (!me->tcvec[i]->equaltype (he->tcvec[i], cache) ||
		    !(*me->labelvec[i] == *he->labelvec[i])) {
		    retval = FALSE;
		    break;
		}
	    }
	    break;
	}
    default:
	assert (0);
    }

    /*
     * remove ourself from cache only if equaltype checking failed ...
     */
    if (!retval)
	cache->erase (make_pair (me, he));

    if (!_cache)
	delete cache;

    return retval;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::resolve_recursion() const
{
    if (tckind != TK_RECURSIVE)
        return (TypeCode_ptr)this;

    assert (!CORBA::is_nil (recurse_tc));
    return recurse_tc;
}

const char *
CORBA::TypeCode::id () const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_objref || tckind == tk_struct ||
	  tckind == tk_union  || tckind == tk_enum ||
	  tckind == tk_alias  || tckind == tk_except ||
	  tckind == tk_native || tckind == tk_abstract_interface ||
	  tckind == tk_local_interface ||
	  tckind == tk_value || tckind == tk_value_box ||
	  (tckind == TK_RECURSIVE && repoid.length() > 0)))
	mico_throw (CORBA::TypeCode::BadKind());
    return repoid.c_str();
}

const char *
CORBA::TypeCode::name () const
{
    if (!(tckind == tk_objref || tckind == tk_struct ||
	  tckind == tk_union  || tckind == tk_enum ||
	  tckind == tk_alias  || tckind == tk_except ||
	  tckind == tk_native || tckind == tk_abstract_interface ||
	  tckind == tk_local_interface ||
	  tckind == tk_value || tckind == tk_value_box))
	mico_throw (CORBA::TypeCode::BadKind());
    return tcname.c_str();
}

CORBA::Long
CORBA::TypeCode::member_index (const Any &a)
{
    if (tckind != tk_union)
	mico_throw (CORBA::TypeCode::BadKind());

    for (mico_vec_size_type i = 0; i < labelvec.size(); ++i) {
	if (a == *labelvec[i])
	    return i;
    }
    return defidx;
}

CORBA::Long
CORBA::TypeCode::member_index (const char *s)
{
    if (!(tckind == tk_union  || tckind == tk_enum ||
	  tckind == tk_struct || tckind == tk_except ||
	  tckind == tk_value))
	mico_throw (CORBA::TypeCode::BadKind());

    for (mico_vec_size_type i = 0; i < namevec.size(); ++i) {
	if (!strcmp (s, namevec[i].c_str()))
	    return i;
    }
    return -1;
}

CORBA::ULong
CORBA::TypeCode::member_count () const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_struct || tckind == tk_union ||
	  tckind == tk_enum || tckind == tk_except ||
	  tckind == tk_value))
	mico_throw (CORBA::TypeCode::BadKind());
    return namevec.size();
}

const char *
CORBA::TypeCode::member_name (ULong idx) const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_struct || tckind == tk_union ||
	  tckind == tk_enum || tckind == tk_except ||
	  tckind == tk_value))
	mico_throw (CORBA::TypeCode::BadKind());
    if (idx >= namevec.size())
	mico_throw (CORBA::TypeCode::Bounds());
    return namevec[idx].c_str();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type (ULong idx) const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_struct || tckind == tk_union ||
	    tckind == tk_except || tckind == tk_value))
	mico_throw (CORBA::TypeCode::BadKind());
    if (idx >= tcvec.size())
	mico_throw (CORBA::TypeCode::Bounds());
    return TypeCode::_duplicate (tcvec[idx]->resolve_recursion());
}

CORBA::Any *
CORBA::TypeCode::member_label (ULong idx) const
{
    MICO_OBJ_CHECK (this);

    if (tckind != tk_union)
	mico_throw (CORBA::TypeCode::BadKind());
    if (idx >= labelvec.size())
	mico_throw (CORBA::TypeCode::Bounds());
    return new CORBA::Any (*labelvec[idx]);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type () const
{
    MICO_OBJ_CHECK (this);

    if (tckind != tk_union)
	mico_throw (CORBA::TypeCode::BadKind());
    return TypeCode::_duplicate (discriminator);
}

CORBA::Long
CORBA::TypeCode::default_index () const
{
    MICO_OBJ_CHECK (this);

    if (tckind != tk_union)
	mico_throw (CORBA::TypeCode::BadKind());
    return defidx;
}

CORBA::ULong
CORBA::TypeCode::length () const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_string || tckind == tk_sequence ||
	  tckind == tk_array || tckind == tk_wstring))
	mico_throw (CORBA::TypeCode::BadKind());
    return len;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::content_type () const
{
    MICO_OBJ_CHECK (this);

    if (!(tckind == tk_sequence || tckind == tk_array ||
	  tckind == tk_alias || tckind == tk_value_box))
	mico_throw (CORBA::TypeCode::BadKind());
    return TypeCode::_duplicate (content->resolve_recursion());
}

CORBA::UShort
CORBA::TypeCode::fixed_digits () const
{
    if (!(tckind == tk_fixed))
	mico_throw (CORBA::TypeCode::BadKind());
    return digits;
}

CORBA::Short
CORBA::TypeCode::fixed_scale () const
{
    if (!(tckind == tk_fixed))
	mico_throw (CORBA::TypeCode::BadKind());
    return scale;
}

CORBA::Visibility
CORBA::TypeCode::member_visibility (ULong idx) const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());
    if (idx >= visvec.size())
	mico_throw (CORBA::TypeCode::Bounds());
    return visvec[idx];
}

CORBA::ValueModifier
CORBA::TypeCode::type_modifier() const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());
    return value_mod;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type() const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());
    return CORBA::TypeCode::_duplicate (content);
}

void
CORBA::TypeCode::strip()
{
    tcname = "";

    mico_vec_size_type i;
    for (i = 0; i < namevec.size(); ++i)
	namevec[i] = "";

    for (i = 0; i < tcvec.size(); ++i)
	tcvec[i]->strip();

    if (!CORBA::is_nil (content))
	content->strip();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::get_compact_typecode() const
{
    CORBA::TypeCode_ptr t = new TypeCode(*this);
    t->strip();
    return t;
}

CORBA::ULong
CORBA::TypeCode::member_count_inherited () const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());

    TypeCode_ptr me = (TypeCode_ptr)this;

    if (len < 0) {
	me->len = member_count();
	if (!CORBA::is_nil (content))
	    me->len += content->member_count_inherited();
    }
    return len;
}

const char *
CORBA::TypeCode::member_name_inherited (ULong idx) const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());

    ULong n = member_count_inherited();
    if (idx >= n)
	mico_throw (CORBA::TypeCode::Bounds());
    idx = n-1-idx;

    TypeCode_ptr t = (TypeCode_ptr)this;

    while (42) {
	n = t->member_count();
	if (idx < n)
	    break;
	idx -= n;
	t = t->content;
	assert (!CORBA::is_nil (t));
    }
    return t->member_name (n-1-idx);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type_inherited (ULong idx) const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());

    ULong n = member_count_inherited();
    if (idx >= n)
	mico_throw (CORBA::TypeCode::Bounds());
    idx = n-1-idx;

    TypeCode_ptr t = (TypeCode_ptr)this;

    while (42) {
	n = t->member_count();
	if (idx < n)
	    break;
	idx -= n;
	t = t->content;
	assert (!CORBA::is_nil (t));
    }
    return t->member_type (n-1-idx);
}

CORBA::Visibility
CORBA::TypeCode::member_visibility_inherited (ULong idx) const
{
    if (tckind != tk_value)
	mico_throw (CORBA::TypeCode::BadKind());

    ULong n = member_count_inherited();
    if (idx >= n)
	mico_throw (CORBA::TypeCode::Bounds());
    idx = n-1-idx;

    TypeCode_ptr t = (TypeCode_ptr)this;

    while (42) {
	n = t->member_count();
	if (idx < n)
	    break;
	idx -= n;
	t = t->content;
	assert (!CORBA::is_nil (t));
    }
    return t->member_visibility (n-1-idx);
}

CORBA::Long
CORBA::TypeCode::param_count () const
{
    switch (tckind) {
    case tk_string:
    case tk_wstring:
    case tk_objref:
    case tk_abstract_interface:
    case tk_local_interface:
    case tk_native:
	return 1;
    case tk_sequence:
    case tk_array:
    case tk_alias:
    case tk_fixed:
    case tk_value_box:
	return 2;
    case tk_enum:
	return 1 + namevec.size();
    case tk_except:
    case tk_struct:
	return 1 + 2 * namevec.size();
    case tk_union:
	return 2 + 3 * namevec.size();
    case tk_value:
	return 3 + 3 * namevec.size();
    default:
	return 0;
    }
}

CORBA::Any *
CORBA::TypeCode::parameter (Long idx) const
{
    if (idx < 0 || idx >= param_count())
	mico_throw (CORBA::TypeCode::Bounds());

    Any *a = new Any;

    switch (tckind) {
    case tk_string:
    case tk_wstring:
	*a <<= length();
	break;

    case tk_objref:
    case tk_native:
    case tk_abstract_interface:
    case tk_local_interface:
	*a <<= id();
	break;

    case tk_sequence:
    case tk_array:
	if (idx == 0) {
	    TypeCode_var t = content_type();
	    *a <<= t;
	} else {
	    *a <<= length();
	}
	break;

    case tk_alias:
    case tk_value_box:
	if (idx == 0) {
	    *a <<= name();
	} else {
	    TypeCode_var t = content_type();
	    *a <<= t;
	}
	break;

    case tk_enum:
	if (idx == 0) {
	    *a <<= name();
	} else {
	    *a <<= member_name (idx-1);
	}
	break;

    case tk_fixed:
	if (idx == 0) {
	    *a <<= fixed_digits();
	} else {
	    *a <<= fixed_scale();
	}
	break;

    case tk_except:
    case tk_struct:
	if (idx == 0) {
	    *a <<= name();
	} else if ((idx-1) % 2 == 0) {
	    *a <<= member_name ((idx-1) / 2);
	} else {
	    TypeCode_var t = member_type ((idx-1) / 2);
	    *a <<= t;
	}
	break;

    case tk_union:
	if (idx == 0) {
	    *a <<= name();
	} else if (idx == 1) {
	    TypeCode_var t = discriminator_type();
	    *a <<= t;
	} else if ((idx-2) % 3 == 0) {
	    Any_var l = member_label ((idx-2) / 3);
	    *a = l.in();
	} else if ((idx-2) % 3 == 1) {
	    *a <<= member_name ((idx-2) / 3);
	} else {
	    TypeCode_var t = member_type ((idx-2) / 3);
	    *a <<= t;
	}
	break;

    case tk_value:
	if (idx == 0) {
	    *a <<= name();
	} else if (idx == 1) {
	    *a <<= type_modifier();
	} else if (idx == 2) {
	    CORBA::TypeCode_var t = concrete_base_type();
	    if (CORBA::is_nil (t))
		t = new TypeCode (tk_null);
	    *a <<= t;
	} else if ((idx-3) % 3 == 0) {
	    *a <<= member_name ((idx-3) / 3);
	} else if ((idx-3) % 3 == 1) {
	    TypeCode_var t = member_type ((idx-3) / 3);
	    *a <<= t;
	} else {
	    *a <<= member_visibility ((idx-3) / 3);
	}
	break;

    default:
	assert (0);
    }
    return a;
}

CORBA::Boolean
CORBA::TypeCode::is_constant () const
{
    return isconst;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::mk_constant ()
{
    isconst = TRUE;
    return this;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::unalias ()
{
    TypeCode_var t = TypeCode::_duplicate (this);
    while (t->kind() == tk_alias)
	t = t->content_type();
    // no duplicate returned ...
    return t;
}

CORBA::ULong
CORBA::TypeCode::array_size ()
{
    TypeCode_var t = TypeCode::_duplicate (this);
    assert (t->kind() == tk_array);
    ULong size = 1;
    while (t->kind() == tk_array) {
	size *= t->length();
	t = t->content_type();
	t = TypeCode::_duplicate (t->unalias());
    }
    return size;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::array_type ()
{
    TypeCode_var t = TypeCode::_duplicate (this);
    assert (t->kind() == tk_array);
    while (t->kind() == tk_array) {
	t = t->content_type();
	t = TypeCode::_duplicate (t->unalias());
    }
    return TypeCode::_duplicate (t);
}

#define check(exp) if (!(exp)) goto bad

CORBA::Boolean
CORBA::TypeCode::decode (DataDecoder &dc, MapPosTC *_omap, ULong level)
{
    free ();

    ULong k, buffer_pos;
    CORBA::DataDecoder::EncapsState state;

    /*
     * omap maps buffer positions to (NestingLevel, TypeCode) pairs.
     * initially TypeCode is set to nil so nested typecodes can
     * detect whether they are recursive or repeated ([12-13]). Only
     * when decoding is finished TypeCode is set to 'this'.
     */
    MapPosTC *omap = _omap ? _omap : new MapPosTC;

    /*
     * XXX this relies on the tk_* constants beeing defined as
     *     described in [12-11]
     */
    check (dc.enumeration (k));
    tckind = (TCKind)k;

    buffer_pos = dc.buffer()->rpos() - sizeof(CORBA::ULong);
    (*omap)[buffer_pos] = make_pair (level, _nil());

    switch (tckind) {
    case tk_null:
    case tk_void:
    case tk_short:
    case tk_long:
    case tk_longlong:
    case tk_ushort:
    case tk_ulong:
    case tk_ulonglong:
    case tk_float:
    case tk_double:
    case tk_longdouble:
    case tk_boolean:
    case tk_char:
    case tk_wchar:
    case tk_octet:
    case tk_any:
    case tk_TypeCode:
    case tk_Principal:
        break;

    case tk_objref:
    case tk_native:
    case tk_abstract_interface:
    case tk_local_interface:
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        check (dc.encaps_end (state));
        break;

    case tk_except:
    case tk_struct: {
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        check (dc.get_ulong (k));
        string s;
        for (CORBA::ULong i = 0; i < k; ++i) {
            check (dc.get_string_stl (s));
            namevec.push_back (s);
            tcvec.push_back (new TypeCode ());
	    check (tcvec.back()->decode (dc, omap, level+1));
            tcvec.back()->connect (this);
        }
        check (dc.encaps_end (state));
        break;
    }
    case tk_union: {
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        discriminator = new TypeCode ();
	check (discriminator->decode (dc, omap, level+1));
        check (dc.get_long (defidx));
        check (dc.get_ulong (k));
        string s;
	Any *a;
	for (CORBA::ULong i = 0; i < k; ++i) {
	    a = new Any;
#if 1
	    /*
	     * CORBA 2.2 spec explicitely says that the default
	     * case label is an instance of the discriminator
	     * type with meanigless value.
	     * However, most ORBs encode it as octet(0) ...
	     */
	    check (a->demarshal (discriminator, dc));
            if (i == (CORBA::ULong)defidx)
		*a <<= CORBA::Any::from_octet (0);
#else
            if (i == (CORBA::ULong)defidx) {
                check (a->demarshal (_tc_octet, dc));
            } else {
                check (a->demarshal (discriminator, dc));
            }
#endif
            labelvec.push_back (a);
            check (dc.get_string_stl (s));
            namevec.push_back (s);
            tcvec.push_back (new TypeCode ());
	    check (tcvec.back()->decode (dc, omap, level+1));
            tcvec.back()->connect (this);
        }
        check (dc.encaps_end (state));
        break;
    }
    case tk_enum: {
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        check (dc.get_ulong (k));
        string s;
        for (CORBA::ULong i = 0; i < k; ++i) {
            check (dc.get_string_stl (s));
            namevec.push_back (s);
        }
        check (dc.encaps_end (state));
        break;
    }
    case tk_string:
    case tk_wstring:
        check (dc.get_ulong (k));
        len = k;
        break;

    case tk_sequence:
    case tk_array:
        check (dc.encaps_begin (state, k));
        content = new TypeCode ();
	check (content->decode (dc, omap, level+1));
        content->connect (this);
        check (dc.get_ulong (k));
        len = k;
        check (dc.encaps_end (state));
        break;

    case tk_alias:
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        content = new TypeCode ();
	check (content->decode (dc, omap, level+1));
        content->connect (this);
        check (dc.encaps_end (state));
        break;

    case tk_fixed:
        check (dc.get_ushort (digits));
        check (dc.get_short (scale));
        break;

    case tk_value: {
	len = -1;
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
	check (dc.get_short (value_mod));
	CORBA::TypeCode_var tmptc = new TypeCode();
	check (tmptc->decode (dc, omap, level+1));
	if (tmptc->kind() == tk_null) {
	    content = CORBA::TypeCode::_nil();
	} else {
	    content = tmptc._retn();
	    content->connect (this);
	}
        check (dc.get_ulong (k));
        string s;
	CORBA::Visibility vis;
        for (CORBA::ULong i = 0; i < k; ++i) {
            check (dc.get_string_stl (s));
            namevec.push_back (s);
            tcvec.push_back (new TypeCode ());
	    check (tcvec.back()->decode (dc, omap, level+1));
            tcvec.back()->connect (this);
	    check (dc.get_short (vis));
	    visvec.push_back (vis);
        }
        check (dc.encaps_end (state));
        break;
    }
    case tk_value_box:
        check (dc.encaps_begin (state, k));
        check (dc.get_string_stl (repoid));
        check (dc.get_string_stl (tcname));
        content = new TypeCode ();
	check (content->decode (dc, omap, level+1));
        content->connect (this);
        check (dc.encaps_end (state));
        break;

    case TK_RECURSIVE: {
	Long recurse_pos;
	check (dc.get_long (recurse_pos));
        Long rpos = dc.buffer()->rpos() - sizeof(CORBA::Long);
        recurse_pos += rpos;

#if 1
        // fix for Visibroker (align to long boundary) ...
        if (!strcmp (dc.type(), "cdr")) {
            /*
             * we dont know the alignment base (AB) for the typecode
             * referred to, but we know that (CAB := current alignment base):
             *   AB % 4 == CAB % 4
             */
            Long alignbase = dc.buffer()->ralign_base() & 3L;
            recurse_pos = alignbase + ((recurse_pos - alignbase + 3) & ~3L);
        }
#endif

	check (recurse_pos >= 0);

	MapPosTC::iterator i = omap->find (recurse_pos);

	check (i != omap->end());

	if (CORBA::is_nil ((*i).second.second)) {
	    // recursive typecode
	    assert ((*i).second.first < level);
	    recurse_depth = level - (*i).second.first - 1;
	} else {
	    // repeated typecode
	    copy (*(*i).second.second);
	}
	break;
    }
    default:
      return FALSE;
    }

    (*omap)[buffer_pos].second = this;

    if (!_omap)
        delete omap;
    return TRUE;

bad:
    if (!_omap)
        delete omap;
    free ();
    return FALSE;
}

#undef check

void
CORBA::TypeCode::encode (DataEncoder &ec, MapTCPos *_omap) const
{
    MapTCPos *omap = _omap ? _omap : new MapTCPos;

    if (tckind == TK_RECURSIVE) {
        TypeCode_ptr p = resolve_recursion();
        if (!omap->count (p)) {
            // first occurrence. encode it as normal typecode ...
            p->encode (ec, omap);
            return;
        }
    }
    /*
     * XXX this relies on the tk_* constants beeing defined as
     *     described in [12-11]
     */
    ec.enumeration (tckind);

    /*
     * omap maps TypeCode_ptr's to the buffer position where they are first
     * encoded. this is needed for turning recursion depths into buffer
     * positions.
     */
    (*omap)[this] = ec.buffer()->wpos() - sizeof(CORBA::ULong);

    switch (tckind) {
    case tk_null:
    case tk_void:
    case tk_short:
    case tk_long:
    case tk_longlong:
    case tk_ushort:
    case tk_ulong:
    case tk_ulonglong:
    case tk_float:
    case tk_double:
    case tk_longdouble:
    case tk_boolean:
    case tk_char:
    case tk_wchar:
    case tk_octet:
    case tk_any:
    case tk_TypeCode:
    case tk_Principal:
        break;

    case tk_objref:
    case tk_native:
    case tk_abstract_interface:
    case tk_local_interface: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        ec.encaps_end (state);
        break;
    }
    case tk_except:
    case tk_struct: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        ec.put_ulong (namevec.size());
        assert (namevec.size() == tcvec.size());
        for (mico_vec_size_type i = 0; i < namevec.size(); ++i) {
            ec.put_string (namevec[i]);
            tcvec[i]->encode (ec, omap);
        }
	ec.encaps_end (state);
        break;
    }
    case tk_union: {
	CORBA::Boolean ret;
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        discriminator->encode (ec, omap);
        ec.put_long (defidx);
        ec.put_ulong (namevec.size());
        assert (namevec.size() == tcvec.size() &&
                namevec.size() == labelvec.size());
        for (mico_vec_size_type i = 0; i < namevec.size(); ++i) {
            // label value *not* encoded as any ...
#if 1
	    /*
	     * CORBA 2.2 spec explicitely says that the default
	     * case label is an instance of the discriminator
	     * type with meanigless value.
	     * However, most ORBs encode it as octet(0) ...
	     */
	    if ((CORBA::Long)i == defidx) {
		CORBA::Any a;
		a.replace (discriminator);
		ret = a.marshal (ec);
	    } else {
		ret = labelvec[i]->marshal (ec);
	    }
#else
	    ret = labelvec[i]->marshal (ec);
#endif
	    assert (ret);
            ec.put_string (namevec[i]);
            tcvec[i]->encode (ec, omap);
        }
	ec.encaps_end (state);
        break;
    }
    case tk_enum: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        ec.put_ulong (namevec.size());
        for (mico_vec_size_type i = 0; i < namevec.size(); ++i)
            ec.put_string (namevec[i]);
	ec.encaps_end (state);
        break;
    }
    case tk_string:
    case tk_wstring:
        ec.put_ulong (len);
        break;

    case tk_sequence:
    case tk_array: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        content->encode (ec, omap);
        ec.put_ulong (len);
	ec.encaps_end (state);
        break;
    }
    case tk_alias: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        content->encode (ec, omap);
	ec.encaps_end (state);
        break;
    }
    case tk_fixed:
        ec.put_ushort (digits);
        ec.put_short (scale);
        break;

    case tk_value: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
	ec.put_short (value_mod);
	if (CORBA::is_nil (content)) {
	    ec.enumeration (CORBA::tk_null);
	} else {
	    content->encode (ec, omap);
	}
        ec.put_ulong (namevec.size());
        assert (namevec.size() == tcvec.size());
        assert (namevec.size() == visvec.size());
        for (mico_vec_size_type i = 0; i < namevec.size(); ++i) {
            ec.put_string (namevec[i]);
            tcvec[i]->encode (ec, omap);
	    ec.put_short (visvec[i]);
        }
	ec.encaps_end (state);
	break;
    }
    case tk_value_box: {
	CORBA::DataEncoder::EncapsState state;
	ec.encaps_begin (state);
        ec.put_string (repoid);
        ec.put_string (tcname);
        content->encode (ec, omap);
	ec.encaps_end (state);
	break;
    }
    case TK_RECURSIVE: {
        TypeCode_ptr p = resolve_recursion();
        MapTCPos::iterator i = omap->find (p);
        assert (i != omap->end());
        ec.put_long ((*i).second - ec.buffer()->wpos());
        break;
    }
    default:
        assert (0);
    }
    omap->erase (this);
    if (!_omap)
        delete omap;
}

CORBA::TypeCode_ptr
CORBA::TypeCode::_duplicate (TypeCode_ptr t)
{
    if (!CORBA::is_nil (t)) {
	MICO_OBJ_CHECK (t);
	if (!t->is_constant())
	    t->_ref();
    }
    return t;
}

void
CORBA::release (TypeCode_ptr t)
{
    if (!CORBA::is_nil (t) && t->_check_nothrow() &&
	!t->is_constant() && t->_deref()) {
	delete t;
    }
}

CORBA::Boolean
CORBA::TypeCode::is_recursive_seq ()
{
    if (tckind != tk_sequence || content->tckind != TK_RECURSIVE)
	return FALSE;
    CORBA::TCKind k = content->resolve_recursion()->unalias()->tckind;
    return (k == tk_struct || k == tk_union);
}

/*
 * For the following IDL specification:
 *   struct S1 {
 *     struct S2 {
 *       sequence<S1> x;
 *     } y;
 *   };
 * the struct S1 is said to be freestanding and S2 is not freestanding,
 * because S2 is nested within S1 and contains a recursive reference to
 * S1 (see [12.3.4]). S2 can not be used as a parameter and therefore
 * does not have a TypeCode.
 */
CORBA::Boolean
CORBA::TypeCode::is_freestanding()
{
  return this->is_freestanding( 1, this );
}

CORBA::Boolean
CORBA::TypeCode::is_freestanding( CORBA::ULong depth, CORBA::TypeCode_ptr tc )
{
  CORBA::TCKind kindval = tc->kind();
  assert( kindval == CORBA::tk_union || kindval == CORBA::tk_struct );
  for( CORBA::ULong i = 0; i < tc->member_count(); i++ ) {
    CORBA::TypeCode_var mem_tc = tc->member_type( i );
    if( mem_tc->is_recursive_seq() ) {
      if( mem_tc->get_recurse_depth() > depth )
	return false;
    }
    kindval = mem_tc->kind();
    if( kindval == CORBA::tk_union || kindval == CORBA::tk_struct ) {
      CORBA::Boolean res = tc->is_freestanding( ++depth, mem_tc );
      if( !res )
	return false;
    }
  }
  return true;
}

CORBA::ULong
CORBA::TypeCode::get_recurse_depth ()
{
    assert (this->is_recursive_seq() && content->recurse_depth >= 0);
    return content->recurse_depth;
}

/*
 * Returns true if TypeCode is a base type
 */
CORBA::Boolean
CORBA::TypeCode::is_base_type()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  switch( tc->kind() ) {
  case CORBA::tk_boolean:
  case CORBA::tk_char:
  case CORBA::tk_wchar:
  case CORBA::tk_octet:
  case CORBA::tk_ushort:
  case CORBA::tk_ulong:
  case CORBA::tk_short:
  case CORBA::tk_long:
  case CORBA::tk_longlong:
  case CORBA::tk_ulonglong:
  case CORBA::tk_float:
  case CORBA::tk_double:
  case CORBA::tk_longdouble:
  case CORBA::tk_string:
  case CORBA::tk_wstring:
  case CORBA::tk_fixed:
    return TRUE;
  default:
    break;
  }
  return FALSE;
}

/*
 * Returns true if TypeCode is a char or an alias of a char
 */
CORBA::Boolean
CORBA::TypeCode::is_char()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_char;
}

/*
 * Returns true if TypeCode is a wchar or an alias of a wchar
 */
CORBA::Boolean
CORBA::TypeCode::is_wchar()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_wchar;
}

/*
 * Returns true if TypeCode is a boolean or an alias of a boolean
 */
CORBA::Boolean
CORBA::TypeCode::is_bool()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_boolean;
}

/*
 * Returns true if TypeCode is an octet or an alias of an octet
 */
CORBA::Boolean
CORBA::TypeCode::is_octet()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_octet;
}

/*
 * Returns true if TypeCode is an enum or an alias of an enum
 */
CORBA::Boolean
CORBA::TypeCode::is_enum()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_enum;
}

/*
 * Returns true if TypeCode is a string or an alias of a string
 */
CORBA::Boolean
CORBA::TypeCode::is_string()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_string;
}

/*
 * Returns true if TypeCode is a wstring or an alias of a wstring
 */
CORBA::Boolean
CORBA::TypeCode::is_wstring()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_wstring;
}

/*
 * Returns true if TypeCode is an any or an alias of an any
 */
CORBA::Boolean
CORBA::TypeCode::is_any()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_any;
}

/*
 * Returns true if TypeCode is a typecode or an alias of a typecode
 */
CORBA::Boolean
CORBA::TypeCode::is_type_code()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_TypeCode;
}

/*
 * Returns true if TypeCode is an alias
 */
CORBA::Boolean
CORBA::TypeCode::is_alias()
{
  return this->kind() == CORBA::tk_alias;
}

/*
 * Returns true if TypeCode is a variable length type [16.8(2)]
 */
CORBA::Boolean
CORBA::TypeCode::is_variable()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  if( tc->is_string() || tc->is_wstring() )
    return TRUE;
  
  CORBA::TCKind kindval = tc->kind();

  if( kindval == CORBA::tk_any || kindval == CORBA::tk_sequence ||
      kindval == CORBA::tk_objref || kindval == CORBA::tk_TypeCode ||
      kindval == CORBA::tk_native || kindval == CORBA::tk_abstract_interface ||
      kindval == CORBA::tk_local_interface ||
      kindval == CORBA::tk_value  || kindval == CORBA::tk_value_box )
    return TRUE;

  if( kindval == CORBA::tk_struct || kindval == CORBA::tk_union ) {
    for( CORBA::ULong i = 0; i < tc->member_count(); i++ ) {
      CORBA::TypeCode_var tc_member;
      tc_member = tc->member_type( i );
      if( tc_member->is_variable() )
	return TRUE;
    }
    return FALSE;
  }
  if( kindval == CORBA::tk_array ) {
    CORBA::TypeCode_var tc_content = tc->content_type();
    if( tc_content->is_variable() )
      return TRUE;
  }
  return FALSE;
}

/*
 * Returns true if TypeCode is an objref or an alias of an objref
 */
CORBA::Boolean
CORBA::TypeCode::is_objref()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return
    tc->kind() == CORBA::tk_objref ||
    tc->kind() == CORBA::tk_abstract_interface ||
    tc->kind() == CORBA::tk_local_interface;
}

/*
 * Returns true if TypeCode is of IDL-type Object or an alias of an Object
 */
CORBA::Boolean
CORBA::TypeCode::is_object()
{
  CORBA::TypeCode_ptr tc = this->unalias();

  if( tc->kind() == CORBA::tk_objref ) {
    CORBA::String_var nameval = tc->name();
    if( strcmp( nameval, "Object" ) == 0 )
      return true;
  }
  return false;
}

/*
 * Returns true if TypeCode is a sequence or an alias of a sequence
 */
CORBA::Boolean
CORBA::TypeCode::is_sequence()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_sequence;
}

/*
 * Returns true if TypeCode is an array or an alias of an array
 */
CORBA::Boolean
CORBA::TypeCode::is_array()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  
  return tc->kind() == CORBA::tk_array;
}

CORBA::Boolean
CORBA::TypeCode::is_valuetype()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  return tc->kind() == CORBA::tk_value || tc->kind() == CORBA::tk_value_box;
}

CORBA::Boolean
CORBA::TypeCode::is_native()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  return tc->kind() == CORBA::tk_native;
}

CORBA::Boolean
CORBA::TypeCode::is_abstract_interface()
{
  CORBA::TypeCode_ptr tc = this->unalias();
  return tc->kind() == CORBA::tk_abstract_interface;
}

/****************************** TypeCodeConst *****************************/


CORBA::TypeCodeConst::TypeCodeConst ()
{
    /*
     * dont do anything here, otherwise we would depend on
     * execution order of global construtors, because for WINDOWS_TC
     * initialization of the _tc_* constants is done in a separate
     * global constructor.
     *
     * for statically allocated objects, everything is set to zero,
     * which is exactly what we would do in the default constructor.
     */
}

CORBA::TypeCodeConst::TypeCodeConst (const char *s)
    : _tc (0), _str (s)
{
}

CORBA::TypeCodeConst &
CORBA::TypeCodeConst::operator= (const char *s)
{
    if (!_str)
        _str = s;
    return *this;
}

CORBA::TypeCode_ptr
CORBA::TypeCodeConst::operator-> ()
{
    if (!_tc) {
	assert (_str);
	_tc = (new CORBA::TypeCode (_str))->mk_constant();
    }
    return _tc;
}

CORBA::TypeCodeConst::operator CORBA::TypeCode_ptr ()
{
    if (!_tc) {
	assert (_str);
	_tc = (new CORBA::TypeCode (_str))->mk_constant();
    }
    return _tc;
}


/******************************* LevelRecord ******************************/


CORBA::LevelRecord::LevelRecord (Level l, TypeCode_ptr t,
				 Long _n, Long _i, Long _x)
{
    _level = l;
    _tc = TypeCode::_duplicate (t);
    n = _n;
    i = _i;
    last_i = _i-1;
    x = _x;
}

CORBA::LevelRecord::LevelRecord (const LevelRecord &l)
{
    _level = l._level;
    _tc = TypeCode::_duplicate (l._tc);
    n = l.n;
    i = l.i;
    last_i = l.last_i;
    x = l.x;
}

CORBA::LevelRecord::LevelRecord ()
{
    _level = LNone;
    _tc = _tc_void;
    n = i = x = 0;
    last_i = -1;
}

CORBA::LevelRecord::~LevelRecord ()
{
    CORBA::release (_tc);
}

CORBA::LevelRecord &
CORBA::LevelRecord::operator= (const LevelRecord &l)
{
    if (this != &l) {
	CORBA::release (_tc);
	_level = l._level;
	_tc = TypeCode::_duplicate (l._tc);
	n = l.n;
	i = l.i;
	x = l.x;
	last_i = l.last_i;
    }
    return *this;
}

inline CORBA::LevelRecord::Level
CORBA::LevelRecord::level () const
{
    return _level;
}

inline CORBA::TypeCode_ptr
CORBA::LevelRecord::tc ()
{
    return _tc;
}

    
/***************************** TypeCodeChecker ****************************/


CORBA::TypeCodeChecker::TypeCodeChecker ()
{
    done = TRUE;
    _tc = _tc_void;
    _top = _tc_void;
}

CORBA::TypeCodeChecker::TypeCodeChecker (TypeCode_ptr t)
{
    done = TRUE;
    _tc = TypeCode::_nil();
    _top = TypeCode::_nil();
    restart (t);
}

CORBA::TypeCodeChecker::TypeCodeChecker (const TypeCodeChecker &tcc)
{
    // XXX no real copy ...
    done = TRUE;
    _tc = TypeCode::_nil();
    _top = TypeCode::_nil();
    restart (tcc._top);
}

CORBA::TypeCodeChecker &
CORBA::TypeCodeChecker::operator= (const TypeCodeChecker &tcc)
{
    if (this != &tcc) {
	// XXX no real copy ...
	restart (tcc._top);
    }
    return *this;
}

CORBA::TypeCodeChecker::~TypeCodeChecker ()
{
    CORBA::release (_top);
}

inline void
CORBA::TypeCodeChecker::tc (TypeCode_ptr t)
{
    _tc = t->unalias ();
}

inline CORBA::LevelRecord *
CORBA::TypeCodeChecker::level ()
{
    return levelvec.size() > 0 ? &levelvec.back() : 0;
}

CORBA::ULong
CORBA::TypeCodeChecker::level_count () const
{
    return levelvec.size();
}

void
CORBA::TypeCodeChecker::override_tc (TypeCode_ptr t)
{
    if (!CORBA::is_nil (t)) {
	LevelRecord *l = level ();
	if (l) {
	    // make sure nexttc() will not overwrite _tc
	    l->last_i = l->i;
	}
	_tc = t->unalias ();
    }
}

CORBA::Boolean
CORBA::TypeCodeChecker::inside (TCKind kind)
{
    LevelRecord *l = level ();
    return l && l->tc()->unalias()->kind() == kind;
}

CORBA::Boolean
CORBA::TypeCodeChecker::level_finished ()
{
    if (done)
        return TRUE;

    LevelRecord *l = level();
    if (!l) {
	// at toplevel, but not finished
	return FALSE;
    }
    if (l->i >= l->n) {
	// no more elements in structured type
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::nexttc ()
{
    if (done)
	return FALSE;

    LevelRecord *l = level();
    if (!l) {
	// at toplevel
	return TRUE;
    }
    if (l->i >= l->n) {
	// no more elements in structured type
	return FALSE;
    }
    if (l->i == l->last_i) {
	// already set
	return TRUE;
    }

    TypeCode_var tv;

    switch (l->level()) {
    case LevelRecord::LArray:
	if (l->i == 0) {
#if 0
	    tv = l->tc()->array_type();
#else
            tv = l->tc()->content_type();
#endif
	    tc (tv);
	}
	break;

    case LevelRecord::LSequence:
	if (l->i == 0) {
	    tv = l->tc()->content_type();
	    tc (tv);
	}
	break;

    case LevelRecord::LValueBox:
	tv = l->tc()->content_type();
	tc (tv);
	break;

    case LevelRecord::LStruct:
    case LevelRecord::LExcept:
	tv = l->tc()->member_type (l->i);
	tc (tv);
	break;

    case LevelRecord::LValue:
	tv = l->tc()->member_type_inherited (l->i);
	tc (tv);
	break;

    case LevelRecord::LUnion:
	if (l->i == 0) {
	    tv = l->tc()->discriminator_type();
	    tc (tv);
	} else if (l->x >= 0) {
	    tv = l->tc()->member_type (l->x);
	    tc (tv);
	}
	break;

    default:
	assert (0);
    }
    l->last_i = l->i;
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::advance ()
{
    if (done)
	return FALSE;

    LevelRecord *l = level();
    if (!l) {
	// at toplevel
	return TRUE;
    }
    if (l->i >= l->n) {
	// no more elements in structured type
	return FALSE;
    }
    l->i += 1;
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::leave (LevelRecord::Level lev, Boolean sloppy)
{
    LevelRecord *l = level ();
    if (!l || l->level() != lev)
	return FALSE;
    if (!sloppy && l->i != l->n)
	return FALSE;
    tc (l->tc());
    levelvec.erase (levelvec.end() - 1);
    if (levelvec.size() == 0)
	done = TRUE;
    return TRUE;
}

inline void
CORBA::TypeCodeChecker::enter (const LevelRecord &rec)
{
    levelvec.push_back (rec);
}

CORBA::TypeCode_ptr
CORBA::TypeCodeChecker::tc ()
{
    nexttc ();
    return done ? _tc_void : _tc;
}

CORBA::Boolean
CORBA::TypeCodeChecker::basic (TypeCode_ptr t)
{
    if (!nexttc())
	return FALSE;
    if (!_tc->equivalent (t->unalias())) {
	return FALSE;
    }
    advance ();
    if (!level())
	done = TRUE;
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::abstract_interface (TypeCode_ptr override)
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_abstract_interface)
	return FALSE;
    override_tc (override);
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::enumeration (Long val)
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_enum || val >= (Long)_tc->member_count())
	return FALSE;
    advance ();
    if (!level())
	done = TRUE;
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::union_begin ()
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_union)
	return FALSE;
    advance ();
    enter (LevelRecord (LevelRecord::LUnion, _tc, 1));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::union_selection (Long sel_idx)
{
    LevelRecord *l = level ();
    if (!l || l->level() != LevelRecord::LUnion || l->i > 1)
	return FALSE;
    if (sel_idx >= 0 && sel_idx >= (Long)l->tc()->member_count())
	return FALSE;
    l->n = sel_idx < 0 ? 1 : 2;
    l->x = sel_idx;
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::union_end (Boolean sloppy)
{
    return leave (LevelRecord::LUnion, sloppy);
}

CORBA::Boolean
CORBA::TypeCodeChecker::struct_begin ()
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_struct)
	return FALSE;
    advance ();
    enter (LevelRecord (LevelRecord::LStruct, _tc, _tc->member_count()));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::struct_end (Boolean sloppy)
{
    return leave (LevelRecord::LStruct, sloppy);
}

CORBA::Boolean
CORBA::TypeCodeChecker::except_begin ()
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_except)
	return FALSE;
    advance ();
    enter (LevelRecord (LevelRecord::LExcept, _tc, _tc->member_count()));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::except_end (Boolean sloppy)
{
    return leave (LevelRecord::LExcept, sloppy);
}

CORBA::Boolean
CORBA::TypeCodeChecker::seq_begin (ULong len)
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_sequence ||
	(_tc->length() > 0 && len > _tc->length()))
	return FALSE;
    advance ();
    enter (LevelRecord (LevelRecord::LSequence, _tc, len));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::seq_end (Boolean sloppy)
{
    return leave (LevelRecord::LSequence, sloppy);
}

CORBA::Boolean
CORBA::TypeCodeChecker::arr_begin ()
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_array)
	return FALSE;
    advance ();
#if 0
    enter (LevelRecord (LevelRecord::LArray, _tc, _tc->array_size()));
#else
    enter (LevelRecord (LevelRecord::LArray, _tc, _tc->length()));
#endif
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::arr_end (Boolean sloppy)
{
    return leave (LevelRecord::LArray, sloppy);
}

CORBA::Boolean
CORBA::TypeCodeChecker::value_begin (TypeCode_ptr override)
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_value)
	return FALSE;
    override_tc (override);
    advance ();
    enter (LevelRecord (LevelRecord::LValue, _tc,
			_tc->member_count_inherited()));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::value_end ()
{
    return leave (LevelRecord::LValue, TRUE);
}

CORBA::Boolean
CORBA::TypeCodeChecker::valuebox_begin ()
{
    if (!nexttc())
	return FALSE;
    if (_tc->kind() != tk_value_box)
	return FALSE;
    advance ();
    enter (LevelRecord (LevelRecord::LValueBox, _tc, 1));
    return TRUE;
}

CORBA::Boolean
CORBA::TypeCodeChecker::valuebox_end ()
{
    return leave (LevelRecord::LValueBox);
}

CORBA::Boolean
CORBA::TypeCodeChecker::completed () const
{
    return done;
}

void
CORBA::TypeCodeChecker::finish ()
{
    levelvec.erase (levelvec.begin(), levelvec.end());
    done = TRUE;
    tc (_top);
}

void
CORBA::TypeCodeChecker::restart ()
{
    levelvec.erase (levelvec.begin(), levelvec.end());
    done = TRUE;
    CORBA::release (_top);
    _top = _tc_void;
    tc (_top);
}

void
CORBA::TypeCodeChecker::restart (TypeCode_ptr t)
{
    levelvec.erase (levelvec.begin(), levelvec.end());
    done = FALSE;
    CORBA::release (_top);
    _top = TypeCode::_duplicate (t);
    tc (_top);
}


/***************************** _tc_* constants ****************************/


static struct InitTypeCodeConstants {
    InitTypeCodeConstants ()
    {
    CORBA::_tc_null =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_null)->mk_constant();

	CORBA::_tc_void =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_void)->mk_constant();

	CORBA::_tc_short =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_short)->mk_constant();

	CORBA::_tc_long =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_long)->mk_constant();

	CORBA::_tc_longlong =
	    CORBA::TypeCode::create_basic_tc(
		CORBA::tk_longlong)->mk_constant();

	CORBA::_tc_ushort =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_ushort)->mk_constant();

	CORBA::_tc_ulong =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_ulong)->mk_constant();

	CORBA::_tc_ulonglong =
	    CORBA::TypeCode::create_basic_tc(
		CORBA::tk_ulonglong)->mk_constant();

	CORBA::_tc_float =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_float)->mk_constant();

	CORBA::_tc_double =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_double)->mk_constant();

	CORBA::_tc_longdouble =
	    CORBA::TypeCode::create_basic_tc(
		CORBA::tk_longdouble)->mk_constant();

	CORBA::_tc_boolean =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_boolean)->mk_constant();

	CORBA::_tc_char =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_char)->mk_constant();

	CORBA::_tc_wchar =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_wchar)->mk_constant();

	CORBA::_tc_octet =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_octet)->mk_constant();

	CORBA::_tc_any =
	    CORBA::TypeCode::create_basic_tc(CORBA::tk_any)->mk_constant();

	CORBA::_tc_TypeCode =
	    CORBA::TypeCode::create_basic_tc(
		CORBA::tk_TypeCode)->mk_constant();

	CORBA::_tc_Principal =
	    CORBA::TypeCode::create_basic_tc(
		CORBA::tk_Principal)->mk_constant();

	CORBA::_tc_Object =
	    CORBA::TypeCode::create_interface_tc(
		"IDL:omg.org/CORBA/Object:1.0",
		"Object")->mk_constant();

	CORBA::_tc_ValueBase =
	    CORBA::TypeCode::create_value_tc(
		"IDL:omg.org/CORBA/ValueBase:1.0",
		"ValueBase",
		CORBA::VM_NONE,
		CORBA::TypeCode::_nil(),
		CORBA::ValueMemberSeq())->mk_constant();

	CORBA::_tc_string =
	    CORBA::TypeCode::create_string_tc(0)->mk_constant();

	CORBA::_tc_wstring =
	    CORBA::TypeCode::create_wstring_tc(0)->mk_constant();
    
	// XXX extension
	CORBA::_tc_Context =
	    CORBA::TypeCode::create_sequence_tc(
		0, CORBA::_tc_string)->mk_constant();

	CORBA::_tc_AbstractBase =
	    CORBA::TypeCode::create_abstract_interface_tc(
		"IDL:omg.org/CORBA/AbstractBase:1.0",
		"AbstractBase")->mk_constant();

	{
	    CORBA::StructMemberSeq ss;
	    ss.length (4);

	    ss[0].name = (const char *)"name";
	    ss[0].type = CORBA::_tc_string;
	    ss[0].type_def = 0;

	    ss[1].name = (const char *)"argument";
	    ss[1].type = CORBA::_tc_any;
	    ss[1].type_def = 0;

	    ss[2].name = (const char *)"len";
	    ss[2].type = CORBA::_tc_long;
	    ss[2].type_def = 0;

	    ss[3].name = (const char *)"arg_modes";
	    ss[3].type = CORBA::_tc_ulong;
	    ss[3].type_def = 0;

	    CORBA::_tc_NamedValue = CORBA::TypeCode::create_struct_tc (
		"IDL:omg.org/CORBA/NamedValue:1.0",
		"NamedValue",
		ss)->mk_constant();
	}

	CORBA::TypeCode_ptr _tc_CompletionStatus;
	{
	    CORBA::EnumMemberSeq es;
	    es.length (3);
	    es[0] = CORBA::string_dup ("COMPLETED_YES");
	    es[1] = CORBA::string_dup ("COMPLETED_NO");
	    es[2] = CORBA::string_dup ("COMPLETED_MAYBE");

	    _tc_CompletionStatus = CORBA::TypeCode::create_enum_tc (
		"IDL:omg.org/CORBA/CompletionStatus:1.0",
		"CompletionStatus",
		es)->mk_constant();
	}

#define SYSEXC(exname)							   \
	{								   \
	    CORBA::StructMemberSeq ss;					   \
	    ss.length (2);						   \
									   \
	    ss[0].name = (const char *)"minor";				   \
	    ss[0].type = CORBA::_tc_ulong;				   \
	    ss[0].type_def = 0;						   \
									   \
	    ss[1].name = (const char *)"completed";			   \
	    ss[1].type = _tc_CompletionStatus;				   \
	    ss[1].type_def = 0;						   \
									   \
	    CORBA::_tc_ ## exname = CORBA::TypeCode::create_exception_tc ( \
		"IDL:omg.org/CORBA/" #exname ":1.0",			   \
		#exname,						   \
		ss)->mk_constant();					   \
	}

#include <mico/sysexc.h>
#undef SYSEXC

    }
} __init_tc_constants;

namespace CORBA {
    CORBA::TypeCode_ptr _tc_null;
    CORBA::TypeCode_ptr _tc_void;
    CORBA::TypeCode_ptr _tc_short;
    CORBA::TypeCode_ptr _tc_long;
    CORBA::TypeCode_ptr _tc_longlong;
    CORBA::TypeCode_ptr _tc_ushort;
    CORBA::TypeCode_ptr _tc_ulong;
    CORBA::TypeCode_ptr _tc_ulonglong;
    CORBA::TypeCode_ptr _tc_float;
    CORBA::TypeCode_ptr _tc_double;
    CORBA::TypeCode_ptr _tc_longdouble;
    CORBA::TypeCode_ptr _tc_boolean;
    CORBA::TypeCode_ptr _tc_char;
    CORBA::TypeCode_ptr _tc_wchar;
    CORBA::TypeCode_ptr _tc_octet;
    CORBA::TypeCode_ptr _tc_any;
    CORBA::TypeCode_ptr _tc_TypeCode;
    CORBA::TypeCode_ptr _tc_Principal;
    CORBA::TypeCode_ptr _tc_Object;
    CORBA::TypeCode_ptr _tc_ValueBase;
    CORBA::TypeCode_ptr _tc_AbstractBase; // extension
    CORBA::TypeCode_ptr _tc_string;
    CORBA::TypeCode_ptr _tc_wstring;
    CORBA::TypeCode_ptr _tc_Context; // extension
    CORBA::TypeCode_ptr _tc_NamedValue; // extension

#define SYSEXC(name) CORBA::TypeCode_ptr _tc_ ## name;
#include <mico/sysexc.h>
#undef SYSEXC

}
