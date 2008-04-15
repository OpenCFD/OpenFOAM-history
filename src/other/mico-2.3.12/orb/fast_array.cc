/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *
 *  simple and fast array object and template
 *  Copyright (C) 1999 Andreas Schultz                                 
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
 *
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
#include <mico/impl.h>

#endif // FAST_PCH


using namespace std;

// this is a dynamic array with O(1) insertion and O(1) deletion time
// resizing is done only on request
// dynamicily grows, but doesn't shrink
// iterating it no possible
// no error checking for the moment - that means you can overflow the array
const unsigned int MICO::__void_array::__EMPTY = UINT_MAX;

MICO::__void_array::__void_array( unsigned int max, unsigned int grow ) {
    _free = _max = max;
    _grow = grow;
    _head = __EMPTY;
    _first_free = _last = 0;
    objs = static_cast<struct item_s*>(malloc( sizeof(struct item_s) * _max ));
}
    
MICO::__void_array::~__void_array() {
    free( objs );
}

unsigned int MICO::__void_array::__fast_insert( void *new_obj ) {
    unsigned int ret;

    if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << "void_array::__fast_insert (" << new_obj << "): ";
    }
    ret = _first_free;

    if (!_free) {
	_max  += _grow;
	_free += _grow;
        objs = static_cast<struct item_s*>(realloc( objs, sizeof(struct item_s) * _max ));
	assert( objs );
    }
    _free--;
    if ( _first_free != _last ) {
	_first_free = objs[_first_free].next;
    } else {
	_first_free++;
	_last++;
    }

    // add it to the item ring
    objs[ret].obj = new_obj;
    if (_head == __EMPTY) {
	_head = ret;
	objs[ret].next = objs[ret].prev = ret;
    } else {
	objs[ret].next = _head;
	objs[ret].prev = objs[_head].prev;
	objs[objs[_head].prev].next = ret;
	objs[_head].prev = ret;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << "   return " << ret << endl;
    }
    return ret;
}


void MICO::__void_array::remove ( unsigned int n ) {
    
    if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << "void_array::remove (" << n << ")" << endl;
    }
    _free++;
    // remove it from the item ring
    if ( n == objs[n].prev ) {             // only one object left
	_head = __EMPTY;
    } else {
	objs[objs[n].prev].next = objs[n].next;
	objs[objs[n].next].prev = objs[n].prev;
    }
    if ( n == _head )
	_head = objs[n].next;
    if (n == _last) {
	if (_last == _first_free) _first_free--;
	_last--;
    } else {
	objs[n].next = _first_free;
	_first_free = n;
    }
#ifdef MTDEBUG
    objs[n].obj = NULL;
    objs[n].prev = 0;
#endif
}

#ifdef MTDEBUG
void MICO::__void_array::print_stats() {
    if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << "_max   = " << _max << endl
	    << "_first_free = " << _first_free << endl
	    << "_last  = " << _last << endl
	    << "_head  = " << _head << endl
	    << "_free  = " << _free << endl;
    }
}

void MICO::__void_array::print_data() {
    unsigned int n;
    
    for (n = 0; n < _last; n++) {
	if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << n << ": " << objs[n].obj
	    << ", next: " << objs[n].next 
	    << ", prev: " << objs[n].prev << endl;
	}
    }
}

void MICO::__void_array::print_holes() {
    unsigned int n;
    
    if (_first_free == _last) {
	if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Support)
	    << "no holes" << endl;
	}
    } else {
	for (n = _first_free;  n != _last; n = objs[n].next) {
	    if (MICO::Logger::IsLogged (MICO::Logger::Support)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::Support)
		    << n << ": " << objs[n].next << endl;
	    }
	}
    }
}

#endif // MTDEBUG
