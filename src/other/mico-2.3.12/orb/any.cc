/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
#ifdef _WIN32
#include <string.h>
#endif
#include <mico/impl.h>
#include <mico/util.h>
#include <mico/throw.h>
#include <mico/os-math.h>
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

CORBA::Any::Any ()
{
    thetc = _tc_void;
    checker = new TypeCodeChecker ();
    ec = new MICO::CDREncoder ();
    dc = new MICO::CDRDecoder (ec->buffer(), FALSE, ec->byteorder(),
			       ec->converter(), FALSE);
    ec->valuestate (&value_estate, FALSE);
    dc->valuestate (&value_dstate, FALSE);
    extracted_value = 0;
}

CORBA::Any::Any (TypeCode_ptr tc, Buffer *b, Boolean release, int /*dummy*/)
{
    thetc = CORBA::TypeCode::_duplicate (tc);
    checker = new TypeCodeChecker ();
    ec = new MICO::CDREncoder (b, FALSE, DefaultEndian, 0, FALSE, 0, TRUE);
    dc = new MICO::CDRDecoder (b, release, DefaultEndian, 0, FALSE, 0, TRUE);
    ec->valuestate (&value_estate, FALSE);
    dc->valuestate (&value_dstate, FALSE);
    extracted_value = 0;
}

CORBA::Any::Any (TypeCode_ptr tc, DataDecoder *_dc, DataEncoder *_ec)
{
    thetc = CORBA::TypeCode::_duplicate (tc);
    checker = new TypeCodeChecker ();
    ec = _ec;
    dc = _dc;
    // use preset valuestates ...
    extracted_value = 0;
}

CORBA::Any::Any (const Any &a)
{
    copy (a);
}

CORBA::Any::Any (TypeCode_ptr tc, void *value, Boolean release)
{
    thetc = _tc_void;
    checker = new TypeCodeChecker ();
    ec = new MICO::CDREncoder ();
    dc = new MICO::CDRDecoder (ec->buffer(), FALSE, ec->byteorder(),
			       ec->converter(), FALSE);
    ec->valuestate (&value_estate, FALSE);
    dc->valuestate (&value_dstate, FALSE);
    extracted_value = 0;

    replace (tc, value, release);
}

CORBA::Any::~Any ()
{
    free ();
}

inline CORBA::TypeCode_ptr
CORBA::Any::tc () const
{
    return thetc;
}

inline void
CORBA::Any::tc (TypeCode_ptr ntc)
{
    CORBA::release (thetc);
    thetc = ntc;
}

inline void
CORBA::Any::tc_if_changed (TypeCode_ptr ntc)
{
    if (thetc->equaltype (ntc)) {
	CORBA::release (ntc);
    } else {
	CORBA::release (thetc);
	thetc = ntc;
    }
}

CORBA::Any &
CORBA::Any::operator= (const Any &a)
{
    if (this != &a) {
	free ();
	copy (a);
    }
    return *this;
}

CORBA::Boolean
CORBA::Any::operator== (const Any &_a) const
{
    if (this == &_a)
	return TRUE;

    Any &me = (Any &)*this;
    Any &he = (Any &)_a;

    if (strcmp (me.ec->type(), he.ec->type()) ||
	strcmp (me.dc->type(), he.dc->type()))
	// different codec types, do a structural compare
	return equivalent (_a);

    if (!me.tc()->equaltype (he.tc()))
	return FALSE;

    return (me.length() == he.length() &&
            !memcmp (me.ec->buffer()->buffer(),
                     he.ec->buffer()->buffer(), me.length()));
}

CORBA::Boolean
CORBA::Any::equivalent (const Any &_a) const
{
    if (this == &_a)
	return TRUE;

    Any me (*this);
    Any he (_a);

    me.prepare_read ();
    he.prepare_read ();

    Boolean retval = me.compare_any (he);

    return retval;
}

void
CORBA::Any::free ()
{
    delete ec;
    delete dc;
    delete checker;
    CORBA::release (thetc);
    if (extracted_value)
	delete extracted_value;
}

void
CORBA::Any::copy (const Any &a)
{
    thetc = TypeCode::_duplicate (a.thetc);
    checker = new TypeCodeChecker ();
    value_estate = a.value_estate;
    value_dstate = a.value_dstate;
    ec = a.ec->clone ();
    dc = a.dc->clone (ec->buffer(), FALSE);
    ec->valuestate (&value_estate, FALSE);
    dc->valuestate (&value_dstate, FALSE);
    extracted_value = 0;
}

void
CORBA::Any::reset_extracted_value ()
{
    if (extracted_value) {
	delete extracted_value;
	extracted_value = 0;
    }
}

void
CORBA::Any::prepare_write ()
{
    if (checker->completed()) {
	value_estate.reset();
	ec->buffer()->reset();
	checker->restart (tc());
	reset_extracted_value();
    }
}

void
CORBA::Any::prepare_read ()
{
    if (checker->completed()) {
	value_dstate.reset();
	dc->buffer()->rseek_beg (0);
	checker->restart (tc());
    }
}

CORBA::Boolean
CORBA::Any::copy_any (Any &a, Boolean recurse)
{
    MapLL vidmap;
    return copy_any (a, vidmap, recurse);
}

CORBA::Boolean
CORBA::Any::copy_any (Any &a, MapLL &vidmap, Boolean recurse)
{
    TypeCode_ptr atc = a.checker->tc()->unalias();

    switch (atc->kind()) {
    case tk_null:
    case tk_void:
	return a.checker->basic (atc) && checker->basic (atc);

    case tk_short: {
	Short s;
	return (a >>= s) && insert (s);
    }
    case tk_long: {
	Long l;
	return (a >>= l) && insert (l);
    }
    case tk_longlong: {
	LongLong l;
	return (a >>= l) && insert (l);
    }
    case tk_ushort: {
	UShort us;
	return (a >>= us) && insert (us);
    }
    case tk_ulong: {
	ULong ul;
	return (a >>= ul) && insert (ul);
    }
    case tk_ulonglong: {
	ULongLong ul;
	return (a >>= ul) && insert (ul);
    }
    case tk_float: {
	Float f;
	return (a >>= f) && insert (f);
    }
    case tk_double: {
	Double d;
	return (a >>= d) && insert (d);
    }
    case tk_longdouble: {
	LongDouble d;
	return (a >>= d) && insert (d);
    }
    case tk_boolean: {
	Boolean b;
	return (a >>= to_boolean (b)) && insert (from_boolean (b));
    }
    case tk_char: {
	Char c;
	return (a >>= to_char (c)) && insert (from_char (c));
    }
    case tk_wchar: {
	WChar c;
	return (a >>= to_wchar (c)) && insert (from_wchar (c));
    }
    case tk_octet: {
	Octet o;
	return (a >>= to_octet (o)) && insert (from_octet (o));
    }
    case tk_any: {
	const Any* any;
	return ((a >>= any) && insert (*any));
    }
    case tk_objref: {
	Object_var obj;
	String_var name;
	return ((a >>= to_object (obj, name.out())) &&
		insert (from_object (obj, name)));
    }
    case tk_enum: {
	ULong en;
	return a.enum_get (en) && enum_put (en);
    }
    case tk_string: {
	const char *thestring;
	return ((a >>= to_string (thestring, atc->length())) &&
		insert (from_string (thestring, atc->length())));
    }
    case tk_wstring: {
	const wchar_t *thestring;
	return ((a >>= to_wstring (thestring, atc->length())) &&
		insert (from_wstring (thestring, atc->length())));
    }
    case tk_fixed: {
        UShort digits = atc->fixed_digits();
        Short scale = atc->fixed_scale();
	Fixed fixed (digits, scale);
	return ((a >>= to_fixed (fixed, digits, scale)) &&
		insert (from_fixed (fixed, digits, scale)));
    }
    case tk_alias:
	assert (0);

    case tk_TypeCode: {
	TypeCode_ptr t;
	return ((a >>= t) && insert (t));
    }
    case tk_Principal: {
	Principal_ptr p;
	return ((a >>= p) && insert (p));
    }
    default:
	break;
    }
    if (!recurse)
	return FALSE;

    switch (atc->kind()) {
    case tk_except: {
	String_var repoid;
	if (!a.except_get_begin (repoid) || !except_put_begin (repoid))
	    return FALSE;
	CORBA::ULong n = atc->member_count();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!copy_any (a, vidmap))
		return FALSE;
	}
	return a.except_get_end() && except_put_end();
    }
    case tk_struct: {
	if (!a.struct_get_begin() || !struct_put_begin())
	    return FALSE;
	CORBA::ULong n = atc->member_count();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!copy_any (a, vidmap))
		return FALSE;
	}
	return a.struct_get_end() && struct_put_end();
    }
    case tk_union: {
	if (!a.union_get_begin() || !union_put_begin())
	    return FALSE;
	Any adisc;
	if (!a.any_get (adisc, FALSE))
	    return FALSE;
	adisc.prepare_read ();
	if (!copy_any (adisc, vidmap, FALSE))
	    return FALSE;
	Long i = atc->member_index (adisc);
	if (i >= 0) {
	    if (!a.union_get_selection (i) || !union_put_selection (i))
		return FALSE;
	    if (!copy_any (a, vidmap))
		return FALSE;
	}
	return a.union_get_end() && union_put_end();
    }
    case tk_sequence: {
	ULong seqlen;
	if (!a.seq_get_begin (seqlen) || !seq_put_begin (seqlen))
	    return FALSE;

	/*
	 * __FP__ begin
	 */

	CORBA::TypeCode_var ctc = atc->content_type ();
	if (ctc->unalias()->kind() == CORBA::tk_octet) {
	  CORBA::Buffer * buf = a.dc->buffer ();
	  if (buf->length() < seqlen) {
	    return FALSE;
	  }
	  /*
	   * Copy all octets at once
	   */
	  ec->put_octets (buf->data(), seqlen);
	  buf->rseek_rel (seqlen);
	  /*
	   * We've just confused the TypeCode Checker
	   */
	  Boolean r;
	  r  = checker->seq_end (TRUE);
	  r &= a.checker->seq_end (TRUE);
	  r &= a.dc->seq_end ();
	  ec->seq_end ();
	  reset_extracted_value ();
	  assert (r);
	  return r;
	}

	/*
	 * __FP__ end
	 */

	for (CORBA::ULong i = 0; i < seqlen; ++i) {
	  if (!copy_any (a, vidmap))
	    return FALSE;
	}
	return a.seq_get_end() && seq_put_end();
    }
    case tk_array: {
	ULong arrlen = atc->length();
	if (!a.array_get_begin() || !array_put_begin())
	    return FALSE;

	/*
	 * __FP__ begin
	 */

	CORBA::TypeCode_var ctc = atc->content_type ();
	if (ctc->unalias()->kind() == CORBA::tk_octet) {
	  CORBA::Buffer * buf = a.dc->buffer ();
	  if (buf->length() < arrlen) {
	    return FALSE;
	  }
	  /*
	   * Copy all octets at once
	   */
	  ec->put_octets (buf->data(), arrlen);
	  buf->rseek_rel (arrlen);
	  /*
	   * We've just confused the TypeCode Checker
	   */
	  Boolean r;
	  r  = checker->arr_end (TRUE);
	  r &= a.checker->arr_end (TRUE);
	  r &= a.dc->arr_end ();
	  ec->arr_end ();
	  reset_extracted_value ();
	  assert (r);
	  return r;
	}

	/*
	 * __FP__ end
	 */

	for (CORBA::ULong i = 0; i < arrlen; ++i) {
	    if (!copy_any (a, vidmap))
		return FALSE;
	}
	return a.array_get_end() && array_put_end();
    }
    case tk_value: {
	Long vid1, vid2;
	Boolean is_ref;
	if (!a.value_get_begin (vid1, is_ref))
	    return FALSE;
	if (is_ref) {
	    vid2 = 0;
	    if (vid1) {
		assert (vidmap.count (vid1));
		vid2 = vidmap[vid1];
	    }
	    return value_put_ref (vid2);
	}
	if (!value_put_begin (vid2))
	    return FALSE;
	vidmap[vid1] = vid2;
	CORBA::ULong n = atc->member_count_inherited();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!copy_any (a, vidmap))
		return FALSE;
	}
	return a.value_get_end (vid1, is_ref) && value_put_end(vid1);
    }
    case tk_value_box: {
	Long vid1, vid2;
	Boolean is_ref;
	if (!a.valuebox_get_begin (vid1, is_ref))
	    return FALSE;
	if (is_ref) {
	    vid2 = 0;
	    if (vid1) {
		assert (vidmap.count (vid1));
		vid2 = vidmap[vid1];
	    }
	    return valuebox_put_ref (vid2);
	}
	if (!valuebox_put_begin (vid2))
	    return FALSE;
	vidmap[vid1] = vid2;
	if (!copy_any (a, vidmap))
	    return FALSE;
	return a.valuebox_get_end (vid1, is_ref) && valuebox_put_end (vid1);
    }
    default:
	assert (0);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::compare_any (Any &a)
{
    MapLL vidmap;
    return compare_any (a, vidmap);
}

CORBA::Boolean
CORBA::Any::compare_any (Any &a, MapLL &vidmap)
{
    TypeCode_ptr atc = a.checker->tc()->unalias();
    TypeCode_ptr mtc = checker->tc()->unalias();

    /*
     * TypeCode::max() returns the "maximum type" of the two typecodes
     * if both typecodes are arithmetic types. Otherwise it returns
     * atc.
     */
    switch (atc->kind()) {
    case tk_null:
    case tk_void:
	return a.checker->basic (atc) && checker->basic (atc);

    case tk_short: {
	Short s1, s2;
	return (a >>= s1) && (*this >>= s2) && s1 == s2;
    }
    case tk_long: {
	Long l1, l2;
	return (a >>= l1) && (*this >>= l2) && l1 == l2;
    }
    case tk_longlong: {
	LongLong l1, l2;
	return (a >>= l1) && (*this >>= l2) && l1 == l2;
    }
    case tk_ushort: {
	UShort us1, us2;
	return (a >>= us1) && (*this >>= us2) && us1 == us2;
    }
    case tk_ulong: {
	ULong ul1, ul2;
	return (a >>= ul1) && (*this >>= ul2) && ul1 == ul2;
    }
    case tk_ulonglong: {
	ULongLong ul1, ul2;
	return (a >>= ul1) && (*this >>= ul2) && ul1 == ul2;
    }
    case tk_float: {
	Float f1, f2;
	return (a >>= f1) && (*this >>= f2) && f1 == f2;
    }
    case tk_double: {
	Double d1, d2;
	return (a >>= d1) && (*this >>= d2) && d1 == d2;
    }
    case tk_longdouble: {
	LongDouble d1, d2;
	return (a >>= d1) && (*this >>= d2) && d1 == d2;
    }
    case tk_boolean: {
	Boolean b1, b2;
	return (a >>= to_boolean (b1)) && (*this >>= to_boolean (b2))
	    && b1 == b2;
    }
    case tk_char: {
	Char c1, c2;
	return (a >>= to_char (c1)) && (*this >>= to_char (c2)) && c1 == c2;
    }
    case tk_wchar: {
	WChar c1, c2;
	return (a >>= to_wchar (c1)) && (*this >>= to_wchar (c2)) && c1 == c2;
    }
    case tk_octet: {
	Octet o1, o2;
	return (a >>= to_octet (o1)) && (*this >>= to_octet (o2)) && o1 == o2;
    }
    case tk_any: {
	const Any *any1, *any2;
	return (a >>= any1) && (*this >>= any2) && *any1 == *any2;
    }
    case tk_objref: {
	Object_var objptr1, objptr2;
	return ((a >>= to_object (objptr1)) &&
		(*this >>= to_object (objptr2)) &&
		objptr1->_is_equivalent (objptr2));
    }
    case tk_enum: {
	ULong en1, en2;
	return a.enum_get (en1) && enum_get (en2) && en1 == en2;
    }
    case tk_string: {
	const char *str1, *str2;
	return ((a >>= to_string (str1, atc->length())) &&
		(*this >>= to_string (str2, mtc->length())) &&
		!strcmp (str1, str2));
    }
    case tk_wstring: {
	const wchar_t *str1, *str2;
	return ((a >>= to_wstring (str1, atc->length())) &&
		(*this >>= to_wstring (str2, mtc->length())) &&
		!xwcscmp (str1, str2));
    }
    case tk_fixed: {
        UShort digits = atc->fixed_digits();
        Short scale = atc->fixed_scale();
	Fixed val1 (digits, scale), val2 (digits, scale);
	return ((a >>= to_fixed (val1, digits, scale)) &&
		(*this >>= to_fixed (val2, digits, scale)) &&
                val1 == val2);
    }
    case tk_alias:
	assert (0);

    case tk_TypeCode: {
	TypeCode_ptr t1, t2;
	return ((a >>= t1) && (*this >>= t2) && t1->equal (t2));
    }
    case tk_Principal: {
	Principal_ptr p1, p2;
	return ((a >>= p1) && (*this >>= p2) && *p1 == *p2);
    }
    case tk_except: {
	String_var repoid1, repoid2;
	if (!a.except_get_begin (repoid1) ||
	    !except_get_begin (repoid2) || strcmp (repoid1, repoid2))
	    return FALSE;
	if (atc->member_count() != mtc->member_count())
	    return FALSE;
	CORBA::ULong n = atc->member_count();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.except_get_end() && except_get_end();
    }
    case tk_struct: {
	if (!a.struct_get_begin() || !struct_get_begin())
	    return FALSE;
	if (atc->member_count() != mtc->member_count())
	    return FALSE;
	CORBA::ULong n = atc->member_count();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.struct_get_end() && struct_get_end();
    }
    case tk_union: {
	if (!a.union_get_begin() || !union_get_begin())
	    return FALSE;
	Any adisc, mdisc;
	if (!a.any_get (adisc, FALSE))
	    return FALSE;
	if (!any_get (mdisc, FALSE))
	    return FALSE;
	if (!adisc.equivalent (mdisc))
	    return FALSE;
	Long i = atc->member_index (adisc);
	if (i != mtc->member_index (mdisc))
	    return FALSE;
	if (i >= 0) {
	    if (!a.union_get_selection (i) || !union_get_selection (i))
		return FALSE;
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.union_get_end() && union_get_end();
    }
    case tk_sequence: {
	ULong seqlen1, seqlen2;
	if (!a.seq_get_begin (seqlen1) || !seq_get_begin (seqlen2))
	    return FALSE;
	if (seqlen1 != seqlen2)
	    return FALSE;
	for (CORBA::ULong i = 0; i < seqlen1; ++i) {
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.seq_get_end() && seq_get_end();
    }
    case tk_array: {
	ULong arrlen1 = atc->length();
	ULong arrlen2 = mtc->length();
	if (arrlen1 != arrlen2)
	    return FALSE;
	if (!a.array_get_begin() || !array_get_begin())
	    return FALSE;
	for (CORBA::ULong i = 0; i < arrlen1; ++i) {
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.array_get_end() && array_get_end();
    }
    case tk_value: {
	Long vid1, vid2;
	Boolean is_ref1, is_ref2;
	if (!a.value_get_begin (vid1, is_ref1))
	    return FALSE;
	if (!value_get_begin (vid2, is_ref2))
	    return FALSE;
	if (!!is_ref1 != !!is_ref2)
	    return FALSE;
	if (is_ref1) {
	    if (!!vid1 != !!vid2)
		return FALSE;
	    if (vid1 == 0)
		return TRUE;
	    assert (vidmap.count (vid1));
	    return (vidmap[vid1] == vid2);
	}
	vidmap[vid1] = vid2;
	CORBA::ULong n = atc->member_count_inherited();
	for (CORBA::ULong i = 0; i < n; ++i) {
	    if (!compare_any (a, vidmap))
		return FALSE;
	}
	return a.value_get_end (vid1, is_ref1) &&
	    value_get_end (vid2, is_ref2);
    }
    case tk_value_box: {
	Long vid1, vid2;
	Boolean is_ref1, is_ref2;
	if (!a.valuebox_get_begin (vid1, is_ref1))
	    return FALSE;
	if (!valuebox_get_begin (vid2, is_ref2))
	    return FALSE;
	if (!!is_ref1 != !!is_ref2)
	    return FALSE;
	if (is_ref1) {
	    if (!!vid1 != !!vid2)
		return FALSE;
	    if (vid1 == 0)
		return TRUE;
	    assert (vidmap.count (vid1));
	    return (vidmap[vid1] == vid2);
	}
	vidmap[vid1] = vid2;
	if (!compare_any (a, vidmap))
	    return FALSE;
	return a.valuebox_get_end (vid1, is_ref1) &&
	    valuebox_get_end (vid2, is_ref2);
    }
    default:
	assert (0);
    }
    return FALSE;
}

void
CORBA::Any::replace (TypeCode_ptr tc, void *value, Boolean release)
{
    reset ();
    set_type (tc);

    if (value) {
	Buffer b (value);
	Any a (tc, &b, FALSE, /*dummy*/ 0);

	prepare_write ();
	a.prepare_read ();
	CORBA::Boolean r = copy_any (a);
	assert (r);

	if (release)
	    // XXX ???
	    delete[] (CORBA::Octet *)value;
    }
}

void
CORBA::Any::replace (TypeCode_ptr tc)
{
    reset ();
    set_type (tc);

    switch (tc->unalias()->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
	break;
    case CORBA::tk_char:
	*this <<= CORBA::Any::from_char (0);
	break;
    case CORBA::tk_wchar:
	*this <<= CORBA::Any::from_wchar (0);
	break;
    case CORBA::tk_boolean:
	*this <<= CORBA::Any::from_boolean (FALSE);
	break;
    case CORBA::tk_octet:
	*this <<= CORBA::Any::from_octet (0);
	break;
    case CORBA::tk_short:
	*this <<= (CORBA::Short)0;
	break;
    case CORBA::tk_ushort:
	*this <<= (CORBA::UShort)0;
	break;
    case CORBA::tk_long:
	*this <<= (CORBA::Long)0;
	break;
    case CORBA::tk_longlong:
	*this <<= (CORBA::LongLong)0;
	break;
    case CORBA::tk_ulong:
	*this <<= (CORBA::ULong)0;
	break;
    case CORBA::tk_ulonglong:
	*this <<= (CORBA::ULongLong)0;
	break;
    case CORBA::tk_float:
	*this <<= (CORBA::Float)0;
	break;
    case CORBA::tk_double:
	*this <<= (CORBA::Double)0;
	break;
    case CORBA::tk_longdouble:
	*this <<= (CORBA::LongDouble)0;
	break;
    case CORBA::tk_any:
	*this <<= CORBA::Any();
	break;
    case CORBA::tk_TypeCode:
	*this <<= CORBA::_tc_void;
	break;
    case CORBA::tk_objref:
	*this <<= CORBA::Any::from_object (CORBA::Object::_nil(), "");
	break;
    case CORBA::tk_string:
        *this <<= "";
        break;
    case CORBA::tk_wstring:
        *this <<= L"";
        break;
    case CORBA::tk_enum:
	enum_put (0);
	break;
    default:
	assert (0);
    }
}

CORBA::TypeCode_ptr
CORBA::Any::type () const
{
    return CORBA::TypeCode::_duplicate (tc());
}

void
CORBA::Any::set_type (TypeCode_ptr t)
{
    if (checker->level_count() == 0) {
	value_estate.reset();
	value_dstate.reset();
        ec->buffer()->reset ();
        tc (CORBA::TypeCode::_duplicate (t));
        checker->restart (tc());
	reset_extracted_value();

        switch (t->unalias()->kind()) {
        case CORBA::tk_void:
        case CORBA::tk_null:
            /*
             * make sure checker is completed, otherwise marshal()
             * will fail.
             */
            checker->finish();
            break;

	default:
	    break;
        }
    }
}

void
CORBA::Any::type (TypeCode_ptr t)
{
    if (checker->level_count() != 0)
        mico_throw (CORBA::BAD_INV_ORDER());
    if (!t->equivalent (tc()))
        mico_throw (CORBA::BAD_TYPECODE());
    tc (CORBA::TypeCode::_duplicate (t));
    checker->restart (tc());

    switch (tc()->unalias()->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
        /*
         * make sure checker is completed, otherwise marshal()
         * will fail.
         */
        checker->finish();
        break;

    default:
        break;
    }
}

void
CORBA::Any::reset ()
{
    value_estate.reset();
    value_dstate.reset();
    ec->buffer()->reset ();
    checker->restart ();
    reset_extracted_value();
}

void
CORBA::Any::rewind ()
{
    value_estate.reset();
    value_dstate.reset();
    dc->buffer()->rseek_beg (0);
    checker->restart ();
}

const void *
CORBA::Any::value () const
{
    if (ec->buffer()->wpos() > 0)
	return ec->buffer()->buffer();
    return 0;
}

CORBA::ULong
CORBA::Any::length () const
{
    return ec->buffer()->wpos();
}

void
CORBA::Any::encode (DataEncoder &e) const
{
    e.put_typecode (*tc());
    ((Any *)this)->marshal (e);
}

CORBA::Boolean
CORBA::Any::decode (DataDecoder &e)
{
    TypeCode_ptr t = TypeCode::create_basic_tc (tk_null);
    if (!e.get_typecode (*t) || !demarshal (t, e))
	return FALSE;
    CORBA::release (t);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::marshal (DataEncoder &e)
{
    assert (checker->completed());
    Any a (tc(), new MICO::CDRDecoder,
	   e.clone (e.buffer(), FALSE,
		    e.converter(), FALSE,
		    e.valuestate(), FALSE));

    // cannot use a.prepare_write() since that will clear the buffer ...
    a.value_estate.reset();
    a.checker->restart (a.tc());
    prepare_read ();
    return a.copy_any (*this);
}

CORBA::Boolean
CORBA::Any::demarshal (TypeCode_ptr t, DataDecoder &d)
{
    reset ();
    set_type (t);
    Any a (t, d.clone (d.buffer(), FALSE,
		       d.converter(), FALSE,
		       d.valuestate(), FALSE),
	   new MICO::CDREncoder);

    // cannot use a.pepare_read() ...
    a.value_dstate.reset();
    a.checker->restart (a.tc());
    prepare_write ();
    return copy_any (a);
}

CORBA::Boolean
CORBA::Any::from_static_any (const StaticAny &_sa, TypeCode_ptr t)
{
    StaticAny &sa = (StaticAny &)_sa;

    if (checker->completed()) {
	value_estate.reset();
	ec->buffer()->reset();
	tc_if_changed (CORBA::TypeCode::_duplicate (t));
    } else if (!checker->basic (t)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    sa.marshal (*ec);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::from_static_any (const StaticAny &_sa)
{
    StaticAny &sa = (StaticAny &)_sa;

    CORBA::TypeCode_ptr t = sa.typecode();
    if (CORBA::is_nil (t))
	t = tc();

    if (checker->completed()) {
	value_estate.reset();
	ec->buffer()->reset();
	tc_if_changed (CORBA::TypeCode::_duplicate (t));
    } else if (!checker->basic (t)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    sa.marshal (*ec);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::to_static_any (StaticAny &sa, CORBA::TypeCode_ptr t) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->basic (t) || !sa.demarshal (*me.dc)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::to_static_any (StaticAny &sa) const
{
    CORBA::TypeCode_ptr t = sa.typecode();
    if (CORBA::is_nil (t))
	t = tc();
    return to_static_any (sa, t);
}

CORBA::Boolean
CORBA::Any::to_static_any (StaticTypeInfo *ti, CORBA::TypeCode_ptr t,
			   void *&value) const
{
    Any &me = (Any &)*this;

    if (!me.extracted_value
	|| !me.checker->completed()
	|| (me.extracted_value->type() != ti
	    && t->kind() == CORBA::tk_objref)) {
	// kcg: FP has added `|| me.extracted_value->type() != ti' and
	// I've added `&& t->kind() == CORBA::tk_objref' because
	// tk_objref seems the be the only case when we really need
	// it, i.e. any holds objref, at the first we will extract
	// CORBA::Object from it and then concrete object from it. For
	// the second extraction we need to execute this code
	// otherwise it would fail because of the code:
	//  	if (me.extracted_value->type() != ti) {
	//  	    return FALSE;
	//  	}
	// below
	me.reset_extracted_value();
	me.extracted_value = new StaticAny (ti);
	if (!to_static_any (*me.extracted_value, t)) {
	    me.reset_extracted_value();
	    return FALSE;
	}
	value = me.extracted_value->value();
	return TRUE;
    }
    me.prepare_read ();
    if (!me.checker->basic (t)) {
	me.rewind ();
	return FALSE;
    }
    if (me.extracted_value->type() != ti) {
	return FALSE;
    }
    value = me.extracted_value->value();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::to_static_any (StaticTypeInfo *ti, void *&value) const
{
    CORBA::TypeCode_ptr t = ti->typecode();
    if (CORBA::is_nil (t))
	t = tc();
    return to_static_any (ti, t, value);
}

CORBA::Boolean
CORBA::Any::insert (Short s)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_short);
    } else if (!checker->basic (_tc_short)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_short (s);
    return TRUE;
}

void
CORBA::Any::operator<<= (Short s)
{
    insert (s);
}

CORBA::Boolean
CORBA::Any::insert (Long l)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_long);
    } else if (!checker->basic (_tc_long)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_long (l);
    return TRUE;
}

void
CORBA::Any::operator<<= (Long l)
{
    insert (l);
}

CORBA::Boolean
CORBA::Any::insert (LongLong l)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_longlong);
    } else if (!checker->basic (_tc_longlong)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_longlong (l);
    return TRUE;
}

void
CORBA::Any::operator<<= (LongLong l)
{
    insert (l);
}

CORBA::Boolean
CORBA::Any::insert (UShort us)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_ushort);
    } else if (!checker->basic (_tc_ushort)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_ushort (us);
    return TRUE;
}

void
CORBA::Any::operator<<= (UShort us)
{
    insert (us);
}

CORBA::Boolean
CORBA::Any::insert (ULong ul)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_ulong);
    } else if (!checker->basic (_tc_ulong)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_ulong (ul);
    return TRUE;
}

void
CORBA::Any::operator<<= (ULong ul)
{
    insert (ul);
}

CORBA::Boolean
CORBA::Any::insert (ULongLong ul)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_ulonglong);
    } else if (!checker->basic (_tc_ulonglong)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_ulonglong (ul);
    return TRUE;
}

void
CORBA::Any::operator<<= (ULongLong ul)
{
    insert (ul);
}

CORBA::Boolean
CORBA::Any::insert (Float f)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_float);
    } else if (!checker->basic (_tc_float)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_float (f);
    return TRUE;
}

void
CORBA::Any::operator<<= (Float f)
{
    insert (f);
}

CORBA::Boolean
CORBA::Any::insert (Double d)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_double);
    } else if (!checker->basic (_tc_double)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_double (d);
    return TRUE;
}

void
CORBA::Any::operator<<= (Double d)
{
    insert (d);
}

CORBA::Boolean
CORBA::Any::insert (LongDouble d)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_longdouble);
    } else if (!checker->basic (_tc_longdouble)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_longdouble (d);
    return TRUE;
}

void
CORBA::Any::operator<<= (LongDouble d)
{
    insert (d);
}

CORBA::Boolean
CORBA::Any::insert (const Any &a)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_any);
    } else if (!checker->basic (_tc_any)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_any (a);
    return TRUE;
}

void
CORBA::Any::operator<<= (const Any &a)
{
    insert (a);
}

CORBA::Boolean
CORBA::Any::insert (Any *a)
{
    CORBA::Boolean _ret = insert (*a);
    delete a;
    return _ret;
}

void
CORBA::Any::operator<<= (Any *a)
{
    insert (a);
}

CORBA::Boolean
CORBA::Any::insert (const Exception &e)
{
    e._encode_any (*this);
    return TRUE;
}

void
CORBA::Any::operator<<= (const Exception &e)
{
    insert (e);
}

CORBA::Boolean
CORBA::Any::insert (Exception *e)
{
    CORBA::Boolean _ret = insert (*e);
    delete e;
    return _ret;
}

void
CORBA::Any::operator<<= (Exception *e)
{
    insert (e);
}

CORBA::Boolean
CORBA::Any::insert (const char *s)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_string);
    } else if (!checker->basic (_tc_string)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_string (s ? s : "");
    return TRUE;
}

void
CORBA::Any::operator<<= (const char *s)
{
    insert (s);
}

CORBA::Boolean
CORBA::Any::insert (const wchar_t *s)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_wstring);
    } else if (!checker->basic (_tc_wstring)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_wstring (s ? s : L"");
    return TRUE;
}

void
CORBA::Any::operator<<= (const wchar_t *s)
{
    insert (s);
}

CORBA::Boolean
CORBA::Any::insert (from_boolean b)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_boolean);
    } else if (!checker->basic (_tc_boolean)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_boolean (b.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_boolean b)
{
    insert (b);
}

CORBA::Boolean
CORBA::Any::insert (from_octet o)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_octet);
    } else if (!checker->basic (_tc_octet)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_octet (o.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_octet o)
{
    insert (o);
}

CORBA::Boolean
CORBA::Any::insert (from_char c)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_char);
    } else if (!checker->basic (_tc_char)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_char (c.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_char c)
{
    insert (c);
}

CORBA::Boolean
CORBA::Any::insert (from_wchar c)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_wchar);
    } else if (!checker->basic (_tc_wchar)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_wchar (c.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_wchar c)
{
    insert (c);
}

CORBA::Boolean
CORBA::Any::insert (from_string s)
{
    if (s.bound > 0 && s.val && strlen (s.val) > s.bound)
        return FALSE;
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (TypeCode::create_string_tc (s.bound));
    } else {
	TypeCode_var tmp = TypeCode::create_string_tc (s.bound);
        if (!checker->basic (tmp)) {
            reset ();
            return FALSE;
        }
    }
    reset_extracted_value();
    ec->put_string (s.val ? s.val : "");
    if (s.nocopy)
	CORBA::string_free (s.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_string s)
{
    insert (s);
}

CORBA::Boolean
CORBA::Any::insert (from_wstring s)
{
    if (s.bound > 0 && s.val && xwcslen (s.val) > s.bound)
        return FALSE;
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (TypeCode::create_wstring_tc (s.bound));
    } else {
	TypeCode_var tmp = TypeCode::create_wstring_tc (s.bound);
        if (!checker->basic (tmp)) {
            reset ();
            return FALSE;
        }
    }
    reset_extracted_value();
    ec->put_wstring (s.val ? s.val : L"");
    if (s.nocopy)
	CORBA::wstring_free (s.val);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_wstring s)
{
    insert (s);
}

CORBA::Boolean
CORBA::Any::insert (from_fixed f)
{
    Fixed::FixedValue_var value = f.fixed.to_digits();

    if ((CORBA::ULong)f.digits+1 != value->length())
        return FALSE;
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (TypeCode::create_fixed_tc (f.digits, f.scale));
    } else {
	TypeCode_var tmp = TypeCode::create_fixed_tc (f.digits, f.scale);
        if (!checker->basic (tmp)) {
            reset ();
            return FALSE;
        }
    }
    reset_extracted_value();
    ec->put_fixed (value.in(), f.digits, f.scale);
    return TRUE;
}

void
CORBA::Any::operator<<= (from_fixed f)
{
    insert (f);
}

CORBA::Boolean
CORBA::Any::insert (from_object o)
{
    IOR *ior;
    const char *repoid;
    if (CORBA::is_nil (o.val)) {
        ior = new IOR;
	repoid = "";
    } else {
        ior = o.val->_ior();
	if (!ior) {
	    // locality contrained object
	    reset ();
	    return FALSE;
	}
	repoid = ior->objid();
    }
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (TypeCode::create_interface_tc (repoid, o.name));
    } else if (checker->tc()->kind() != tk_objref ||
	       !checker->basic (checker->tc())) {
	// XXX should compare checker->tc()->id() and repoid somehow ...
	if (CORBA::is_nil (o.val))
	    delete ior;
	reset ();
	return FALSE;
    }

    reset_extracted_value();
    ec->put_ior (*ior);
    if (CORBA::is_nil (o.val))
        delete ior;
    return TRUE;
}

void
CORBA::Any::operator<<= (from_object o)
{
    insert (o);
}

CORBA::Boolean
CORBA::Any::insert (Object_ptr t)
{
    return insert (from_object (t, ""));
}

void
CORBA::Any::operator<<= (Object_ptr t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (Object_ptr *t)
{
    CORBA::Boolean _ret = insert (*t);
    CORBA::release (*t);
    return _ret;
}

void
CORBA::Any::operator<<= (Object_ptr *t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (TypeCode_ptr t)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_TypeCode);
    } else if (!checker->basic (_tc_TypeCode)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_typecode (*t);
    return TRUE;
}

void
CORBA::Any::operator<<= (TypeCode_ptr t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (TypeCode_ptr *t)
{
    CORBA::Boolean _ret = insert (*t);
    CORBA::release (*t);
    return _ret;
}

void
CORBA::Any::operator<<= (TypeCode_ptr *t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (Context_ptr ctx)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_Context);
    } else if (!checker->basic (_tc_Context)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_context (*ctx);
    return TRUE;
}

void
CORBA::Any::operator<<= (Context_ptr t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (Context_ptr *t)
{
    CORBA::Boolean _ret = insert (*t);
    CORBA::release (*t);
    return _ret;
}

void
CORBA::Any::operator<<= (Context_ptr *t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (Principal_ptr pr)
{
    if (checker->completed()) {
	ec->buffer()->reset();
	tc_if_changed (_tc_Principal);
    } else if (!checker->basic (_tc_Principal)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->put_principal (*pr);
    return TRUE;
}

void
CORBA::Any::operator<<= (Principal_ptr t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (Principal_ptr *t)
{
    CORBA::Boolean _ret = insert (*t);
    CORBA::release (*t);
    return _ret;
}

void
CORBA::Any::operator<<= (Principal_ptr *t)
{
    insert (t);
}

CORBA::Boolean
CORBA::Any::insert (AbstractBase_ptr t)
{
  CORBA::StaticAny _sa (CORBA::_stc_AbstractBase, &t);
  return from_static_any (_sa);
}

void
CORBA::Any::operator<<= (AbstractBase_ptr t)
{
  insert (t);
}

CORBA::Boolean
CORBA::Any::insert (AbstractBase_ptr *t)
{
  CORBA::Boolean _ret = insert (*t);
  CORBA::release (*t);
  return _ret;
}

void
CORBA::Any::operator<<= (AbstractBase_ptr *t)
{
  insert (t);
}

CORBA::Boolean
CORBA::Any::get_short (Short &s)
{
    return (checker->basic (_tc_short) && dc->get_short (s));
}

CORBA::Boolean
CORBA::Any::operator>>= (Short &s) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_short (s)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_long (Long &l)
{
    if (checker->basic (_tc_long)) {
	return dc->get_long (l);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Long &l) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_long (l)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_longlong (LongLong &l)
{
    if (checker->basic (_tc_longlong)) {
	return dc->get_longlong (l);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (LongLong &l) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_longlong (l)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_ushort (UShort &us)
{
    return (checker->basic (_tc_ushort) && dc->get_ushort (us));
}

CORBA::Boolean
CORBA::Any::operator>>= (UShort &us) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_ushort (us)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_ulong (ULong &ul)
{
    if (checker->basic (_tc_ulong)) {
	return dc->get_ulong (ul);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (ULong &ul) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_ulong (ul)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_ulonglong (ULongLong &ul)
{
    if (checker->basic (_tc_ulonglong)) {
	return dc->get_ulonglong (ul);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (ULongLong &ul) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_ulonglong (ul)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_float (Float &f)
{
    if (checker->basic (_tc_float)) {
	return dc->get_float (f);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Float &f) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_float (f)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_double (Double &d)
{
    if (checker->basic (_tc_double)) {
	return dc->get_double (d);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Double &d) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_double (d)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::get_longdouble (LongDouble &d)
{
    if (checker->basic (_tc_longdouble)) {
	return dc->get_longdouble (d);
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (LongDouble &d) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.get_longdouble (d)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (const Any *&a) const
{
    return to_static_any (_stc_any, (void *&)a);
}

CORBA::Boolean
CORBA::Any::operator>>= (const char * &s) const
{
    char **p;
    if (to_static_any (_stc_string, (void *&)p)) {
	s = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (const wchar_t * &s) const
{
    wchar_t **p;
    if (to_static_any (_stc_wstring, (void *&)p)) {
	s = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_boolean b) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->basic (_tc_boolean) || !me.dc->get_boolean (b.ref)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_octet o) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->basic (_tc_octet) || !me.dc->get_octet (o.ref)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_char c) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->basic (_tc_char) || !me.dc->get_char (c.ref)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_wchar c) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->basic (_tc_wchar) || !me.dc->get_wchar (c.ref)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_string s) const
{
    char **p;
    TypeCode_var tmp = TypeCode::create_string_tc (s.bound);
    if (to_static_any (_stc_string, tmp, (void *&)p)) {
	s.ref = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_wstring s) const
{
    wchar_t **p;
    TypeCode_var tmp = TypeCode::create_wstring_tc (s.bound);
    if (to_static_any (_stc_wstring, tmp, (void *&)p)) {
	s.ref = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_fixed f) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    TypeCode_var tmp = TypeCode::create_fixed_tc (f.digits, f.scale);
    FixedBase::FixedValue val;
    if (!me.checker->basic (tmp) ||
	!me.dc->get_fixed (val, f.digits, f.scale)) {
	me.rewind ();
	return FALSE;
    }
    f.fixed.from_digits (val);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_object o) const
{
    Any &me = (Any &)*this;

    me.prepare_read();
    if (me.checker->tc()->kind () != tk_objref) {
	me.rewind ();
	return FALSE;
    }

    Object_ptr *p;
    if (to_static_any (_stc_Object, checker->tc(), (void *&)p)) {
	o.ref = CORBA::Object::_duplicate (*p);
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (to_abstract_base o) const
{
    Any &me = (Any &)*this;

    me.prepare_read();
    if (me.checker->tc()->kind () != tk_abstract_interface) {
        me.rewind ();
        return FALSE;
    }

    AbstractBase_ptr *p;
    if (!to_static_any (_stc_AbstractBase, checker->tc(), (void *&)p)) {
	o.ref = CORBA::AbstractBase::_duplicate (*p);
        return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Object_ptr &t) const
{
    CORBA::Object_var tv;
    if (!(*this >>= to_object (tv))) {
        return FALSE;
    }
    // to_object extraction returns a duplicate ...
    t = tv.in();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::operator>>= (TypeCode_ptr &t) const
{
    TypeCode_ptr *p;
    if (to_static_any (_stc_TypeCode, (void *&)p)) {
	t = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Context_ptr &ctx) const
{
    Context_ptr *p;
    if (to_static_any (_stc_Context, (void *&)p)) {
	ctx = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::operator>>= (Principal_ptr &pr) const
{
    Principal_ptr *p;
    if (to_static_any (_stc_Principal, (void *&)p)) {
	pr = *p;
	return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Any::struct_put_begin ()
{
    prepare_write ();
    if (!checker->struct_begin()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->struct_begin ();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::struct_put_end ()
{
    if (!checker->struct_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->struct_end ();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_put_begin ()
{
    prepare_write ();
    if (!checker->union_begin()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->union_begin ();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_put_selection (Long idx)
{
    if (!checker->union_selection (idx)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_put_end ()
{
    if (!checker->union_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->union_end();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::enum_put (ULong val)
{
    prepare_write ();
    if (!checker->enumeration (val)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->enumeration (val);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::any_put (Any &a, Boolean recurse)
{
    prepare_write ();
    a.prepare_read ();
    reset_extracted_value();
    if (!copy_any (a, recurse)) {
	reset ();
	a.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::seq_put_begin (ULong len)
{
    prepare_write ();
    if (!checker->seq_begin (len)) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->seq_begin (len);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::seq_put_end ()
{
    if (!checker->seq_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->seq_end();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::array_put_begin ()
{
    prepare_write ();
    if (!checker->arr_begin()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->arr_begin ();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::array_put_end ()
{
    if (!checker->arr_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->arr_end();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::except_put_begin (const char *repoid)
{
    prepare_write ();
    if (!checker->except_begin()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->except_begin (repoid);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::except_put_end ()
{
    if (!checker->except_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->except_end();
    return TRUE;
}

CORBA::Boolean
CORBA::Any::value_put_begin (Long &value_id)
{
    prepare_write ();

    CORBA::TypeCode_var vtc = CORBA::TypeCode::_duplicate (checker->tc());

    if (!checker->value_begin()) {
	reset ();
	return FALSE;
    }

    // use chunked encoding if value is truncatable
    Boolean chunked = !!(vtc->type_modifier() & VM_TRUNCATABLE);

    // collect repoids of base types we can truncate to
    vector<string> repoids;
    repoids.push_back (vtc->id());
    while (vtc->type_modifier() & VM_TRUNCATABLE) {
	vtc = vtc->concrete_base_type();
	assert (!CORBA::is_nil (vtc));
	repoids.push_back (vtc->id());
    }

    reset_extracted_value();
    ec->value_begin ("", repoids, chunked, value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::value_put_end (Long value_id)
{
    if (!checker->value_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->value_end (value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::value_put_ref (Long value_id)
{
    checker->basic (checker->tc());
    reset_extracted_value();
    ec->value_ref (value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::valuebox_put_begin (Long &value_id)
{
    prepare_write ();

    CORBA::TypeCode_var vtc = CORBA::TypeCode::_duplicate (checker->tc());

    if (!checker->valuebox_begin()) {
	reset ();
	return FALSE;
    }

    vector<string> repoids;
    repoids.push_back (vtc->id());

    reset_extracted_value();
    ec->value_begin ("", repoids, FALSE, value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::valuebox_put_end (Long value_id)
{
    if (!checker->valuebox_end()) {
	reset ();
	return FALSE;
    }
    reset_extracted_value();
    ec->value_end (value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::valuebox_put_ref (Long value_id)
{
    checker->basic (checker->tc());
    reset_extracted_value();
    ec->value_ref (value_id);
    return TRUE;
}

CORBA::Boolean
CORBA::Any::struct_get_begin () const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->struct_begin() || !me.dc->struct_begin ()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::struct_get_end () const
{
    Any &me = (Any &)*this;

    if (!me.checker->struct_end() || !me.dc->struct_end()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_get_begin () const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->union_begin() || !me.dc->union_begin()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_get_selection (Long idx) const
{
    Any &me = (Any &)*this;

    if (!me.checker->union_selection (idx)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::union_get_end () const
{
    Any &me = (Any &)*this;

    if (!me.checker->union_end() || !me.dc->union_end()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::enum_get (ULong &val) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.dc->enumeration (val) || !me.checker->enumeration (val)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::any_get (Any &a, Boolean recurse) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    a.set_type (checker->tc());
    a.prepare_write ();
    if (!a.copy_any (me, recurse)) {
	me.rewind ();
	a.reset ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::seq_get_begin (ULong &len) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.dc->seq_begin (len) || !me.checker->seq_begin (len)) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::seq_get_end () const
{
    Any &me = (Any &)*this;

    if (!me.checker->seq_end() || !me.dc->seq_end()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::array_get_begin () const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    if (!me.checker->arr_begin() || !me.dc->arr_begin()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::array_get_end () const
{
    Any &me = (Any &)*this;

    if (!me.checker->arr_end() || !me.dc->arr_end()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::except_get_begin (String_out repoid) const
{
    Any &me = (Any &)*this;

    me.prepare_read ();
    string s;
    if (!me.checker->except_begin() || !me.dc->except_begin (s)) {
	me.rewind ();
	return FALSE;
    }
    repoid = CORBA::string_dup (s.c_str());
    return TRUE;
}

CORBA::Boolean
CORBA::Any::except_get_end () const
{
    Any &me = (Any &)*this;

    if (!me.checker->except_end() || !me.dc->except_end()) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::value_get_begin (Long &value_id, Boolean &is_ref) const
{
    Any &me = (Any &)*this;
    me.prepare_read ();

    CORBA::TypeCode_var vtc = CORBA::TypeCode::_duplicate (checker->tc());

    string url;
    vector<string> repoids;

    if (!me.checker->value_begin() ||
	!me.dc->value_begin (url, repoids, value_id, is_ref)) {
	me.rewind ();
	return FALSE;
    }
    if (is_ref) {
	me.checker->basic (me.checker->tc());
	return TRUE;
    }

    // check if we can truncate to the requested type
    string myrepoid = vtc->id();
    for (mico_vec_size_type i = 0; i < repoids.size(); ++i) {
	if (repoids[i] == myrepoid) {
	    return TRUE;
	}
    }
    me.rewind ();
    return FALSE;
}

CORBA::Boolean
CORBA::Any::value_get_end (Long value_id, Boolean is_ref) const
{
    Any &me = (Any &)*this;

    if (!me.checker->value_end() ||
	(!is_ref && !me.dc->value_end (value_id))) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Any::valuebox_get_begin (Long &value_id, Boolean &is_ref) const
{
    Any &me = (Any &)*this;
    me.prepare_read ();

    CORBA::TypeCode_var vtc = CORBA::TypeCode::_duplicate (checker->tc());

    string url;
    vector<string> repoids;

    if (!me.checker->valuebox_begin() ||
	!me.dc->value_begin (url, repoids, value_id, is_ref)) {
	me.rewind ();
	return FALSE;
    }
    if (is_ref) {
	me.checker->basic (me.checker->tc());
	return TRUE;
    }

    // check if types are equal (repoids should only contain one entry) ...
    string myrepoid = vtc->id();
    for (mico_vec_size_type i = 0; i < repoids.size(); ++i) {
	if (repoids[i] == myrepoid) {
	    return TRUE;
	}
    }
    me.rewind ();
    return FALSE;
}

CORBA::Boolean
CORBA::Any::valuebox_get_end (Long value_id, Boolean is_ref) const
{
    Any &me = (Any &)*this;

    if (!me.checker->valuebox_end() ||
	(!is_ref && !me.dc->value_end (value_id))) {
	me.rewind ();
	return FALSE;
    }
    return TRUE;
}


class ConstValue {
    CORBA::ULongLong _uv;
    CORBA::LongLong _sv;
    CORBA::LongDouble _fv;
    enum Kind { SignedValue, UnsignedValue, FloatValue } _kind;
public:
    ConstValue ()
    {
	_uv = 0;
	_sv = 0;
	_fv = 0;
	_kind = SignedValue;
    }
    void from_signed (CORBA::LongLong v)
    {
	_sv = v;
	_kind = SignedValue;
    }
    void from_unsigned (CORBA::ULongLong v)
    {
	_uv = v;
	_kind = UnsignedValue;
    }
    void from_float (CORBA::LongDouble v)
    {
	_fv = v;
	_kind = FloatValue;
    }
    CORBA::LongLong to_signed () const
    {
	switch (_kind) {
	case SignedValue:
	    return _sv;
	case UnsignedValue:
	    return _uv;
	case FloatValue:
	    return (CORBA::LongLong)_fv;
	}
	return 0;
    }
    CORBA::ULongLong to_unsigned () const
    {
	switch (_kind) {
	case SignedValue:
	    return _sv;
	case UnsignedValue:
	    return _uv;
	case FloatValue:
	    return (CORBA::ULongLong)_fv;
	}
	return 0;
    }
    CORBA::LongDouble to_float () const
    {
	switch (_kind) {
	case SignedValue:
	    return _sv;
	case UnsignedValue:
#ifdef _WIN32
	    return (CORBA::LongLong)_uv;
#else
	    return _uv;
#endif
	case FloatValue:
	    return _fv;
	}
	return 0;
    }
    CORBA::Boolean signed_range (CORBA::LongLong lower,
				 CORBA::LongLong upper) const
    {
	assert(upper >= 0);
	switch (_kind) {
	case SignedValue:
	    return lower <= _sv && _sv <= upper;
	case UnsignedValue:
	    return _uv <= (CORBA::ULongLong)upper;
	case FloatValue:
	    return lower <= _fv && _fv <= upper;
	}
	return 0;
    }
    CORBA::Boolean unsigned_range (CORBA::ULongLong upper) const
    {
	switch (_kind) {
	case SignedValue:
	    return 0 <= _sv && (CORBA::ULongLong)_sv <= upper;
	case UnsignedValue:
	    return _uv <= upper;
	case FloatValue:
#ifdef _WIN32
	    return 0 <= _fv && _fv <= (CORBA::LongLong)upper;
#else
	    return 0 <= _fv && _fv <= upper;
#endif
	}
        return 0;
    }
    CORBA::Boolean float_range (CORBA::LongDouble lower,
				CORBA::LongDouble upper) const
    {
	switch (_kind) {
	case SignedValue:
	    return lower <= _sv && _sv <= upper;
	case UnsignedValue:
#ifdef _WIN32
	    return lower <= (CORBA::LongLong)_uv && (CORBA::LongLong)_uv <= upper;
#else
	    return lower <= _uv && _uv <= upper;
#endif
	case FloatValue:
	    return lower <= _fv && _fv <= upper;
	}
	return 0;
    }
};

CORBA::Boolean
CORBA::Any::coerce (CORBA::Any &dst) const
{
    const CORBA::Any &src = *this;
    CORBA::TypeCode_var stype = src.type();
    stype = CORBA::TypeCode::_duplicate (stype->unalias());
    CORBA::TypeCode_var dtype = dst.type();
    dtype = CORBA::TypeCode::_duplicate (dtype->unalias());

    if (stype->equal (dtype)) {
	dst = *this;
	return TRUE;
    }

    ConstValue cv;

    switch (stype->kind()) {
    case CORBA::tk_char: {
	if (dtype->kind() != CORBA::tk_wchar)
	    return FALSE;
	CORBA::Char c;
	src >>= CORBA::Any::to_char (c);
	dst <<= CORBA::Any::from_wchar ((CORBA::WChar)(c & 0xff));
	return TRUE;
    }
    case CORBA::tk_wchar: {
	if (dtype->kind() != CORBA::tk_char)
	    return FALSE;
	CORBA::WChar c;
	src >>= CORBA::Any::to_wchar (c);
	if (c & ~0xff)
	    return FALSE;
	dst <<= CORBA::Any::from_char ((CORBA::Char)c);
	return TRUE;
    }
    case CORBA::tk_string: {
        if (dtype->kind() == CORBA::tk_string) {
	  // either stype or dtype must be a bounded string
	  const char *s;
	  src >>= CORBA::Any::to_string (s, stype->length());
	  if (dtype->length() != 0 && dtype->length() < strlen (s))
	    return FALSE;
	  dst <<= CORBA::Any::from_string (s, dtype->length());
	  return TRUE;
        }
	if (dtype->kind() != CORBA::tk_wstring)
	    return FALSE;
	const char *s;
	src >>= CORBA::Any::to_string (s, 0);
	CORBA::WString_var w = CORBA::wstring_alloc (strlen (s));
	for (size_t i = 0; i < strlen (s); i++)
	    w[(CORBA::ULong)i] = s[i] & 0xff;
	w[(CORBA::ULong)strlen(s)] = 0;
	//dst <<= CORBA::Any::from_wstring (w, 0);
	dst <<= w.in();
	return TRUE;
    }
    case CORBA::tk_wstring: {
        if (dtype->kind() == CORBA::tk_wstring) {
	  // either stype or dtype must be a bounded wstring
	  const wchar_t *w;
	  src >>= CORBA::Any::to_wstring (w, stype->length());
	  if (dtype->length() != 0 && dtype->length() < xwcslen (w))
	    return FALSE;
	  dst <<= CORBA::Any::from_wstring (w, dtype->length());
	  return TRUE;
        }
	if (dtype->kind() != CORBA::tk_string)
	    return FALSE;
	const wchar_t *w;
	src >>= CORBA::Any::to_wstring (w, 0);
	CORBA::String_var s = CORBA::string_alloc (xwcslen (w));
	for (size_t i = 0; i < xwcslen (w); i++) {
	    if (w[i] & ~0xff)
		return FALSE;
	    s[(CORBA::ULong)i] = w[i] & 0xff;
	}
	s[(CORBA::ULong)xwcslen(w)] = 0;
	dst <<= CORBA::Any::from_string (s, 0);
	return TRUE;
    }
    case CORBA::tk_short: {
	CORBA::Short v;
	src >>= v;
	cv.from_signed (v);
	break;
    }
    case CORBA::tk_ushort: {
	CORBA::Short v;
	src >>= v;
	cv.from_unsigned (v);
	break;
    }
    case CORBA::tk_long: {
	CORBA::Long v;
	src >>= v;
	cv.from_signed (v);
	break;
    }
    case CORBA::tk_ulong: {
	CORBA::ULong v;
	src >>= v;
	cv.from_unsigned (v);
	break;
    }
    case CORBA::tk_longlong: {
	CORBA::LongLong v;
	src >>= v;
	cv.from_signed (v);
	break;
    }
    case CORBA::tk_ulonglong: {
	CORBA::ULongLong v;
	src >>= v;
	cv.from_unsigned (v);
	break;
    }
    case CORBA::tk_float: {
	CORBA::Float v;
	src >>= v;
	cv.from_float (v);
	break;
    }
    case CORBA::tk_double: {
	CORBA::Double v;
	src >>= v;
	cv.from_float (v);
	break;
    }
    case CORBA::tk_longdouble: {
	CORBA::LongDouble v;
	src >>= v;
	cv.from_float (v);
	break;
    }
    case CORBA::tk_fixed: {
	FixedBase f (stype->fixed_digits(), stype->fixed_scale());
	src >>= CORBA::Any::to_fixed (f, stype->fixed_digits(),
				      stype->fixed_scale());
	cv.from_float (f);
	break;
    }
    default:
	return FALSE;
    }

    switch (dtype->kind()) {
    case CORBA::tk_octet: {
	CORBA::ULongLong upper = 0xfful;
	if (!cv.unsigned_range (upper))
	    return FALSE;
	dst <<= CORBA::Any::from_octet ((CORBA::Octet)cv.to_unsigned());
	break;
    }
    case CORBA::tk_short: {
	CORBA::LongLong upper = 0x7fffl;
	CORBA::LongLong lower = - upper - 1;
	if (!cv.signed_range (lower, upper))
	    return FALSE;
	dst <<= (CORBA::Short)cv.to_signed();
	break;
    }
    case CORBA::tk_ushort: {
	CORBA::ULongLong upper = 0xfffful;
	if (!cv.unsigned_range (upper))
	    return FALSE;
	dst <<= (CORBA::UShort)cv.to_unsigned();
	break;
    }
    case CORBA::tk_long: {
	CORBA::LongLong upper = 0x7fffffffl;
	CORBA::LongLong lower = - upper - 1;
	if (!cv.signed_range (lower, upper))
	    return FALSE;
	dst <<= (CORBA::Long)cv.to_signed();
	break;
    }
    case CORBA::tk_ulong: {
	CORBA::ULongLong upper = 0xfffffffful;
	if (!cv.unsigned_range (upper))
	    return FALSE;
	dst <<= (CORBA::ULong)cv.to_unsigned();
	break;
    }
    case CORBA::tk_longlong: {
	CORBA::LongLong upper = ~(((CORBA::LongLong)1) << 63);
	CORBA::LongLong lower = - upper - 1;
	if (!cv.signed_range (lower, upper))
	    return FALSE;
	dst <<= (CORBA::LongLong)cv.to_signed();
	break;
    }
    case CORBA::tk_ulonglong: {
	CORBA::ULongLong upper = ~((CORBA::ULongLong)0);
	if (!cv.unsigned_range (upper))
	    return FALSE;
	dst <<= (CORBA::ULongLong)cv.to_unsigned();
	break;
    }
    case CORBA::tk_float: {
	CORBA::LongDouble bound = ldexpl (1.999, 128);
	if (!cv.float_range (-bound, bound))
	    return FALSE;
	dst <<= (CORBA::Float)cv.to_float();
	break;
    }
    case CORBA::tk_double: {
	CORBA::LongDouble bound = ldexpl (1.999, 1024);
	if (!cv.float_range (-bound, bound))
	    return FALSE;
	dst <<= (CORBA::Double)cv.to_float();
	break;
    }
    case CORBA::tk_longdouble: {
	// everything fits into LongDouble
	dst <<= (CORBA::LongDouble)cv.to_float();
	break;
    }
    case CORBA::tk_fixed: {
	// everything fits into Fixed
	FixedBase f (dtype->fixed_digits(), dtype->fixed_scale());
	f = FixedBase ((CORBA::LongDouble)cv.to_float());
	dst <<= CORBA::Any::from_fixed (f, dtype->fixed_digits(),
					dtype->fixed_scale());
	break;
    }
    default:
	return FALSE;
    }
    return TRUE;
}
