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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <ctype.h>
#include <mico/os-math.h>
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

/*
 * On AIX, the div() function is implemented as a macro, which screws
 * our definition of div below.
 */

#ifdef div
#undef div
#endif

/*
 * one would usually use pow(10,x) but for some strange
 * reasons this segfaults on AIX using egcs 1.0.2 in
 * from_longdouble(). However, if you write a test program
 * with just what from_longdouble() does in it everything
 * works fine. This is what is called a Heisenbug.
 */
static MICO_LongDouble
power10 (int x)
{
    if (x == 0)
        return 1;
    int neg = 0;
    if (x < 0) {
	neg = 1;
	x = -x;
    }
    MICO_LongDouble s = 1, t = 10;
    while (x) {
	if (x & 1)
	    s *= t;
	x >>= 1;
	t *= t;
    }
    if (neg)
	s = 1/s;
    return s;
}

#ifdef CRAY
static MICO_LongDouble
ceill (MICO_LongDouble value)
{
    MICO_LongLong f = (MICO_LongLong)value;
    if ( (MICO_LongDouble)f == value ) {
	return (MICO_LongDouble)f;
    } else {
	return (MICO_LongDouble) (f + 1);
    }
}
#endif // CRAY

static MICO_LongDouble
epsilonl (MICO_LongDouble v)
{
    static MICO_LongDouble d = 0.0;
    if (d == 0.0) {
	/*
	 * calculate the error in representing 0.6 in a LongDouble.
	 */
	d = 6;
	d /= 10;
	/*
	 * subtract 0.5 before multiply instead of subtracting 5
	 * afterwards to make sure multiplying by 10 doesn't overflow
	 * mantissa.
	 */
	d -= 0.5;
	d *= 10;
	d -= 1;

	int exp;
	frexpl (d, &exp);

	d = ldexpl (1, exp);
	/*
	 * now d is the smallest power of 2 that is greater than
	 * the error in representing 0.6 as a LongDouble
	 */
    }

    int exp;
    frexpl (v, &exp);
    return ldexpl (d, exp);
}

static MICO_Double
epsilon (MICO_Double v)
{
    static MICO_Double d = 0.0;
    if (d == 0.0) {
	/*
	 * calculate the error in representing 0.6 in a Double.
	 */
	d = 6;
	d /= 10;
	/*
	 * subtract 0.5 before multiply instead of subtracting 5
	 * afterwards to make sure multiplying by 10 doesn't overflow
	 * mantissa.
	 */
	d -= 0.5;
	d *= 10;
	d -= 1;

	int exp;
	frexpl (d, &exp);

	d = ldexpl (1, exp);
	/*
	 * now d is the smallest power of 2 that is greater than
	 * the error in representing 0.6 as a Double
	 */
    }

    int exp;
    frexp (v, &exp);
    return ldexp (d, exp);
}

static MICO_UShort
digitsl ()
{
    static MICO_UShort digits = 1;

    if (digits == 1) {
	MICO_LongDouble v = 1;

	while (42) {
	    v = v*10 + 1;
	    if (fmodl (v, 10) != 1)
		break;
	    ++digits;
	}
    }
    return digits - 2;
}

static MICO_UShort
digits ()
{
    static MICO_UShort digits = 1;

    if (digits == 1) {
	MICO_Double v = 1;

	while (42) {
	    v = v*10 + 1;
	    if (fmodl (v, 10) != 1)
		break;
	    ++digits;
	}
    }
    return digits - 2;
}

FixedBase::FixedBase ()
    : _digits(0), _scale(0), _val(0.0)
{
}

FixedBase::FixedBase (MICO_Long v)
{
    char buf[100];
    sprintf (buf, "%i", (int)v);
    compute_params (buf, _digits, _scale);
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_ULong v)
{
    char buf[100];
    sprintf (buf, "%u", (unsigned int)v);
    compute_params (buf, _digits, _scale);
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_LongLong v)
{
    char buf[100];
    // XXX
    sprintf (buf, "%i", (int)v);
    compute_params (buf, _digits, _scale);
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_ULongLong v)
{
    char buf[100];
    // XXX
    sprintf (buf, "%u", (unsigned int)v);
    compute_params (buf, _digits, _scale);
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_Double v)
{
    MICO_Double v2 = (v < 0 ? -v : v) + epsilon (v);

    char buf[100];
    sprintf (buf, "%.31e", v2);
    compute_params (buf, _digits, _scale, digits());
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_LongDouble v)
{
    MICO_LongDouble v2 = (v < 0 ? -v : v) + epsilonl (v);

    char buf[100];
#ifdef HAVE_PRINTF_LF
    sprintf (buf, "%.31Le", v2);
    compute_params (buf, _digits, _scale, digitsl());
#else
    sprintf (buf, "%.31le", (MICO_Double)v2);
    compute_params (buf, _digits, _scale, digits());
#endif
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (const char *s)
{
    CORBA::LongDouble v;
#ifdef HAVE_SCANF_LF
    sscanf ((char *)s, "%Lf", &v);
#else
    /*
     * this is only an approximation and will probably break fixed<>
     * parameter calculation on systems where
     * sizeof(double) < sizeof(long double). but fortunately all
     * systems where scanf("%Lf") is known to be broken (Linux/Alpha
     * and HPUX) have sizeof(double) == sizeof(long double).
     */
    v = strtod ((char *)s, NULL);
#endif
    compute_params (s, _digits, _scale);
    _val = v * power10 (_scale);
    adjust ();
}

FixedBase::FixedBase (MICO_UShort d, MICO_Short s)
    : _digits(d), _scale(s), _val(0.0)
{
    assert (_digits <= 31);
}

FixedBase::FixedBase (const FixedBase &f)
    : _digits(f._digits), _scale(f._scale), _val(f._val)
{
}

FixedBase::~FixedBase ()
{
}

FixedBase::operator CORBA::LongLong () const
{
    return (CORBA::LongLong)(_val / power10 (_scale));
}

FixedBase::operator CORBA::LongDouble () const
{
    return _val / power10 (_scale);
}

FixedBase
FixedBase::round (CORBA::UShort nscale) const
{
    FixedBase f (_digits, nscale);
    f._val = transform (nscale);
    f.adjust (TRUE);
    return f;
}

FixedBase
FixedBase::truncate (MICO_UShort nscale) const
{
    FixedBase f (_digits, nscale);
    f._val = transform (nscale);
    f.adjust (FALSE);
    return f;
}

void
FixedBase::adjust (CORBA::Boolean round)
{
    _val = fmodl (_val, power10 (_digits));
    CORBA::LongDouble delta = round ? 0.5 : 0.0;
    if (_val < 0) {
	_val = ceill (_val - delta);
    } else {
	_val = floorl (_val + delta);
    }
}

MICO_LongDouble
FixedBase::transform (MICO_Short nscale) const
{
    if (_scale == nscale)
	return _val;
    return _val * power10 (nscale - _scale);
}

void
FixedBase::compute_params (const char *str,
                           MICO_UShort &digits, MICO_Short &scale,
                           MICO_UShort max_significant_digits)
{
    while (*str && (isspace(*str) || *str=='+' || *str=='-' || *str=='0'))
        ++str;

    int pos = 0;
    int dot_pos = -1;
    int first_significant_pos = -1;
    int last_significant_pos = 0;

    while (*str && (isdigit(*str) || *str=='.')) {
        if (*str == '.') {
            dot_pos = pos;
        } else {
	    if (*str != '0') {
		if (first_significant_pos < 0)
		    first_significant_pos = pos;
		if (pos < max_significant_digits)
		    last_significant_pos = pos+1;
	    }
	    ++pos;
	}
        ++str;
    }

    if (first_significant_pos < 0)
	first_significant_pos = 0;
    if (dot_pos < 0)
	// rfweber@bluewin.de; was dot_pos = last_significant_pos;
	dot_pos = pos;

    if (*str == 'e' || *str == 'E')
	dot_pos += atol (++str);

    digits = last_significant_pos - first_significant_pos;
    scale = last_significant_pos - dot_pos;
}

#if 0
static MICO_LongDouble
epsilon ()
{
    static MICO_LongDouble d = 0.0;
    if (d == 0.0) {
	/*
	 * calculate the error in representing 0.6 in a LongDouble.
	 */
	d = 6;
	d /= 10;
	/*
	 * subtract 0.5 before multiply instead of subtracting 5
	 * afterwards to make sure multiplying by 10 doesn't overflow
	 * mantissa.
	 */
	d -= 0.5;
	d *= 10;
	d -= 1;

	int exp;
	frexpl (d, &exp);

	d = ldexpl (1, exp);
	/*
	 * now d is the smallest power of 2 that is greater than
	 * the error in representing 0.6 as a LongDouble
	 */
    }
    return d;
}

void
FixedBase::compute_params (MICO_LongDouble val,
			   MICO_UShort &digits, MICO_Short &scale)
{
    digits = 0;
    scale = 0;

    if (val == 0.0)
	return;
    if (val < 0)
	val = -val;

    int exp;
    frexpl (val, &exp);
    MICO_LongDouble eps = ldexpl (epsilon(), exp);

    /*
     * if there is no real floorl() then fabs(rem) is still >= 1 afterwards
     * if val doesn't fit into a double. but subtracting floor(rem)
     * again will make fabs(rem) < 1, which is done in the below loop...
     *
     * this way we get the full long double precision even if there are
     * no math functions for long double.
     */
    MICO_LongDouble rem = val - floorl (val);

    /*
     * shift val to the left, until the fractional part or 1 minus
     * the fractional part becomes <= eps.
     * eps takes into account the number of mantissa bits used for the
     * fractional part of the value and grows as we shift to the left.
     * this will only work if the value has at least one significant
     * digit less than the maximum number of significant digits supported
     * by LongDouble. otherwise we cannot decide whether e.g. 0.2999999...
     * is the LongDouble approximation for 0.3 (which has one siginificant
     * digit) or the user specified number 0.2999999... (which has > 1
     * significant digits).
     */
    while (42) {
	rem -= floorl (rem);
	if (1 - rem <= eps || rem <= eps) {
	    val = floorl (val);
	    break;
	}
	val *= 10;
	eps *= 10;
	rem *= 10;
	++scale;
    }

    // shift val to the right, until the 10^0 coeff becomes nonzero
    if (scale == 0) {
	while (fmodl (val, 10) == 0.0) {
	    val /= 10;
	    --scale;
	}
    }

    // now count the nonzero digits to the left of the decimal point
    while (val != 0.0) {
	++digits;
	val = floorl (val / 10);
    }

    if (digits > 31) {
	scale = 31 + digits + scale;
	digits = 31;
    }
}
#endif

FixedBase::FixedValue*
FixedBase::to_digits () const
{
    FixedValue *digs = new FixedValue;
    digs->length (_digits+1);

    MICO_LongDouble d = fabsl (_val) + 0.5;
    MICO_LongDouble q = power10 (_digits-1);

    for (int i = 0; i < _digits; ++i) {
	int r = (int)(d/q);
	d -= r*q;
	q /= 10;
	(*digs)[i] = (MICO_Octet)r;
    }
    (*digs)[_digits] = _val < 0 ? 1 : 0;

    return digs;
}

void
FixedBase::from_digits (const FixedValue &digs)
{
    assert (digs.length() == (CORBA::ULong)_digits+1);

    _val = 0.0;

    for (int i = 0; i < _digits; ++i) {
	_val *= 10;
	_val += (CORBA::Octet)digs[i];
    }

    if (digs[_digits])
	_val = -_val;

    // shouldn't be necessary ...
    adjust ();
}

istream &
FixedBase::read (istream &i)
{
    /*
     * operator>>(istream &, long double &) is broken on most systems,
     * so we cheat a bit and use operator>>(istream &, double &)...
     */
    CORBA::Double d;
    i >> d;
    _val = d * power10 (_scale);
    adjust ();
    return i;
}

ostream &
FixedBase::write (ostream &o) const
{
    FixedValue_var digs = to_digits();
    if (digs[(CORBA::ULong)_digits])
	o << "-";
    if (_scale > _digits) {
	o << ".";
	for (int i = _digits; i < _scale; ++i)
	    o << "0";
    }
    for (CORBA::ULong i0 = 0; i0 < _digits; ++i0) {
	if (i0 == (CORBA::ULong)(_digits-_scale))
	    o << ".";
        o << (int)(CORBA::Octet)digs[i0];
    }
    for (int i1 = _scale; i1 < 0; ++i1)
	o << "0";
    return o;
}

void
FixedBase::add (FixedBase &res, const FixedBase &a1, const FixedBase a2)
{
    MICO_UShort res_scale;

    if (a1._scale == a2._scale) {
	res._val = a1._val + a2._val;
	res_scale = a1._scale;
    } else if (a1._scale > a2._scale) {
	res._val = a1._val + a2.transform (a1._scale);
	res_scale = a1._scale;
    } else {
	res._val = a1.transform (a2._scale) + a2._val;
	res_scale = a2._scale;
    }
    if (res._scale != res_scale)
	res._val *= power10 (res._scale - res_scale);
    res.adjust ();
}

void
FixedBase::sub (FixedBase &res, const FixedBase &a1, const FixedBase a2)
{
    MICO_UShort res_scale;

    if (a1._scale == a2._scale) {
	res._val = a1._val - a2._val;
	res_scale = a1._scale;
    } else if (a1._scale > a2._scale) {
	res._val = a1._val - a2.transform (a1._scale);
	res_scale = a1._scale;
    } else {
	res._val = a1.transform (a2._scale) - a2._val;
	res_scale = a2._scale;
    }
    if (res._scale != res_scale)
	res._val *= power10 (res._scale - res_scale);
    res.adjust ();
}

void
FixedBase::mul (FixedBase &res, const FixedBase &a1, const FixedBase a2)
{
    res._val = (a1._val * a2._val) / power10 (a2._scale);
    if (res._scale != a1._scale)
	res._val *= power10 (res._scale - a1._scale);
    res.adjust ();
}

void
FixedBase::div (FixedBase &res, const FixedBase &a1, const FixedBase a2)
{
    res._val = (a1._val * power10 (a2._scale)) / a2._val;
    if (res._scale != a1._scale)
	res._val *= power10 (res._scale - a1._scale);
    res.adjust ();
}

void
FixedBase::neg (FixedBase &res, const FixedBase &a)
{
    res._val = - a._val;
    if (res._scale != a._scale)
	res._val *= power10 (res._scale - a._scale);
}

int
FixedBase::eq (const FixedBase &a1, const FixedBase &a2)
{
    if (a1._scale == a2._scale)
	return fabsl (a1._val - a2._val) < 0.1;
    if (a1._scale > a2._scale)
	return fabsl (a1._val - a2.transform (a1._scale)) < 0.1;
    return fabsl (a1.transform (a2._scale) - a2._val) < 0.1;
}

int
FixedBase::lt (const FixedBase &a1, const FixedBase &a2)
{
    if (a1._scale == a2._scale)
	return a1._val < a2._val;
    if (a1._scale > a2._scale)
	return a1._val < a2.transform (a1._scale);
    return a1.transform (a2._scale) < a2._val;
}

int
FixedBase::le (const FixedBase &a1, const FixedBase &a2)
{
    if (a1._scale == a2._scale)
	return a1._val <= a2._val;
    if (a1._scale > a2._scale)
	return a1._val <= a2.transform (a1._scale);
    return a1.transform (a2._scale) <= a2._val;
}

FixedBase &
FixedBase::operator= (const FixedBase &f)
{
    _val = f.transform (_scale);
    adjust ();
    return *this;
}

FixedBase &
FixedBase::operator+= (const FixedBase &f)
{
    FixedBase res (_digits, _scale);
    add (res, *this, f);
    *this = res;
    return *this;
}

FixedBase &
FixedBase::operator-= (const FixedBase &f)
{
    FixedBase res (_digits, _scale);
    sub (res, *this, f);
    *this = res;
    return *this;
}

FixedBase &
FixedBase::operator*= (const FixedBase &f)
{
    FixedBase res (_digits, _scale);
    mul (res, *this, f);
    *this = res;
    return *this;
}

FixedBase &
FixedBase::operator/= (const FixedBase &f)
{
    FixedBase res (_digits, _scale);
    div (res, *this, f);
    *this = res;
    return *this;
}

FixedBase &
FixedBase::operator++ ()
{
    FixedBase res (_digits, _scale);
    add (res, *this, FixedBase ((CORBA::LongDouble)1.0));
    *this = res;
    return *this;
}

FixedBase
FixedBase::operator++ (int)
{
    FixedBase res (*this);
    add (*this, res, FixedBase ((CORBA::LongDouble)1.0));
    return res;
}

FixedBase &
FixedBase::operator-- ()
{
    FixedBase res (_digits, _scale);
    sub (res, *this, FixedBase ((CORBA::LongDouble)1.0));
    *this = res;
    return *this;
}

FixedBase
FixedBase::operator-- (int)
{
    FixedBase res (*this);
    sub (*this, res, FixedBase ((CORBA::LongDouble)1.0));
    return res;
}

FixedBase
FixedBase::operator+ () const
{
    return *this;
}

FixedBase
FixedBase::operator- () const
{
    FixedBase res (_digits, _scale);
    neg (res, *this);
    return res;
}

CORBA::Boolean
FixedBase::operator!() const
{
    return _val == 0;
}

#define FMAX(a,b) ((a)>(b)?(a):(b))

FixedBase
operator+ (const FixedBase &v1, const FixedBase &v2)
{
    CORBA::Short scale = FMAX (v1.fixed_scale(), v2.fixed_scale());
    CORBA::UShort digits = 1+scale + FMAX (v1.fixed_digits()-v1.fixed_scale(),
					   v2.fixed_digits()-v2.fixed_scale());
    if (digits > 31) {
	scale = 31 - digits + scale;
	digits = 31;
    }
    FixedBase res (digits, scale);
    FixedBase::add (res, v1, v2);
    return res;
}

FixedBase
operator- (const FixedBase &v1, const FixedBase &v2)
{
    CORBA::Short scale = FMAX (v1.fixed_scale(), v2.fixed_scale());
    CORBA::UShort digits = 1+scale + FMAX (v1.fixed_digits()-v1.fixed_scale(),
					   v2.fixed_digits()-v2.fixed_scale());
    if (digits > 31) {
	scale = 31 - digits + scale;
	digits = 31;
    }
    FixedBase res (digits, scale);
    FixedBase::sub (res, v1, v2);
    return res;
}

FixedBase
operator* (const FixedBase &v1, const FixedBase &v2)
{
    CORBA::Short scale = v1.fixed_scale() + v2.fixed_scale();
    CORBA::UShort digits = v1.fixed_digits() + v2.fixed_digits();
    if (digits > 31) {
	scale = 31 - digits + scale;
	digits = 31;
    }
    FixedBase res (digits, scale);
    FixedBase::mul (res, v1, v2);
    return res;
}

FixedBase
operator/ (const FixedBase &v1, const FixedBase &v2)
{
    // XXX what are the correct digits and scale for result???
    FixedBase res (v1.fixed_digits(), v1.fixed_scale());
    FixedBase::div (res, v1, v2);
    return res;
}

#undef FMAX

CORBA::Boolean
operator> (const FixedBase &v1, const FixedBase &v2)
{
    return !FixedBase::le (v1, v2);
}

CORBA::Boolean
operator< (const FixedBase &v1, const FixedBase &v2)
{
    return FixedBase::lt (v1, v2);
}

CORBA::Boolean
operator>= (const FixedBase &v1, const FixedBase &v2)
{
    return !FixedBase::lt (v1, v2);
}

CORBA::Boolean
operator<= (const FixedBase &v1, const FixedBase &v2)
{
    return FixedBase::le (v1, v2);
}

CORBA::Boolean
operator== (const FixedBase &v1, const FixedBase &v2)
{
    return FixedBase::eq (v1, v2);
}

CORBA::Boolean
operator!= (const FixedBase &v1, const FixedBase &v2)
{
    return !FixedBase::eq (v1, v2);
}
