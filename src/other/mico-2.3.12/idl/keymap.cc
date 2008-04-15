//
//  MICO --- an Open Source CORBA implementation
//  Copyright (c) 1997-2003 by The Mico Team
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  For more information, visit the MICO Home Page at
//  http://www.mico.org/
//

#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <keymap.h>
#include <iostream>
#include <CORBA.h>

#endif // FAST_PCH


using namespace std;

KeywordMap::KeywordMap()
{
    // initialization
    counter_ = 0;
    // at the first CORBA 2.x keywords
    this->add_keyword("abstract");
    this->add_keyword("any");
    this->add_keyword("attribute");
    this->add_keyword("boolean");
    this->add_keyword("case");
    this->add_keyword("char");
    this->add_keyword("const");
    this->add_keyword("context");
    this->add_keyword("custom");
    this->add_keyword("default");
    this->add_keyword("double");
    this->add_keyword("exception");
    this->add_keyword("enum");
    this->add_keyword("factory");
    this->add_keyword("FALSE");
    this->add_keyword("fixed");
    this->add_keyword("float");
    this->add_keyword("in");
    this->add_keyword("inout");
    this->add_keyword("interface");
    this->add_keyword("local");
    this->add_keyword("long");
    this->add_keyword("module");
    this->add_keyword("native");
    this->add_keyword("Object");
    this->add_keyword("octet");
    this->add_keyword("oneway");
    this->add_keyword("out");
    this->add_keyword("private");
    this->add_keyword("public");
    this->add_keyword("raises");
    this->add_keyword("readonly");
    this->add_keyword("sequence");
    this->add_keyword("short");
    this->add_keyword("string");
    this->add_keyword("struct");
    this->add_keyword("supports");
    this->add_keyword("switch");
    this->add_keyword("TRUE");
    this->add_keyword("truncatable");
    this->add_keyword("typedef");
    this->add_keyword("unsigned");
    this->add_keyword("union");
    this->add_keyword("ValueBase");
    this->add_keyword("valuetype");
    this->add_keyword("void");
    this->add_keyword("wchar");
    this->add_keyword("wstring");
    // CORBA 3.x (CCM) keywords
    // kcg: we can #ifdef USE_CCM them, but I would
    // rather prefer to be more strict since MICO can
    // be used as an underlying ORB for other CCM
    // implementations too.
    this->add_keyword("component");
    this->add_keyword("consumes");
    this->add_keyword("emits");
    this->add_keyword("eventtype");
    this->add_keyword("finder");
    this->add_keyword("getraises");
    this->add_keyword("home");
    this->add_keyword("import");
    this->add_keyword("multiple");
    this->add_keyword("primaryKey");
    this->add_keyword("provides");
    this->add_keyword("publishes");
    this->add_keyword("setraises");
    this->add_keyword("typeid");
    this->add_keyword("typeprefix");
    this->add_keyword("uses");
}

void
KeywordMap::add_keyword(const char* key)
{
    // fillin both map_ and keywords_
    string t = key;
    string low = "";
    for (unsigned long i = 0; i < t.size(); i++) {
	low += ::tolower(t[i]);
    }
    map_[low] = counter_;
    counter_++;
    keywords_.push_back(t);    
}

char*
KeywordMap::keyword(const char* key)
{
    string t = key;
    string low = "";
    for (unsigned long i = 0; i < t.size(); i++) {
	low += ::tolower(t[i]);
    }
    if (map_.count(low)) {
	string retval = keywords_[map_[low]];
	return CORBA::string_dup(retval.c_str());
    }
    return CORBA::string_dup("");
}

