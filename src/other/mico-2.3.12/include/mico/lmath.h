// -*- c++ -*-
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

#ifndef __mico_lmath_h__
#define __mico_lmath_h__


// autoconf does not check for defines...
#if !defined(HAVE_INFNAN) && defined(infnan)
#define HAVE_INFNAN
#endif

#if !defined(HAVE_INFNANL) && defined(infnanl)
#define HAVE_INFNANL
#endif

#if !defined(HAVE_ISINF) && defined(isinf)
#define HAVE_ISINF
#endif

#if !defined(HAVE_ISINFL) && defined(isinfl)
#define HAVE_ISINFL
#endif

#if !defined(HAVE_ISNAN) && defined(isnan)
#define HAVE_ISNAN
#endif

#if !defined(HAVE_ISNANL) && defined(isnanl)
#define HAVE_ISNANL
#endif

#if !defined(HAVE_FLOORL) && defined(floorl)
#define HAVE_FLOORL
#endif

#if !defined(HAVE_CEILL) && defined(ceill)
#define HAVE_CEILL
#endif

#if !defined(HAVE_POWL) && defined(powl)
#define HAVE_POWL
#endif

#if !defined(HAVE_FMODL) && defined(fmodl)
#define HAVE_FMODL
#endif

#if SIZEOF_DOUBLE == SIZEOF_LONG_DOUBLE
#undef HAVE_INFNANL
#undef HAVE_ISNANL
#undef HAVE_ISINFL
#undef HAVE_ASINL
#undef HAVE_LDEXPL
#undef HAVE_FREXPL
#undef HAVE_FABSL
#undef HAVE_FLOORL
#undef HAVE_CEILL
#undef HAVE_POWL
#undef HAVE_FMODL
#endif

// provide the missing long double functions by using the double functions
#if !defined(HAVE_INFNANL) && defined(HAVE_INFNAN)
#define infnanl(x) infnan(x)
#define HAVE_INFNANL
#endif

#if !defined(HAVE_ISNANL) && defined(HAVE_ISNAN)
#define isnanl(x) isnan((double)(x))
#define HAVE_ISNANL
#endif

#if !defined(HAVE_ISINFL) && defined(HAVE_ISINF)
#define isinfl(x) isinf((double)(x))
#define HAVE_ISINFL
#endif

#ifndef HAVE_ASINL
#define asinl(x) asin((double)(x))
#define HAVE_ASINL
#endif

#ifndef HAVE_LDEXPL
#define ldexpl(x,y) ldexp((double)(x), (y))
#define HAVE_LDEXPL
#endif

#ifndef HAVE_FREXPL
#define frexpl(x,y) frexp((double)(x), (y))
#define HAVE_FREXPL
#endif

#ifndef HAVE_FABSL
#define fabsl(x) fabs((double)(x))
#define HAVE_FABSL
#endif

#ifndef HAVE_FLOORL
#define floorl(x) floor((double)(x))
#define HAVE_FLOORL
#endif

#ifndef HAVE_CEILL
#define ceill(x) ceil((double)(x))
#define HAVE_CEILL
#endif

#ifndef HAVE_POWL
#define powl(x,y) pow((double)(x), (double)(y))
#define HAVE_POWL
#endif

#ifndef HAVE_FMODL
#define fmodl(x,y) fmod((double)(x), (double)(y))
#define HAVE_FMODL
#endif

#endif // __mico_lmath_h__
