/* -*- mode: c++; c-basic-offset: 4; -*-
 *
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
 */


#ifndef __FAST_ARRAY_H__
#define __FAST_ARRAY_H__

namespace MICO {

// this is a dynamic array with O(1) insert and O(1) delete time
// resizing is done only on request
// iterating it no possible
// no error checking for the moment - that means you can overflow the array
class __void_array {
protected:
    static const unsigned int __EMPTY;

    struct item_s {
	void         *obj;
	unsigned int next;
	unsigned int prev;
    };

    struct item_s* objs;

    unsigned int _max;
    unsigned int _grow;
    unsigned int _first_free;
    unsigned int _last;
    unsigned int _head;
    unsigned int _free;
public:
    __void_array( unsigned int, unsigned int );
    ~__void_array();

    unsigned int __fast_insert( void *new_obj );
    unsigned int __front_insert( void *new_obj ) {
	_head =__fast_insert( new_obj );
	return _head;
    };

    unsigned int __back_insert( void *new_obj ) {
	return __fast_insert( new_obj );
    };


    void remove ( unsigned int n );

    unsigned int first() { return _head; };
    unsigned int last() { 
	return empty() ? objs[_head].prev : _head;
    };
    unsigned int count() { return _max - _free; };
    unsigned int empty() { return _head != __EMPTY; };

    unsigned int next( unsigned int n ) { return objs[n].next; };
    unsigned int prev( unsigned int n ) { return objs[n].prev; };

    int set_size( unsigned int max );

#ifdef MTDEBUG
    void print_stats();
    void print_data();
    void print_holes();
#endif // MTDEBUG
};

template <class T>
class FastArray: public __void_array {
public:
    FastArray( unsigned int max = 10, unsigned int grow = 10 ): __void_array( max, grow ) {};

    //unsigned int insert( T& new_obj ) {
    //	return __back_insert( &new_obj );
    //}
    unsigned int insert( T* new_obj ) {
	return __back_insert( new_obj );
    }
    //unsigned int front_insert( T& new_obj ) {
    //	return __front_insert( &new_obj );
    //}
    unsigned int front_insert( T* new_obj ) {
	return __front_insert( new_obj );
    }
    T *operator[](const unsigned int n) { 
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "T *operator[](" << n << "): returns " << objs[n].obj << std::endl;
	}
	return static_cast<T*>(objs[n].obj); 
    }
    const T *operator[](const unsigned int n) const {
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "const T *operator[](" << n << ") const: returns " << objs[n].obj
		<< std::endl;
	}
	return const_cast<T*>(objs[n].obj); 
    }
};

}
#endif // __FAST_ARRAY_H__
