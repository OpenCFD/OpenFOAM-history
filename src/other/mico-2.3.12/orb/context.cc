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

#ifndef _WIN32
#include <string.h>
#endif
#include <CORBA-SMALL.h>
#include <mico/throw.h>
#include <mico/template_impl.h>

#endif // FAST_PCH

CORBA::Context::Context (const char *name, Context_ptr parent)
    : _myparent (parent), _properties (new NVList), _name (name)
{
}

CORBA::Context::Context (DataDecoder &dc)
{
    CORBA::Boolean r = decode (dc);
    assert (r);
}

CORBA::Context::~Context ()
{
}

CORBA::Boolean
CORBA::Context::match (const char *value, const char *pattern)
{
    ULong l = strlen (pattern);
    if (l > 0 && pattern[l-1] == '*')
	return !strncmp (value, pattern, l-1);

    return !strcmp (value, pattern);
}

void
CORBA::Context::encode (DataEncoder &ec, ContextList_ptr clist) const
{
    NVList_var out;
    if (!CORBA::is_nil (clist)) {
	((Context_ptr)this)->get_values ("", 0, clist, out);
    } else {
	((Context_ptr)this)->get_values ("", 0, "*", out);
    }
    
    ec.seq_begin (2*out->count());
    for (CORBA::ULong i = 0; i < out->count(); ++i) {
	NamedValue_ptr nv = out->item(i);
	ec.put_string (nv->name());
	nv->value()->marshal (ec);
    }
    ec.seq_end ();
}

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
CORBA::Context::decode (DataDecoder &dc)
{
    // delete existing properties, but not child contexts
    _properties = new NVList;

    ULong len;
    check (dc.seq_begin (len));
    check ((len & 1) == 0);
    {
	String_var s;
	Any a;
	for (CORBA::ULong i = 0; i < len; i += 2) {
	    check (dc.get_string (s.out()));
	    check (a.demarshal (_tc_string, dc));
	    set_one_value (s, a);
	}
    }
    check (dc.seq_end ());
    return TRUE;
}

#undef check

const char *
CORBA::Context::context_name () const
{
    return _name.c_str();
}

CORBA::Context_ptr
CORBA::Context::parent () const
{
    return _myparent;
}

void
CORBA::Context::create_child (const char *name, Context_out out)
{
    out = new Context (name, this);
    _childs.push_back (_duplicate (out));
}

void
CORBA::Context::set_one_value (const char *name, const Any &val)
{
    MICO_OBJ_CHECK (this);

    if (!name || !&val)
	mico_throw (CORBA::BAD_PARAM());

    TypeCode_var tc = ((Any &)val).type();
    if (tc->unalias()->kind() != tk_string)
	mico_throw (CORBA::BAD_PARAM());

    for (CORBA::ULong i = 0; i < _properties->count(); ++i) {
	NamedValue_ptr nv = _properties->item(i);
	if (!strcmp (nv->name(), name)) {
	    *nv->value() = val;
	    return;
	}
    }
    _properties->add_value (name, val, 0);
}

void
CORBA::Context::set_values (NVList_ptr nvlist)
{
    for (CORBA::ULong i = 0; i < nvlist->count(); ++i) {
	NamedValue_ptr nv = nvlist->item (i);
	set_one_value (nv->name(), *nv->value());
    }
}

void
CORBA::Context::delete_values (const char *pattern)
{
    if (!pattern || !strlen (pattern))
	mico_throw (CORBA::BAD_PARAM());

    CORBA::Boolean found = FALSE;
    for (CORBA::ULong i = 0; i < _properties->count(); ) {
	if (match (_properties->item(i)->name(), pattern)) {
	    found = TRUE;
	    _properties->remove(i);
	} else {
	    ++i;
	}
    }
    if (!found)
	mico_throw (CORBA::BAD_CONTEXT());
}

void
CORBA::Context::get_values (const char *scope,
			    Flags flags, const char *pattern,
			    NVList_out out)
{
    if (!pattern || !strlen (pattern))
	mico_throw (CORBA::BAD_PARAM());

    ContextList_var patterns = new ContextList;
    patterns->add (pattern);
    get_values (scope, flags, patterns, out);
}

void
CORBA::Context::get_values (const char *scope,
			    Flags flags,
			    ContextList_ptr patterns,
			    NVList_out out)
{
    MICO_OBJ_CHECK (this);

    if (scope && strlen (scope) && strcmp (scope, _name.c_str())) {
	if (CORBA::is_nil (_myparent))
	    mico_throw (CORBA::BAD_CONTEXT());
	_myparent->get_values (scope, flags, patterns, out);
	return;
    }
    if (!(flags & CTX_RESTRICT_SCOPE) && !CORBA::is_nil (_myparent)) {
	_myparent->get_values ("", flags, patterns, out);
    } else {
	out = new NVList;
    }
    for (CORBA::ULong i = 0; i < _properties->count(); ++i) {
	NamedValue_ptr nv = _properties->item(i);
	for (CORBA::ULong k = 0; k < patterns->count(); ++k) {
	    if (match (nv->name(), patterns->item(k))) {
		CORBA::ULong j;
		for (j = 0; j < out->count(); ++j) {
		    if (!strcmp (out->item(j)->name(), nv->name())) {
			*out->item(j)->value() = *nv->value();
			break;
		    }
		}
		if (j == out->count()) {
		    out->add_value (nv->name(), *nv->value(), nv->flags());
		}
	    }
	}
    }
}
