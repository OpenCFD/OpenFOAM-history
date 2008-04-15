/* -*- c++ -*- */
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

#ifndef __mico_types_h__
#define __mico_types_h__

/* basic types */
#if SIZEOF_SHORT == 2
typedef short MICO_Short;
#else
#error "sizeof(short) != 2"
#endif

#if SIZEOF_LONG == 4
typedef long MICO_Long;
#elif SIZEOF_INT == 4
typedef int MICO_Long;
#else
#error "sizeof(long) != 4 and sizeof(int) != 4"
#endif

#if SIZEOF_LONG == 8
typedef long MICO_LongLong;
#elif SIZEOF_LONG_LONG == 8 || defined(__MINGW32__)
typedef long long MICO_LongLong;
#elif defined(_WIN32) || defined(_POCKET_PC)
typedef __int64 MICO_LongLong;
#else
#error "sizeof(long) != 8 and sizeof(long long) != 8"
#endif

#if SIZEOF_UNSIGNED_SHORT == 2
typedef unsigned short MICO_UShort;
#else
#error "sizeof(unsigned short) != 2"
#endif

#if SIZEOF_UNSIGNED_LONG == 4
typedef unsigned long MICO_ULong;
#elif SIZEOF_UNSIGNED_INT == 4
typedef unsigned int MICO_ULong;
#else
#error "sizeof(unsigned long) != 4 and sizeof(unsigned int) != 4"
#endif

#if SIZEOF_UNSIGNED_LONG == 8
typedef unsigned long MICO_ULongLong;
#elif SIZEOF_UNSIGNED_LONG_LONG == 8 || defined(__MINGW32__)
typedef unsigned long long MICO_ULongLong;
#elif defined(_WIN32) || defined(_POCKET_PC)
typedef unsigned __int64 MICO_ULongLong;
#else
#error "sizeof(unsigned long) != 8 and sizeof(unsigned long long) != 8"
#endif

#if SIZEOF_FLOAT == 4
typedef float MICO_Float;
#else
#error "sizeof(float) != 4"
#endif

#if SIZEOF_DOUBLE == 8
typedef double MICO_Double;
#else
#error "sizeof(double) != 8"
#endif

typedef long double MICO_LongDouble;

#if SIZEOF_UNSIGNED_CHAR == 1
// XXX changed to char by AP for testsuite
typedef char MICO_Char;
typedef unsigned char MICO_Boolean;
typedef unsigned char MICO_Octet;
#else
#error "sizeof(unsigned char) != 1"
#endif

typedef wchar_t MICO_WChar;

#define MICO_TID_DEF   0
#define MICO_TID_BOOL  1
#define MICO_TID_CHAR  2
#define MICO_TID_OCTET 3
#define MICO_TID_WCHAR 4

#ifdef __cplusplus


#define MICO_MAKE_WRAPPER(T,V)			\
class T {					\
    V rep;					\
public:						\
    T ()					\
    {						\
    }						\
    T (const T &t)				\
	: rep (t.rep)				\
    {						\
    }						\
    T (V t)					\
	: rep(t)				\
    {						\
    }						\
    T &operator= (const T &t)			\
    {						\
	rep = t.rep;				\
	return *this;				\
    }						\
    T &operator= (V t)				\
    {						\
	rep = t;				\
	return *this;				\
    }						\
    int operator== (const T &t) const		\
    {						\
	return rep == t.rep;			\
    }						\
    int operator!= (const T &t) const		\
    {						\
	return rep != t.rep;			\
    }						\
    operator V & () const			\
    {						\
	return (V &)rep;			\
    }						\
};

#endif /* __cplusplus */

#endif /* __mico_types_h__ */
