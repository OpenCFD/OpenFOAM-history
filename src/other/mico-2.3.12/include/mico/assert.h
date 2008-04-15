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

#ifndef __mico_assert_h__
#define __mico_assert_h__

#ifndef _POCKET_PC
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#endif

/*
 * this makes the data section of the generated .o files tiny compared
 * to the real assert (which prints out the failed expression, thus
 * generating a string in the data section for each assertion), because
 * asserts are used in the templates...
 */

void mico_assert (const char *, int);

#undef assert

#if defined (_POCKET_PC) && defined (DEBUG)
#define assert(exp) if (!(exp)) mico_assert (__FILE__, __LINE__)
#elif !defined (NDEBUG)
#define assert(exp) if (!(exp)) mico_assert (__FILE__, __LINE__)
#else
#define assert(exp)
#endif

#define MICO_NOT_IMPLEMENTED mico_assert (__FILE__, __LINE__)

#endif /* __mico_assert_h__ */
