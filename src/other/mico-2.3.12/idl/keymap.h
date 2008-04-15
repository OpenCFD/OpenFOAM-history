// -*- c++ -*-
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

#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#if defined(_WIN32) && !defined(__MINGW32__)
#include <mico/config-win32.h>
#else
#include <mico/config.h>
#endif

#include <map>
#include <vector>
#include <string>

//
// KeywordMap
//
class KeywordMap
{
    std::map<std::string, unsigned long, std::less<std::string> > map_;
    std::vector<std::string> keywords_;
    unsigned long counter_;
public:
    KeywordMap();

    void
    add_keyword(const char* key);

    char*
    keyword(const char* key);
};

#endif // __KEYMAP_H__
