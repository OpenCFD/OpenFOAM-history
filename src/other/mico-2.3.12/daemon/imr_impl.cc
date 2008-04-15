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
#include "imr_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <string.h>
#include <ctype.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <mico/util.h>
#include <mico/template_impl.h>
#include "imr_impl.h"

#endif // FAST_PCH


using namespace std;

/************************ ImplementationDef *************************/


ImplementationDef_impl::ImplementationDef_impl (ActivationMode mode,
						const ObjectInfoList &objs,
						const char *name,
						const char *command)
{
    _mode = mode;
    _objs = objs;
    _name = name;
    _command = command;
}

ImplementationDef_impl::ImplementationDef_impl (const char *asstring)
{
    string s = asstring;

    _name = CORBA::string_dup (get_str(s).c_str());
    assert (strlen (_name) > 0);

    string mode = get_str(s);
    if (mode == "poa")
	_mode = CORBA::ImplementationDef::ActivatePOA;
    else
	assert (0);

    _command = CORBA::string_dup (get_str(s).c_str());
    assert (strlen (_command) > 0);

    for (string rid = get_str(s); rid.length() > 0; rid = get_str(s)) {
	CORBA::ULong len = _objs.length();
	_objs.length (len+1);

	int pos = rid.rfind ("#");
	if (pos >= 0) {
	    _objs[len].repoid = CORBA::string_dup (rid.substr(0,pos).c_str());

	    CORBA::ORB::ObjectTag_var tag =
		CORBA::ORB::string_to_tag (rid.substr(pos+1).c_str());
	    _objs[len].tag = *tag;
	} else {
	    _objs[len].repoid = CORBA::string_dup (rid.c_str());
	}
    }
    assert (_objs.length() > 0);
}

ImplementationDef_impl::~ImplementationDef_impl ()
{
}

void
ImplementationDef_impl::put_str (string &s, const string &el)
{
    bool needquotes = ((int)el.find_first_of (" \t") >= 0);

    if (s.length() > 0)
	s += " ";
    if (needquotes)
	s += "'";
    for (string::size_type i = 0; i < el.length(); ++i) {
	char c = el[i];
	switch (c) {
	case '\'':
	case '\\':
	    s += "\\";
	    break;
	}
	s += c;
    }
    if (needquotes)
	s += "'";
}

string
ImplementationDef_impl::get_str (string &s)
{
    string::size_type pos = s.find_first_not_of (" ");
    if ((int)pos < 0)
	return "";
    bool havequotes = (s[pos] == '\'');
    if (havequotes)
	++pos;

    string out;
    for (bool esc = false; pos < s.length(); ++pos) {
	char c = s[pos];
	if (esc) {
	    out += c;
	    esc = false;
	} else if (c == '\\') {
	    esc = true;
	} else if (havequotes && c == '\'') {
	    ++pos;
	    break;
	} else if (!havequotes && (c == ' ' || c == '\t')) {
	    ++pos;
	    break;
	} else {
	    out += c;
	}
    }
    s = s.substr (pos);

    return out;
}

void
ImplementationDef_impl::mode (ActivationMode mode)
{
    _mode = mode;
}

CORBA::ImplementationDef::ActivationMode
ImplementationDef_impl::mode ()
{
    return _mode;
}

void
ImplementationDef_impl::objs (const ObjectInfoList &objs)
{
    _objs = objs;
}

CORBA::ImplementationDef::ObjectInfoList *
ImplementationDef_impl::objs ()
{
    return new ObjectInfoList (_objs);
}

char *
ImplementationDef_impl::name ()
{
    return CORBA::string_dup (_name);
}

void
ImplementationDef_impl::command (const char *command)
{
    _command = command;
}

char *
ImplementationDef_impl::command ()
{
    return CORBA::string_dup (_command);
}

char *
ImplementationDef_impl::tostring ()
{
    string s;
    put_str (s, (const char *)_name);

    switch (_mode) {
    case CORBA::ImplementationDef::ActivatePOA:
	put_str (s, "poa");
	break;
    default:
	assert (0);
    }
    put_str (s, (const char *)_command);
    for (CORBA::ULong i = 0; i < _objs.length(); ++i) {
	if (_objs[i].tag.length() > 0) {
	    string oi = _objs[i].repoid.in();
	    oi += "#";
	    CORBA::String_var tag = CORBA::ORB::tag_to_string (_objs[i].tag);
	    oi += tag.in();
	    put_str (s, oi.c_str());
	} else {
	    put_str (s, _objs[i].repoid.in());
	}
    }

    return CORBA::string_dup (s.c_str());
}


/************************** ImplRepository ***************************/


ImplRepository_impl::ImplRepository_impl ()
{
}

ImplRepository_impl::~ImplRepository_impl ()
{
    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	CORBA::release (*i);
    }
}

CORBA::ImplementationDef_ptr
ImplRepository_impl::create (ActivationMode mode, const ObjectInfoList &objs,
			     const char *name, const char *command)
{
    ImplementationDef_impl* serv = new ImplementationDef_impl(mode, objs, name, command);
    CORBA::ImplementationDef_var def = serv->_this();
    defs.push_back (def);
    return CORBA::ImplementationDef::_duplicate (def);
}

CORBA::ImplementationDef_ptr
ImplRepository_impl::restore (const char *asstring)
{
    ImplementationDef_impl* serv = new ImplementationDef_impl(asstring);
    CORBA::ImplementationDef_var def = serv->_this();
    defs.push_back (def);
    return CORBA::ImplementationDef::_duplicate (def);
}

void
ImplRepository_impl::destroy (CORBA::ImplementationDef_ptr def)
{
    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	if ((*i)->_is_equivalent (def)) {
	    CORBA::release (*i);
	    defs.erase (i);
	    break;
	}
    }
}

CORBA::ImplRepository::ImplDefSeq *
ImplRepository_impl::find_by_name (const char *name)
{
    ImplDefSeq *res = new ImplDefSeq;

    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	CORBA::String_var defname = (*i)->name();
	if (!strcmp (defname, name)) {
	    res->length (res->length()+1);
	    (*res)[res->length()-1] =
		CORBA::ImplementationDef::_duplicate (*i);
	}
    }
    return res;
}

CORBA::ImplRepository::ImplDefSeq *
ImplRepository_impl::find_by_repoid (const char *repoid)
{
    ImplDefSeq *res = new ImplDefSeq;

    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	CORBA::ImplementationDef::ObjectInfoList_var objs
	    = (*i)->objs();
	for (CORBA::ULong j = 0; j < objs->length(); ++j) {
	    if (mico_fnmatch (repoid, objs[j].repoid)) {
		res->length (res->length()+1);
		(*res)[res->length()-1] =
		    CORBA::ImplementationDef::_duplicate (*i);
		break;
	    }
	}
    }
    return res;
}

CORBA::ImplRepository::ImplDefSeq *
ImplRepository_impl::find_by_repoid_tag (const char *repoid,
					 const ObjectTag &tag)
{
    ImplDefSeq *res = new ImplDefSeq;

    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	CORBA::ImplementationDef::ObjectInfoList_var objs
	    = (*i)->objs();
	for (CORBA::ULong j = 0; j < objs->length(); ++j) {
	    if (mico_fnmatch (repoid, objs[j].repoid) &&
		(tag.length() == 0 || tag == objs[j].tag)) {
		res->length (res->length()+1);
		(*res)[res->length()-1] =
		    CORBA::ImplementationDef::_duplicate (*i);
		break;
	    }
	}
    }
    return res;
}

CORBA::ImplRepository::ImplDefSeq *
ImplRepository_impl::find_all ()
{
    ImplDefSeq *res = new ImplDefSeq;
    res->length (defs.size());

    int idx = 0;
    for (ListImplDef::iterator i = defs.begin(); i != defs.end(); ++i) {
	(*res)[idx++] =
	    CORBA::ImplementationDef::_duplicate (*i);
    }
    return res;
}
