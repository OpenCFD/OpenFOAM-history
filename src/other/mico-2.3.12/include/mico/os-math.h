// -*- c++ -*-
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

#ifndef __mico_os_math_h__
#define __mico_os_math_h__

#if defined(_WIN32) && !defined(__MINGW32__)

#include <math.h>
#include <limits>
#ifndef __BORLANDC__
#include <fpieee.h>
#endif

struct OSMath {
    static MICO_Double infinity (MICO_Boolean neg)
    {
	static MICO_Double minus_inf = 0, plus_inf = 0;
	if (plus_inf == 0) {
	    plus_inf = std::numeric_limits<double>::infinity();
	    minus_inf = -plus_inf;
	}
	return neg ? minus_inf : plus_inf;
    }

    static MICO_Double nan ()
    {
	static MICO_Double not_a_number = 0;
	if (not_a_number == 0) {
	    not_a_number = std::numeric_limits<double>::quiet_NaN();
	}
	return not_a_number;
    }

    static MICO_Boolean is_infinity (MICO_Double d)
    {
	if (!_finite (d) && !_isnan (d)) 
	    return TRUE;
	else
	    return FALSE;
    }

    static MICO_Boolean is_nan (MICO_Double d)
    {
	if (_isnan (d))
	    return TRUE;
	else 
	    return FALSE;
    }

    static MICO_LongDouble infinityl (MICO_Boolean neg)
    {
	static MICO_LongDouble minus_inf = 0, plus_inf = 0;
	if (plus_inf == 0) {
	    plus_inf = std::numeric_limits<long double>::infinity();
	    minus_inf = -plus_inf;
	}
	return neg ? minus_inf : plus_inf;
    }

    static MICO_LongDouble nanl ()
    {
	static MICO_LongDouble not_a_number = 0;
	if (not_a_number == 0) {
	    not_a_number = std::numeric_limits<long double>::quiet_NaN();
	}
	return not_a_number;
    }

    static MICO_Boolean is_infinityl (MICO_LongDouble d)
    {
	if (!_finite (d) && !_isnan (d)) 
	    return TRUE;
	else
	    return FALSE;
    }

    static MICO_Boolean is_nanl (MICO_LongDouble d)
    {
	if (_isnan (d))
	    return TRUE;
	else 
	    return FALSE;
    }
};

#elif defined(_WIN32) && defined(__MINGW32__)

/*
 * No long double math support in Mingw32
 */

#define frexpl frexp
#define ldexpl ldexp
#define fmodl fmod
#define ceill ceil
#define floorl floor

/*
 * Mingw32 does not seem to support all that infinity and nan stuff.
 * Is that a problem? Neither inf nor nan can be transported over the
 * wire anyway, can they? (GIOP does not mention them.)
 */

#include <math.h>
#include <float.h>

struct OSMath {
    static MICO_Double infinity (MICO_Boolean neg)
    {
	static MICO_Double minus_inf = 0, plus_inf = 0;
	if (plus_inf == 0) {
  	  plus_inf = DBL_MAX; // ???
	  minus_inf = -plus_inf;
	}
	return neg ? minus_inf : plus_inf;
    }

    static MICO_Double nan ()
    {
	static MICO_Double not_a_number = 0;
	if (not_a_number == 0) {
	  not_a_number = DBL_MAX; // ???
	}
	return not_a_number;
    }

    static MICO_Boolean is_infinity (MICO_Double d)
    {
	if (!::_finite (d) && !::_isnan (d)) 
	    return TRUE;
	else
	    return FALSE;
    }

    static MICO_Boolean is_nan (MICO_Double d)
    {
	if (::_isnan (d))
	    return TRUE;
	else 
	    return FALSE;
    }

    static MICO_LongDouble infinityl (MICO_Boolean neg)
    {
	static MICO_LongDouble minus_inf = 0, plus_inf = 0;
	if (plus_inf == 0) {
	  plus_inf = LDBL_MAX;
	  minus_inf = -plus_inf;
	}
	return neg ? minus_inf : plus_inf;
    }

    static MICO_LongDouble nanl ()
    {
	static MICO_LongDouble not_a_number = 0;
	if (not_a_number == 0) {
	    not_a_number = LDBL_MAX;
	}
	return not_a_number;
    }

    static MICO_Boolean is_infinityl (MICO_LongDouble d)
    {
	if (!::_finite (d) && !::_isnan (d)) 
	    return TRUE;
	else
	    return FALSE;
    }

    static MICO_Boolean is_nanl (MICO_LongDouble d)
    {
	if (::_isnan (d))
	    return TRUE;
	else 
	    return FALSE;
    }
};

#elif defined(_POCKET_PC)

#include <unistd.h>
#include <signal.h>

#if !defined(__GNUG__) || !defined(__sgi)
// conflict for initstate ()
#include <math.h>
#endif

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#ifdef HAVE_SUNMATH_H
#include <sunmath.h>
#endif

#include <mico/lmath.h>

class OSMath {
    static MICO_Double _infinity;
    static MICO_LongDouble _long_infinity;
    static MICO_Double _notanumber;
    static MICO_LongDouble _long_notanumber;
public:
    static MICO_Double infinity (MICO_Boolean neg)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_Double nan ()
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_Boolean is_infinity (MICO_Double d)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_Boolean is_nan (MICO_Double d)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_LongDouble infinityl (MICO_Boolean neg)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_LongDouble nanl ()
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_Boolean is_infinityl (MICO_LongDouble d)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }

    static MICO_Boolean is_nanl (MICO_LongDouble d)
    {
		MICO_NOT_IMPLEMENTED;
		return 0;
    }
};

#else // neither _WIN32 nor _POCKET_PC

#if !defined(__GNUG__) || !defined(__sgi)
// conflict for initstate ()
#include <math.h>
#endif

#ifdef __FreeBSD__
#include <sys/param.h>
#if (defined(__FreeBSD__) && __FreeBSD_version >= 500035)
#ifndef fpclassify
#define	fpclassify(x) \
    ((sizeof (x) == sizeof (float)) ? __fpclassifyf(x) \
    : (sizeof (x) == sizeof (double)) ? __fpclassifyd(x) \
    : __fpclassifyl(x))
#endif
#ifndef isinf
#define	isinf(x)	(fpclassify(x) == FP_INFINITE)
#endif
#ifndef isnan
#define	isnan(x)	(fpclassify(x) == FP_NAN)
#endif
#endif

#ifndef asinl
#define asinl asin
#endif
#ifndef ldexpl
#define ldexpl ldexp
#endif
#ifndef frexpl
#define frexpl frexp
#endif
#ifndef fmodl
#define fmodl fmod
#endif
#ifndef ceill
#define ceill ceil
#endif
#ifndef floorl
#define floorl floor
#endif
#endif // __FreeBSD__

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#ifdef HAVE_SUNMATH_H
#include <sunmath.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#define isinf( x ) ( ( sizeof ( x ) == sizeof(double) ) ?           \
                   __isinfd ( x ) :                                 \
                   ( sizeof ( x ) == sizeof( float) ) ?             \
                   __isinff ( x ) :                                 \
                   __isinf  ( x ) )
#define isnan( x ) ( ( sizeof ( x ) == sizeof(double) ) ?           \
                   __isnand ( x ) :                                 \
                   ( sizeof ( x ) == sizeof( float) ) ?             \
                   __isnanf ( x ) :                                 \
                   __isnan  ( x ) )
#endif // __APPLE__ && __MACH__

#include <mico/lmath.h>

class OSMath {
    static MICO_Double _infinity;
    static MICO_LongDouble _long_infinity;
    static MICO_Double _notanumber;
    static MICO_LongDouble _long_notanumber;
public:
    static MICO_Double infinity (MICO_Boolean neg)
    {
#if defined(HAVE_INFNAN)
	return infnan (neg ? -ERANGE : ERANGE);
#elif defined(HUGE_VAL)
        return HUGE_VAL;
#else
	if (_infinity == 0) {
	    struct sigaction act, oldact;
	    act.sa_handler = SIG_IGN;
	    act.sa_flags = 0;
	    sigemptyset (&act.sa_mask);
	    sigaction (SIGFPE, &act, &oldact);
	    // XXX assumption: +/- 1/0 == +/- infinity
	    _infinity = -1 / floor (0.1);
	    sigaction (SIGFPE, &oldact, NULL);
	}
	return neg ? -_infinity : _infinity;
#endif
    }

    static MICO_Double nan ()
    {
#if defined(HAVE_INFNAN)
	return infnan (0);
#else
	if (_notanumber == 0) {
	    struct sigaction act, oldact;
	    act.sa_handler = SIG_IGN;
	    act.sa_flags = 0;
	    sigemptyset (&act.sa_mask);
	    sigaction (SIGFPE, &act, &oldact);
	    // XXX assumption: arcsin (2.0) == NAN
	    _notanumber = asin (2.0);
	    sigaction (SIGFPE, &oldact, NULL);
	}
	return _notanumber;
#endif
    }

    static MICO_Boolean is_infinity (MICO_Double d)
    {
#if defined(HAVE_ISINF)
	return isinf (d);
#elif defined(HAVE_FINITE) && defined(HAVE_ISNAN)
	return !finite (d) && !isnan (d);
#else
	return d == infinity (TRUE) || d == infinity (FALSE);
#endif
    }

    static MICO_Boolean is_nan (MICO_Double d)
    {
#if defined(HAVE_ISNAN)
	return isnan (d);
#else
	// d == mk_nan() does not work ...
	assert (0);
#endif
    }

    static MICO_LongDouble infinityl (MICO_Boolean neg)
    {
#if defined(HAVE_INFNANL)
	return infnanl (neg ? -ERANGE : ERANGE);
#elif defined(HUGE_VALL)
        return HUGE_VALL;
#else
	if (_long_infinity == 0) {
	    struct sigaction act, oldact;
	    act.sa_handler = SIG_IGN;
	    act.sa_flags = 0;
	    sigemptyset (&act.sa_mask);
	    sigaction (SIGFPE, &act, &oldact);
	    // XXX assumption: +/- 1/0 == +/- infinity
	    _long_infinity = (long double)-1 / floor (0.1);
	    sigaction (SIGFPE, &oldact, NULL);
	}
	return neg ? -_long_infinity : _long_infinity;
#endif
    }

// this undef is needed for VisualAge C++ 6.0 on AIX 5.x
#undef nanl
    static MICO_LongDouble nanl ()
    {
#if defined(HAVE_INFNANL)
	return infnanl (0);
#else
	if (_long_notanumber == 0) {
	    struct sigaction act, oldact;
	    act.sa_handler = SIG_IGN;
	    act.sa_flags = 0;
	    sigemptyset (&act.sa_mask);
	    sigaction (SIGFPE, &act, &oldact);
	    // XXX assumption: arcsin (2.0) == NAN
	    _long_notanumber = asinl (2.0);
	    sigaction (SIGFPE, &oldact, NULL);
	}
	return _long_notanumber;
#endif
    }

    static MICO_Boolean is_infinityl (MICO_LongDouble d)
    {
#if defined(HAVE_ISINFL)
	return isinfl (d);
#elif defined(HAVE_ISINF)
	return isinf (d);
#else
	return d == infinityl (TRUE) || d == infinityl (FALSE);
#endif
    }

    static MICO_Boolean is_nanl (MICO_LongDouble d)
    {
#if defined(HAVE_ISNANL)
	return isnanl (d);
#elif defined(HAVE_ISNAN)
	return isnan (d);
#else
	// d == mk_nan() does not work ...
	assert (0);
#endif
    }
};

#endif

#endif // __mico_os_math_h__
